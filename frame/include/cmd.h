#ifndef _CMD_H
#define _CMD_H

#include <stdlib.h>
#include "sonar.h"

// 内部使用
// 启用内部指令
// 仅供内部调试
#define USE_INSIDE_CMD  1

// 指令结构体
struct SonarCommand {
    char *cmd_buf;           // 指令 buffer
    size_t size;             // buffer size
};

enum CmdID {
    // 通用指令
    // ...
    CMD_STARTUP_SONAR,       // 启动声呐系统
    CMD_SHUTDOWN_SONAR,      // 启动声呐系统

    // 内部指令
#if USE_INSIDE_CMD
    // ...
#endif
};

// 生成指令
struct SonarCommand new_cmd(const enum CmdID id);

// 指令有效性检验
Bool cmd_check(const struct SonarCommand *cmd);

// 显示指令
void print_cmd(char* cmd_buf, size_t length);

// 指令释放
void cmd_destroy(struct SonarCommand *cmd);

#endif /* _CMD_H */
