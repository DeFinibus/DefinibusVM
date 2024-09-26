import sys
import struct
def string_to_int(source_string):
    res = []
    for i in range(0, len(source_string), 4):
        chunk = source_string[i:i+4]
        ascii_values = [ord(c) for c in chunk]
        while len(ascii_values) < 4:
            ascii_values.append(0)  # pad with zeros if necessary
        num = ascii_values[0] + (ascii_values[1] << 8) + (ascii_values[2] << 16) + (ascii_values[3] << 24)
        inum = int(hex(num),16 )
        res.append(inum)  # format as little-endian hex


    return res

if __name__=='__main__':
    if len(sys.argv) != 2:
        print("Usage: python3 string_to_int.py <source_string>")
        sys.exit(1)
    ret = string_to_int(sys.argv[1])
    print("result:")
    print(ret)
    
