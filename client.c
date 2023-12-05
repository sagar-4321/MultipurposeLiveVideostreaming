// client
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_BUFFER_SIZE 1024

FILE *fp1; // File pointer declaraction
FILE *fp2;
FILE *fp3;

void error_handler(char *message)
 {
  perror(message);
  exit(1);
}

void send_file(FILE *file, int socket, struct sockaddr_in server_address,socklen_t server_address_len) 
{
  char buffer[MAX_BUFFER_SIZE];
  size_t bytesRead;
  // size_t 
  // reading 1 byte of data at a time from the file and storing it into character buffer

  while ((bytesRead = fread(buffer,1, sizeof(buffer),file))>0) // fp is the file pointer
  {
    /
    if (sendto(socket, buffer, bytesRead, 0,(struct sockaddr *)&server_address,server_address_len) == -1) 
    {
      error_handler("sendto() failed");
    }
   
     
  }

  // Send an empty packet to signal the end of the file
  if (sendto(socket, NULL, 0, 0, (struct sockaddr *)&server_address,server_address_len) == -1) 
  {
    error_handler("sendto() failed");
  }
}

int main(int argc, char *argv[]) {
  if (argc != 6) {

    fprintf(stderr, "Usage: %s <filename> <server_ip> <server_port>\n",argv[0]);
    exit(1);
  }

  char *filename1 = argv[1];
  char *filename2 = argv[2];
  char *filename3 = argv[3];
  char *server_ip = argv[4];
  int server_port = atoi(argv[5]); // port number converted into integer

  fp1 = fopen(filename1,"rb"); 
  fp2 = fopen(filename2,"rb"); 
  fp3 = fopen(filename3,"rb"); 
  if(!fp1 || !fp2 || !fp3)
   {
    error_handler("File not found");
   }

  // Create a UDP socket

  int udp_socket =socket(AF_INET,SOCK_DGRAM, 0); 
  
  if (udp_socket == -1)
   {
    error_handler("socket() failed");
   }

  // Set up the server address structure
  struct sockaddr_in server_address;
  memset(&server_address,0,sizeof(server_address)); // clearing the memory space of server_address
                                  // variable
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = inet_addr(server_ip); 
 
 
  server_address.sin_port = htons(server_port); 

  // Send the file
  send_file(fp1, udp_socket, server_address,sizeof(server_address)); // calling the defined function
  /* function will transfer the file pointed by file pointer fp to the
   * server_address through UDP socket */
   send_file(fp2,udp_socket,server_address,sizeof(server_address));
   send_file(fp3,udp_socket,server_address,sizeof(server_address));         

  fclose(fp1);  
  fclose(fp2);  
  fclose(fp3);        // closing the file
  close(udp_socket); // closing the UDP socket
  return 0;
}

