#ifndef _SONAR_H
#define _SONAR_H

// buffer 最大允许字节数
#define MAX_BUF_SIZE 1024
// buffer 最小允许字节数
#define MIN_BUF_SIZE 18

enum Result {
    TRUE = 1,
    FALSE = 0,
};
typedef enum Result Bool;

// 传输协议
enum TransportProtocol {
    TCP = 0,
    UDP = 1,
};

#endif /* _SONAR_H */
