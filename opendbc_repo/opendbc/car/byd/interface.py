#!/usr/bin/env python3

from opendbc.car import get_safety_config, structs
from opendbc.car.common.conversions import Conversions as CV
from opendbc.car.byd.carcontroller import CarController
from opendbc.car.byd.carstate import CarState
from opendbc.car.interfaces import CarInterfaceBase


class CarInterface(CarInterfaceBase):
  CarState = CarState
  CarController = CarController

  @staticmethod
  def _get_params(ret: structs.CarParams, candidate, fingerprint, car_fw, alpha_long, is_release, docs) -> structs.CarParams:
    ret.brand = "byd"
    ret.safetyConfigs = [get_safety_config(
      structs.CarParams.SafetyModel.byd)]

    ret.dashcamOnly = True

    ret.alphaLongitudinalAvailable = False
    ret.radarUnavailable = True

    ret.minEnableSpeed = -1
    ret.minSteerSpeed = 2 * CV.KPH_TO_MS

    # Measured at 0.4s, however in torqued.py, line 55, code will add 0.2
    ret.steerActuatorDelay = 0.2
    ret.steerLimitTimer = 0.4

    ret.lateralTuning.init('pid')
    ret.lateralTuning.pid.kpBP, ret.lateralTuning.pid.kiBP = [
      [10., 40.], [10., 40.]]
    ret.lateralTuning.pid.kpV, ret.lateralTuning.pid.kiV = [
      [0.16, 0.33], [0.015, 0.025]]
    ret.lateralTuning.pid.kf = 0.00004

    return ret
