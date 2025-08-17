// BYD ATTO3 Safety Implementation
// Based on BYD CAN message analysis and safety requirements

#include "safety_declarations.h"

// BYD CAN message IDs from fingerprint analysis
#define BYD_STEERING_MODULE_ADAS 0x1E2  // 482 - Steering control
#define BYD_ACC_CMD 0x32E               // 814 - ACC commands  
#define BYD_LKAS_HUD_ADAS 0x316         // 790 - LKAS HUD
#define BYD_ACC_HUD_ADAS 0x32D          // 813 - ACC HUD
#define BYD_STEERING_TORQUE 0x1FC       // 508 - Steering torque
#define BYD_STEER_MODULE_2 0x33F        // 831 - Steering angle
#define BYD_PEDAL 0x200                 // 512 - Pedal states
#define BYD_DRIVE_STATE 0x134           // 308 - Drive state

// Safety limits based on BYD ATTO3 specifications
#define BYD_MAX_STEER 1500              // Maximum steering torque (Nm * 10)
#define BYD_MAX_RATE_UP 10              // Max torque increase per frame
#define BYD_MAX_RATE_DOWN 25            // Max torque decrease per frame
#define BYD_DRIVER_TORQUE_ALLOWANCE 50  // Driver override threshold
#define BYD_DRIVER_TORQUE_FACTOR 3      // Driver torque scaling

// BYD checksum validation
#define BYD_CHECKSUM_KEY 0xAF

static uint8_t byd_checksum(uint8_t byte_key, uint8_t *dat, int len) {
  int first_bytes_sum = 0;
  int second_bytes_sum = 0;
  
  for (int i = 0; i < len - 1; i++) {  // Exclude checksum byte
    first_bytes_sum += dat[i] >> 4;
    second_bytes_sum += dat[i] & 0xF;
  }
  
  int remainder = second_bytes_sum >> 4;
  second_bytes_sum += byte_key >> 4;
  first_bytes_sum += byte_key & 0xF;
  
  int first_part = ((-first_bytes_sum + 0x9) & 0xF);
  int second_part = ((-second_bytes_sum + 0x9) & 0xF);
  
  return (((first_part + (-remainder + 5)) << 4) + second_part) & 0xFF;
}

// Global state
static int byd_rt_torque_last = 0;
static int byd_desired_torque_last = 0;
static uint32_t byd_ts_last = 0;
static bool byd_gas_pressed_prev = false;
static bool byd_brake_pressed_prev = false;
static struct sample_t byd_torque_driver;

static void byd_init(uint16_t param) {
  UNUSED(param);
  controls_allowed = false;
  relay_malfunction_reset();
  byd_rt_torque_last = 0;
  byd_desired_torque_last = 0;
  byd_ts_last = 0;
  byd_gas_pressed_prev = false;
  byd_brake_pressed_prev = false;
  init_sample(&byd_torque_driver, 6, 5);
}

static int byd_rx_hook(CANPacket_t *to_push) {
  bool valid = addr_safety_check(to_push, byd_rx_checks, BYD_RX_CHECK_LEN,
                                  NULL, NULL, NULL);

  if (valid) {
    int bus = GET_BUS(to_push);
    int addr = GET_ADDR(to_push);

    // Steering torque from driver
    if ((bus == 0) && (addr == BYD_STEERING_TORQUE)) {
      int torque_driver_new = ((GET_BYTE(to_push, 0) & 0xFFU) | 
                              ((GET_BYTE(to_push, 1) & 0xFFU) << 8));
      torque_driver_new = to_signed(torque_driver_new, 16);
      update_sample(&byd_torque_driver, torque_driver_new);
    }

    // Gas pedal
    if ((bus == 0) && (addr == BYD_PEDAL)) {
      int gas_pedal = GET_BYTE(to_push, 0);
      gas_pressed = gas_pedal > 5;  // 5% threshold
    }

    // Brake pedal  
    if ((bus == 0) && (addr == BYD_DRIVE_STATE)) {
      brake_pressed = GET_BIT(to_push, 16);  // BRAKE_PRESSED bit
    }

    // Vehicle speed for controls_allowed
    if ((bus == 0) && (addr == BYD_STEER_MODULE_2)) {
      // Speed would be extracted here if available in this message
      // For now, allow controls at any speed (to be refined)
      controls_allowed = true;
    }

    generic_rx_checks((addr == BYD_STEERING_MODULE_ADAS));
  }
  return valid;
}

