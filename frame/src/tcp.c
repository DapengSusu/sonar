#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "cmd.h"
#include "tcp.h"
#include "sonar.h"
#include "log.h"
#include "error.h"

// 解析并执行指令
// TODO
int parse_execute(char *cmd)
{
    int ret = SONAR_OK;
    // check 指令有效性（指令头3字节）
    // 根据指令号确定指令功能（指令号1字节）
    // 确定指令内容（从第5字节开始）

    return ret;
}

// 生成执行结果消息
// TODO
void generate_returned_msg(char *buf, size_t size, int code)
{
    // 查询指令 6 字节
    // 其余指令返回 3 字节状态码
    // 执行成功返回 0x4F 0x4B 0x0a（三字节即ASCII的“OK\n”）
    // 执行失败返回 0x45 0x52 0x52（三字节即ASCII的“ERR”）

    char res_ok[3] = { 0x4F, 0x4B, 0x0A };
    char res_err[3] = { 0x45, 0x52, 0x52 };

    switch (code) {
    case SONAR_OK: {
        memcpy(buf, res_ok, 3);
        break;
    }
    // TODO
    // ...
    default: {
        memcpy(buf, res_err, 3);
        break;
    }
    }
}

int start_tcp_server(
    const char *ip_addr,        // IP 地址: 必须是合法可解析的IPv4地址
    const u_int16_t port,       // 端口: 避免冲突
    int max_connection,         // 最大支持连接数
    const size_t buf_size       // 命令 buffer size
) {
	char cmd_buf[buf_size];
    int ret = SONAR_OK;

    // 创建监听套接字
    int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (-1 == socket_fd) {
        perror("create socket failed");
        return TCP_ERROR_SOCKET;
    }

    do {
        // 确定 IP 地址和端口
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET; // IPv4
        addr.sin_port = htons(port);
        // 判断是动态获取 IP 还是使用固定 IP
        if (NULL == ip_addr) {
            // 动态获取 IP
            addr.sin_addr.s_addr = htonl(INADDR_ANY);
            log(DEBUG, "Dynamic IP\n");
        } else {
            // 使用静态 IP
            inet_aton(ip_addr, &addr.sin_addr);
            log(DEBUG, "Static IP\n");
        }

        // 绑定
        ret = bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr));
        if (-1 == ret) {
            perror("bind port failed");
            ret = TCP_ERROR_BIND;
            // 关闭 socket
            continue;
        }

        // 转换为被动套接字
        if (max_connection > SOMAXCONN) {
            max_connection = SOMAXCONN;
        }
        ret = listen(socket_fd, max_connection);
        if (-1 == ret) {
            perror("listen failed");
            ret =  TCP_ERROR_LISTEN;
            continue;
        }
        log(INFO, "Waiting for the connection...\n");

        while (TRUE) {
            // 获取连接请求并建立连接
            int connect_fd = accept(socket_fd, (struct sockaddr*)(NULL), NULL);
            if (-1 == connect_fd) {
                perror("connect socket failed");
                ret = TCP_ERROR_ACCEPT;
                break;
            }
            log(INFO, "Connected with Client: fd[%d]\n", connect_fd);

            memset(cmd_buf, 0, buf_size);
            ret = read(connect_fd, cmd_buf, buf_size);
            if (-1 == ret) {
                perror("read error");
                ret = SONAR_ERROR_IO;
                break;
            }
            // if (0 == ret) {
            //     break;
            // }

            // 命令解析
            // TODO
            Bool is_end = TRUE;
            unsigned long length = strlen(cmd_buf);
            log(DEBUG, "Received: %s, len: %lu, ret: %d\n", cmd_buf, length, ret);
            printf("Received cmd:[%d] ", ret);
            print_cmd(cmd_buf, length);

            // 执行
            // TODO
            int exec_result = parse_execute(cmd_buf);
            if (SONAR_OK != exec_result) {
                // failed ?
                // ...
            }
            // TODO
            // 执行结果回传
            // 生成执行结果反馈消息
            memset(cmd_buf, 0, buf_size);
            generate_returned_msg(cmd_buf, buf_size, exec_result);
            ret = write(connect_fd, cmd_buf, buf_size);
            if (-1 == ret) {
                perror("read error");
                ret = SONAR_ERROR_IO;
                break;
            }
            ret = SONAR_OK;
            log(INFO, "The message returned.\n");
            // 控制服务端接收指令后是否退出
#if DEBUG_MODE
            if (TRUE) {
                // TODO
                // debug 模式下可直接退出
                // 正常情况服务端需要收到退出指令才能退出
                close_tcp_socket(connect_fd);
                break;
            }
#endif
            // TODO: 服务端收到关机指令才能断开连接
            close_tcp_socket(connect_fd);
        }
    } while (FALSE);
    
    // TODO: 服务端收到关机指令才能断开连接
    close_tcp_socket(socket_fd);
    
    return ret;
}

int start_tcp_client(
    const char *ip_addr,        // IP 地址: 必须是合法可解析的IPv4地址
    const u_int16_t port,       // 端口: 避免冲突
    struct SonarCommand cmd     // 请求指令
) {
    // 参数检查
    if (NULL == cmd.cmd_buf) {
        log(ERROR, "Buffer has no space\n");
        return SONAR_ERROR_NULL_POINTER;
    } else if (cmd.size <= 4) {
        log(ERROR, "Invalid cmd %s\n", cmd.cmd_buf);
        return SONAR_ERROR_INVALID_CMD;
    }

    int ret = SONAR_OK;

    // 创建监听套接字
	int connect_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (-1 == connect_fd) {
        perror("create socket failed");
        return TCP_ERROR_SOCKET;
    }

    do {
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
        ret = connect(connect_fd, (struct sockaddr*)&addr, sizeof(addr));
        if (-1 == ret) {
            perror("connect failed");
            ret = TCP_ERROR_CONNECT;
            continue;
        }
        log(INFO, "Connect to Server\n");

        // 发送指令
        ret = write(connect_fd, cmd.cmd_buf, cmd.size);
        if (-1 == ret) {
            perror("write cmd error");
            ret = SONAR_ERROR_IO;
            continue;
        }
        log(DEBUG, "Send cmd: %s, len: %lu, ret: %d\n", cmd.cmd_buf, cmd.size, ret);
        printf("Send cmd:[%lu] ", cmd.size);
        print_cmd(cmd.cmd_buf, cmd.size);

        memset(cmd.cmd_buf, 0, cmd.size);
        // 接收服务端回传结果
        ret = read(connect_fd, cmd.cmd_buf, cmd.size);
        if (-1 == ret) {
            perror("read returned message fail");
            ret = SONAR_ERROR_IO;
            continue;
        }
        ret = SONAR_OK;
        log(INFO, "Get returned msg: %s\n", cmd.cmd_buf);

        // 确认回传信息无误
        // TODO
        // 指令执行结果?
        // ...
    } while (FALSE);

    close_tcp_socket(connect_fd);

    return ret;
}

int close_tcp_socket(int fd)
{
    int ret = SONAR_OK;
    if (-1 == fd) {
        log(WARNING, "Invalid fd: [%d]\n", fd);
        ret = SONAR_ERROR_UNKNOWN;
        return ret;
    }
    ret = close(fd);
    if (-1 == ret) {
        perror("close socket failed");
        return TCP_ERROR_CLOSE;
    }
    log(INFO, "Closed: fd[%d]\n", fd);

    return ret;
}
