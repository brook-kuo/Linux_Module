#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>

#define handle_error(msg) \
        do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char *argv[])
{
    int srv_fd, cli_fd;
    socklen_t cli_len;
    struct sockaddr_un srv_addr, cli_addr;
    char buf[128] = "Brook: ";
    ssize_t len;

    unlink("server_socket");
    if ((srv_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        handle_error("socket");
    }

    srv_addr.sun_family = AF_UNIX;
    strcpy(srv_addr.sun_path, "/tmp/unix_sock");
    if (bind(srv_fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0) {
        handle_error("bind");
    }

    if (listen(srv_fd, 1) < 0) {
         handle_error("listen");
    }

    while (1) {
        cli_fd = accept(srv_fd, (struct sockaddr *)&cli_addr, &cli_len);
        len = read(cli_fd, buf + 6, sizeof(buf) - 6);
        buf[6 + len] = 0;
        write(cli_fd, buf, strlen(buf));
        close(cli_fd);
    }
    return 0;
}

