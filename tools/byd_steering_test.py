#!/usr/bin/env python3
import time
import sys
from panda import Panda
from opendbc.can.packer import CANPacker
from opendbc.car.byd import bydcan
from opendbc.car.byd.values import DBC, CAR, CHECKSUM_KEY

# Connect to Panda
p = Panda()
p.set_safety_mode(Panda.SAFETY_ALLOUTPUT)

# Initialize packer with BYD DBC
packer = CANPacker(DBC[CAR.BYD_ATTO3]['pt'])

# Mock CP and cam_msg for testing


class MockCP:
    carFingerprint = CAR.BYD_ATTO3


CP = MockCP()
cam_msg = {
    "AutoFullBeamState": 0,
    "LeftLaneState": 0,
    "LKAS_Config": 1,
    "SETME2_0x1": 1,
    "MPC_State": 0,
    "AutoFullBeam_OnOff": 0,
    "LKAS_Output": 0,
    "LKAS_Active": 0,
    "SETME3_0x0": 0,
    "TrafficSignRecognition_OnOff": 0,
    "SETME4_0x0": 0,
    "SETME5_0x1": 1,
    "RightLaneState": 0,
    "LKAS_State": 0,
    "SETME6_0x0": 0,
    "TrafficSignRecognition_Result": 0,
    "LKAS_AlarmType": 0,
    "SETME7_0x3": 3,
}

counter = 0
active = False
req_prepare = 1  # Request LKAS preparation
req_torque = 0

try:
    print("Starting BYD steering test...")
    print("Press Ctrl+C to exit")

    while True:
        # Toggle active state every 5 seconds
        if int(time.time()) % 10 < 5:
            if not active:
                print("Activating LKAS...")
                active = True
                req_prepare = 0
        else:
            if active:
                print("Deactivating LKAS...")
                active = False
                req_prepare = 1
                req_torque = 0

        # Apply torque when active
        if active:
            # Oscillate between positive and negative torque
            req_torque = 50 * (1 if int(time.time()) % 4 < 2 else -1)
            print(f"Applying torque: {req_torque}")

        # Create and send steering control message
        addr, data = bydcan.create_steering_control(
            packer, CP, cam_msg, req_torque, req_prepare, active, counter
        )
        p.can_send(addr, data, 0)  # Send on bus 0

        # Increment counter
        counter = (counter + 1) % 16

        time.sleep(0.02)  # 50Hz

except KeyboardInterrupt:
    print("Test ended by user")
    # Send zero torque before exiting
    addr, data = bydcan.create_steering_control(
        packer, CP, cam_msg, 0, 0, False, counter)
    p.can_send(addr, data, 0)
