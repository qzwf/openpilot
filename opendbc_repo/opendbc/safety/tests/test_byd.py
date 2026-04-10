#!/usr/bin/env python3
import unittest

from opendbc.car.structs import CarParams
from opendbc.safety.tests.libsafety import libsafety_py
import opendbc.safety.tests.common as common
from opendbc.safety.tests.common import CANPackerPanda


class TestBydSafety(common.PandaCarSafetyTest, common.DriverTorqueSteeringSafetyTest):

  TX_MSGS = [[790, 0], [508, 2], [814, 0]]
  STANDSTILL_THRESHOLD = 0
  RELAY_MALFUNCTION_ADDRS = {0: (790,), 2: (508,)}
  FWD_BLACKLISTED_ADDRS = {2: [790], 0: [508]}

  MAX_RATE_UP = 17
  MAX_RATE_DOWN = 17
  MAX_TORQUE_LOOKUP = [0], [300]

  MAX_RT_DELTA = 170

  DRIVER_TORQUE_ALLOWANCE = 68
  DRIVER_TORQUE_FACTOR = 3

  def setUp(self):
    self.packer = CANPackerPanda("byd_general_pt")
    self.safety = libsafety_py.libsafety
    self.safety.set_safety_hooks(CarParams.SafetyModel.byd, 0)
    self.safety.init_tests()

  def _torque_driver_msg(self, torque):
    values = {"SteerDriverTorque": torque, "Counter": 0}
    return self.packer.make_can_msg_panda("ACC_EPS_STATE", 0, values)

  def _torque_cmd_msg(self, torque, steer_req=1):
    values = {"LKAS_Output": torque, "LKAS_Active": steer_req, "Counter": 0}
    return self.packer.make_can_msg_panda("ACC_MPC_STATE", 0, values)

  def _speed_msg(self, speed):
    # speed in m/s, convert to raw value: raw = speed / (0.07143644 * KPH_TO_MS)
    speed_kph = speed * 3.6
    raw = int(speed_kph / 0.07143644)
    values = {"CarDisplaySpeed": raw}
    return self.packer.make_can_msg_panda("CARSPEED", 0, values)

  def _user_brake_msg(self, brake):
    values = {"BrakePedal": brake}
    return self.packer.make_can_msg_panda("PEDAL", 0, values)

  def _user_gas_msg(self, gas):
    values = {"AcceleratorPedal": gas}
    return self.packer.make_can_msg_panda("PEDAL", 0, values)

  def _pcm_status_msg(self, enable):
    # AccState: 3 or 5 = engaged, anything else = not engaged
    values = {"AccState": 3 if enable else 0, "Counter": 0}
    return self.packer.make_can_msg_panda("ACC_HUD_ADAS", 2, values)


if __name__ == "__main__":
  unittest.main()
