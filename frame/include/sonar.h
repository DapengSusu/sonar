#ifndef _SONAR_H
#define _SONAR_H

// true or false
#define TRUE 1
#define FALSE 0

// 传输协议
enum TransportProtocol {
    TCP = 0,
    UDP = 1,
};

// 系统错误码
enum SonarError {
    Sonar_OK = 0,
    Sonar_ERROR_NULL_POINTER = 21,  // 空指针错误
    Sonar_ERROR_IO = 22,            // IO 错误
    Sonar_ERROR_CMD_NONEXIST = 23,  // 不存在的指令
    Sonar_ERROR_OVERFLOW = 24,      // 溢出
    Sonar_ERROR_PARSE_INT = 25,     // 数值解析错误
    // ...
    Sonar_ERROR_UNKNOWN = 100,      // 未知错误
};

// 声纳系统指令
enum SonarSystemCmd {
    CMD_SET_SYNC_MODE = 0x01,            // 设置同步模式
    CMD_SET_DISTANCE_RANGE = 0x03,       // 设置距离量程
    // ...
    CMD_STARTUP_SONAR = 0x06,            // 启动声纳系统
    // ...
};

#endif /* _SONAR_H */
