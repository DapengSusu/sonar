#include <stdio.h>
#include <string.h>
#include "cmd.h"
#include "log.h"
#include "sonar.h"

// 指令头
// 固定3字节: 0x43, 0x4D, 0x44
// "CMD"的ASCII码
const char g_cmd_header[3] = { 0x43, 0x4D, 0x44 };

// 生成指令
// 指令头
// 固定3字节: 0x43, 0x4D, 0x44
// 指令头+指令号 固定4字节
struct SonarCommand new_cmd(const enum CmdID id)
{
    // char *cmd_content = NULL;
    struct SonarCommand cmd;

    switch (id) {
    // case xxx: {
    //     break;
    // }
    // 启动声呐系统
    case CMD_STARTUP_SONAR: {
        // 指令号 0x06
        // 指令内容 1字节
        // 0x01 - 启动声呐
        cmd.size = 5;
        cmd.cmd_buf = (char *) malloc(cmd.size * sizeof(char));

        if (NULL != cmd.cmd_buf) {
            memset(cmd.cmd_buf, 0, cmd.size);
            // 添加指令头
            memcpy(cmd.cmd_buf, g_cmd_header, 3);
            // 添加指令号和指令内容
            char buf[] = { 0x06, 0x01 };
            memcpy(cmd.cmd_buf + 3, buf, sizeof(buf));
            // print_cmd(cmd.cmd_buf, cmd.size);
            log(DEBUG, "generate cmd: %s\n", cmd.cmd_buf);
        }
        break;
    }
    // 关闭声呐系统
    case CMD_SHUTDOWN_SONAR: {
        // 指令号 0x06
        // 指令内容 1字节
        // 0x00 - 关闭声呐
        cmd.size = 5;
        cmd.cmd_buf = (char *) malloc(cmd.size * sizeof(char));

        if (NULL != cmd.cmd_buf) {
            memset(cmd.cmd_buf, 0, cmd.size);
            // 添加指令头
            memcpy(cmd.cmd_buf, g_cmd_header, 3);
            // 添加指令号和指令内容
            char buf[] = { 0x06, 0x00 };
            memcpy(cmd.cmd_buf + 3, buf, sizeof(buf));
            // print_cmd(cmd.cmd_buf, cmd.size);
            log(DEBUG, "generate cmd: %s\n", cmd.cmd_buf);
        }
        break;
    }
    default: {
        break;
    }
    }
    return cmd;
}

// 指令有效性检验
// TODO
Bool cmd_check(const struct SonarCommand *cmd)
{
    return FALSE;
}

// 显示指令
void print_cmd(char* cmd_buf, size_t length)
{
    if (NULL != cmd_buf) {
        for (size_t i = 0; i != length; ++i) {
            printf("%#X ", cmd_buf[i]);
        }
        printf("\n");
    }
}

// 指令释放
void cmd_destroy(struct SonarCommand *cmd)
{
    if (NULL != cmd->cmd_buf) {
        log(DEBUG, "destroyed space size: [%lu]\n", cmd->size);

        free(cmd->cmd_buf);
        cmd->cmd_buf = NULL;
        cmd->size = 0;
    }
}
