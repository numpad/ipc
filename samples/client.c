#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "ipc.h"

int main(int argc, char *argv[]) {
	
	ipc_client cli = ipc_client_create("echo_socket");
	
	char str[128];
	while (printf(">"), fgets(str, 128, stdin), !feof(stdin)) {
		ipc_client_send(&cli, str, strlen(str));

		if (ipc_client_read(&cli, str, 128) > 0) {
			printf("echo> %s", str);
		} else {
			if (cli.t < 0) puts("error on recv");
			else puts("Server closed connection");
			return 1;
		}
	}
	close(cli.s);

	return 0;
}

