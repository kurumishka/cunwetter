
#include<stdio.h>

/* If an any socket error has occured. */
#ifndef FAILED
#define FAILED(sock, msg)\
    do {\
        close(sock);\
        fprintf(stderr, msg);\
        fprintf(stderr, "\n");\
        return -1;\
    } while(0)
#endif


#ifndef BOOL 
  typedef char BOOL;
  #define TRUE 1
  #define FALSE 0
#endif

typedef char* _string;
typedef const struct sockaddr SA;

typedef struct CONFIG {
    BOOL p_logs;
    int threadsc;
    _string host, port;
}
CONFIG;

/* Initalise config structure and make socket functions prototypes. */
CONFIG* init_config(BOOL p_value, int threads, _string host, _string port);

int make_socket(CONFIG* config);

