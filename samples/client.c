#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "ipc.h"

void handle_error(ipc_status err) {
	switch (err) {
	case IPC_SOCKET_ERROR:
		puts("socket call failed");
		break;
	case IPC_CONNECT_FAILED:
		puts("socket connection failed. is the daemon running?");
		break;
	};
}

int main(int argc, char *argv[]) {
	
	/* connect to socket*/
	ipc_client cli = ipc_connect("echo_socket");
	if (cli.error) {
		handle_error(cli.error);
		return 1;
	}

	char str[128];
	while (printf(">"), fgets(str, 128, stdin), !feof(stdin)) {
		ipc_client_send(&cli, str, strlen(str));

		if (ipc_client_read(&cli, str, 128)) {
			printf("server> %s", str);
		} else {
			if (cli.t < 0) puts("error on recv");
			else
				puts("Server closed connection");
			return 1;
		}
	}
	ipc_client_close(cli);

	return 0;
}

