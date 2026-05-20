import serial
import struct

# Open serial port
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)

def read_packet():
    # Wait until we find header byte 0x54
    while True:
        b = ser.read(1)
        if not b:
            return None
        if b[0] == 0x54:
            break

    # Read the rest of the packet (43 more bytes)
    rest = ser.read(43)
    if len(rest) < 43:
        return None

    packet = b'\x54' + rest
    return packet

def parse_packet(packet):
    # Unpack header fields
    # Format: Header(1) Length(1) Speed(2) StartAngle(2) Data(36) EndAngle(2) Timestamp(2) CRC(1)
    header, length = struct.unpack_from('<BB', packet, 0)
    speed = struct.unpack_from('<H', packet, 2)[0]
    start_angle = struct.unpack_from('<H', packet, 4)[0]
    end_angle = struct.unpack_from('<H', packet, 42)[0]
    timestamp = struct.unpack_from('<H', packet, 44 - 3)[0]  # last 3 bytes before CRC
    crc = packet[-1]

    # Convert raw values
    speed_deg_s = speed / 64.0        # per Robotis docs (approx. deg/s)
    start_angle_deg = start_angle / 100.0
    end_angle_deg = end_angle / 100.0

    # Parse 12 distance/confidence sets (36 bytes)
    data = packet[6:42]
    distances = []
    confidences = []
    for i in range(12):
        offset = i * 3
        dist = struct.unpack_from('<H', data, offset)[0]
        conf = data[offset + 2]
        distances.append(dist)
        confidences.append(conf)

    return {
        "speed": speed_deg_s,
        "start_angle": start_angle_deg,
        "end_angle": end_angle_deg,
        "timestamp": timestamp,
        "crc": crc,
        "distances": distances,
        "confidences": confidences,
    }

print("Reading LDS-02 packets... (Ctrl+C to stop)\n")

try:
    while True:
        '''data = ser.read(100)
        if data:
            print(" ".join(f"{b:02X}" for b in data))'''
        packet = read_packet()
        if not packet:
            continue
        parsed = parse_packet(packet)
        print(f"Speed: {parsed['speed']:.1f} °/s | "
              f"Angle: {parsed['start_angle']:.2f}° - {parsed['end_angle']:.2f}° | "
              f"Ts: {parsed['timestamp']} | CRC: {hex(parsed['crc'])}")
        for i, (d, c) in enumerate(zip(parsed['distances'], parsed['confidences'])):
            print(f"  P{i+1:02d}: {d} mm, Conf: {c}")
        print('-'*50)

except KeyboardInterrupt:
    ser.close()
    print("Stopped.")

