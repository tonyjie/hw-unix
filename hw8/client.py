import socket
import struct

def main():
    host = socket.gethostname()
    port_list = [50001, 50002, 50003]
    server_num = len(port_list)
    server_list = []
    for i in range(server_num):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((host, port_list[i]))
        server_list.append(s)

    with open("info.csv", 'r') as fin:
        idx = 0
        for line in fin.readlines():
            send_msg = str(idx) + '+' + line
            server_list[idx % server_num].send(send_msg.encode('utf-8'))
            idx += 1
    
    # with open("info.bin", 'wb') as fout:
    #     for i in range(idx):
            


    


if __name__ == "__main__":
    main()