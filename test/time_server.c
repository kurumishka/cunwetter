
/* UNIX Socket time server for test purpose. */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include<unistd.h>

#include<sys/socket.h>
#include<sys/types.h>

#include<netinet/in.h>
#include<arpa/inet.h>

#include<time.h>


typedef const struct sockaddr SA;

#define FAILED(socket, msg)\
    do {\
        close(socket);\
        fprintf(stderr, msg);\
        fprintf(stderr, "\n");\
        return -1;\
    }while(0)

typedef char* _string;


int make_server_socket(_string port) {
    int _socket = socket(AF_INET, SOCK_STREAM, 0);
    static struct sockaddr_in socket_st;
    memset(&socket_st, 0, sizeof(socket_st));

    if (_socket < 0) {
        fprintf(stderr, "socket initialisation failed.");
        return -1;
    }

    socket_st.sin_port = htons(atoi(port));
    socket_st.sin_family = AF_INET;
    socket_st.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(_socket, (SA*)&socket_st, sizeof(socket_st)) < 0)
            FAILED(_socket, "failed to bind host to the server socket.");

#ifndef LISTENQ
#define LISTENQ 5
#endif

    if (listen(_socket, LISTENQ) < 0)
        FAILED(_socket, "failed to listen to server socket.");

    return _socket;
}

int accept_connections(int _socket) {
    int conn;
    time_t ticks;
#ifndef _maxline
#define _maxline 1000
#endif
    char buffer[_maxline+1];

    while(1) {
        if ( (conn = accept(_socket, (struct sockaddr*)NULL, NULL)) < 0 )
            FAILED(_socket, "failed to perform accept operation on socket.");

        ticks = time(NULL);
        snprintf(buffer, sizeof(buffer), "%.24s\en\er", ctime(&ticks));

        if (write(conn, buffer, strlen(buffer)) < 0) {
            close(conn);
            FAILED(_socket, "failed to write to socket.");
        }
        close(conn);
    }
    return 0;
}

int main(int argc, char** argv) {
    /* Just a port to open server socket on. */
    if (argc < 2) {
        fprintf(stderr, "invalid command line arguments!\n");
        exit(0);
    }

    int _socket = make_server_socket(argv[1]);
    if (_socket == -1) exit(0);

    int status = accept_connections(_socket);
    if (status == -1) exit(0);
    close(_socket);

    return 0;
}

