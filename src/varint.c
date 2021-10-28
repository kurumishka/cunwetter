
/* VarInt module (src/varint.c) */
/* Pack varints to byte arrays, unpack from socket, read, etc. */

#include"varint.h"
#include"structs.h"

#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

#include<sys/socket.h>
#include<sys/types.h>


_byte_array pack_varint(int data) {
    static _byte_array result;
    /* Ten bytes is more than enough tho. */
    result = (_byte_array)malloc(10);
    memset(result, 0, sizeof(result));

    char addr = 0;
    _byte local_byte;
    while(1) {
        local_byte = data & 0x7F;
        data >>= 7;
        result[addr++] = (_byte)(local_byte | ((data > 0) ? 0x80 : 0));
        if (!data)
            break;
    }
    return result;
}

int read_varint(int socketfd) {
    int result = 0;
    /* Average varint is <= 5 bytes, varlong <= 10 bytes. */
    _byte bytes[10], local_byte;
    memset(bytes, 0, sizeof(bytes));

    for (int i = 0; i < 5; i++) {
        int n = read(socketfd, bytes, 1);
        if (n < 1) {
            fprintf(stderr, "failed to read from socket (read_varint).");
            return -1;
        }
        bytes[n] = -1;
        local_byte = bytes[0];
        result |= (local_byte & 0x7F) << 7*i;
        if (!(local_byte & 0x80)) 
            break;
    }
    return result;
}

_byte_array pack_string_arg(_string arg) {
    static _byte_array result, result_len;
    result_len = pack_varint(strlen(arg));
    int bytes_am = 0;
    for (int i = 0; i < 10; i++) {
        if (result_len[i] != 0) bytes_am++;
        else break;
    }

    result = (_byte_array)malloc(bytes_am + strlen(arg) + 1);
    for (int i = 0; i < bytes_am; i++)
        result[i] = result_len[i];

    strcat(result, arg);
    result[bytes_am-1+strlen(arg)] = 0;
    return result;
}

int test_read_varint(_byte_array _bytes) {
    return 0;
}

