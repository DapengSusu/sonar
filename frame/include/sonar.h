#ifndef _SONAR_H
#define _SONAR_H

#include <stddef.h>
#include <stdio.h>

// buffer 最大允许字节数
#define MAX_BUF_SIZE 1024
// buffer 最小允许字节数
#define MIN_BUF_SIZE 6

// true or false
#define TRUE 1
#define FALSE 0

// 传输协议
enum TransportProtocol {
    TCP = 0,
    UDP = 1,
};

// LOG 等级控制
enum LogLevel {
    LOG_OFF = 0,
    LOG_ERROR = 1,
    LOG_WARNING = 2,
    LOG_INFO = 3,
    LOG_DEBUG = 4,
};

// 控制 log 输出等级
static int log_level = LOG_INFO;
 
#define log(level, fmt, ...) \
do { \
    if (LOG_##level <= log_level) { \
        printf("[" #level "] " fmt, ##__VA_ARGS__); \
    } \
} while(FALSE)

// 系统错误
enum SonarError {
    SONAR_OK = 0,
    SONAR_ERROR_NULL_POINTER = 21,       // 空指针错误
    SONAR_ERROR_IO = 22,                 // IO 错误
    SONAR_ERROR_CMD_NONEXIST = 23,       // 不存在的指令
    SONAR_ERROR_OVERFLOW = 24,           // 溢出
    SONAR_ERROR_PARSE_INT = 25,          // 数值解析错误
    // ...
    SONAR_ERROR_UNKNOWN = 100,           // 未知错误
};

// 声纳系统指令
enum SonarSystemCmd {
    CMD_SET_SYNC_MODE = 0x01,            // 设置同步模式
    CMD_SET_DISTANCE_RANGE = 0x03,       // 设置距离量程
    // ...
    CMD_STARTUP_SONAR = 0x06,            // 启动声纳系统
    // ...
};

// 用于传输的消息
struct Message {
    enum SonarSystemCmd cmd_request;     // 请求的指令
    char msg_buf[MIN_BUF_SIZE];          // 消息 buffer
    const size_t buf_size;               // buffer size
};

#endif /* _SONAR_H */
