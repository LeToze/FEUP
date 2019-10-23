#include "datalink.c"
int llopen(char port[], int mode);
int llwrite(int fd, char * buffer, int length, int mode);
int llread(int fd, char * buffer, int mode);
int llclose(int fd, int mode);
