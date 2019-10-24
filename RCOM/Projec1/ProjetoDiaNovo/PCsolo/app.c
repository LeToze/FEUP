#include "datalink.h"
#include "app.h"
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
#define CONTROLEND 0x03
#define CONTROLSTART 0x02
#define CONTROLDATA 0X01
#define CONTROLT1 0X00
#define CONTROLT2 0X01


struct controlpack {
	int filesize;
	unsigned char filename[20];
	unsigned char data[255];
};

struct datapack {
	int SeqNum;
	int DataSize[2];
	unsigned char data[255];
};

unsigned char DATA[255];
struct controlpack c1;
struct datapack d1;

void go(int mode){
	if(mode == TRANSMITTER){
		sendData();
	}
	if(mode == RECEIVER){
		receiveData();
	}
}

int ControlPackage(int c){
	int j = 0;
	c1.filesize = 0;
	strcpy(c1.filename,"pinguim");
	c1.data[0] = c;// 2 se start,3 se end
	c1.data[1] = 1;
	for ( j = 0;j < strlen(c1.filename)+1;j++) {
		c1.data[j+3] = c1.filename[j];
	}
	c1.data[2] = strlen(&c1.data[3]) + 1;
	c1.data[j+4] = 0;
	c1.data[j+5] = c1.filesize;
	c1.data[j+6] = strlen(&c1.data[3]) + 1;

	if(llwrite(applay.fd,c1.data,1,TRANSMITTER) < 0){
		printf("Error 45\n");
		return -1;
	}
return 0;
}

int setFile() {

  printf("Name of the file to be transmitted: ");
  scanf("%s", c1.filename);

  if(c1.filename == NULL) {
    printf("Filename is null\n");
    return -1;
  }

  if((applay.fileDescriptor = open(c1.filename, O_RDONLY)) < 0) {
    perror("Error opening the file");
    return -1;
  }

  return 0;
}

int PingOP(){
	if(open(c1.filename,O_CREAT|O_WRONLY|O_APPEND, S_IWUSR|S_IRUSR) < 0){
		perror("Error opening the file");
		return -1;
	}

	return 0;
}

/*int DataPackage(int datalength) {
	int i = 0;
	d1.data[0] = 1;
	d1.SeqNum = 0;
	d1.data[1]= 0;
	d1.data[2] = datalength % 256;
	d1.data[3] = datalength / 256;

	memcpy(&d1.data[4],data.frame,datalength);
	if(llwrite(applay.fileDescriptor,d1.data,46,TRANSMITTER) < 0){
		printf("Error 46\n");
		return -1;
	}

	for (i = 0;i < datalength;i++) {
		d1.data[2 + j] = DATA[i];
	}

	return 0;
}*/

int sendData() {
		printf("SENT11111\n");
	int i=0;
	//printf("SENTconasMode         %d\n", ControlPackage(CONTROLSTART));
  if (sendControlPacket(CONTROLSTART) < 0) {
    printf("Error in sendControlPacket\n");
    return -1;
  }

  i++;
		printf("SENT11\n");
  int bytesRead = 0, seqNumber = 0;
  unsigned char * buffer = (unsigned char *) malloc(applay.fragmentSize + 1);
		printf("SENT22\n");

  while ((bytesRead = read(applay.fileDescriptor, buffer, applay.fragmentSize)) > 0) {

    i++;
		printf("SENT\n");
    if (sendPacket(seqNumber, buffer, bytesRead) < 0) {
      printf("Error in sendPacket\n");
      free(buffer);
      return -1;
    }

    seqNumber++;
    seqNumber %= 255;
  }

  if (sendControlPacket(CONTROLEND) < 0) {
    printf("Error in sendControlPacket\n");
    return -1;
  }

  i++;
	memcpy(&data.frame,&buffer,bytesRead);
    if (close(applay.fileDescriptor) < 0) {
    printf("Error closing the file.\n");
    return -1;
  }

  llclose(applay.fd,TRANSMITTER);
  return 0;
}

int sendControlPacket(unsigned char control_byte) {
	if (control_byte == 0x02) {
		if (setFile() < 0) {
			printf("Error getting the file\n");
			return -1;
		}
	}
	printf("1ºponto\n");
	struct stat f_information;

	if (fstat(applay.fileDescriptor, &f_information) < 0) {
		perror("Couldn't obtain information regarding the file");
		return -1;
	}

	off_t f_size = f_information.st_size; /* total size in bytes, in signed integer */
	c1.filesize = f_size;
	unsigned int l1 = sizeof(f_size);
	unsigned int l2 = strlen(c1.filename) + 1;

	unsigned char startpackage[255];
	printf("2ºponto\n");
	startpackage[0] = control_byte; // CONTROLSTART or CONTROLEND
	startpackage[1] = CONTROLT1;
	startpackage[2] = l1;
	*((off_t*)(startpackage + 3)) = f_size;
	startpackage[3 + l1] = CONTROLT2;
	startpackage[3 + l1 + 1] = l2; /* +1 from CONTROLT2 */

	strcat((unsigned char*)startpackage + 5 + l1, c1.filename);
	printf("3ºponto\n");
	if (llwrite(applay.fd, startpackage,255,TRANSMITTER) < 0) {
		printf("Couldn't write control package.\n");
		return -1;
	}
	printf("4ºponto\n");
	return 0;
}

