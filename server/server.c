#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include "../frame/include/tcp.h"

int main()
{
	// 固定 IP
	int ret = start_tcp_server(
		"192.168.13.132",
		TCP_PORT,
		10,
		MIN_BUF_SIZE
	);

	// 动态获取 IP
	// int ret = start_tcp_server(
	// 	"",
	// 	TCP_PORT,
	// 	10,
	// 	MIN_BUF_SIZE
	// );

	log(INFO, "Exit: {%d}\n", ret);

	return ret;
}
