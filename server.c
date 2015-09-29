/*	Server.c 
	TTP3121 TCP/IP Programming(T1 2015/16) 
	************(Members)**************** 
	1) Lee Kuan Zheng	1122701078
	2) Srivekneas Baskaran	1122700398 */

#include "inet.h"

#define PORT 8000
#define MAXSZ 1024
#define MAXSZ1 2048
int main()
{
int sockfd;//to create socket
int newsockfd;//to accept connection
int clilen;
char msg1[MAXSZ1+1];

struct sockaddr_in serverAddress;//server receive on this address
struct sockaddr_in clientAddress;//server sends to client on this address

int n;
char msg[MAXSZ];
int clientAddressLength;
int pid;
int bfrec = 0;

//create socket
sockfd=socket(AF_INET,SOCK_STREAM,0);
//initialize the socket addresses
memset(&serverAddress,0,sizeof(serverAddress));
serverAddress.sin_family=AF_INET;
serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
serverAddress.sin_port=htons(PORT);

//bind the socket with the server address and port
bind(sockfd,(struct sockaddr *)&serverAddress, sizeof(serverAddress));

/* Get the linux's username of that client */ 
	char *usr; 
	usr=(char *)malloc(10*sizeof(char)); 
	usr=getlogin(); 

/* initialize the 'Server' File path */ 
	char str[30]; strcpy(str, "/home/"); strcat(str, usr); strcat(str, "/Server/"); 

	/* Make Directory if path not exist */ 
	struct stat s; 
	if(stat(str, &s) == -1){ 
	mkdir(str, 0700); }


//listen for connection from client maximum 5
listen(sockfd,5);

while(1)
{
//parent process waiting to accept a new connection
printf("\n*****server waiting for client connection:*****\n");
clientAddressLength=sizeof(clientAddress);
newsockfd=accept(sockfd,(struct sockaddr*)&clientAddress,&clientAddressLength);
printf("++.........A client connected......++++++++++++\n");

//child process is created for serving each new clients
pid=fork();
if(pid==0)//child process rec and send
{
 strcpy(msg1,"** Hello, you are connecting to the server please..** \n\nChoose your action...\n1)Send File\t\t2)Download File\n3)Create Directory\t4)Delete Directory\n[type q to quit] : \nEnter (1,2,3,4):\n");
	  send(newsockfd, msg1, MAXSZ1, 0);
//rceive from client
do
{
n=recv(newsockfd,msg,MAXSZ,0);

 if(!strcmp(msg, "1"))
	{
		
		bzero( msg, sizeof(msg));
		recv(newsockfd, msg, MAXSZ1, 0);

		char filename[30];strcpy(filename, "/home/"); strcat(filename, usr); strcat(filename, "/Server/");strcat(filename, msg);
		

		FILE *file;
   	 	file = fopen(filename, "ab"); 
		bzero( msg, sizeof(msg));
		bfrec = recv(newsockfd, msg, MAXSZ1, 0);
		fwrite(msg,1,bfrec,file);
		fclose(file);

		bzero( msg, sizeof(msg));
		strcat(msg,"Managed to send... [Press q to exit]");
	}

 if(!strcmp(msg, "2"))
	{	
		bzero( msg, sizeof(msg));
		strcat(msg,"Please enter q to exit\n\n");

		DIR *directory;
		struct dirent *dir;

		char directoryName[30];strcpy(directoryName, "/home/"); strcat(directoryName, usr); strcat(directoryName, "/Server/");

		if ((directory = opendir (directoryName)) != NULL) {
		
			//print all the files and directories within directory 
 			while ((dir = readdir (directory)) != NULL) {

			strcat(msg, dir->d_name);
			strcat(msg, "\n");
 			}
 		 closedir (directory);
		send(newsockfd, msg, MAXSZ1, 0);
		 
		}else {

  		// could not open directory 
 		 perror ("Directory does not exist...");
  		return EXIT_FAILURE;
		}

	bool check = true; 

	do{

        
	bzero( msg, sizeof(msg));
	recv(newsockfd, msg, MAXSZ1, 0);

	char filename[30];strcpy(filename, "/home/"); strcat(filename, usr); strcat(filename, "/Server/");strcat(filename, msg);
    
	
	FILE *file = fopen(filename, "r");
	if(file==NULL)
        {
            strcpy(msg," No such file in ...Please try again...");
	    send(newsockfd, msg, MAXSZ1, 0);  
	    check = false; 
	   
        }  
	
	if(check == true )
	{
	bzero( msg, sizeof(msg));
	int nread = fread(msg,1,256,file);
	send(newsockfd, msg, nread, 0);
	}

	bzero( msg, sizeof(msg));
	strcpy(msg,"Managed to Download... [Press q to exit ]");

	}while(check == false);
	}
		
	 
	if(!strcmp(msg, "3"))
	{
		bzero( msg, sizeof(msg));
		strcat(msg,"The Directory is created... [Press q to exit]");
	}

	if(!strcmp(msg, "4"))
	{
		bzero( msg, sizeof(msg));
		strcat(msg,"The Directory is deleted... [Press q to exit]");
	}
if(n==0)
{
close(newsockfd);
break;
}
msg[n]=0;
send(newsockfd,msg,MAXSZ1,0);

}while(strcmp(msg1, "q"));//close the do while loop
exit(0);
}
else
{
close(newsockfd);//sock is closed BY PARENT
}
}//close exterior while loop

return 0;
}

