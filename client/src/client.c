#include "client.h"

#include <unistd.h>
#include <string.h>
#include <errno.h>

#define CLIENT_ERR(msg) \
    do { if (self->log) fprintf(self->log, "ERROR: %s: %s\n", msg, strerror(errno)); return false; } while (0)

#define CLIENT_LOG(...) \
    do { if (self->log) fprintf(self->log, "INFO: " __VA_ARGS__); } while (0)

Client client_new(FILE* const log_file) {
    return (Client) {
        .conn = 0,
        .server_addr = {0},
        .log = log_file,
    };
}

// deconstruct the client
void client_drop(Client const self) {
    if (self.conn > 0) close(self.conn);
}

// connect the client to a given port and address
bool client_connect(
    Client* const self,
    in_port_t const port,
    in_addr_t const addr
) {
    int res;
    res = self->conn = socket(CONNECTION_SOCKET_FAMILY, CONNECTION_SOCKET_TYPE, 0);
    if (res < 0) CLIENT_ERR("client socket creation failed");
    CLIENT_LOG("client socket creation success\n");
    self->server_addr.sin_family = CONNECTION_SOCKET_FAMILY;
    self->server_addr.sin_port = htons(port);
    self->server_addr.sin_addr.s_addr = htonl(addr);
    res = connect(self->conn, (struct sockaddr*) &self->server_addr, sizeof self->server_addr);
    if (res < 0) CLIENT_ERR("client connect failed");
    CLIENT_LOG("client connect success\n");
    return true;
}

// send data through the connection
bool client_send(
    Client* const self,
    void const* const data,
    size_t const data_len,
    size_t* const total_sent // can be NULL
) {
    int res;
    res = write(self->conn, data, data_len);
    if (res < 0) CLIENT_ERR("client write failed");
    if (total_sent) *total_sent = res;
    CLIENT_LOG("written %zu bytes to the connection\n", *total_sent);
    return true;
}

// recieve data from the connection
bool client_read(
    Client* const self,
    void* const buf,
    size_t const buf_capacity,
    size_t* const total_read
) {
    int res;
    res = read(self->conn, buf, buf_capacity);
    if (res < 0) CLIENT_ERR("client read failed");
    if (total_read) *total_read = res;
    CLIENT_LOG("read %zu bytes from the connection", *total_read);
    return true;
}
