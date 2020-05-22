# -*- coding: UTF-8 -*-
#from imageai.Prediction import ImagePrediction
import socket
import os
import sys
import struct
import time
 
 
def socket_service():
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)       # 创建TCP套接字
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)     # 设置套接字选项值
        s.bind(('192.168.137.1', 6666))                             # 绑定ip和端口
        s.listen(10)                                                # 开始监听（最多10个套接字）
    except socket.error as msg:
        print(msg)
        sys.exit(1)
 
    print("Wait")
 
    while True:
        sock, addr = s.accept()                                     # 套接字接受一个连接，返回一个新的套接字和源地址
        deal_data(sock, addr)                                       # 处理数据
        break
    s.close()
 
 
def deal_data(sock, addr):
    print("Accept connection from {0}".format(addr))
 
    while True:
        fileinfo_size = struct.calcsize('128sl')                    # 计算格式字符串所对应的结果的长度
        buf = sock.recv(fileinfo_size)                              # 接受该长度数据到buffer
        if buf:
            filename, filesize = struct.unpack('128sl', buf)        # 接收文件名，文件大小
            fn = filename.decode().strip('\x00')                    # 字节流转str，并去除不可见字符\r,\\n,\t
            new_filename = os.path.join('./', 'new_' + fn)          # 连接路径名组件
 
            recvd_size = 0
            fp = open(new_filename, 'wb')
 
            while not recvd_size == filesize:
                if filesize - recvd_size > 1024:                    # 不是最后一块数据
                    data = sock.recv(1024)
                    recvd_size += len(data)
                else:                                               # 是最后一块数据
                    data = sock.recv(1024)
                    recvd_size = filesize
                fp.write(data)
            fp.close()
        sock.close()
    
        break
 
 
if __name__ == '__main__':
    socket_service()
    