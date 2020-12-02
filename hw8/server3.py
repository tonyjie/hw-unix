import socket
import struct

def main():
    serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    serversocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, True)
    host = socket.gethostname()
    port = 50003
    serversocket.bind((host, port))
    serversocket.listen(5)
    clientsocket, addr = serversocket.accept()

    while True:
        recv_content = clientsocket.recv(1024).decode('utf-8')
        content_list = recv_content.split('+')
        index = content_list[0]
        line = content_list[1]

        if (index == '0'): # tabletop
            table_top_list = line.strip().split(',')
            for i in range(len(table_top_list)):
                table_top_list[i] = table_top_list[i].encode('utf-8')
            table_top = tuple(table_top_list)
            s = struct.pack('>2s4s10s3s', *table_top)
        else: # data
            personal_info = line.strip().split(',')
            personal_info_tuple = (int(personal_info[0]), personal_info[1].encode('utf-8'), personal_info[2].encode('utf-8'), int(personal_info[3]))
            s = struct.pack('>i20s5si', *personal_info_tuple)

        print(s)
        clientsocket.send(s)
        
        if (len(content_list) == 3):
            if (content_list[2] == '\n'):
                break
            
    clientsocket.close()
    

if __name__ == "__main__":
    main()