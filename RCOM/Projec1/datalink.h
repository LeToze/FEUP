#include "datalink.c"
int llopen(char port[], int mode);
int llwrite(int fd, char * buffer, int length);
int llread(int fd, char * buffer);
int llclose(int fd);
