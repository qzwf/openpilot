#pragma once

#include "opendbc/safety/safety_declarations.h"

// CAN message addresses (from byd_general_pt.dbc)
#define BYD_EPS               482U   // RX: steering angle (bus 0)
#define BYD_CARSPEED           496U   // RX: vehicle speed (bus 0)
#define BYD_ACC_EPS_STATE      508U   // RX: driver torque, EPS state (bus 0); TX: fake torque (bus 2)
#define BYD_PEDAL              834U   // RX: gas/brake pedal (bus 0)
#define BYD_PCM_BUTTONS        944U   // RX: ACC on/off (bus 0)
#define BYD_ACC_MPC_STATE      790U   // RX: LKAS state from MPC (bus 2); TX: steering command (bus 0)
#define BYD_ACC_HUD_ADAS       813U   // RX: cruise state (bus 2)
#define BYD_ACC_CMD            814U   // TX: accel command (bus 0)

// CAN bus numbers
#define BYD_MAIN_BUS 0U
#define BYD_CAM_BUS  2U

// BYD custom checksum: matches bydcan.py byd_checksum(byte_key, dat)
static uint8_t byd_compute_checksum_raw(uint8_t byte_key, const uint8_t *dat, int len) {
  uint8_t first_bytes_sum = 0;
  uint8_t second_bytes_sum = 0;
  for (int i = 0; i < len; i++) {
    first_bytes_sum += dat[i] >> 4;
    second_bytes_sum += dat[i] & 0xFU;
  }
  uint8_t remainder = second_bytes_sum >> 4;
  second_bytes_sum += byte_key >> 4;
  first_bytes_sum += byte_key & 0xFU;
  uint8_t first_part = ((-first_bytes_sum + 0x9U) & 0xFU);
  uint8_t second_part = ((-second_bytes_sum + 0x9U) & 0xFU);
  return (((first_part + (uint8_t)(-remainder + 5)) << 4) + second_part) & 0xFFU;
}

static uint8_t byd_get_counter(const CANPacket_t *msg) {
  uint8_t cnt = 0;
  if ((msg->addr == BYD_ACC_HUD_ADAS) || (msg->addr == BYD_ACC_MPC_STATE)) {
    // Counter: bits 48-51 = byte 6 low nibble
    cnt = msg->data[6] & 0xFU;
  } else if (msg->addr == BYD_ACC_EPS_STATE) {
    // Counter: bits 52-55 = byte 6 high nibble
    cnt = (msg->data[6] >> 4) & 0xFU;
  } else if (msg->addr == BYD_ACC_CMD) {
    // Counter: bits 48-51 = byte 6 low nibble
    cnt = msg->data[6] & 0xFU;
  } else {
  }
  return cnt;
}

static uint32_t byd_get_checksum(const CANPacket_t *msg) {
  uint8_t chk = 0;
  if ((msg->addr == BYD_ACC_HUD_ADAS) || (msg->addr == BYD_ACC_MPC_STATE) ||
      (msg->addr == BYD_ACC_CMD)) {
    // CheckSum: bits 56-63 = byte 7
    chk = msg->data[7];
  } else if (msg->addr == BYD_ACC_EPS_STATE) {
    // CheckSum: bits 56-63 = byte 7
    chk = msg->data[7];
  } else {
  }
  return chk;
}

static uint32_t byd_compute_checksum(const CANPacket_t *msg) {
  // All BYD checksummed messages use key 0xAF
  return byd_compute_checksum_raw(0xAFU, msg->data, GET_LEN(msg));
}

