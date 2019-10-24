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

#define FLAG 0X7E
//int A = 0x03;
#define C 0x03
#define SET 0X03
#define DISC 0X0A
#define UA 0X07
#define REJ0 0X01
#define REJ1 0X81 // 0x21
#define RR0 0X05
#define RR1 0X85 //0x25
/*#define BCC_UA (A^UA)
#define BCC_DISC (A^DISC)
#define BCC_SET (A^C)
#define BCC_RR (RR^A)
#define BCC_REJ (REJ^A)*/
#define SEQ_0 0x00
#define SEQ_1 0x20
#define MAX_SIZE 255


struct linkLayer {
	char port[20];
	int baudRate;
	unsigned int sequenceNumber;
	unsigned int timeout;
	unsigned int numTransmissions;
	char frame[MAX_SIZE];
};

struct linkLayer data;
int llopen(char port[], int mode);
int llwrite(int fd, char * buffer, int length, int mode);
int llread(int fd, char * buffer, int mode);
int llclose(int fd, int mode);
