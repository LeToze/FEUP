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
int A = 0x03;
#define C 0x03
#define SET 0X03
#define DISC 0X0A
#define UA 0X07
#define REJ0 0X01
#define REJ1 0X21
#define RR0 0X05
#define RR1 0X25
/*#define BCC_UA (A^UA)
#define BCC_DISC (A^DISC)
#define BCC_SET (A^C)
#define BCC_RR (RR^A)
#define BCC_REJ (REJ^A)*/
#define SEQ_0 0x00
#define SEQ_1 0x40
#define MAX_SIZE 255

int llopen(char port[], int mode);
int llwrite(int fd, char * buffer, int length,int mode);
int llread(int fd, char * buffer,int mode);
int llclose(int fd,int mode);


struct linkLayer {
	char port[20];
	int baudRate;
	unsigned int sequenceNumber;
	unsigned int timeout;
	unsigned int numTransmissions;
	char frame[MAX_SIZE];
};

volatile int STOP=FALSE;
int LIDO=FALSE;
int ABERTO=FALSE;
int big_estado=0;
int RRDONE=FALSE;
int DISCDONE= FALSE;
int IDONE = FALSE;
int REJDONE = FALSE;

int fd,c, res;
struct termios oldtio,newtio;
struct linkLayer data;
char buf[255];
char buf2[255];
char dbcc=0x00;//data BCC
char ESC_BYTE = 0x7d;
char set_buf[SIZE];
char rec[255];
int i, sum = 0,fseq=1;
int estado=0,speed =0;

void atende()                   // atende alarme
{
  int flag=1, conta=1;
	printf("%d\n", conta);
	flag=1;
	conta++;
}

void maq_estados(char c,char sign, int mode){
  printf(" Recebi o caracter %x  no estado %d \n",c,estado);
  if(mode == TRANSMITTER){
	  A=0x01;
  }
  if(mode == RECEIVER){
	  A=0x03;
  }

  char BCC = A ^ sign;
  printf("A = %x",A);
  switch(estado){
    case 0:
      if (c==FLAG){
        estado=1;
        break;
      }
    case 1:
      if (c == A){
        estado=2;
        break;
      }
      else if( c == sign || c == BCC){
        estado=0;
        break;
      }
      else if(c ==FLAG){
        estado=1;
        break;
      }
    case 2:
      if (c == sign){
        estado=3;
        break;
      }
      else if( c== A|| c == BCC){
        estado=0;
        break;
      }
      else if(c == FLAG){
        estado=1;
        break;
      }
    case 3:
      if (c==BCC){
        estado=4;
        break;
      }
      else if( c== A || c == sign){
        estado=0;
        break;
      }
      else if(c == FLAG){
        estado=1;
        break;
      }
    case 4:
      if (c==FLAG){
        estado=5;

		if (sign == SET) {
			STOP = TRUE;
		}
		else if (sign == DISC) {
			DISCDONE = TRUE;
		}
		else if (sign == UA) {
			LIDO = TRUE;
		}
		else if (sign == RR0 || sign == RR1) {
			RRDONE = TRUE;
		}
		else if (sign == RR0 || sign == RR1) {
			REJDONE = TRUE;
		}
        break;
      }
      else if( c== A || c==sign || c== BCC){
        estado=0;
        break;
      }
  }
}

