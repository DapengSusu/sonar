#ifndef _TCP_H
#define _TCP_H

#include <sys/types.h>
#include "sonar.h"

// 端口
#define TCP_PORT 5007

// IP 地址
#define TCP_ADDRESS "192.168.1.100"

// buffer 最大允许字节数
#define MAX_TCP_BUF_SIZE 1024

// TCP 操作
enum TCPOperationType {
    SOCKET = 1,       // 创建 socket
    BIND = 2,         // 绑定端口 bind
    LISTEN = 3,       // 监听 listen
    ACCEPT = 4,       // 接收连接请求 accept
    CONNECT = 5,      // 发起连接 connect
    CLOSE = 6,        // 关闭连接 close
};
enum TCPError {
    TCP_ERROR_SOCKET = 11,       // 创建 socket fail
    TCP_ERROR_BIND = 12,         // 绑定端口 bind fail
    TCP_ERROR_LISTEN = 13,       // 监听 listen fail
    TCP_ERROR_ACCEPT = 14,       // 接收连接请求 accept fail
    TCP_ERROR_CONNECT = 15,      // 发起连接 connect fail
    TCP_ERROR_CLOSE = 16,        // 关闭连接 close fail
};
struct TCPOperation {
    enum TCPOperationType op_type; // 操作类型
    enum TCPError err_code;  // 错误码
};

// 启动 TCP 服务，等待连接
int start_tcp_server(
    const char *ip_addr,        // IP 地址: 必须是合法可解析的IPv4地址
    u_int16_t port,             // 端口: 避免冲突
    int max_connection,         // 最大支持连接数
    const size_t cmd_buf_size   // 命令 buffer size
);

// 启动 TCP 服务，等待连接
int start_tcp_client(
    const char *ip_addr,        // IP 地址: 必须是合法可解析的IPv4地址
    u_int16_t port,             // 端口: 避免冲突
    const size_t cmd_buf_size   // 命令 buffer size
);

int close_tcp_socket(int fd);

// 关闭服务端 socket
int close_tcp_server(int socket_fd, int connect_fd);
// 关闭客户端 socket
int close_tcp_client(int connect_fd);

#endif /* _TCP_H */
