import struct

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

def main():
    student_info = []

    with open("info.csv", 'r') as fin:
        line = fin.readline()
        table_top_list = line.strip().split(',')
        table_top = tuple(table_top_list)

        lines = fin.readlines()
        for line in lines:
            personal_info = line.strip().split(',')
            student_info.append((int(personal_info[0]), personal_info[1], personal_info[2], int(personal_info[3])))

    with open("info.bin", 'wb') as fout:
        fout.write(struct.pack('>2s4s10s3s', *table_top)) # bin format for tabletop
        write_records(student_info, '>i20s5si', fout) # bin format for data
    


if __name__ == "__main__":
    main()