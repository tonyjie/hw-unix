import struct
import os
from multiprocessing import Process
from multiprocessing import Pipe

def write_records(records, format, f):
    record_struct = struct.Struct(format)
    for r in records:
        f.write(record_struct.pack(*r))

def csv2bin_part(ChildEnd, index):
    line = ChildEnd.recv()

    if (index == 1): # tabletop
        table_top_list = line.strip().split(',')
        for i in range(len(table_top_list)):
            table_top_list[i] = table_top_list[i].encode('utf-8')
        table_top = tuple(table_top_list)
        s = struct.pack('>2s4s10s3s', *table_top)

    else: # data
        personal_info = line.strip().split(',')
        personal_info_tuple = (int(personal_info[0]), personal_info[1].encode('utf-8'), personal_info[2].encode('utf-8'), int(personal_info[3]))
        s = struct.pack('>i20s5si', *personal_info_tuple)

    ChildEnd.send(s)

def main():
    line_num = 0
    ParentEnd, ChildEnd = Pipe()
    with open("info.csv", 'r') as fin:
        while True:
            line = fin.readline()
            if not line:
                break
            line_num += 1

            child = Process(target = csv2bin_part, args = (ChildEnd, line_num))
            child.start()
            ParentEnd.send(line)

    with open("info.bin", 'wb') as fout:
        for i in range(line_num):
            fout.write(ParentEnd.recv())

if __name__ == "__main__":
    main()