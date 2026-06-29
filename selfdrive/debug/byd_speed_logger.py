#!/usr/bin/env python3
"""
BYD ATTO3 speed calibration logger.
Auto-starts with openpilot, logs wheel speed CAN values vs Comma's computed vEgo.
Output: /data/byd_speed_log.csv  (max 5 MB, then stops to avoid filling storage)

Columns: timestamp_s, FL_raw, FR_raw, BL_raw, FL_kmh, FR_kmh, BL_kmh, avg_kmh, vEgo_kmh
"""
import os
import sys
import time
import signal

sys.path.insert(0, '/data/openpilot')

MAX_BYTES = 5 * 1024 * 1024  # 5 MB cap
OUT_FILE = '/data/byd_speed_log.csv'
WHEEL_SPEED_ADDR = 290   # 0x122
WHEEL_SPEED_BUS  = 0

running = True
def _stop(sig, frame):
  global running
  running = False
signal.signal(signal.SIGTERM, _stop)
signal.signal(signal.SIGINT, _stop)

def main():
  import cereal.messaging as messaging
  sm = messaging.SubMaster(['can', 'carState'])

  # Rotate old file if it exists and is large
  if os.path.exists(OUT_FILE) and os.path.getsize(OUT_FILE) > MAX_BYTES:
    os.rename(OUT_FILE, OUT_FILE + '.old')

  header_needed = not os.path.exists(OUT_FILE)
  f = open(OUT_FILE, 'a')
  if header_needed:
    f.write('timestamp_s,FL_raw,FR_raw,BL_raw,FL_kmh,FR_kmh,BL_kmh,avg_kmh,vEgo_kmh\n')
    f.flush()

  last_vego = 0.0

  while running:
    sm.update(100)

    # Update vEgo from carState
    if sm.updated['carState']:
      last_vego = sm['carState'].vEgo * 3.6

    # Log wheel speed when we see the CAN frame
    if sm.updated['can']:
      for msg in sm['can']:
        if msg.address == WHEEL_SPEED_ADDR and msg.src == WHEEL_SPEED_BUS:
          d = bytes(msg.dat)
          fl_raw = int.from_bytes(d[0:2], 'little')
          fr_raw = int.from_bytes(d[2:4], 'little')
          bl_raw = int.from_bytes(d[4:6], 'little')
          fl = fl_raw * 0.1
          fr = fr_raw * 0.1
          bl = bl_raw * 0.1
          avg = (fl + fr + bl) / 3.0

          # Only log when moving (> 2 km/h)
          if avg > 2.0 or last_vego > 2.0:
            line = f'{time.monotonic():.2f},{fl_raw},{fr_raw},{bl_raw},{fl:.1f},{fr:.1f},{bl:.1f},{avg:.1f},{last_vego:.1f}\n'
            f.write(line)

    # Rotate/stop if file gets too large
    f.flush()
    if f.tell() > MAX_BYTES:
      f.close()
      os.rename(OUT_FILE, OUT_FILE + '.old')
      f = open(OUT_FILE, 'w')
      f.write('timestamp_s,FL_raw,FR_raw,BL_raw,FL_kmh,FR_kmh,BL_kmh,avg_kmh,vEgo_kmh\n')

  f.close()

if __name__ == '__main__':
  main()
