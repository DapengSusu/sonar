#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../frame/include/tcp.h"

int main()
{
    const size_t cmd_buf_size = 24;

    int ret = start_tcp_client("192.168.13.132", TCP_PORT, cmd_buf_size);
    if (Sonar_OK == ret) {
        printf("OK\n");
    } else {
        printf("Error: {%d}\n", ret);
		return ret;
    }

    return 0;
}
