#!/usr/bin/env python3
import pytest
from panda import Panda
from panda.tests.safety import libpandasafety_py
from panda.tests.safety.common import StdTest

MSG_BYD_STEERING_MODULE_ADAS = 0x1E2  # 482
MSG_BYD_ACC_CMD = 0x32E               # 814
MSG_BYD_STEERING_TORQUE = 0x1FC       # 508
MSG_BYD_PEDAL = 0x200                 # 512
MSG_BYD_DRIVE_STATE = 0x134           # 308

class TestBydSafety(StdTest):
  TX_MSGS = [[MSG_BYD_STEERING_MODULE_ADAS, 0], [MSG_BYD_ACC_CMD, 0]]
  STANDSTILL_THRESHOLD = 0
  RELAY_MALFUNCTION_ADDR = MSG_BYD_STEERING_MODULE_ADAS
  RELAY_MALFUNCTION_BUS = 0
  FWD_BLACKLISTED_ADDRS = {2: [MSG_BYD_STEERING_MODULE_ADAS, MSG_BYD_ACC_CMD]}
  FWD_BUS_LOOKUP = {0: 2, 2: 0}

  def setUp(self):
    self.packer = None
    self.safety = libpandasafety_py.libpandasafety
    self.safety.set_safety_hooks(Panda.SAFETY_BYD, 0)
    self.safety.init_tests()

  def _set_prev_torque(self, t):
    self.safety.set_desired_torque_last(t)
    self.safety.set_rt_torque_last(t)

  def _torque_driver_msg(self, torque):
    values = {"MAIN_TORQUE": torque}
    return self.packer.make_can_msg_panda("STEERING_TORQUE", 0, values)

  def _torque_cmd_msg(self, torque, steer_req=1):
    values = {"STEER_ANGLE": torque, "STEER_REQ": steer_req}
    return self.packer.make_can_msg_panda("STEERING_MODULE_ADAS", 0, values)

  def _gas_msg(self, gas):
    values = {"GAS_PEDAL": gas}
    return self.packer.make_can_msg_panda("PEDAL", 0, values)

  def _brake_msg(self, brake):
    values = {"BRAKE_PRESSED": brake}
    return self.packer.make_can_msg_panda("DRIVE_STATE", 0, values)

  def test_steer_safety_check(self):
    for enabled in [0, 1]:
      for t in range(-3000, 3000):
        self.safety.set_controls_allowed(enabled)
        self._set_prev_torque(t)
        if abs(t) > 1500 or (not enabled and t != 0):
          self.assertFalse(self.safety.safety_tx_hook(self._torque_cmd_msg(t)))
        else:
          self.assertTrue(self.safety.safety_tx_hook(self._torque_cmd_msg(t)))

  def test_manually_enable_controls_allowed(self):
    self.safety.set_controls_allowed(1)
    self.assertTrue(self.safety.get_controls_allowed())
    self.safety.set_controls_allowed(0)
    self.assertFalse(self.safety.get_controls_allowed())

  def test_spam_cancel_safety_check(self):
    self.safety.set_controls_allowed(0)
    self.assertTrue(self.safety.safety_tx_hook(self._torque_cmd_msg(0)))
    self.assertFalse(self.safety.safety_tx_hook(self._torque_cmd_msg(1)))

  def test_non_realtime_limit_up(self):
    self.safety.set_controls_allowed(1)

    self._set_prev_torque(0)
    self.assertTrue(self.safety.safety_tx_hook(self._torque_cmd_msg(10)))
    self.assertTrue(self.safety.safety_tx_hook(self._torque_cmd_msg(10)))
    self.assertTrue(self.safety.safety_tx_hook(self._torque_cmd_msg(10)))

    self._set_prev_torque(0)
    self.assertTrue(self.safety.safety_tx_hook(self._torque_cmd_msg(10)))
    self.assertFalse(self.safety.safety_tx_hook(self._torque_cmd_msg(21)))

  def test_non_realtime_limit_down(self):
    self.safety.set_controls_allowed(1)

    self.safety.set_rt_torque_last(1000)
    self.safety.set_desired_torque_last(1000)
    self.assertTrue(self.safety.safety_tx_hook(self._torque_cmd_msg(975)))
    self.assertTrue(self.safety.safety_tx_hook(self._torque_cmd_msg(975)))
    self.assertFalse(self.safety.safety_tx_hook(self._torque_cmd_msg(924)))

  def test_gas_disengage(self):
    self.safety.set_controls_allowed(1)
    self.safety.safety_rx_hook(self._gas_msg(0))
    self.assertTrue(self.safety.get_controls_allowed())
    self.safety.safety_rx_hook(self._gas_msg(10))
    self.assertFalse(self.safety.get_controls_allowed())

  def test_brake_disengage(self):
    self.safety.set_controls_allowed(1)
    self.safety.safety_rx_hook(self._brake_msg(0))
    self.assertTrue(self.safety.get_controls_allowed())
    self.safety.safety_rx_hook(self._brake_msg(1))
    self.assertFalse(self.safety.get_controls_allowed())

