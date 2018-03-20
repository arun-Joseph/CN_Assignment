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

int main(int argc,char *argv[])
{
	char buffer[1024];
	int oldsocket,newsocket;
	oldsocket=socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in server,client;
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(argv[2]));
	server.sin_addr.s_addr=inet_addr(argv[1]);
	bind(oldsocket,(struct sockaddr*)&server,sizeof(server));
	
	int k,t,i,j;
	socklen_t h;
	
	
	Frame recv_frame;
	Frame send_frame;
	int counter=0;

	while(1)
	{
  		h=sizeof(client);
  		int  recv_size=recvfrom(oldsocket,&recv_frame,sizeof(Frame),0,(struct sockaddr*)&client,&h);
  		if((recv_size > 0)&&(recv_frame.seq_no==counter))
	  	{
			printf("Frame received correctly with sequence number %d and data %s",counter,recv_frame.data);
			send_frame.seq_no=counter;
			send_frame.ack=counter;
			sendto(oldsocket,&send_frame,sizeof(Frame),0,(struct sockaddr*)&client,sizeof(client));
			printf("ACK %d Sent\n",counter);
			counter=(counter+1)%2;
	  	}
	  	else if((recv_size > 0)&&(recv_frame.seq_no!=counter))
	  	{
//
	   		send_frame.seq_no=recv_frame.seq_no;
	   		send_frame.ack=recv_frame.seq_no;
	   		sendto(oldsocket,&send_frame,sizeof(Frame),0,(struct sockaddr*)&client,sizeof(client));
	   		printf("ACK %d Sent\n",send_frame.ack);
	  	}
	
	}

close(oldsocket);
return 0;
}
