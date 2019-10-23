#include "datalink.h"
struct applicationLayer {
	int fileDescriptor; /*Descritor correspondente à porta série*/
	int status; /*TRANSMITTER | RECEIVER*/
};
 struct applicationLayer applay;

struct controlpack {
	int filesize;
	char filename[20];
	char data[255];
};

struct datapack {
	int SeqNum;
	int DataSize[2];
	char data[255];
};

char DATA[255];
struct controlpack c1;
struct datapack d1; 

int ControlPackage(int c){
	int j = 0;
	c1.filesize = 0;
	strcpy(c1.filename,"penguin");
	c1.data[0] = c;// 2 se start,3 se end
	c1.data[1] = 1;
	for ( j = 0;j < strlen(c1.filename);j++) {
		c1.data[j+3] = c1.filename[j];
	}
	c1.data[2] = strlen(c1.data[3]) + 1;
	c1.data[j+4] = 0;
	c1.data[j+5] = c1.filesize;
	c1.data[j+6] = strlen(c1.data[3]) + 1;

	if(llwrite(applay.fileDescriptor,c1.data,45,TRANSMITTER) < 0){
		printf("Error 45\n");
		return -1;
	}
	
return 0;
}

int PingOP(){
	if(applay.fileDescriptor=open(c1.filename,O_CREAT|O_WRONLY|O_APPEND, S_IWUSR|S_IRUSR) < 0){
		perror("Error opening the file");
		return -1;
	}
	
	return 0;
}

int DataPackage(int datalength, char buffer[]) {
	int i = 0;
	d1.data[0] = 1;
	d1.SeqNum = 0;
	d1.data[1]= 0;
	d1.data[2] = datalength % 256;
	d1.data[3] = datalength / 256;

	memcpy(&d1.data[4],buffer,datalength);
	if(llwrite(applay.fileDescriptor,d1.data,46,TRANSMITTER) < 0){
		printf("Error 46\n");
		return -1;
	}
	
	/*for (i = 0;i < datalength;i++) {
		d1.data[2 + j] = DATA[i];
	}*/

	return 0;
}

int sendControlPacket(unsigned char control_byte) {

	if (control_byte == CONTROLSTART) {
		if (setFile() < 0) {
			printf("Error getting the file\n");
			return -1;
		}
	}

	struct stat f_information;

	if (fstat(al.fileDescriptor, &f_information) < 0) {
		perror("Couldn't obtain information regarding the file");
		return -1;
	}

	off_t f_size = f_information.st_size; /* total size in bytes, in signed integer */
	st.filesize = f_size;
	unsigned int l1 = sizeof(f_size);
	unsigned int l2 = strlen(al.filename) + 1;

	int startpackage_len = 5 + l1 + l2;

	unsigned char startpackage[startpackage_len];

	startpackage[0] = control_byte; // CONTROLSTART or CONTROLEND
	startpackage[1] = CONTROLT1;
	startpackage[2] = l1;
	*((off_t*)(startpackage + 3)) = f_size;
	startpackage[3 + l1] = CONTROLT2;
	startpackage[3 + l1 + 1] = l2; /* +1 from CONTROLT2 */

	strcat((char*)startpackage + 5 + l1, al.filename);

	if (llwrite(al.fd, startpackage, startpackage_len) < 0) {
		printf("Couldn't write control package.\n");
		return -1;
	}

	return 0;
}

int sendPacket(int seqNumber, unsigned char* buffer, int length) {

	int totalLength = length + 4;
	unsigned char dataPacket[totalLength];

	dataPacket[0] = CONTROLDATA;
	dataPacket[1] = seqNumber;
	dataPacket[2] = length / 256;
	dataPacket[3] = length % 256;

	memcpy(&dataPacket[4], buffer, length);

	if (llwrite(al.fd, dataPacket, totalLength) < 0) {
		printf("Error sending data packet");
		return -1;
	}

	return 0;
}

int receiveControlPacket() {
	unsigned char* read_package;
	unsigned int package_size = llread(al.fd, &read_package);
	int i;

	if (package_size < 0) {
		perror("Couldn't read linklayer whilst receiving package.");
		return -1;
	}

	int pck_index = 0;

	if (read_package[pck_index] == CONTROLEND) {
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

			al.fileSize = *((off_t*)(read_package + pck_index));
			st.filesize = al.fileSize;
			al.file_data = (unsigned char*)malloc(al.fileSize); /* Allocating file length not inicialized */
			pck_index += n_bytes; //update to T2
			break;

		case CONTROLT2:
			n_bytes = (unsigned int)read_package[pck_index++]; // read L2, update to V2
			al.filename = (char*)malloc(n_bytes + 1); /* Allocating filename memory block not inicialized */
			memcpy(al.filename, (char*)&read_package[pck_index + 1], n_bytes + 1); /* Transfering block of memory to a.layer's filename */
			getFile();
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

	if (llread(al.fd, &information) < 0) {
		printf("error in llread\n");
		return -1;
	}

	if (information == NULL) {
		printf("receivePacket: information = NULL\n");
		return -1;
	}

	unsigned char C = information[0]; // control field
	int N = information[1]; // sequence number

	if (C != CONTROLDATA) {
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