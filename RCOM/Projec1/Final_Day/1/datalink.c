#include "datalink.h"
#include "app.h"
int A = 3;



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
int sender;
struct linkLayer data;
unsigned char buf[255];
unsigned char buf2[255];
unsigned char dbcc=0x00;//data BCC
unsigned char ESC_BYTE = 0x7d;
unsigned char set_buf[SIZE];
unsigned char rec[255];
int i, sum = 0,fseq=0,rcv=0;
int estado=0,speed =0;

void atende()                   // atende alarme
{
  int flag=1, conta=1;
	printf("%d\n", conta);
	flag=1;
	conta++;
}

void maq_estados(unsigned char c,unsigned char sign, int mode){

  fflush(stdout);
  printf(" Recebi o caracter %x  no estado %d \n",c,estado);

  if(mode == TRANSMITTER){
	  A=0x01;
  }
  if(mode == RECEIVER){
	  A=0x03;
  }

  unsigned char BCC = A ^ sign;
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
		else if (sign == REJ0 || sign == REJ1) {
			REJDONE = TRUE;
		}
		estado = 0;
        break;
      }
      else if( c== A || c==sign || c== BCC){
        estado=0;
        break;
      }
  }
}

int trama(int mode,char frame[]){
	unsigned char set_buf[50];
  int j=0;
  if(mode == TRANSMITTER){
	  A = 0x03;
  }
  if(mode == RECEIVER){
	  A = 0x01;
  }
  set_buf[0] = FLAG;//F
	set_buf[1] = A;
	set_buf[2] = data.sequenceNumber;
	set_buf[3] = set_buf[1] ^ set_buf[2]; //0x00
  //set_buf[4] = FLAG;//
  for(j=0;frame[j] != '\0';j++){
    set_buf[j+4]=frame[j];
  }
}

unsigned char *Destuffing (unsigned char *frame){
	int length = strlen(frame);
	unsigned char *stuffedFrame=malloc(sizeof(unsigned char)*length+1);
	int i,j=0;

	for(i=0;i < length-1 ;i++){
		if(frame[i] == ESC_BYTE){
			//stuffedFrame[j] = ESC_BYTE;
			i++;
			stuffedFrame[j] = frame[i]^0x20;
			j++;

		}
		else{
			stuffedFrame[j] = frame[i];
			j++;
		}

	}

	return stuffedFrame;
}