int maq_estados_I(char c) {
	printf(" Recebi o caracter %x  no estado %d \n", c, estado);
	int fseq;
	char ESC_BYTE = 0x7d;
	char buffer[255];
	int i = 0;
	switch (estado) {
	case 0:
		if (c == FLAG) {
			estado = 1;
			break;
		}
	case 1:
		if (c == A) {
			estado = 2;
			break;
		}
		else if (FLAG) {
			estado = 1;
			break;
		}
		else  {
			estado = 0;
			break;
		}
	case 2:
		if (c == SEQ_0) {
			estado = 3;
			fseq = 0;
			break;
		}
		else if (c == SEQ_1) {
			estado = 3;
			fseq = 1;
			break;
		}
		else if (FLAG) {
			estado = 1;
			break;
		}
		else {
			estado = 0;
			break;
		}
	case 3:
		if (fseq) {
			if (c == (A ^ SEQ_0)) {
				estado = 4;
				break;
			}
		}
		else if (!fseq) {
			if (c== (A ^ SEQ_1)) {
				estado = 4;
				break;
			}
		}
		else if (FLAG) {
			estado = 0;
			break;
		}
		else {
			estado=0;
		}
	case 4:
		if (c == FLAG) {
			estado = 1;
			break;
		}
		else if (c == dbcc) {
			estado = 6;
			break;
		}
		else if (c == ESC_BYTE) {
			estado = 5;
			break;
		}
		else { //Cria o BCC
			buffer[i++] = c;
			dbcc = dbcc ^ c;
			break;
		}
	case 5:
		if (c == 0x5e) {
			buffer[i++] = 0x7e;
			if (dbcc == FLAG) {
				estado = 6;
				break;
			}
			dbcc = 0x7e ^ dbcc;
			estado = 4;
		}
		else if (c == 0x5d) {
			buffer[i++] = ESC_BYTE;
			if (dbcc == ESC_BYTE) {
				estado = 6;
				break;
			}
			dbcc = ESC_BYTE ^ dbcc;
			estado = 4;
		}
		else {
			buffer[i++] = ESC_BYTE;
			dbcc = ESC_BYTE ^ dbcc;

			if (c == dbcc) {
				estado = 6;
				break;
			}
			else if (c == FLAG) {
				estado = 1;
				break;
			}
			else if (c == ESC_BYTE) {
				estado = 5;
				break;
			}
			else {
				buffer[i++] = c;
				dbcc = c ^ dbcc;
				estado = 4;
				break;
			}
		}
	case 6:
		if (c == FLAG) {
			estado = 7;
			IDONE = TRUE;
			break;
		}
		else {
			buffer[i++] = c;
			dbcc = c ^ dbcc;
			if (c == ESC_BYTE) {
				estado = 5;
				break;
			}
			else if (c == dbcc) {
				estado = 6;
				return fseq;
				break;
			}
			else if (c == FLAG) {
				estado = 1;
				break;
			}
			else {
				buffer[i++] = c;
				dbcc = c ^ dbcc;
			}
		}

	}
	return 0;
}

void big_maq_estados(){
printf("%d\n\n\n\n", big_estado);
switch(big_estado){
  case '0':
    if (ABERTO){
      big_estado=1;
    }
  case '1':
    if (LIDO){
      big_estado=2;
    }
  case '2':

    exit(1);
    }
}

int send_I(int fd,char frame[]) {
	char set_buf[MAX_SIZE];
	char dbcc;
	int size;
	set_buf[0] = FLAG;
	set_buf[1] = A;
	printf("Fseq=%d",fseq);
	if (fseq) {
		set_buf[2] = 0x20;
	}
	if (!fseq) {
		set_buf[2] = 0x00;
	}
	set_buf[3] = set_buf[1] ^ set_buf[2];//i percorre set_buf
	for (int j = 0,i = 4; j < strlen(frame);i++,j++) {
		dbcc ^= set_buf[i];
		if (frame[j] == 0x7E) {
			set_buf[i] = ESC_BYTE;
			i++;
			set_buf[i] = 0x5E;
		}
		else if (frame[j] == ESC_BYTE) {
			set_buf[i] = ESC_BYTE;
			++i;
			set_buf[i] = 0x5D;
		}
		else {
			set_buf[i] = frame[j];
		}
	}
	if (dbcc != FLAG && dbcc != ESC_BYTE) {
		set_buf[i] = dbcc;
		set_buf[i + 1] = FLAG;
		set_buf[i + 2] = '\0';
	}
	size = strlen(set_buf);//size of I
	for (i = 0;i < 5;i++) {
		write(fd, &set_buf[i],1);
	}
	return 0;
}

int send(int fd,int sign, int mode){
	char set_buf[5];
  if(mode == TRANSMITTER){
	  A = 0x03;
  }
  if(mode == RECEIVER){
	  A = 0x01;
  }
    set_buf[0] = FLAG;//F
	set_buf[1] = A;
	set_buf[2] = sign;
	set_buf[3] = set_buf[1] ^ set_buf[2]; //0x00
    set_buf[4] = FLAG;//F

      for (i = 0; i < 5; i++) {
           write(fd,&set_buf[i],1); // carater a carater e o /0 j� t� no buf na ultima posi��o
      }
    return 1;
}

