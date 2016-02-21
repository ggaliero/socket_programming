

#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 100


void deleteCharacter(char *strg);
int search(char *movie, char *info);

int main() {

   struct sockaddr_in client;
   struct sockaddr_in server;
   socklen_t len_client;
   int sockfd;
   int bindfd;
   int recv;
   char input_buff[MAX_LEN];
   char output_buff[MAX_LEN];

   sockfd=socket(AF_INET, SOCK_DGRAM, 0);

   /* If success the flow of the program continues */
   
   if(sockfd < 0) {
   
      perror("simple-talk: socket");
      exit(1);
   }
   
   memset(&client, 0, sizeof(client));
   memset(&server, 0, sizeof(server));
   server.sin_family=AF_INET;
   server.sin_port=htons(12345);
   inet_pton(AF_INET, "127.0.0.1", &(server.sin_addr));
   
   bindfd=bind(sockfd,(struct sockaddr *) &server, sizeof(server));
   if(bindfd < 0){
      perror("simple-talk:bind");
      exit(1);
   }

   len_client=sizeof(struct sockaddr_in);
   while(1){

         while((recv=recvfrom(sockfd, input_buff, MAX_LEN, 0, (struct sockaddr *) &client, &len_client)) > 0){
            input_buff[recv]='\0';

            if(strcmp(input_buff,"exit")==0){ 
               break;
            }else{
               search(input_buff,output_buff);

               sendto(sockfd, output_buff,strlen(output_buff), 0, (struct sockaddr *) &client, len_client);
            }
         }
      }
      close(sockfd);
   return 0;
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

/**********************************************************************************************
* FUNCTION: search
* DESCRIPTION: Reads the file movie.txt and get the information about the movie required.
* INPUT PARAMETERS:
* movie the parameter which we want to get information.
* OUTPUT PARAMETERS:
* info the info read in the file
* RESTURN:1 if success, 0 resoults not found, -1 if ERROR.
*
***********************************************************************************************/

int search(char *movie, char *info){
   int res=0;
   char aux[MAX_LEN];
   char *include;
   FILE *pfile;
   pfile=fopen("movie.txt","r");
   if(pfile==NULL){
      res=-1;
   }else{
      do{
         fgets(aux,MAX_LEN,pfile);
         deleteCharacter(aux);
         include=strstr(aux,movie);
         if(include!=NULL){
            strcpy(info,aux);
            res=1;
         }
      }while(!feof(pfile)  && res!=1);

      if(res==0){

         strcpy(info,"No results found");
      }
     
      fclose(pfile);
   }
   return res;
}
   
