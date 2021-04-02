import re
import struct
import math
import pandas as pd

def chunkstring(string, length):
    return (string[0+i:length+i] for i in range(0, len(string), length))

def splitPacket(packet):
    header = packet[0:6]
    fncode = packet[6:8]
    pc = packet[8:10]
    payload = packet[10:]
    func = fnmap.get(fncode) or ("UNKN",hex2uint16,2)
    values = splitPayload(payload,func[1],func[2])
    return [(func[0],fncode,pc,v[0],v[1],i) for i,v in enumerate(values)]

def hex2float(hex):
    if (len(hex) != 8):
        return math.nan
    return struct.unpack('<f', bytes.fromhex(hex))[0]

def hex2uint16(hex):
    if (len(hex) != 4):
        return math.nan
    return struct.unpack('<H', bytes.fromhex(hex))[0]

def hex2uint8(hex):
    if (len(hex) != 2):
        return math.nan
    return struct.unpack('<B', bytes.fromhex(hex))[0]

def splitPayload(payload, func, bytesPer):
    chunks = chunkstring(payload,2*bytesPer)
    return [(c,func(c)) for c in chunks if len(c)==2*bytesPer]

fnmap = {
    '43': ("GYRX",hex2float, 4),
    '44': ("GYRY",hex2float, 4),
    '45': ("GYRZ",hex2float, 4),
    '46': ("MAGX",hex2float, 4),
    '47': ("MAGY",hex2float, 4),
    '48': ("MAGZ",hex2float, 4),
    '11': ("UVA",hex2uint16, 2),
    '12': ("UVC",hex2uint16, 2),
    '13': ("TMP1",hex2uint16, 2),
    '14': ("TMP2",hex2uint16, 2),
    '15': ("TMP3",hex2uint16, 2),
    '16': ("LTF",hex2uint16, 2),
    '17': ("RAD",hex2uint8, 1)
}

def deserialize_raw(path):
    with open(path, 'rb') as f:
        hexdata = f.read().hex()
    #strip bytes preceding first header, '505050'
    hexdata = re.sub(r'^.*?(505050)', '505050', hexdata)
    packets = [p for c in chunkstring(hexdata,70) for p in splitPacket(c)]
    frame = pd.DataFrame(packets,columns=["NAME","FN","PC","PAYLOAD","VALUE","VIDX"])
    return frame