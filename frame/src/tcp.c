#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../../frame/include/tcp.h"
#include "../../frame/include/sonar.h"

int start_tcp_server(
    const char *ip_addr,
    u_int16_t port,
    int max_connection,
    const size_t cmd_buf_size
) {
	char cmd_buf[cmd_buf_size];
    // 创建监听套接字
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == socket_fd) {
        perror("create socket failed");
        return TCP_ERROR_SOCKET;
    }

    // 确定 IP 地址和端口
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET; // IPv4
	addr.sin_port = htons(port);
	// addr.sin_addr.s_addr = htonl(inet_addr(ip_addr));
    inet_aton(ip_addr, &addr.sin_addr);
    // 绑定
	int ret = bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr));
	if (-1 == ret) {
		perror("bind port failed");
		return TCP_ERROR_BIND;
	}

    // 转换为被动套接字
    if (max_connection > SOMAXCONN) {
        max_connection = SOMAXCONN;
    }
	ret = listen(socket_fd, max_connection);
	if (-1 == ret) {
		perror("listen failed");
		return TCP_ERROR_LISTEN;
	}
	printf("Waiting for the connection...\n");

    while (TRUE) {
        // 获取连接请求并建立连接
        int connect_fd = accept(socket_fd, (struct sockaddr*)(NULL), NULL);
        if (-1 == connect_fd) {
            perror("connect socket failed");
            return TCP_ERROR_ACCEPT;
        }
        printf("Connected with Client: id[%d]\n", connect_fd);

        memset(cmd_buf, 0, cmd_buf_size);
        int ret = read(connect_fd, cmd_buf, cmd_buf_size);
        if (-1 == ret) {
            perror("read error");
            return Sonar_ERROR_IO;
        }
        if (0 == ret) {
            break;
        }
        // 命令解析
        // TODO
        unsigned long length = strlen(cmd_buf);
        cmd_buf[length - 1] = '\0';
        int cmd = atoi(cmd_buf);
        if (0 == cmd) {
            return Sonar_ERROR_PARSE_INT;
        }
        printf("received cmd: %s, op: %d\n", cmd_buf, cmd);

        // 执行指令
        // parse_cmd(int cmd)
        // TODO
        // 执行结果回传
        // 生成执行结果对应指令
        int result = Sonar_OK;
        // ...
        // result = generate_cmd(cmd_buf, cmd_buf_size, result);
        if (Sonar_OK == result) {
            write(connect_fd, cmd_buf, ret);
            close_tcp_socket(connect_fd);
            // 控制服务端接收指令后是否退出
            break;
        }

        close_tcp_socket(connect_fd);
    }
    close_tcp_socket(socket_fd);
    
    return Sonar_OK;
}

int start_tcp_client(const char *ip_addr, u_int16_t port, const size_t cmd_buf_size)
{
    // 创建监听套接字
	int connect_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == connect_fd) {
        perror("create socket failed");
        return TCP_ERROR_SOCKET;
    }

    // 确定 IP 地址和端口
    struct sockaddr_in addr;
	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET; // IPv4
	addr.sin_port = htons(port);
	// addr.sin_addr.s_addr = htonl(inet_addr(ip_addr));
    inet_aton(ip_addr, &addr.sin_addr);

    // 与服务端进行连接
    int ret = connect(connect_fd, (struct sockaddr*)&addr, sizeof(addr));
    if (-1 == ret) {
		perror("connect failed");
		return TCP_ERROR_CONNECT;
	}
    printf("Connect to Server\n");

    // 获取输入指令
    char buf[cmd_buf_size];
    memset(buf, 0, cmd_buf_size);
    fgets(buf, cmd_buf_size, stdin);
    // 发送指令
    write(connect_fd, buf, strlen(buf));
    memset(buf, 0, cmd_buf_size);
    // 接收服务端回传结果
    ret = read(connect_fd, buf, cmd_buf_size);
    if (ret == 0) { // 表示连接断开
        close(connect_fd);
    }
    printf("get result: %s\n", buf);
    // memset(buf, 0, cmd_buf_size);
    close_tcp_client(connect_fd);

    return Sonar_OK;
}

int close_tcp_socket(int fd)
{
    int ret = close(fd);
    if (-1 == ret) {
        perror("close socket failed");
        return TCP_ERROR_CLOSE;
    }
	printf("Closed: fd[%d]\n", fd);

    return Sonar_OK;
}

int close_tcp_server(int socket_fd, int connect_fd)
{
    int ret = close_tcp_socket(socket_fd);
    ret = close_tcp_socket(connect_fd);

    return ret;
}

int close_tcp_client(int connect_fd)
{
    return close_tcp_socket(connect_fd);
}
