#include "config.h"
#include "auth.h"

#include <libssh/libssh.h>
#include <libssh/server.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <getopt.h>
#include <errno.h>
#include <sys/wait.h>

#define MINPORT 0
#define MAXPORT 65535

/* Global so they can be cleaned up at SIGINT. */
static ssh_session session;
static ssh_bind sshbind;


/* Signal handler for cleaning up after children. We want to do cleanup
 * at SIGCHILD instead of waiting in main so we can accept multiple
 * simultaneous connections. */
static void cleanup(int)
{
	int status;
	int pid;

	while ((pid=wait3(&status, WNOHANG, NULL)) > 0) {
		if (DEBUG) { printf("process %d reaped\n", pid); }
	}

	/* Re-install myself for the next child. */
	signal(SIGCHLD, cleanup);
}


/* SIGINT handler. Cleanup the ssh* objects and exit. */
static void wrapup(int)
{
	ssh_disconnect(session);
	ssh_bind_free(sshbind);
	ssh_finalize();
	exit(0);
}


int main()
{
	int port = DEFAULTPORT;

	/* Install the signal handlers to cleanup after children and at exit. */
	signal(SIGCHLD, cleanup);
	signal(SIGINT, wrapup);

	/* Create and configure the ssh session. */
	session=ssh_new();
	sshbind=ssh_bind_new();
	ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_BINDADDR, LISTENADDRESS);
	ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_BINDPORT, &port);
	ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_HOSTKEY, "ssh-rsa");
	ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_RSAKEY,RSAKEYFILE);

	/* Listen on `port' for connections. */
	if (ssh_bind_listen(sshbind) < 0) {
		printf("Error listening to socket: %s\n",ssh_get_error(sshbind));
		return -1;
	}
	if (DEBUG) { printf("Listening on port %d.\n", port); }

	/* Loop forever, waiting for and handling connection attempts. */
	while (1) {
		if (ssh_bind_accept(sshbind, session) == SSH_ERROR) {
			fprintf(stderr, "Error accepting a connection: `%s'.\n",ssh_get_error(sshbind));
			return -1;
		}
		if (DEBUG) { printf("Accepted a connection.\n"); }

		switch (fork())  {
		case -1:
			fprintf(stderr,"Fork returned error: `%d'.\n",-1);
			exit(-1);

		case 0:
			exit(handle_auth(session));

		default:
			break;
		}
	}

	return 0;
}
