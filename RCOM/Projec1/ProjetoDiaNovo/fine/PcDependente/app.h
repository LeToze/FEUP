
int ControlPackage(int c);
int setFile();
int PingOP();
int DataPackage(int datalength);
int sendData();
int sendControlPacket(unsigned char control_byte);
int receiveData();
int sendPacket(int seqNumber, unsigned char* buffer, int length);
int receiveControlPacket();
int receivePacket(unsigned char** buffer, int seqNumber);
void go(int mode);

struct applicationLayer{
int fileDescriptor;
int status;
int fd;
int fragmentSize;
};
struct applicationLayer applay;
