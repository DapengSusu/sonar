#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../frame/include/tcp.h"

int main()
{
    struct Message msg_startup = {
        CMD_STARTUP_SONAR,
        "",
        MIN_BUF_SIZE
    };
    int ret = start_tcp_client(
        "192.168.13.132",
        TCP_PORT,
        msg_startup
    );

    log(INFO, "Exit: {%d}\n", ret);

    return ret;
}
