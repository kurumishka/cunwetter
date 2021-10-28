
#include"src/structs.h"
#include"src/varint.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include<sys/socket.h>
#include<sys/types.h>

/* Main types for communication through mc protocol. */
typedef unsigned char _byte;
typedef _byte* _byte_array;

#define READ_SOCKET


void help_usage() {
    puts("Usage:\n\tcunwetter [host] [port] (threads count)");
    puts("Flags:\n\t-h,--help - show this message.");
    puts("\t-v, --verbose - more logs.");
    exit(0);
}


int main(int argc, char** argv) {
    if (argc <= 2)
        help_usage();
    
    _byte_array test;
    test = pack_string_arg("hello world its me..");
    for (int i = 0; i <= test[0]+2; i++)
        printf("%d ", test[i]);
    printf("\n");
    return 0;


    BOOL plogs = FALSE;
    int threads = 1;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--verbose"))
            plogs = TRUE;
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
            help_usage();
    }

    CONFIG* main_config = init_config(plogs, threads, argv[1], argv[2]);
    if (main_config->p_logs)
        printf("Config was initialised with address:\n%s:%s\n",
                main_config->host, main_config->port);

    int _socket = make_socket(main_config);

#ifdef READ_SOCKET
    if (_socket == -1)
        return 0;

#define _maxline 1000
    int n;
    char content[_maxline+1];
    while ((n = read(_socket, content, _maxline)) > 0) {
        content[n] = 0;
        if (fprintf(stdout, content) == EOF)
            fprintf(stderr, "fprintf error.\n");
        fprintf(stdout, "\n");
    }
    if (n < 0)
        fprintf(stderr, "read error.");
    
    exit(0);
#endif

    return 0;
}

