import serial
import json
import time
from vpython import *

# === Configuration ===
SERIAL_PORT = '/dev/ttyUSB0'  # Update this to your Ground Station port
BAUD_RATE = 115200

# === Initialize Serial ===
try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=0.1)
    print(f"Connected to {SERIAL_PORT}")
except Exception as e:
    print(f"Error connecting to serial: {e}")
    ser = None

# === Setup 3D Scene ===
scene = canvas(title='RC Plane 3D Tracker Dashboard', width=800, height=600, center=vector(0,0,0), background=color.black)
scene.range = 50

# Create a simple aircraft model (Body, Wings, Tail)
plane_body = cylinder(pos=vector(0,0,0), axis=vector(5,0,0), radius=0.5, color=color.gray(0.5))
wing = box(pos=vector(1.5,0,0), size=vector(1, 0.1, 10), color=color.red)
tail_h = box(pos=vector(-4,0,0), size=vector(0.8, 0.1, 3), color=color.red)
tail_v = box(pos=vector(-4,0.5,0), size=vector(0.8, 1, 0.1), color=color.red)

# Group them into a single object
plane = compound([plane_body, wing, tail_h, tail_v])
plane.pos = vector(0,0,0)

# Create the Path Ribbon
path = curve(color=color.yellow, radius=0.1)
path.append(plane.pos)

# Ground Plane (Grid)
grid = []
for i in range(-50, 51, 10):
    grid.append(curve(pos=[vector(i,0,-50), vector(i,0,50)], color=color.gray(0.2)))
    grid.append(curve(pos=[vector(-50,0,i), vector(50,0,i)], color=color.gray(0.2)))

# Labels
label_status = label(pos=vector(0, 30, 0), text='Waiting for data...', height=16, border=4, font='monospace')

# === Main Loop ===
while True:
    rate(100) # 100Hz refresh
    
    if ser and ser.in_waiting > 0:
        try:
            line = ser.readline().decode('utf-8').strip()
            if line.startswith('{'):
                data = json.loads(line)
                
                # 1. Update Rotation (Pitch, Roll, Yaw)
                # Convert Euler degrees to VPython vector axis
                roll = radians(data.get('roll', 0))
                pitch = radians(data.get('pitch', 0))
                yaw = radians(data.get('yaw', 0))
                
                # Apply rotations
                plane.up = vector(0,1,0)
                plane.axis = vector(1,0,0)
                plane.rotate(angle=yaw, axis=vector(0,1,0), origin=plane.pos)
                plane.rotate(angle=pitch, axis=vector(0,0,1), origin=plane.pos)
                plane.rotate(angle=roll, axis=vector(1,0,0), origin=plane.pos)
                
                # 2. Update Position (X, Y, Z)
                # Note: VPython axes are different (X=Forward, Y=Up, Z=Left/Right)
                # Mapping: Tracker_X -> VPython_X, Tracker_Z -> VPython_Y, Tracker_Y -> VPython_Z
                new_pos = vector(data.get('x', 0), data.get('z', 0), -data.get('y', 0))
                plane.pos = new_pos
                
                # 3. Update Path
                path.append(new_pos)
                
                # 4. Update UI
                label_status.text = f"Roll: {data['roll']:>5.1f} | Pitch: {data['pitch']:>5.1f} | Yaw: {data['yaw']:>5.1f}\nPos: X:{data['x']:>5.1f} Y:{data['y']:>5.1f} Z:{data['z']:>5.1f}"

        except Exception as e:
            # print(f"Parser Error: {e}")
            pass
