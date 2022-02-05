#include <sys/types.h> // socket, bind
#include <sys/socket.h> // socket, bind, listen, inet_ntoa
#include <netinet/in.h> // htonl, htons, inet_ntoa
#include <arpa/inet.h> // inet_ntoa
#include <netdb.h> // gethostbyname
#include <unistd.h> // read, write, close
#include <strings.h> // bzero
#include <netinet/tcp.h> // SO_REUSEADDR
#include <sys/uio.h> // writev
#include <cstring>
#include <iostream>
#include <pthread.h>
#include <sys/time.h> 

const int BUFSIZE = 1500;

struct thread_data {
   int sd;
};

void *serverThreadFunction(void *data_param) {
   struct thread_data *data = static_cast<thread_data*>(data_param);
   char databuf[BUFSIZE]; // Allocate buffer
   int count = 0;

   read(data->sd, databuf, BUFSIZE;

   write(data->sd, &count, sizeof(count)); // Send number of reads to client as reponse

   close(data->sd);

   free(data);
   return NULL;
}

int main(int argc, char **argv) {
// Program arguments (specified in command line)
   std::string serverPort = argv[1]; // server port number
   int iterations = std::stoi(argv[2]); // number of reads
// Create TCP socket listening on port
   // Load address structs with getaddrinfo()
   struct addrinfo hints, *res; 
   memset(&hints, 0, sizeof(hints)); // 2. Clear struct data initially
   hints.ai_family = AF_UNSPEC; // Use IPv4 or IPv6, don't specify just one
   hints.ai_socktype = SOCK_STREAM; // Use TCP
   hints.ai_flags = AI_PASSIVE; // Auto-fill my IP

   // Call getaddrinfor to update res
   getaddrinfo(NULL, serverPort.c_str(), &hints, &res);

   // Create socket
   int serverSd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

   if (serverSd < 0) {
      std::cout << "Error connecting." << std::endl;
   } 

   // Avoid "address already in use" error
   const int yes = 1;
   setsockopt(serverSd, SOL_SOCKET, SO_REUSEADDR, (char *)&yes, sizeof(yes));

// Bind socket
   if (bind(serverSd, res->ai_addr, res->ai_addrlen) < 0) {
      std::cout << "Error binding socket." << std::endl;
   }

   listen(serverSd, 50); // Listen to up to 50 concurrent connections

// Accept incoming connection
   struct sockaddr_storage clientAddr;
   socklen_t clientAddrSize = sizeof(clientAddr);
   while (1) {
      int newSd = accept(serverSd, (struct sockaddr *)&clientAddr, &clientAddrSize);
      // Create a new thread to handle connection
      pthread_t newThread;
      struct thread_data *data = new thread_data;
      data->iterations = iterations;
      data->sd = newSd;
      pthread_create(&newThread, NULL, serverThreadFunction, (void*) data);
   }
}