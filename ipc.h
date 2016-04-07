#ifndef IPC_H
#define IPC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

/*
 * Inter Process Communication server
 */
typedef struct {
	unsigned int s, s2;
	struct sockaddr_un local, remote;
	unsigned int msglen;
} ipc_server;

/* create unix domain socket inter process communication server */
ipc_server ipc_server_create(const char *path) {
	ipc_server srv;
	srv.s = socket(AF_UNIX, SOCK_STREAM, 0);
	
	/* bind */
	srv.local.sun_family = AF_UNIX;
	strcpy(srv.local.sun_path, path);
	unlink(srv.local.sun_path);
	srv.msglen = strlen(srv.local.sun_path) + sizeof(srv.local.sun_family);
	
	bind(srv.s, (struct sockaddr *)&srv.local, srv.msglen);

	/* listen */
	listen(srv.s, 5);
	return srv;
}

/* accept ipc client */
void ipc_server_accept(ipc_server *srv) {
	srv->msglen = sizeof(srv->remote);
	srv->s2 = accept(srv->s, (struct sockaddr *)&(srv->remote), &srv->msglen);
}

/* read from ipc server */
int ipc_server_read(ipc_server *srv, char *buf, int buflen) {
	srv->msglen = recv(srv->s2, buf, buflen, 0);
	return srv->msglen;
}

/* write to ipc client */
void ipc_server_send(ipc_server *srv, char *buf, int buflen) {
	send(srv->s2, buf, buflen, 0);
}

/* close srv */
void ipc_server_close(ipc_server srv) {
	close(srv.s2);
}

/*
 * Inter Process Communication client
 */

typedef struct {
	int s, t, len;
	struct sockaddr_un remote;
	
} ipc_client;

ipc_client ipc_client_create(const char *path) {
	ipc_client cli;

	cli.s = socket(AF_UNIX, SOCK_STREAM, 0);
	if (cli.s == -1) {
		puts("socket error");
	}
	
	cli.remote.sun_family = AF_UNIX;
	strcpy(cli.remote.sun_path, path);
	cli.len = strlen(cli.remote.sun_path) + sizeof(cli.remote.sun_family);
	if (connect(cli.s, (struct sockaddr *)&cli.remote, cli.len) == -1) {
		puts("connect failed");
	}

	return cli;
}

/* sends ipc data */
void ipc_client_send(ipc_client *cli, char *buf, int buflen) {
	send(cli->s, buf, buflen, 0);
}

/* read ipc data */
int ipc_client_read(ipc_client *cli, char *buf, int buflen) {
	cli->t = recv(cli->s, buf, buflen, 0);
	if (cli->t > 0) {
		buf[cli->t] = '\0';
	}
	return cli->t;
}

#endif
