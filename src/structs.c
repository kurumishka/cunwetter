
#include"structs.h"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

#include<sys/socket.h>
#include<sys/types.h>

#include<netinet/in.h>
#include<arpa/inet.h>


CONFIG* init_config(BOOL p_value, int threads, _string host, _string port) {
    static CONFIG* config;
    config = (CONFIG*)malloc(sizeof(CONFIG));

    config->p_logs = p_value;
    config->threadsc = threads;
    config->host = (_string)malloc(sizeof(host));
    config->port = (_string)malloc(sizeof(port));

    strcpy(config->host, host);
    strcpy(config->port, port);

    return config;
}

int make_socket(CONFIG* conf) {
    struct sockaddr_in socket_st;
    int _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0) {
        fprintf(stderr, "socket initialisation failed.");
        return -1;
    }
    
    memset(&socket_st, 0, sizeof(socket_st));
    socket_st.sin_family = AF_INET;
    socket_st.sin_port = htons(atoi(conf->port));

    if (inet_pton(AF_INET, conf->host, &socket_st.sin_addr) <= 0)
        FAILED(_socket, "inet_pton host initialisation failed.");

    if (connect(_socket, (SA*)&socket_st, sizeof(socket_st)) < 0)
        FAILED(_socket, "connection failed.");

    return _socket;
}

