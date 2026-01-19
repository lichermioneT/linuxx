import socket

# 1. 创建 UDP 套接字
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# 定义服务端的地址
server_address = ('127.0.0.1', 8888)

try:
    # 2. 发送数据
    message = "你好，我是客户端！"
    print(f"正在发送: {message}")
    client_socket.sendto(message.encode('utf-8'), server_address)

    # 3. 接收服务端的回应
    data, server = client_socket.recvfrom(1024)
    print(f"来自服务端的回复: {data.decode('utf-8')}")

finally:
    # 4. 关闭套接字
    print("关闭客户端。")
    client_socket.close()
