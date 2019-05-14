#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>


void scanplease(int port, char host[]);
 
int main(int argc, char **argv) {
 
 char hostname[100];
 char *p;
 int ports[5];
 int i = 0;
 int var;
 char token[] = ",";   
  
 if (argc < 2) {
       fprintf(stderr,"[+]usage: %s <hostname> <port,port,port...>\n", argv[0]);
       exit(0);
    }
     
    p = strtok(argv[2], token);
    strcpy(hostname, argv[1]);
    
 while(p != NULL) {
  sscanf(p, "%d", &var);
  ports[i++] = var;
  p = strtok(NULL, token);
 }
  
 for(i=0; i< (sizeof(ports)/sizeof(ports[0])); i++) {
  fprintf(stdout, "\n[+]testing port: %d\n", ports[i]);
  scanplease(ports[i], hostname);
 }
 return 0;
}
 
void scanplease(int port, char host[]) {
 
    int sock, n;
    struct hostent *server;
    struct sockaddr_in serv_addr;
     
    char buffer[4096];
  
    server = gethostbyname(host);
     
 sock = socket(AF_INET, SOCK_STREAM, 0);

 if(sock < 0) {
  fprintf(stderr, "[-]error creating socket");
  return;
 }
  
 bzero((char *) &serv_addr, sizeof(serv_addr));
 serv_addr.sin_family = AF_INET;
 // AF_UNIX for Unix style socket
  
 bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
 serv_addr.sin_port = htons(port);
  
 n = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
 sleep(2);
 if(n < 0) {
  fprintf(stderr, "[-]error connecting to port\n");
  return;
 }
  
 memset(buffer, 0, sizeof(buffer));
 strcpy(buffer, "cöp\r\n");
  
 n = write(sock, buffer, strlen(buffer));
 if(n < 0) {
  fprintf(stderr, "[-]error writing (port closed maybe?!)\n");
  return;
 }
  
 bzero(buffer, 4096);
 n = read(sock, buffer, 4096);
 if(n < 0) {
  fprintf(stderr, "[-]error reading (port closed maybe?!)\n");
  return;
 }
  
 fprintf(stdout,"[*]%s\n", buffer);
 close(sock);
     
}
