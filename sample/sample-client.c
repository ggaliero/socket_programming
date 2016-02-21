/* ========================================================> client1.c 
 * Simple client model for Internet communication using stream sockets.
 * This program simply accesses a server socket and writes a few messages.
 * Then it closes the socket and terminates.
 * ====================================================================
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h> // For exit()
#include <unistd.h> // For close()

int main()
{
   int                sock, addrsize;
   struct sockaddr_in addr;
   unsigned int       in_address;

   /* 
    * Open a socket for Internet stream services.
    */

   sock = socket(AF_INET, SOCK_STREAM,0);
   if (sock == -1)
   {   perror("opening socket");
       exit(-1);
   }

   /*
    * The address structure requires an address family (Internet), a port
    * for the server (32351) and an address for the machine (esus is
    * 153.90.192.1).  Note that anyting that is not defined or a byte
    * should be converted to the appropriate network byte order.  Look
    * in in.h to see the address structure itself.  Just for fun, look at the
    * function net_addr and see if you can convert 153.90.192.1 to
    * the address more easily.
    */

   addr.sin_family = AF_INET;
   addr.sin_port = htons (32351);
   in_address = 127 << 24 | 0 << 16 | 0 << 8 | 1;
   addr.sin_addr.s_addr = htonl (in_address);
   if (connect (sock, (struct sockaddr *) &addr, 
       sizeof (struct sockaddr_in)) == -1)
   {   
      perror("on connect");
      exit(-1);
   } 

   /* 
    * Write a few simple messages. Note that the null character at
    * the end of the string is included in the message length by adding
    * 1 to the total character count of the message.  Obviously this can
    * be done better by using strlen, but it makes a point.
    */ 

   send (sock, "client calling server, do you read me", 38, 0);
   send (sock, "the socket is operational", 26, 0);
   send (sock, "client is not reading the socket, but it could", 47, 0);
   send (sock, "client terminating session", 27, 0);

   /* 
    * Do a shutdown to gracefully terminate by saying - "no more data"
    * and then close the socket -- the shutdown is optional in a one way
    * communication that is going to terminate, but a good habit to get
    * into. 
    */

   if (shutdown(sock, 1) == -1)
   {  
      perror("on shutdown");
      exit(-1);
   }
   printf ("Client is done\n");
   close(sock);
}
