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
        lines = fin.readlines()
        line_num = len(lines)
        for line in lines:
            send_msg = str(idx) + '+' + line
            if (idx >= line_num - server_num): # if this is the last line for this server, set a termination symbol
                send_msg = send_msg + '+' + '\n'
            server_list[idx % server_num].send(send_msg.encode('utf-8'))
            idx += 1
        
    with open("info.bin", 'wb') as fout:
        for i in range(line_num):
            if (i == 0):
                fout.write(server_list[i % server_num].recv(19))
            else:
                fout.write(server_list[i % server_num].recv(33))

    for i in range(server_num):
        server_list[i].close()

if __name__ == "__main__":
    main()