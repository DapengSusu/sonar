#include <stdio.h>
#include "tcp.h"
#include "log.h"
#include "cmd.h"

int main()
{
    // TODO: 获取输入
    // fgets(cmd_buf, cmd, stdin);
    // 转换 cmd -> enum SonarSystemCmd

    struct SonarCommand cmd_startup = new_cmd(CMD_STARTUP_SONAR);

    log(INFO, "new cmd: %s\n", cmd_startup.cmd_buf);
    // int ret = start_tcp_client(
    //     "192.168.13.132",
    //     TCP_PORT,
    //     msg_startup
    // );

    int ret = start_tcp_client(
        NULL,
        TCP_PORT,
        cmd_startup
    );

    log(INFO, "Exit: {%d}\n", ret);

    return ret;
}
