#ifndef AUTH_H
#define AUTH_H

#include <libssh/libssh.h>

#define MAXBUF 100

struct connection {
    ssh_session session;
    ssh_message message;
    char client_ip[MAXBUF];
    char con_time[MAXBUF];
	char const *user;
	char const *pass;
};

int handle_auth(ssh_session session);

#endif
