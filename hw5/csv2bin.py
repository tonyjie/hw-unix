import struct

def write_records(records, format, f):
    record_struct = struct.Struct(format)
    for r in records:
        f.write(record_struct.pack(*r))

def main():
    student_info = []

    with open("info.csv", 'r') as fin:
        line = fin.readline()
        table_top_list = line.strip().split(',')
        for i in range(len(table_top_list)):
            table_top_list[i] = table_top_list[i].encode('utf-8')
        table_top = tuple(table_top_list)

        lines = fin.readlines()
        for line in lines:
            personal_info = line.strip().split(',')
            student_info.append((int(personal_info[0]), personal_info[1].encode('utf-8'), personal_info[2].encode('utf-8'), int(personal_info[3])))

    with open("info.bin", 'wb') as fout:
        fout.write(struct.pack('>2s4s10s3s', *table_top)) # bin format for tabletop
        # fout.write(struct.pack('>2s4s10s3s', table_top[0], table_top[1], table_top_list[2], table_top_list[3])) # bin format for tabletop
        write_records(student_info, '>i20s5si', fout) # bin format for data

if __name__ == "__main__":
    main()