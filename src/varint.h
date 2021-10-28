
#ifndef _byte
typedef unsigned char _byte;
#endif

#ifndef _byte_array
typedef _byte* _byte_array;
#endif

#ifndef _string
typedef char* _string;
#endif

_byte_array pack_varint(int data);
int read_varint(int socketfd);
int test_read_varint(_byte_array _bytes);
_byte_array pack_string_arg(_string arg);

