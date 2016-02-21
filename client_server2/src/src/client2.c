
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_LEN 150

void deleteCharacter(char *strg);
void getMovie(char *movie);

int main() {

   struct sockaddr_in server;
   int sockfd;
   int connected;
   int sended;
   int loop=0;
   int rec;
   char movie[MAX_LEN];
   char received[MAX_LEN];



   sockfd=socket(AF_INET, SOCK_STREAM, 0);  // Creates a socket connection

   /* If success the flow of the program continues */
   
   if(sockfd < 0) {
   
      perror("simple-talk: socket");
      exit(1);
   }

   //bzero(&server, sizeof(server)); // Sets the number of bytes to 0 in the socket address
   memset(&server, 0, sizeof(server));
   server.sin_port=htons(12345); // Conversion between host byte order and network byte order
   server.sin_family=AF_INET;
   inet_pton(AF_INET, "127.0.0.1", &(server.sin_addr)); // Conversion from presentation IPV4 address to numeric

   connected=connect(sockfd,(struct sockaddr *) &server, sizeof(server));  // Connects with server
      
   if(connected<0){
         
      perror("simple talk: connection failed");
      exit(1);
   }
         
   while(!loop){

      getMovie(movie);
   
      if(strcmp(movie,"exit")== 0){ 
      
         loop=1;  // Brokes the loop

      }else{

       sended=write(sockfd, movie, strlen(movie));
	
         if(sended < 0){

            fprintf(stdout, "Error sending\n");

         }else{

            rec=read(sockfd, received, MAX_LEN);
            received[rec]='\0';
     
            if(rec < 0){
  
               fprintf(stdout, "Error receiving\n");

            }else if(rec==0){
               fprintf(stdout, "Nothing has been received\n");
            }else{

               
               fputs(received,stdout);
                 
            }

         }
      }
   }

   close(sockfd);
   
   return 0;
                                      
}


  /*********************************************************************************************
  * FUNCTION: getMovie
  * DESCRIPTION: Gets a char string by the stdin buffer
  * INPUT PARAMETERS: No one
  * OUTPUT PARAMETERS:
  * movie: Points to the char string introduced by stdin buffer uploaded without the char '/n'
  *********************************************************************************************/
   void getMovie(char *movie){

      fprintf(stdout,"\nIntroduce the title of the movie: ");

      fgets(movie, MAX_LEN, stdin);

      fflush(stdin);

      deleteCharacter(movie);
   }

  /*********************************************************************************************
  * FUNCTION: deleteCharacter
  * DESCRIPTION: Searchs the character '/n' in the string strg. If success it replace it by
  * '/0'. Otherwise does not make anything.
  * INPUT PARAMETERS:
  * strg point at the begining of a string
  * OUTPUT PARAMETERS:
  * strg string updated without '/n'
  **********************************************************************************************/
   void deleteCharacter(char *strg){
   
      char *aux;

      aux=strchr(strg, '\n');

      if(aux!= NULL){
      
         *aux='\0';
      }
   }
   