static void byd_rx_hook(const CANPacket_t *msg) {
  if (msg->bus == BYD_MAIN_BUS) {
    // Vehicle speed from CARSPEED: CarDisplaySpeed at bits 0-15, unsigned, scale 0.07143644 kph
    if (msg->addr == BYD_CARSPEED) {
      int speed_raw = (msg->data[1] << 8) | msg->data[0];
      vehicle_moving = speed_raw > 0;
      // Convert to m/s: raw * 0.07143644 kph * (1/3.6) m/s/kph ≈ raw * 0.01984
      UPDATE_VEHICLE_SPEED(speed_raw * 0.07143644 * KPH_TO_MS);
    }

    // Gas pedal from PEDAL: AcceleratorPedal at bits 0-7
    if (msg->addr == BYD_PEDAL) {
      gas_pressed = msg->data[0] > 0U;
    }

    // Brake pedal from PEDAL: BrakePedal at bits 8-15
    if (msg->addr == BYD_PEDAL) {
      brake_pressed = msg->data[1] > 0U;
    }

    // Driver torque from ACC_EPS_STATE: SteerDriverTorque at bits 27-42, signed
    if (msg->addr == BYD_ACC_EPS_STATE) {
      // bits 27-42: 16 bits starting at bit 27, little-endian signed
      int torque_driver_new = to_signed(((msg->data[5] & 0x7U) << 13) |
                                        (msg->data[4] << 5) |
                                        ((msg->data[3] >> 3) & 0x1FU), 16);
      update_sample(&torque_driver, torque_driver_new);
    }
  }

  if (msg->bus == BYD_CAM_BUS) {
    // Cruise state from ACC_HUD_ADAS: AccState at bits 24-27
    if (msg->addr == BYD_ACC_HUD_ADAS) {
      int acc_state = (msg->data[3]) & 0xFU;
      // Cruise engaged when AccState == 3 or AccState == 5
      bool cruise_engaged = (acc_state == 3) || (acc_state == 5);
      pcm_cruise_check(cruise_engaged);
    }
  }
}

static bool byd_tx_hook(const CANPacket_t *msg) {
  bool tx = true;

  static const TorqueSteeringLimits BYD_STEERING_LIMITS = {
    .max_torque = 300,
    .max_rate_up = 17,
    .max_rate_down = 17,
    .max_rt_delta = 170,
    .driver_torque_allowance = 68,
    .driver_torque_multiplier = 3,
    .type = TorqueDriverLimited,
    .has_steer_req_tolerance = true,
    .min_valid_request_frames = 2,
    .max_invalid_request_frames = 2,
    .min_valid_request_rt_interval = 100000,   // 100ms
  };

  if ((msg->bus == BYD_MAIN_BUS) && (msg->addr == BYD_ACC_MPC_STATE)) {
    // LKAS_Output: bits 11-26, 16-bit signed = steering torque command
    int desired_torque = to_signed(((msg->data[3] & 0x7U) << 13) |
                                   (msg->data[2] << 5) |
                                   ((msg->data[1] >> 3) & 0x1FU), 16);
    // LKAS_Active: bit 27
    int steer_req = (msg->data[3] >> 3) & 0x1U;

    if (steer_torque_cmd_checks(desired_torque, steer_req, BYD_STEERING_LIMITS)) {
      tx = false;
    }
  }

  return tx;
}

static safety_config byd_init(uint16_t param) {
  UNUSED(param);

  static const CanMsg BYD_TX_MSGS[] = {
    {BYD_ACC_MPC_STATE, BYD_MAIN_BUS, 8, .check_relay = true},   // steering command (OP -> EPS via bus 0)
    {BYD_ACC_EPS_STATE, BYD_CAM_BUS, 8, .check_relay = true},    // fake torque feedback (OP -> MPC via bus 2)
    {BYD_ACC_CMD,       BYD_MAIN_BUS, 8, .check_relay = false},  // accel passthrough
  };

  // NOTE: checksums and counters are ignored until DBC signal positions are verified against real CAN data
  static RxCheck byd_rx_checks[] = {
    {.msg = {{BYD_CARSPEED,      BYD_MAIN_BUS, 8, 50U, .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},
    {.msg = {{BYD_PEDAL,         BYD_MAIN_BUS, 8, 50U, .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},
    {.msg = {{BYD_ACC_EPS_STATE, BYD_MAIN_BUS, 8, 50U, .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},
    {.msg = {{BYD_ACC_HUD_ADAS,  BYD_CAM_BUS,  8, 50U, .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},
  };

  return BUILD_SAFETY_CFG(byd_rx_checks, BYD_TX_MSGS);
}

const safety_hooks byd_hooks = {
  .init = byd_init,
  .rx = byd_rx_hook,
  .tx = byd_tx_hook,
  .get_counter = byd_get_counter,
  .get_checksum = byd_get_checksum,
  .compute_checksum = byd_compute_checksum,
};