int main(int argc, char** argv)
{
    strcpy(data.port, argv[1]);
    data.baudRate = BAUDRATE;
    data.timeout = 3;

 	fd = open(argv[1], O_RDWR | O_NOCTTY );

  if (fd <0) {perror(argv[1]); exit(-1); }

  if ( tcgetattr(fd,&oldtio) == -1) { /* save current port settings */
     perror("tcgetattr");
     exit(-1);
   }

  bzero(&newtio, sizeof(newtio));
  newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
  newtio.c_iflag = IGNPAR;
  newtio.c_oflag = 0;

   /* set input mode (non-canonical, no echo,...) */
  newtio.c_lflag = 0;

  newtio.c_cc[VTIME]    = 1;   /* inter-character timer unused */
  newtio.c_cc[VMIN]     = 0;   /* blocking read until 5 chars received */
  tcflush(fd, TCIOFLUSH);

  if ( tcsetattr(fd,TCSANOW,&newtio) == -1) {
    perror("tcsetattr");
    exit(-1);
  }
   printf("New termios structure set\n");
   llopen(argv[1],TRANSMITTER);


    //llread(fd,buf2,RECEIVER);
    llwrite(fd,set_buf,SIZE,TRANSMITTER);

    if ( (argc < 2) ||
  	     ((strcmp("/dev/ttyS0", argv[1])!=0) &&
  	      (strcmp("/dev/ttyS1", argv[1])!=0) )) {
      printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
      exit(1);
    }

    (void) signal(SIGALRM, atende);
	llclose(fd,TRANSMITTER);
}

int llopen(char port[],int mode) {
   gets(buf);
   int len=strlen(buf);
   buf[len]='\0';
   printf("MODE:%d",mode);
  if(mode == TRANSMITTER){
	send(fd,SET,mode);
	printf("Cheguei");
	while(LIDO == FALSE){
			res = read(fd,buf,1);
			buf[res]='\0';
			printf(":%x:%d\n", buf[0], res);
			maq_estados(buf[0],UA,TRANSMITTER);
  		}
  }

   if(mode == RECEIVER){
   		while (STOP==FALSE) {       /* loop for input */
        	res = read(fd,buf,1);   /* returns after 5 chars have been input */
        	buf[res]='\0';               /* so we can printf... */
        	printf(":%x:%d\n", buf[0], res);
        	if (buf[0]=='z') STOP=TRUE; /*linha que para a leitura de carateres*/
  	    	maq_estados(buf[0],SET,RECEIVER);
      }

  send(fd,UA,RECEIVER);
  }

  ABERTO = TRUE;
  printf("Conexão Efetuada com sucesso\n");
  return fd;
}

int llwrite(int fd, char * buffer, int length,int mode){

char buf[20];

if(mode == RECEIVER){
	printf("llwrite não funciona em receiver mode");
	exit(1);
}
send_I(fd, data.frame);

while (RRDONE == FALSE) {       /* loop for input */
	res = read(fd, buf, 1);   /* returns after 5 chars have been input */
	buf[res] = '\0';               /* so we can printf... */
	printf(":%x:%d\n", buf[0], res);
	if(fseq){
	maq_estados(buf[0], RR1,TRANSMITTER);
	}
	if(!fseq){
	maq_estados(buf[0], RR0,TRANSMITTER);
	}
}

return 0;
}

int llread(int fd, char * buffer,int mode){
int fseq = 0;
if(mode == TRANSMITTER){
	printf("llread não funciona em transmiter mode");
	exit(1);
}

while (IDONE == FALSE) {
	res = read(fd, buf, 1);
	buf[res] = '\0';
	printf(":%x:%d\n", buf[0], res);
	fseq = maq_estados_I(buf[0]);
}
data.sequenceNumber = fseq;
if (fseq) {
	send(fd,RR1,TRANSMITTER);//RR1
}
if (!fseq) {
	send(fd,RR0,TRANSMITTER);//RR0
}


return 0;
}

int llclose(int fd,int mode){
	char buf[255];
	if ( tcsetattr(fd,TCSANOW,&oldtio) == -1) {
      perror("tcsetattr");
  }

  if(mode == TRANSMITTER){
	send(fd,DISC,TRANSMITTER);
	while (DISCDONE==FALSE) {       /* loop for input */
        	res = read(fd,buf,1);   /* returns after 5 chars have been input */
        	buf[res]='\0';               /* so we can printf... */
        	printf(":%x:%d\n", buf[0], res);
  	    	maq_estados(buf[0],DISC,TRANSMITTER);
      }
		send(fd,UA,TRANSMITTER);
  }

  if(mode == RECEIVER){
	while (DISCDONE==FALSE) {       /* loop for input */
        	res = read(fd,buf,1);   /* returns after 5 chars have been input */
        	buf[res]='\0';               /* so we can printf... */
        	printf(":%x:%d\n", buf[0], res);
  	    	maq_estados(buf[0],DISC,RECEIVER);
      }

		send(fd,DISC,RECEIVER);
		while (LIDO == FALSE) {       /* loop for input */
			res = read(fd, buf, 1);   /* returns after 5 chars have been input */
			buf[res] = '\0';               /* so we can printf... */
			printf(":%x:%d\n", buf[0], res);
			maq_estados(buf[0],UA,RECEIVER);
		}
  }

    close(fd);
    return 0;
}
