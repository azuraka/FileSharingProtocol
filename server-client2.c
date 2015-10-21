#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/socket.h>


int  ChildProcess(void);                /* child process prototype  */
int  ParentProcess(void);               /* parent process prototype */

void  main()
{
     pid_t  pid;

     pid = fork();
     if (pid == 0)
          ChildProcess();
     else
          ParentProcess();
}

// Server
int ParentProcess(void){
     
     int listenSocket = 0;    // This is my server's socket which is created to                                    //   listen to incoming connections
     int connectionSocket = 0;
     struct sockaddr_in serv_addr;      // This is for addrport for listening

     // Creating a socket

     listenSocket = socket(AF_INET,SOCK_STREAM,0);
     if(listenSocket<0)
     {
          printf("ERROR WHILE CREATING A SOCKET\n");
          return 0;
     }
     else
          printf("[SERVER] SOCKET ESTABLISHED SUCCESSFULLY\n");

     // Its a general practice to make the entries 0 to clear them of malicious entry

     bzero((char *) &serv_addr,sizeof(serv_addr));

     // Binding the socket

     int portno = 5006;
     serv_addr.sin_family = AF_INET;    //For a remote machine
     serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
     serv_addr.sin_port = htons(portno);

     if(bind(listenSocket,(struct sockaddr * )&serv_addr,sizeof(serv_addr))<0)
          printf("ERROR WHILE BINDING THE SOCKET\n");
     else
          printf("[SERVER] SOCKET BINDED SUCCESSFULLY\n");

     // Listening to connections

     if(listen(listenSocket,10) == -1)  //maximum connections listening to 10
          printf("[SERVER] FAILED TO ESTABLISH LISTENING \n");
     
     printf("[SERVER] Waiting fo client to connect....\n" );

     // Accepting connections
     while((connectionSocket=accept(listenSocket , (struct sockaddr*)NULL,NULL))<0);

     // NULL will get filled in by the client's sockaddr once a connection is establised

     printf("[CONNECTED]\n");
     sleep(1);
     char buffer2[1024];
     char content[10000];
     char result[1024];
     bzero(buffer2,1024);
     if(recv(connectionSocket,buffer2,1023,0)<0)
          printf("ERROR whiile reading from Client\n");
     if(recv(connectionSocket,content,9999,0)<0)
          printf("ERROR whiile reading from Client\n");
     // temporary for IndexGet --longlist (Shared folder is current folder)
     if (strlen(buffer2)==20){
          FILE *fp;
          char path[1035];
          fp = popen("ls -l", "r");
          if (fp == NULL) {
               printf("Failed to run command\n" );
               exit(1);
          }
          while (fgets(path, sizeof(path)-1, fp) != NULL) {
               strcat(buffer2,path);
          }
          send(connectionSocket,buffer2,strlen(buffer2),0);
          bzero(buffer2,1024);
          pclose(fp);
     }
     else{
          char filename[100];
          int l,i=0;
          l=strlen(buffer2);
          filename[0]='2';
          for(i=11;i<l-1;i++){
               filename[i-10]=buffer2[i];
          }
          FILE *fp;
          if(i>0){
               fp = fopen(filename,"wb");
               fprintf(fp, "%s",content );
          }
     }
     return 0;
}

// Client
int ChildProcess(void){
     int ClientSocket = 0;
     struct sockaddr_in serv_addr;

     // Creating a socket

     ClientSocket = socket(AF_INET,SOCK_STREAM,0);
     if(ClientSocket<0)
     {
          printf("ERROR WHILE CREATING A SOCKET\n");
          return 0;
     }
     else
          printf("[CLIENT] Socket created \n");

     int portno = 5005;

     serv_addr.sin_family = AF_INET;
     serv_addr.sin_port = htons(portno);
     serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

     //Connection Establishment

     while(connect(ClientSocket,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0);
     sleep(1);
     char buffer[1024];
     bzero(buffer,1024);
     printf("\nEnter Command: ");
     fgets(buffer,1023,stdin);
     if(send(ClientSocket,buffer,strlen(buffer),0)<0)
          printf("ERROR while writing to the socket\n");
     if(recv(ClientSocket,buffer,1023,0)<0)
          printf("ERROR while reading from the socket\n");
     else
          printf("%s\n",buffer );
     return 0;
}