import serial
import uinput
import time

# --- Open UART connected to BT-410 ---
ser = serial.Serial('/dev/ttyAMA0', baudrate=57600, timeout=0.1)

# --- Define joystick buttons (10 total) ---
events = (
    uinput.BTN_A, uinput.BTN_B, uinput.BTN_X, uinput.BTN_Y,
    uinput.BTN_TL, uinput.BTN_TR,       # Shoulder buttons
    uinput.BTN_SELECT, uinput.BTN_START,
    uinput.BTN_THUMBL, uinput.BTN_THUMBR
)

device = uinput.Device(events, name="RC100B Joystick")
print("RC100B virtual joystick active on /dev/input/js0")

# --- RC-100B bit mapping (updated) ---
button_map = {
    1: 4,    # Up
    2: 5,    # Down
    4: 6,    # Left
    8: 7,    # Right
    16: 0,   # Button 1 (A)
    32: 1,   # Button 2 (B)
    64: 2,   # Button 3 (X)
    128: 3,  # Button 4 (Y)
    256: 8,  # Button 5 (Left shoulder)
    512: 9,  # Button 6 (Right shoulder)
}

last_code = 0

while True:
    frame = ser.read(6)
    if len(frame) != 6:
        continue

    #if frame[0] != 0xFF or frame[1] != 0x55:
        #continue
    if frame[0] != 0xFF:
        continue

    #data_l, inv_l = frame[2], frame[3]
   # data_h, inv_h = frame[4], frame[5]
    
    data_l= frame[2]
    data_h= frame[4]

    #if (data_l ^ inv_l) != 0xFF or (data_h ^ inv_h) != 0xFF:
        #continue

    code = (data_h << 8) | data_l

    if code != last_code:
        # Release all buttons
        for e in events:
            device.emit(e, 0)

        # Press active buttons
        for bit, idx in button_map.items():
            if code & bit:
                device.emit(events[idx], 1)

        last_code = code

    time.sleep(0.02)

