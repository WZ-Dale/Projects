import socket
import time
import os
import sys
import struct
 
 
def sock_client():
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)       # 创建TCP套接字
        s.connect(('192.168.137.1', 6666))                          # 发起连接
    except socket.error as msg:
        print(msg)
        print(sys.exit(1))
 
    while True:
        #filepath = input('input the file: ')
        #os.system("fswebcam -S 10 image2.jpg")                      # 将字符串转化成命令行，并运行
        #time.sleep(1)
        filepath = 'image2.jpg'
        fhead = struct.pack(b'128sl', bytes(os.path.basename(filepath).encode('utf-8')), os.stat(filepath).st_size)
        s.send(fhead)       # 通过struct库进行打包，然后再用套接字来发送头信息（文件名和文件大小）
        print('client filepath: {0}'.format(filepath))
        
        # 开始发送数据  
        fp = open(filepath, 'rb')
        while 1:
            data = fp.read(1024)
            if not data:
                print('{0} file send over...'.format(filepath))
                break
            s.send(data)
        s.close()
        break
 
 
if __name__ == '__main__':
    sock_client()
