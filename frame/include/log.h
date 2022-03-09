#ifndef _LOG_LEVEL_H
#define _LOG_LEVEL_H

// 调试模式控制（正式版本请关闭）
#define DEBUG_MODE 1

// LOG 等级控制
enum LogLevel {
    LOG_OFF = 0,
    LOG_ERROR = 1,
    LOG_WARNING = 2,
    LOG_INFO = 3,
    LOG_DEBUG = 4,
};

// 控制 log 输出等级
// 默认 INFO
#if DEBUG_MODE
static int log_level = LOG_DEBUG;
#else
static int log_level = LOG_INFO;
#endif
 
#define log(level, fmt, ...) \
do { \
    if (LOG_##level <= log_level) { \
        printf("[" #level "] " fmt, ##__VA_ARGS__); \
    } \
} while(0)

#endif /* _LOG_LEVEL_H */
