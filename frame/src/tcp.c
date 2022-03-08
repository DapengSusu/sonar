#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../../frame/include/tcp.h"
#include "../../frame/include/sonar.h"

// 解析并执行指令
// TODO
int parse_execute(int cmd)
{
    int ret = SONAR_OK;

    switch (cmd) {
    case CMD_SET_SYNC_MODE:
        break;
    case CMD_SET_DISTANCE_RANGE:
        break;
    case CMD_STARTUP_SONAR: {
        // 启动声纳系统
        // ...
        // ret = startup_sonar();
        log(INFO, "Start up!\n");
        break;
    }
    // ...
    default:
        return SONAR_ERROR_CMD_NONEXIST;
        break; 
    }
    return ret;
}

// 生成执行结果消息
// TODO
int generate_msg(char *buf, size_t size, int code)
{
    if (NULL == buf) {
        return SONAR_ERROR_NULL_POINTER;
    }
    if (size < MIN_BUF_SIZE) {
        // 查询指令 6 字节
        // 其余指令返回 3 字节状态码
        // 执行成功返回 0x4F 0x4B 0x0a（三字节即ASCII的“OK\n”）
        // 执行失败返回 0x45 0x52 0x52（三字节即ASCII的“ERR”）
        return SONAR_ERROR_OVERFLOW;
    }
    switch (code) {
    case SONAR_OK: {
        char res_ok[3] = { 0x4F, 0x4B, 0x0A };
        memcpy(buf, res_ok, 3);
        break;
    }
    // TODO
    // ...
    default: {
        char res_err[3] = { 0x45, 0x52, 0x52 };
        memcpy(buf, res_err, 3);
        break;
    }
    }
    return SONAR_OK; // TODO: 返回固定错误码不严谨
}

int start_tcp_server(
    const char *ip_addr,        // IP 地址: 必须是合法可解析的IPv4地址
    const u_int16_t port,       // 端口: 避免冲突
    int max_connection,         // 最大支持连接数
    const size_t buf_size       // 命令 buffer size
) {
	char cmd_buf[buf_size];
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
    // 判断是动态获取 IP 还是使用固定 IP
    if (NULL == ip_addr) {
        // 动态获取 IP
	    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    } else {
        // 使用静态 IP
        inet_aton(ip_addr, &addr.sin_addr);
    }

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
    log(INFO, "Waiting for the connection...\n");

    while (TRUE) {
        // 获取连接请求并建立连接
        int connect_fd = accept(socket_fd, (struct sockaddr*)(NULL), NULL);
        if (-1 == connect_fd) {
            perror("connect socket failed");
            return TCP_ERROR_ACCEPT;
        }
        log(INFO, "Connected with Client: fd[%d]\n", connect_fd);

        memset(cmd_buf, 0, buf_size);
        int ret = read(connect_fd, cmd_buf, buf_size);
        if (-1 == ret) {
            perror("read error");
            return SONAR_ERROR_IO;
        }
        // if (0 == ret) {
        //     break;
        // }

        // 命令解析
        // TODO
        unsigned long length = strlen(cmd_buf);
        log(DEBUG, "buf: %s, len: %lu\n", cmd_buf, length);

        int cmd = atoi(cmd_buf);
        if (0 == cmd) {
            return SONAR_ERROR_PARSE_INT;
        }
        log(INFO, "received cmd: %s, op: %d\n", cmd_buf, cmd);

        // 执行
        // TODO
        int exec_result = parse_execute(cmd);
        if (SONAR_OK != exec_result) {
            // failed
            // ...
        }
        // TODO
        // 执行结果回传
        // 生成执行结果反馈消息
        memset(cmd_buf, 0, buf_size);
        int result = generate_msg(cmd_buf, buf_size, exec_result);
        write(connect_fd, cmd_buf, ret);
        log(INFO, "The message returned.\n");
        // 控制服务端接收指令后是否退出
        // close_tcp_socket(connect_fd);
        // break;
        // TODO: 服务端收到关机指令才能断开连接
        close_tcp_socket(connect_fd);
    }
    // TODO: 服务端收到关机指令才能断开连接
    close_tcp_socket(socket_fd);
    
    return SONAR_OK;
}

int start_tcp_client(
    const char *ip_addr,        // IP 地址: 必须是合法可解析的IPv4地址
    const u_int16_t port,       // 端口: 避免冲突
    struct Message message      // 传输消息
) {
    // 参数检查
    if (message.buf_size < MIN_BUF_SIZE
        || message.buf_size > MAX_BUF_SIZE) {
            log(ERROR, "Buffer size [%lu] is inappropriate.\n", message.buf_size);
            return SONAR_ERROR_OVERFLOW;
    }

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
    if (NULL == ip_addr) {
        // 动态获取 IP
	    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    } else {
        // 使用静态 IP
        inet_aton(ip_addr, &addr.sin_addr);
    }

    // 与服务端进行连接
    int ret = connect(connect_fd, (struct sockaddr*)&addr, sizeof(addr));
    if (-1 == ret) {
		perror("connect failed");
		return TCP_ERROR_CONNECT;
	}
    log(INFO, "Connect to Server\n");

    // 处理输入指令
    memset(message.msg_buf, 0, message.buf_size);
    sprintf(message.msg_buf, "%X", message.cmd_request);
    log(DEBUG, "buf: %s\n", message.msg_buf);

    // 发送指令
    ret = write(connect_fd, message.msg_buf, message.buf_size);
    if (ret < 0) {
        return SONAR_ERROR_IO;
    }
    log(INFO, "Send cmd msg: %s\n", message.msg_buf);

    memset(message.msg_buf, 0, message.buf_size);
    // 接收服务端回传结果
    ret = read(connect_fd, message.msg_buf, message.buf_size);
    // if (ret == 0) { // 表示连接断开
    //     close(connect_fd);
    // }
    log(INFO, "Get returned msg: %s\n", message.msg_buf);

    // 确认回传信息无误
    // TODO
    // 指令执行结果?
    // ...

    close_tcp_socket(connect_fd);

    return SONAR_OK;
}

int close_tcp_socket(int fd)
{
    int ret = close(fd);
    if (-1 == ret) {
        perror("close socket failed");
        return TCP_ERROR_CLOSE;
    }
    log(INFO, "Closed: fd[%d]\n", fd);

    return SONAR_OK;
}
