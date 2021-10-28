
/* VarInt module (src/varint.c) */
/* Pack data to byte arrays, unpack from socket, read, etc. */

#include"varint.h"
#include"structs.h"

#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

#include<sys/socket.h>
#include<sys/types.h>

#define _EX_I_BYTE(val, i) ((val >> i) & 0xFF)
// Float will be casted to int by default..
#define _PACK_ARG(arg) _Genecric((arg), int: pack_int_arg,\
                                        float: pack_int_arg,\
                                        _string: pack_string_arg\
                                        default: pack_failed)(arg)

int pack_failed(void* arg) {
    fprintf(stderr, "This type is not supported for _PACK_ARG macro.");
    return -1;
}

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
        if (n < 0) {
            fprintf(stderr, "failed to read from socket (read_varint).");
            return -1;
        }
        bytes[n] = 0;
        local_byte = bytes[0];
        result |= (local_byte & 0x7F) << 7*i;
        if (!(local_byte & 0x80)) 
            break;
    }
    return result;
}

// Pack arguments of the different types to _byte_array.
// 
// {
// Cast _string type to _byte_array. Length is varint by MC protocol.
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

// Cast int to _byte_array type. Also using 5 bytes instead of 4.
_byte_array pack_int_arg(int arg) {
    static _byte_array result;
    result = (_byte_array)malloc(5);
    memset(result, 0, sizeof(result));

    for (int i = 0; i < 4; i++)
        result[i] = _EX_I_BYTE(arg, i);
    return result;
}
// }
//

int test_read_varint(_byte_array _bytes) {
    int test_result = 0;
    _byte local_byte;

    for (int i = 0; i < 5; i++) {
        local_byte = _bytes[i];
        test_result |= (local_byte & 0x7F) << 7*i;
        if (!(local_byte & 0x80))
            break;
    }
    return test_result;
}

