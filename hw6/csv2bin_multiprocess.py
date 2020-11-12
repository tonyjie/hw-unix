import struct
import os
from multiprocessing import Pool

class Stu_Info:
    def __init__(self, id, name, department, age):
        self.id = id
        self.name = name
        self.department = department
        self.age = age

def write_records(records, format, f):
    record_struct = struct.Struct(format)
    for r in records:
        f.write(record_struct.pack(*r))

def csv2bin_part(index):
    part_file_name = "part/info_" + str(index)
    if (index == 1): # tabletop
        with open(part_file_name, 'r') as fin:
            line = fin.readline()
            table_top_list = line.strip().split(',')
            for i in range(len(table_top_list)):
                table_top_list[i] = table_top_list[i].encode('utf-8')
            table_top = tuple(table_top_list)

        bin_file_name = "bin/" + str(index) + ".bin"
        with open(bin_file_name, 'wb') as fout:
            fout.write(struct.pack('>2s4s10s3s', *table_top))
    else: # data
        with open(part_file_name, 'r') as fin:
            line = fin.readline()
            personal_info = line.strip().split(',')
            personal_info_tuple = (int(personal_info[0]), personal_info[1].encode('utf-8'), personal_info[2].encode('utf-8'), int(personal_info[3]))
        bin_file_name = "bin/" + str(index) + ".bin"
        with open(bin_file_name, 'wb') as fout:
            fout.write(struct.pack('>i20s5si', *personal_info_tuple))

def main():
    line_num = 0
    if not os.path.exists("part"):
        os.mkdir("part")
    if not os.path.exists("bin"):
        os.mkdir("bin")
    with open("info.csv", 'r') as fin:
        lines = fin.readlines()
        for line in lines:
            line = line.strip()
            line_num += 1
            part_file_name = "part/info_" + str(line_num)
            with open(part_file_name, 'w') as f_part:
                f_part.write(line)

    p = Pool()
    for i in range(line_num):
        p.apply_async(csv2bin_part, args = (i+1, ))
    print("Waiting for all subprocesses done...")
    p.close()
    p.join()
    print("All subprocesses done.")
    os.system("rm -r part")

    with open("info.bin", 'wb') as fout:
        for i in range(line_num):
            bin_file_name = "bin/" + str(i+1) + ".bin"
            with open(bin_file_name, 'rb') as fin:
                line = fin.read()
                fout.write(line)
    os.system("rm -r bin")

if __name__ == "__main__":
    main()