#include "io.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

typedef struct io_s {
    int accept_fd;
    int data_fd;

    int buf_size;
} io_t;

static io_t io = { 0 };

int io_init(const char *sock_file, int buf_size) {
    io.buf_size = buf_size;

    (void)unlink(sock_file);

    io.accept_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (io.accept_fd == -1) {
        return -1;
    }

    struct sockaddr_un addr = { 0 };
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, sock_file, sizeof(addr.sun_path) - 1);

    int result = bind(io.accept_fd, (const struct sockaddr *)&addr, sizeof(addr));
    if (result == -1) {
        return -1;
    }

    result = listen(io.accept_fd, 1);
    if (result == -1) {
        return -1;
    }

    return 0;
}

int io_wait_connection() {
    io.data_fd = accept(io.accept_fd, NULL, NULL);
    if (io.data_fd == -1) {
        return -1;
    }

    return 0;
}

void io_sendstr(const char *buf) {
    write(io.data_fd, buf, strnlen(buf, io.buf_size));
}

void io_recvstr(char *buf) {
    memset(buf, '\0', io.buf_size);

    read(io.data_fd, buf, io.buf_size);
    size_t len = strnlen(buf, io.buf_size - 1);

    buf[len] = '\0';
}
