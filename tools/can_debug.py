#!/usr/bin/env python3
import time
from panda import Panda

# Connect to the Panda
p = Panda()

# Set safety mode to allow all outputs
p.set_safety_mode(Panda.SAFETY_ALLOUTPUT)

# Monitor CAN messages
try:
  while True:
    can_recv = p.can_recv()
    for address, _, dat, bus in can_recv:
      print(f"Bus: {bus}, Addr: 0x{address:X}, Data: {dat.hex()}")
    time.sleep(0.01)
except KeyboardInterrupt:
  print("Exiting...")