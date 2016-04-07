#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "ipc.h"

int main(int argc, char *argv[]) {
	
	ipc_server srv = ipc_server_create("echo_socket");
	
	char buf[128];
	while (1) {
		/* call accept */
		puts("waiting for connection");
		ipc_server_accept(&srv);
		puts("connected!");

		/* handle connection and loop back to accept*/
		while (ipc_server_read(&srv, buf, 128) > 0) {
			printf("-> '%.*s'\n", srv.msglen - 1, buf);
			if (!strncmp(buf, "end", 3)) {
				puts("closing");
				ipc_server_close(srv);
				exit(0);
			}

			ipc_server_send(&srv, buf, srv.msglen);
		}
		
	}

	return 0;
}

