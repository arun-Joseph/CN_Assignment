#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>


typedef struct frame
{
	int seq_no;
	int ack;
	char data[1024];
}Frame;



int main(int argc,char *argv[]){

	char buffer[1024];
	int oldsocket;
	struct sockaddr_in server;
	Frame send_frame;
	Frame recv_frame;
	int counter=0;
	int send=1;
	fd_set readfds;
	struct timeval timer;
	int selector;
	int x=0;
	oldsocket=socket(PF_INET,SOCK_DGRAM,0);	
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(argv[2]));
	server.sin_addr.s_addr=inet_addr(argv[1]);
	memset(server.sin_zero,'\0',sizeof(server.sin_zero));
	/*
	0---->ack;
	1-->    seq;
	*/
	FD_SET(oldsocket,&readfds);

	while(1)
	{
		socklen_t h=sizeof(server);
		if(send==1) //sender is sending 
		{
			x=0;
			send_frame.seq_no=counter;
			send_frame.ack=1;

			printf("Enter data:");
			
			fgets(buffer,1024,stdin);
			strcpy(send_frame.data,buffer);
			while(x==0)
			{
				FD_SET(oldsocket,&readfds);
				sendto(oldsocket,&send_frame,sizeof(Frame),0,(struct sockaddr*)&server,sizeof(server));
				printf("Frame sent\n");
				send=0;
				timer.tv_sec=2;
				timer.tv_usec=0;
				selector=select(oldsocket+1,&readfds,NULL,NULL,&timer);
				if(selector<0)
					printf("select error ....\n");
				else if(selector==0)
					printf("Time out ......\n"); 
				else
				{
					x=1;//correct ack is recieved
				}
			}
		}

	int recv_size=recvfrom(oldsocket,&recv_frame,sizeof(Frame),0,(struct sockaddr*)&server,&h);
	if((recv_size > 0)&&(recv_frame.ack==counter))
	{
		printf("ACK %d recieved\n",counter);
		send=1;
		counter=(counter+1)%2;
	}
	else
		printf("ACK Not received\n");
	}
	close(oldsocket);
	
	return 0;
}
