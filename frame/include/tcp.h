#ifndef _TCP_H
#define _TCP_H

#include <sys/types.h>
#include "cmd.h"

// 默认端口
#define TCP_PORT 5007

// 默认 IP 地址
// #define TCP_ADDRESS "192.168.1.100"

// 启动声纳
// TODO
// int startup_sonar();

// 解析并执行指令
// TODO
int parse_execute(char *cmd);

// 依据指令执行结果生成回传消息
// TODO
void generate_returned_msg(char *buf, size_t size, int code);

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
    struct SonarCommand cmd     // 请求指令
);

// 关闭 socket
int close_tcp_socket(int fd);

#endif /* _TCP_H */
