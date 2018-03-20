#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

int cmpfn(const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void* sortFun(void* socketDesc){
		int n,sockFd = *(int*)socketDesc,recvBytes;
		read(sockFd,&n,4);
		int array[n];
		recvBytes = recv(sockFd, array, sizeof array, 0);
		qsort(array,n,sizeof(int),cmpfn);
		send(sockFd, array, sizeof array, 0);
		printf("Sorted array:\n");
		for(int i=0;i<n;i++){
			printf("%d\n",array[i]);
		}
	}
#define BUFFER_SIZE 50
int main(int argc, char* argv[]){
	int sockFd, newSockFd;
	struct sockaddr_in servAddr;
	struct sockaddr_storage servStorage;
	socklen_t addrSize;
	// buffer[BUFFER_SIZE];

	sockFd = socket(PF_INET,SOCK_STREAM,0);
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(atoi(argv[2]));
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	memset(servAddr.sin_zero,'\0', sizeof servAddr.sin_zero);
	bind(sockFd, (struct sockaddr *)&servAddr, sizeof(servAddr));

	if(listen(sockFd,5) == 0){
		printf("Listening...\n");
	}else{
		perror("Error connecting\n");
	}

	addrSize = sizeof servStorage;
	pthread_t thread_id;

	while(newSockFd= accept(sockFd,(struct sockaddr *) &servStorage, &addrSize)){
		puts("Connection accepted");
		if( pthread_create( &thread_id , NULL , sortFun, (void*) &newSockFd) < 0){
			perror("could not create thread");
			return 1;
		}
		puts("Handler assigned");
		
	}
	if(newSockFd < 0){
		perror("Accept failed");
		return 1;
	}
	close(newSockFd);

	return 0;
}