import struct

def read_records(format, f):
    record_struct = struct.Struct(format)
    chunks = iter(lambda: f.read(record_struct.size), b'')
    return (record_struct.unpack(chunk) for chunk in chunks)

def main():
    with open("info.bin", 'rb') as fin:
        tabletop_struct = struct.Struct('>2s4s10s3s') # bin format for tabletop
        tabletop_chunk = fin.read(tabletop_struct.size)
        tabletop = tabletop_struct.unpack(tabletop_chunk)
        
        
        print("%s,%s,%s,%s" % (tabletop[0].decode('utf-8'), tabletop[1].decode('utf-8'), tabletop[2].decode('utf-8'), tabletop[3].decode('utf-8')))
        
        for rec in read_records('>i20s5si', fin): # bin format for data
            print("%d,%s,%s,%d" % (rec[0], rec[1].decode('utf-8'), rec[2].decode('utf-8'), rec[3]))
        
if __name__ == "__main__":
    main()