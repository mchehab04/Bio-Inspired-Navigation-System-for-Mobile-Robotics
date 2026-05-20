import serial,time
ser = serial.Serial('/dev/i2c-14', baudrate=9600, timeout=0.1)

print("Reading raw bytes...")
while True:
    data=ser.read(100)
    if data:
        hex_str = ' '.join(f'{b:02X}' for b in data)
        print(hex_str)
