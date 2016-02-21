/* ======================================================> server1.c 
 * Generic sort of server process for Unix 
 *
 * This is an extremely simple use of sockets for communication.
 * The server opens a socket and then reads messages and prints them out
 * without further ado until the client closes the socket. 
 * =====================================================================
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
   int                sock, clientsock, mlen, addrsize, msgct, chc, chct;
   struct sockaddr_in addr;
   char                ch, buf[80];

   /*
    * Create a socket.
    */

   sock = socket(AF_INET, SOCK_STREAM,0);
   if (sock == -1)
   {   
      perror("opening socket");
      exit(-1);
   }

   /*
    * Bind a name to the socket.  Since the server will bind with
    * any client, the machine address is zero or INADDR_ANY.  The port
    * has to be the same as the client uses.
    */

   addr.sin_family = AF_INET;
   addr.sin_port = htons (32351);
   addr.sin_addr.s_addr = htonl (INADDR_ANY);

   if (bind(sock, (struct sockaddr *) &addr, 
      sizeof (struct sockaddr_in)) == -1) 
   {  
      perror ("on bind");
      exit (-1);
   }

   /*
    * Make the socket available for potential clients.
    */

   if (listen(sock,1) == -1)  
   {  
      perror("on listen");
      exit(-1);
   }

   /*
    * Wait for a client to connect.  When this happens, print out a 
    * message including the address of the client.  Note that clientsock
    * is a socket just like sock, except that it is specifically connected
    * to the client.  sock is used just to get connections.
    */

   addrsize = sizeof(struct sockaddr_in);
   clientsock = accept(sock, (struct sockaddr *) &addr, 
      &addrsize);
   if (clientsock == -1)
   {  
      perror("on accept");
      exit(-1);
   }
   printf("connection made with client ");

   printf ("%s\n", inet_ntoa (addr.sin_addr));

   /*
    * Print four messages from client. Note that the messages are processed
    * one character at a time due to the nature of stream communication.
    * There is no end-of-message type of character, and part of a message
    * could arrive, or serveral messages could arrive together.  Therefore
    * look for the null character ('\0'), which the client has to send.
    */

   msgct = 0;
   while (msgct < 4)
   {
      mlen = recv (clientsock, buf, 80, 0);
      for (chct = 0; chct < mlen; chct++)
      {
         if (*(buf+chct) == '\0')
         {
            printf ("\n");
            msgct++;
         }
         else
            printf ("%c", *(buf+chct));
      }
   }
   printf(" --> all messages read - connection being closed\n");

   /*  
    * Shutdown the socket to insure that it is handled properly, and then
    * close the client socket and also the server socket 
    */

   shutdown (clientsock, SHUT_RDWR);
   close(clientsock);
   close(sock);
}