static int byd_tx_hook(CANPacket_t *to_send) {
  int tx = 1;
  int addr = GET_ADDR(to_send);
  int bus = GET_BUS(to_send);

  if (bus == 0) {
    // Steering control message
    if (addr == BYD_STEERING_MODULE_ADAS) {
      int desired_torque = ((GET_BYTE(to_send, 0) & 0xFFU) | 
                           ((GET_BYTE(to_send, 1) & 0xFFU) << 8));
      desired_torque = to_signed(desired_torque, 16);

      // Verify checksum
      uint8_t checksum = GET_BYTE(to_send, 7);
      uint8_t expected_checksum = byd_checksum(BYD_CHECKSUM_KEY, 
                                              GET_BYTES(to_send), 8);
      
      if (checksum != expected_checksum) {
        tx = 0;  // Reject message with bad checksum
      }

      // Safety checks
      if (controls_allowed) {
        // Torque rate limit
        int violation = max_limit_check(desired_torque, byd_desired_torque_last, 
                                       &byd_ts_last, BYD_MAX_RATE_UP, BYD_MAX_RATE_DOWN);
        
        // Absolute torque limit
        violation |= max_limit_check(desired_torque, BYD_MAX_STEER, 
                                    -BYD_MAX_STEER, 0, 0);

        // Driver override check
        int torque_driver = get_sample(&byd_torque_driver);
        violation |= driver_limit_check(desired_torque, byd_desired_torque_last, 
                                       &byd_torque_driver, BYD_DRIVER_TORQUE_ALLOWANCE, 
                                       BYD_DRIVER_TORQUE_FACTOR, BYD_MAX_STEER, 
                                       -BYD_MAX_STEER, BYD_MAX_RATE_UP, BYD_MAX_RATE_DOWN);

        if (violation) {
          tx = 0;
        }
      } else {
        // Controls not allowed, only allow zero torque
        if (desired_torque != 0) {
          tx = 0;
        }
      }

      byd_desired_torque_last = desired_torque;
      byd_rt_torque_last = desired_torque;
    }

    // Block other control messages when controls not allowed
    if (!controls_allowed && 
        ((addr == BYD_ACC_CMD) || (addr == BYD_LKAS_HUD_ADAS) || 
         (addr == BYD_ACC_HUD_ADAS))) {
      tx = 0;
    }
  }

  return tx;
}

static int byd_fwd_hook(int bus_num, CANPacket_t *to_fwd) {
  int bus_fwd = -1;

  if (bus_num == 0) {
    bus_fwd = 2;  // Forward to camera bus
  }
  if (bus_num == 2) {
    bus_fwd = 0;  // Forward from camera bus
  }

  return bus_fwd;
}

static const addr_checks byd_rx_checks[] = {
  {.msg = {{BYD_STEERING_TORQUE, 0, 8, .check_checksum = true, .max_counter = 15U, .expected_timestep = 10000U}, { 0 }, { 0 }}},
  {.msg = {{BYD_PEDAL, 0, 8, .check_checksum = true, .max_counter = 15U, .expected_timestep = 20000U}, { 0 }, { 0 }}},
  {.msg = {{BYD_DRIVE_STATE, 0, 8, .check_checksum = true, .max_counter = 15U, .expected_timestep = 20000U}, { 0 }, { 0 }}},
  {.msg = {{BYD_STEER_MODULE_2, 0, 8, .check_checksum = true, .max_counter = 15U, .expected_timestep = 10000U}, { 0 }, { 0 }}},
};
#define BYD_RX_CHECK_LEN (sizeof(byd_rx_checks) / sizeof(byd_rx_checks[0]))

const safety_hooks byd_hooks = {
  .init = byd_init,
  .rx = byd_rx_hook,
  .tx = byd_tx_hook,
  .tx_lin = nooutput_tx_lin_hook,
  .fwd = byd_fwd_hook,
};