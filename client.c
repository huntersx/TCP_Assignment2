/*	client.c 
	TTP3121 TCP/IP Programming(T1 2015/16) 
	************(Members)**************** 
	1) Lee Kuan Zheng	1122701078
	2) Srivekneas Baskaran	1122700398 */


#include "inet.h"


#define PORT 8000
#define SERVER_IP "127.0.0.1"
#define MAXSZ 1024
int main()
{
int sockfd;//to create socket

struct sockaddr_in serverAddress;//client will connect on this

int n;
int bfreceiv = 0;
char mmo[MAXSZ+1];
char cr8[25];
char dlt[25];

static struct sigaction act;

void catchinterrupt(int);

act.sa_handler = catchinterrupt;
sigfillset(&(act.sa_mask));

sigaction(SIGINT, &act, (void *) 0);


//create socket
sockfd=socket(AF_INET,SOCK_STREAM,0);
//initialize the socket addresses
memset(&serverAddress,0,sizeof(serverAddress));
serverAddress.sin_family=AF_INET;
serverAddress.sin_addr.s_addr=inet_addr(SERVER_IP);
serverAddress.sin_port=htons(PORT);

/* Get the linux's username of that client */ 
connect(sockfd,(struct sockaddr *)&serverAddress,sizeof(serverAddress));

/* Get the user name */ 
	char *usr; 
	usr=(char *)malloc(10*sizeof(char)); 
	usr=getlogin(); 

/* initialize the 'client' File path */ 
	char str[30]; strcpy(str, "/home/"); strcat(str, usr); strcat(str, "/Client/"); 

	/* Make Directory if path not exist */ 
	struct stat s; 
	if(stat(str, &s) == -1){ 
	mkdir(str, 0700); }

//send to sever and receive from server
do
{
bzero( mmo, sizeof(mmo));
recv(sockfd,mmo,MAXSZ,0);

printf("%s\n",mmo);


gets(mmo);

send(sockfd,mmo,MAXSZ,0);

 if(!strcmp(mmo, "1"))
{

	DIR *directory;
	struct dirent *dir;
		//path of Client directory
	char directoryName[30];strcpy(directoryName, "/home/"); strcat(directoryName, usr); strcat(directoryName, "/Client/");

	if ((directory = opendir (directoryName)) != NULL) {
		
		printf("\n[List of files in Client Directory]\n");
  		// print all the files of that directory 
  		while ((dir = readdir (directory)) != NULL) {

		printf("%s\n", dir->d_name);    }

 		closedir (directory);
	}

	printf("\nPlease simply create a filename or enter one of the filename from above including extension[eg:  a.txt ]\nTo send to [Server]:  ");
	
	bzero( mmo, sizeof(mmo));
	
	gets(mmo);
	send(sockfd,mmo, MAXSZ, 0);

	char NamaFile[40];strcpy(NamaFile, "/home/"); strcat(NamaFile, usr); strcat(NamaFile, "/Server/");strcat(NamaFile, mmo);

	FILE *file;
    	file = fopen(NamaFile, "r"); 

	bzero( mmo, sizeof(mmo));
	int nread = fread(mmo,1,256,file);
    	bfreceiv = recv(sockfd, mmo, MAXSZ, 0);
    	fwrite(mmo,1,bfreceiv,file);

}


else if(!strcmp(mmo, "2"))
{
	
printf("Please simply write down a filename to be downloaded to [Client] file\n\n" );

	bzero( mmo, sizeof(mmo));
	recv(sockfd, mmo, MAXSZ, 0);
	printf("\n%s\n", mmo); 
	gets(mmo);
	send(sockfd,mmo, MAXSZ, 0);
	
   	char NamaFile[40]; strcpy(NamaFile, "/home/");  strcat(NamaFile, usr); strcat(NamaFile, "/Client/"); strcat(NamaFile, mmo);
    
    	FILE *file;
    	file = fopen(NamaFile, "ab"); 

		if(NULL == file)
    	{
        	printf("Error opening file");
        
    	}
    
    	bzero( mmo, sizeof(mmo));
    
    	bfreceiv = recv(sockfd, mmo, MAXSZ, 0);
    	fwrite(mmo,1,bfreceiv,file);
   
}



else if(!strcmp(mmo, "3"))
{
	printf("Please name the Directory which is to create: ");
	scanf("%s", cr8);

	/* create the directory */ 
	char cr8Direc[40]; strcpy(cr8Direc, "/home/"); strcat(cr8Direc, usr); strcat(cr8Direc, "/"); strcat(cr8Direc, cr8);

	/* Make one directory if the path no exist */ 
	struct stat s; 
	if(stat(cr8Direc, &s) == -1){ 
	mkdir(cr8Direc, 0700); } 
}


else if(!strcmp(mmo, "4"))
{
	printf("Please name the Directory which is to delete: ");
	scanf("%s", dlt);

	/* delete the path of that directory*/ 
	char dltDirec[40]; strcpy(dltDirec, "/home/"); strcat(dltDirec, usr); strcat(dltDirec, "/"); strcat(dltDirec, dlt);

	/* Delete all the files of that directory */
	char slctSubDirec[60];strcpy(slctSubDirec, "exec rm -r ");strcat(slctSubDirec, "/home/"); strcat(slctSubDirec, usr); 
	strcat(slctSubDirec, "/"); strcat(slctSubDirec, dlt);strcat(slctSubDirec, "/*"); 

	/* Delete the exist path*/ 
	struct stat s; 
	if(stat(dltDirec, &s) != -1){
	system(slctSubDirec);
	rmdir(dltDirec); } 
   }

 }while (strcmp(mmo, "q"));
close(sockfd);
  return 0;
}

void catchinterrupt(int signo){
	printf("\n[ Interrupt **** signal.....]\n");
}

