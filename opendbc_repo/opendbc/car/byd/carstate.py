import copy
from opendbc.can.parser import CANDefine, CANParser
from opendbc.car import structs, Bus
from opendbc.car.common.conversions import Conversions as CV
from opendbc.car.interfaces import CarStateBase
from opendbc.car.byd.values import DBC

ButtonType = structs.CarState.ButtonEvent.Type


class CarState(CarStateBase):
    def __init__(self, CP):
        super().__init__(CP)

        can_define = CANDefine(DBC[CP.carFingerprint]["pt"])
        self.shifter_values = can_define.dv["DRIVE_STATE"]["GEAR"]

        self.acc_hud_adas_counter = 0
        self.acc_mpc_state_counter = 0
        self.acc_cmd_counter = 0

        self.acc_active_last = False
        self.low_speed_alert = False
        self.lkas_allowed_speed = False

        self.lkas_prepared = False  # 318, EPS to OP
        self.acc_state = 0

        self.mpc_laks_output = 0
        self.mpc_laks_active = False
        self.mpc_laks_reqprepare = False

        self.cam_lkas = 0
        self.cam_acc = 0
        self.esc_eps = 0

        # self.button_states = {button.event_type: False for button in BUTTONS}

    def update(self, can_parsers) -> structs.CarState:
        cp = can_parsers[Bus.pt]
        cp_cam = can_parsers[Bus.cam]
        ret = structs.CarState()

        self.lkas_prepared = cp.vl["ACC_EPS_STATE"]["LKAS_Prepared"]

        lkas_config_isAccOn = (cp_cam.vl["ACC_MPC_STATE"]["LKAS_Config"] != 0)
        lkas_isMainSwOn = (cp.vl["PCM_BUTTONS"]["BTN_TOGGLE_ACC_OnOff"] == 1)

        lkas_hud_AccOn1 = (cp_cam.vl["ACC_HUD_ADAS"]["AccOn1"] == 1)
        self.acc_state = cp_cam.vl["ACC_HUD_ADAS"]["AccState"]

        # use wheels averages if you like
        # ret.wheelSpeeds = self.get_wheel_speeds(
        #     cp.vl["IPB1"]["WheelSpeed_FL"],
        #     cp.vl["IPB1"]["WheelSpeed_FR"],
        #     cp.vl["IPB1"]["WheelSpeed_RL"],
        #     cp.vl["IPB1"]["WheelSpeed_RR"],
        # )
        # speed_kph = mean([ret.wheelSpeeds.fl, ret.wheelSpeeds.fr, ret.wheelSpeeds.rl, ret.wheelSpeeds.rr])

        # use dash speedo as speed reference
        speed_raw = int(cp.vl["CARSPEED"]["CarDisplaySpeed"])
        speed_kph = speed_raw * 0.07143644  # this constant varies with vehicles
        ret.vEgoRaw = speed_kph * CV.KPH_TO_MS  # KPH to m/s
        ret.vEgo, ret.aEgo = self.update_speed_kf(ret.vEgoRaw)

        ret.standstill = (speed_raw == 0)

        if self.CP.minSteerSpeed > 0:
            if speed_kph > 2:
                self.lkas_allowed_speed = True
            elif speed_kph < 0.1:
                self.lkas_allowed_speed = False
        else:
            self.lkas_allowed_speed = True

        can_gear = int(cp.vl["DRIVE_STATE"]["GEAR"])
        ret.gearShifter = self.parse_gear_shifter(
            self.shifter_values.get(can_gear, None))

        # for button in BUTTONS:
        #    state = (cp.vl[button.can_addr][button.can_msg] in button.values)
        #   if self.button_states[button.event_type] != state:
        #        event = structs.CarState.ButtonEvent.new_message()
        #        event.type = button.event_type
        #        event.pressed = state
        #        self.button_events.append(event)
        #    self.button_states[button.event_type] = state

        # ret.genericToggle = bool(cp.vl["STALKS"]["HeadLight"])

        # ret.leftBlindspot = cp.vl["BSD_RADAR"]["LEFT_ROACH"] != 0
        # ret.rightBlindspot = cp.vl["BSD_RADAR"]["RIGHT_ROACH"] != 0

        ret.leftBlinker = (cp.vl["STALKS"]["LeftIndicator"] == 1)
        ret.rightBlinker = (cp.vl["STALKS"]["RightIndicator"] == 1)

        ret.steeringAngleDeg = cp.vl["EPS"]["SteeringAngle"]
        ret.steeringRateDeg = cp.vl["EPS"]["SteeringAngleRate"]

        ret.steeringTorque = cp.vl["ACC_EPS_STATE"]["SteerDriverTorque"]
        ret.steeringTorqueEps = cp.vl["ACC_EPS_STATE"]["MainTorque"]

        self.eps_state_counter = int(cp.vl["ACC_EPS_STATE"]["Counter"])

        ret.steeringPressed = abs(ret.steeringTorque) > 30

        ret.brake = int(cp.vl["PEDAL"]["BrakePedal"])

        ret.brakePressed = (ret.brake != 0)

        ret.seatbeltUnlatched = (cp.vl["BCM"]["DriverSeatBeltFasten"] != 1)

        ret.doorOpen = any([cp.vl["BCM"]["FrontLeftDoor"], cp.vl["BCM"]["FrontRightDoor"],
                            cp.vl["BCM"]["RearLeftDoor"],  cp.vl["BCM"]["RearRightDoor"]])

        ret.gasPressed = cp.vl["PEDAL"]["AcceleratorPedal"] != 0

        ret.cruiseState.available = lkas_isMainSwOn and lkas_config_isAccOn and lkas_hud_AccOn1
        ret.cruiseState.enabled = (
            self.acc_state == 3) or (self.acc_state == 5)
        ret.cruiseState.standstill = ret.standstill
        ret.cruiseState.speed = cp_cam.vl["ACC_HUD_ADAS"]["SetSpeed"] * CV.KPH_TO_MS

        if ret.cruiseState.enabled:
            if not self.lkas_allowed_speed and self.acc_active_last:
                self.low_speed_alert = True
            else:
                self.low_speed_alert = False
        ret.lowSpeedAlert = self.low_speed_alert

        ret.steerFaultTemporary = (self.acc_state == 7)

        self.acc_active_last = ret.cruiseState.enabled

        # use to fool mpc
        self.mpc_laks_output = cp_cam.vl["ACC_MPC_STATE"]["LKAS_Output"]
        # use to fool mpc
        self.mpc_laks_reqprepare = cp_cam.vl["ACC_MPC_STATE"]["LKAS_ReqPrepare"] != 0
        # use to fool mpc
        self.mpc_laks_active = cp_cam.vl["ACC_MPC_STATE"]["LKAS_Active"] != 0

        self.acc_hud_adas_counter = cp_cam.vl["ACC_HUD_ADAS"]["Counter"]
        self.acc_mpc_state_counter = cp_cam.vl["ACC_MPC_STATE"]["Counter"]
        self.acc_cmd_counter = cp_cam.vl["ACC_CMD"]["Counter"]

        self.cam_lkas = copy.copy(cp_cam.vl["ACC_MPC_STATE"])
        self.cam_adas = copy.copy(cp_cam.vl["ACC_HUD_ADAS"])
        self.cam_acc = copy.copy(cp_cam.vl["ACC_CMD"])
        self.esc_eps = copy.copy(cp.vl["ACC_EPS_STATE"])

        # EPS give up all inputs until restart
        ret.steerFaultPermanent = (cp.vl["ACC_EPS_STATE"]["TorqueFailed"] == 1)

        return ret

    @staticmethod
    def get_can_parsers(CP):
        pt_messages = [
            ("EPS", 100),
            ("CARSPEED", 50),
            ("PEDAL", 50),
            ("ACC_EPS_STATE", 50),
            ("DRIVE_STATE", 50),
            ("STALKS", 1),
            ("BCM", 1),
            ("PCM_BUTTONS", 20),
        ]

        cam_messages = [
            ("ACC_HUD_ADAS", 50),
            ("ACC_CMD", 50),
            ("ACC_MPC_STATE", 50),
        ]

        return {
            Bus.pt: CANParser(DBC[CP.carFingerprint][Bus.pt], pt_messages, 0),
            Bus.cam: CANParser(DBC[CP.carFingerprint][Bus.pt], cam_messages, 2),
        }