int maq_estados_I(unsigned char c, int mode) {
	printf(" Recebi o caracter %x  no estado %d \n", c, estado);
	int fseq;
	unsigned char buffer[255];
	int k = 0;

if(mode == TRANSMITTER){
	  A=0x01;
  }
  if(mode == RECEIVER){
	  A=0x03;
  }

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
		if (!fseq) {
			if (c == (A ^ SEQ_0)) {
				estado = 4;
				break;
			}
		}
		else if (fseq) {
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
			buffer[k] = c;
			k++;
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
			estado = 0;
      	    ++rcv;
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
	for(int j=0;j <strlen(buffer)+1 ;j++){
		fflush(stdout);
		printf("BUFFER final:%x\n",buffer[j]);
		fflush(stdout);
	}

	return 0;
}

/*void big_maq_estados(){
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
}*/

int send_I(int fd,unsigned char frame[]) {
	unsigned char set_buf[MAX_SIZE];
	unsigned char dbcc;
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
	for (int j = 0,i = 4; j < strlen(frame)+1;i++,j++) {
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
	unsigned char set_buf[5];
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

 	applay.fd  = open(argv[1], O_RDWR | O_NOCTTY );
	fd=applay.fd;

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

  newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
  newtio.c_cc[VMIN]     = 1;   /* blocking read until 5 chars received */
  tcflush(fd, TCIOFLUSH);

  if ( tcsetattr(fd,TCSANOW,&newtio) == -1) {
    perror("tcsetattr");
    exit(-1);
  }
   printf("New termios structure set\n");
   llopen(argv[1],RECEIVER);

	go(RECEIVER);

    if ( (argc < 2) ||
  	     ((strcmp("/dev/ttyS0", argv[1])!=0) &&
  	      (strcmp("/dev/ttyS1", argv[1])!=0) )) {
      printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
      exit(1);
    }

    (void) signal(SIGALRM, atende);
	llclose(applay.fd,RECEIVER);
}

int llopen(unsigned char port[],int mode) {
   printf("MODE:%d\n",mode);
  if(mode == TRANSMITTER){
	send(fd,SET,mode);
	printf("Cheguei\n");
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

int llwrite(int fd, unsigned char * buffer, int length,int mode){

unsigned char buf[20];

if(mode == RECEIVER){
	printf("llwrite não funciona em receiver mode\n");
	exit(1);
}
send_I(fd, buffer);

while (!(estado == 5)) {       /* loop for input */
	res = read(fd, buf, 1);   /* returns after 5 chars have been input */
	buf[res] = '\0';               /* so we can printf... */
	printf(":%x:%d\n", buf[0], res);
	fflush(stdout);
	fseq=0;
	if(fseq){
	maq_estados(buf[0], RR1,TRANSMITTER);
	}
	if(!fseq){
	maq_estados(buf[0], RR0,TRANSMITTER);
	}
}

return 0;
}

int llread(int fd, unsigned char *buffer,int mode){
  unsigned char buf[50];
  int i=0;
  A=0x03;
  fflush(stdout);

if(mode == TRANSMITTER){
	printf("llread não funciona em transmiter mode\n");
	exit(1);
}

while (IDONE == FALSE) {
  /*if((res = read(fd,&buf[0],1)) < 0){
    fflush(stdout);
    printf("Reading Error\n");
    fflush(stdout);
  }*/
	read(fd,&buf[0],1);
	buf[res] = '\0';
	buffer[i++]=buf[0];
	c=buf[0];

printf(" Recebi o caracter %x  no estado %d \n",c,estado);
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
		else if (c == FLAG) {
			estado = 1;
			break;
		}
		else {
			estado = 0;
			break;
		}
	case 3:
		if (!fseq) {
			if (c == (A ^ SEQ_0)) {
				estado = 4;
				break;
			}
		}
		else if (fseq) {
			if (c== (A ^ SEQ_1)) {
				estado = 4;
				break;
			}
		}
		else {
			estado=0;
			break;
		}
	case 4:
	if (c == FLAG) {
		if (createBCC2(buffer,i)) {
			if(fseq){
				send(fd,(unsigned char)RR1,RECEIVER);//RR1
			}
			else{
				send(fd,(unsigned char)RR0,RECEIVER);//RR0
			}
			estado = 6;
			IDONE=TRUE;
			sender=1;
			break;
		}
		else{
			if(fseq){
				send(fd,(unsigned char)REJ1,RECEIVER);//REJ1
			}
			else{
				send(fd,(unsigned char)REJ0,RECEIVER);//REJ0
			}
			estado = 0;
			IDONE=TRUE;
			sender=0;
			break;
		}
	}
	else if (c == ESC_BYTE) {
			estado = 5;
			break;
	}
	else {
		buffer[i++]=c;
		break;
	}
	case 5:
		if(c == ESC_BYTE){
			buffer[i] = FLAG;
		}
		else{
			if(c == 0x5d){
				buffer[i]=ESC_BYTE;
			}
			else{
				printf("Not valid");
				exit(-1);
			}
		}
		estado=4;
		break;
	}
}

strcpy(buffer,Destuffing(buffer));
for(int y=0;y < 4;y++){
	buffer[y]=buffer[y+4];
	//buffer[y+4]=0;
}


IDONE=FALSE;

return 0;
}



int createBCC2(unsigned char *mess,int size){
	int i=1;
	unsigned char BCC2;
	BCC2=mess[0];
	for(i=1;i < size;i++){
		BCC2 ^= mess[i];
	}
	if(BCC2 == mess[size-1]){
		return 1;
	}
	else{
		return 0;
	}
}


int llclose(int fd,int mode){
	unsigned char buf[255];
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
	printf("\nFoi terminada a conexao\n");
    close(fd);
    return 0;
}
