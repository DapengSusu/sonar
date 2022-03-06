#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include "../frame/include/tcp.h"

// 执行指令
// TODO
int parse_cmd(int cmd)
{
    switch (cmd) {
    case CMD_SET_SYNC_MODE:
        break;
    case CMD_SET_DISTANCE_RANGE:
        break;
    case CMD_STARTUP_SONAR: {
        break;
    }
    // ...
    default:
        return Sonar_ERROR_CMD_NONEXIST;
        break; 
    }
    return Sonar_OK;
}

// 生成命令
int generate_cmd(char *buf, size_t size, int code)
{
    if (NULL == buf) {
        return Sonar_ERROR_NULL_POINTER;
    }
    if (size < 6) {
        // 查询指令 6 字节
        // 其余指令返回 3 字节状态码
        // 执行成功返回 0x4F 0x4B 0x0a（三字节即ASCII的“OK\n”）
        // 执行失败返回 0x45 0x52 0x52（三字节即ASCII的“ERR”）
        return Sonar_ERROR_OVERFLOW;
    }
    switch (code) {
    case Sonar_OK: {
        char res_ok[3] = { 0x4F, 0x4B, 0x0A };
        memcpy(buf, res_ok, 3);
        break;
    }
    // ...
    default:
        break;
    }
    return Sonar_OK;
}

int main()
{
	const size_t cmd_buf_size = 24;
	int socket_fd, connect_fd;
	int ret = start_tcp_server("192.168.13.132", TCP_PORT, 10, cmd_buf_size);

	if (Sonar_OK == ret) {
		printf("OK\n");
	} else {
		printf("Error: {%d}\n", ret);
		return ret;
	}

	return 0;
}
