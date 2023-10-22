#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define CONNECTION_SOCKET_FAMILY   AF_INET
#define CONNECTION_SOCKET_TYPE     SOCK_STREAM
#define CONNECTION_SOCKET_PROTOCOL 0

// connection type
typedef int conn_t;

// client with all of the needed information
typedef struct {
    struct sockaddr_in server_addr;
    conn_t conn;
    FILE* log;
} Client;

// construct a new client
Client client_new(FILE* log_file);

// deconstruct the client
void client_drop(Client self);

// connect the client to a given port and address
bool client_connect(
    Client* self,
    in_port_t port,
    in_addr_t addr
);

// send data through the connection
bool client_send(
    Client* self,
    void const* data,
    size_t data_len,
    size_t* total_sent // can be NULL
);

// recieve data from the connection
bool client_read(
    Client* self,
    void* buf,
    size_t buf_capacity,
    size_t* total_read
);
