#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../frame/include/tcp.h"

int main()
{
    // TODO: 获取输入
    // fgets(cmd_buf, cmd, stdin);
    // 转换 cmd -> enum SonarSystemCmd

    struct Message msg_startup = {
        CMD_STARTUP_SONAR,
        "",
        MIN_BUF_SIZE
    };
    // int ret = start_tcp_client(
    //     "192.168.13.132",
    //     TCP_PORT,
    //     msg_startup
    // );

    int ret = start_tcp_client(
        NULL,
        TCP_PORT,
        msg_startup
    );

    log(INFO, "Exit: {%d}\n", ret);

    return ret;
}
