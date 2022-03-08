#ifndef _ERROR_H
#define _ERROR_H

// TCP 操作错误
enum TCPError {
    TCP_ERROR_SOCKET = 11,              // 创建 socket fail
    TCP_ERROR_BIND = 12,                // 绑定端口 bind fail
    TCP_ERROR_LISTEN = 13,              // 监听 listen fail
    TCP_ERROR_ACCEPT = 14,              // 接收连接请求 accept fail
    TCP_ERROR_CONNECT = 15,             // 发起连接 connect fail
    TCP_ERROR_CLOSE = 16,               // 关闭连接 close fail
};

// 系统错误
enum SonarError {
    SONAR_OK = 0,
    SONAR_ERROR_NULL_POINTER = 21,       // 空指针错误
    SONAR_ERROR_IO = 22,                 // IO 错误
    SONAR_ERROR_CMD_NONEXIST = 23,       // 不存在的指令
    SONAR_ERROR_OVERFLOW = 24,           // 溢出
    SONAR_ERROR_PARSE_INT = 25,          // 数值解析错误
    SONAR_ERROR_INVALID_CMD = 26,        // 无效命令
    // ...
    SONAR_ERROR_UNKNOWN = 100,           // 未知错误
};

#endif /* _ERROR_H */