int receiveData() {
  int i=0;

  if (receiveControlPacket() < 0) {
    printf("error in receiveControlPacket\n");
    return -1;
  }

  i++;

  int bytesRead = 0, seqNumber = 0, counter = 0;
  unsigned char * buffer;

  while(counter < c1.filesize) {

  i++;

    bytesRead = receivePacket(&buffer, seqNumber);
    if(bytesRead < 0) {
      printf("receivePacket didn't read \n");
      continue;
    }

    counter+= bytesRead;
    if(write(applay.fileDescriptor, buffer, bytesRead) <= 0) {
      perror("Couldn't write to file");
    }

    seqNumber++;
    free(buffer);
    }

  if (receiveControlPacket() < 0) {
    printf("Error in receiveControlPacket\n");
    return -1;
  }

  	i++;

    if (close(applay.fileDescriptor) < 0) {
    printf("Error closing the file.\n");
    return -1;
  }
  llclose(applay.fd,RECEIVER);
  system("clear"); //*nix
  printf("<<< Finished >>>\n");
  return 0;
}



int sendPacket(int seqNumber, unsigned char* buffer, int length) {

	int totalLength = length + 4;
	unsigned char dataPacket[255];

	dataPacket[0] = CONTROLT2;
	dataPacket[1] = seqNumber;
	dataPacket[2] = length / 256;
	dataPacket[3] = length % 256;

	memcpy(&dataPacket[4], buffer, length);

	if (llwrite(applay.fd, dataPacket, totalLength,TRANSMITTER) < 0) {
		printf("Error sending data packet");
		return -1;
	}

	return 0;
}

int receiveControlPacket() {
	unsigned char* read_package;
	unsigned int package_size = llread(applay.fd, read_package,RECEIVER);
	int i;

	if (package_size < 0) {
		perror("Couldn't read linklayer whilst receiving package.");
		return -1;
	}

	int pck_index = 0;

	if (read_package[pck_index] == 0x03) {
		free(read_package);
		return 0;
	} /*End of transfer process, nothing to process any further.*/

	pck_index++; //move on to T1
	unsigned int n_bytes;

	unsigned char pck_type; //T
	for (i = 0; i < 2; i++) {
		pck_type = read_package[pck_index++]; // read T, update to L

		switch (pck_type) {
		 case CONTROLT1:
			n_bytes = (unsigned int)read_package[pck_index++]; // read L1, update to V1

			c1.filesize = *((off_t*)(read_package + pck_index));
			//d1.data = (unsigned char*)malloc(c1.filesize); /* Allocating file length not inicialized */
			pck_index += n_bytes; //update to T2
			break;

		case CONTROLT2:
			n_bytes = (unsigned int)read_package[pck_index++]; // read L2, update to V2
			//c1.filename = (char*)malloc(n_bytes + 1); /* Allocating filename memory block not inicialized */
			memcpy(c1.filename, (unsigned char*)&read_package[pck_index + 1], n_bytes + 1); /* Transfering block of memory to a.layer's filename */
			PingOP();
			break;

		default:
			printf("T: %x \n", pck_type);
			printf("T parameter in start control packet couldn't be recognised, moving ahead...\n");
		}
	}

	free(read_package);
	return 0;
}

int receivePacket(unsigned char** buffer, int seqNumber) {

	unsigned char* information;
	int K = 0; // number of octets

	if (llread(applay.fd, information,RECEIVER) < 0) {
		printf("error in llread\n");
		return -1;
	}

	if (information == NULL) {
		printf("receivePacket: information = NULL\n");
		return -1;
	}

	unsigned char Ch = information[0]; // control field
	int N = information[1]; // sequence number

	if (Ch != CONTROLT2) {
		printf("receivePacket: C doesn't indicate data\n");
		return -1;
	}

	if (N != seqNumber) {
		printf("receivePacket: wrong sequence number\n");
		return -1;
	}

	int L2 = information[2];
	int L1 = information[3];
	K = 256 * L2 + L1;

	*buffer = (unsigned char*)malloc(K);
	memcpy((*buffer), (information + 4), K);

	free(information);

	return K;
}
