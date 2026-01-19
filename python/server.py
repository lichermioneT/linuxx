import socket

# 1. 创建 UDP 套接字 (AF_INET 表示 IPv4, SOCK_DGRAM 表示 UDP)
server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# 2. 绑定 IP 地址和端口
address = ('127.0.0.1', 8888)
server_socket.bind(address)

print(f"UDP 服务端已启动，监听端口: {address[1]}...")

while True:
    # 3. 接收数据 (返回数据内容和客户端的地址)
    # 1024 表示本次接收的最大字节数
    data, client_addr = server_socket.recvfrom(1024)

    print(f"来自 {client_addr} 的消息: {data.decode('utf-8')}")

    # 4. 回复客户端（可选）
    response = "服务器已收到消息！"
    server_socket.sendto(response.encode('utf-8'), client_addr)
