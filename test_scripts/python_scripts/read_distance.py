import serial
import struct

ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)

def find_packet():
    """Find and return one full 44-byte LDS-02 packet."""
    while True:
        b1 = ser.read(1)
        if not b1:
            return None
        if b1[0] == 0x54:               # header
            b2 = ser.read(1)
            if not b2:
                return None
            if b2[0] == 0x2C:           # length = 44
                rest = ser.read(42)     # remaining bytes
                if len(rest) == 42:
                    return b1 + b2 + rest

def extract_distances(packet):
    """Return list of 12 distance readings (mm)."""
    data = packet[6:42]  # 36-byte data block
    distances = []
    for i in range(12):
        offset = i * 3
        dist = struct.unpack_from('<H', data, offset)[0]
        distances.append(dist)
    return distances

print("Reading LDS-02 distances... (Ctrl+C to stop)\n")


while True:
    pkt = find_packet()
    if not pkt:
        continue
    distances = extract_distances(pkt)
    print("Distances (mm):", distances)

