#pragma once

int io_init(const char *sock_file, int buf_size);
int io_wait_connection();
void io_sendstr(const char *buf);
void io_recvstr(char *buf);
