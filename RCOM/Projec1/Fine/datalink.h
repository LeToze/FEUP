#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
//#include <io.h>
#include <signal.h>
#define BAUDRATE B38400
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1
#define TRANSMITTER 0
#define RECEIVER 1
#define SIZE 5

int llopen(char port[], int mode);
int llwrite(int fd, char * buffer, int length, int mode);
int llread(int fd, char * buffer, int mode);
int llclose(int fd, int mode);
