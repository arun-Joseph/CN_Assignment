/****************** CLIENT CODE ****************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char * argv[]){
  int clientSocket;
  //int buffer[BUFFER_SIZE];
  int n;
  struct sockaddr_in serverAddr;
  socklen_t addr_size;
  int connect_stat;

  /*---- Create the socket. The three arguments are: ----*/
  /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  /*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;
  /* Set port number, using htons function to use proper byte order */
  serverAddr.sin_port = htons(atoi(argv[2]));
  /* Set IP address to localhost */
  serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
  /* Set all bits of the padding field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
  /*---- Connect the socket to the server using the address struct ----*/
  addr_size = sizeof serverAddr;
  connect_stat = connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
  if(connect_stat == 0)
    printf("Connection success\n");
  else{
    perror("Connection error");
    close(clientSocket);
    exit(0);
  }
  /*---- Read the message from the server into the buffer ----*/
 // fgets(buffer,BUFFER_SIZE,stdin);
  scanf("%d",&n);
  write(clientSocket,&n,4);
  int array[n];
  printf("Enter the elements\n");
  for(int i = 0 ;i < n;i++){
    scanf("%d",&array[i]);
  }
  send(clientSocket,array,sizeof array,0);
  recv(clientSocket, array, sizeof array, 0); 
  printf("Sorted array\n");
  for(int i =0;i<n;i++){
    printf("%d\n", array[i]);
  }
  /*---- Print the received message ----*/  

  return 0;
}
