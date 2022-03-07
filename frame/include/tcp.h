#ifndef _TCP_H
#define _TCP_H

#include <sys/types.h>
#include "sonar.h"

// 端口
#define TCP_PORT 5007

// IP 地址
#define TCP_ADDRESS "192.168.1.100"

// TCP 操作错误
enum TCPError {
    TCP_ERROR_SOCKET = 11,       // 创建 socket fail
    TCP_ERROR_BIND = 12,         // 绑定端口 bind fail
    TCP_ERROR_LISTEN = 13,       // 监听 listen fail
    TCP_ERROR_ACCEPT = 14,       // 接收连接请求 accept fail
    TCP_ERROR_CONNECT = 15,      // 发起连接 connect fail
    TCP_ERROR_CLOSE = 16,        // 关闭连接 close fail
};

// 启动声纳
// TODO
// int startup_sonar();

// 解析指令
// TODO
int parse_execute(int cmd);

// 生成命令
// TODO
int generate_msg(char *buf, size_t size, int code);

// 启动 TCP 服务，等待连接
int start_tcp_server(
    const char *ip_addr,        // IP 地址: 必须是合法可解析的IPv4地址
    const u_int16_t port,       // 端口: 避免冲突
    int max_connection,         // 最大支持连接数
    const size_t buf_size       // 命令 buffer size
);

// 启动 TCP 客户端，请求连接
int start_tcp_client(
    const char *ip_addr,        // IP 地址: 必须是合法可解析的IPv4地址
    const u_int16_t port,       // 端口: 避免冲突
    struct Message message      // 传输消息
);

// 关闭 socket
int close_tcp_socket(int fd);

#endif /* _TCP_H */
