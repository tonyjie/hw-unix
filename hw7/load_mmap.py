import os
import mmap

def memory_map(filename, access=mmap.ACCESS_WRITE):
    size = os.path.getsize(filename)
    fd = os.open(filename, os.O_RDWR)
    return mmap.mmap(fd, size, access=access)

def main():
    with memory_map("info.bin") as m:
        file_length = len(m)

        print("%s,%s,%s,%s" % (m[0:2].decode('utf-8'), m[2:6].decode('utf-8'), m[6:16].decode('utf-8'), m[16:19].decode('utf-8')))
        seek = 19

        while (seek < file_length):
            print("%d,%s,%s,%d" % (int.from_bytes(m[seek:seek+4], byteorder='big'), m[seek+4:seek+24].decode('utf-8'), m[seek+24:seek+29].decode('utf-8'), int.from_bytes(m[seek+29:seek+33], byteorder='big')))
            seek += 33

if __name__ == "__main__":
    main()