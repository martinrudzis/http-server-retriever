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
#include <stdio.h>
#include <errno.h>
#include <sstream>
#include <string>


int main(int argc, char **argv) {
   // Program arguments (specified in command line)
   (void)argc; // Remove unused parameter warning
   std::string serverName = argv[1]; // server host name or IP
   std::string fileName = argv[2]; // name of file to retrieve
   std::string serverPort = argv[3]; // Use port 80 for HTTP

   // Create new socket
   // Load address structs with getaddrinfo()
   struct addrinfo hints, *servinfo; 
   memset(&hints, 0, sizeof(hints)); // 2. Clear struct data initially
   hints.ai_family = AF_UNSPEC; // Use IPv4 or IPv6, don't specify just one
   hints.ai_socktype = SOCK_STREAM; // Use TCP

   // Call getaddrinfor to update servInfo
   getaddrinfo(serverName.c_str(), serverPort.c_str(), &hints, &servinfo); 

   // Open a new socket and establish a connection to the server
   // Make a socket, bind it, and listen on it
   int clientSd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

   // Avoid "Address already in use" error
   int status = connect(clientSd, servinfo->ai_addr, servinfo->ai_addrlen);
   if (status < 0) {
      std::cerr << "Failed to connect to server." << std::endl;
      close(clientSd);
      return -1;
   } 

   // Build our HTTP request using serverName and fileName
   std::string request = "GET /" + fileName + " HTTP/1.1\r\n" + "Connection: close\r\n" + "Host: " + serverName + "\r\n\r\n";
   // std::stringstream ss;
   // ss << "GET /" << fileName << " HTTP/1.1\r\n" << "Host: " << serverName << "\r\n\r\n";
   // std::string request = ss.str();

   // std::cerr << request << std::endl;

   // Send request to server
   // char databuf[] = request.c_str();
   write(clientSd, request.c_str(), strlen(request.c_str()));

   // Read server response 
   // char buffer[1350];
   // read(clientSd, buffer, 1350);
   // // buffer[20] = '\0';

   // std::string response(buffer);
   // std::istringstream input(response); 
   // std::string firstLine;
   // std::getline(input,firstLine);

   // std::cout << "The first line is " << firstLine;

   // std::string response(buffer);
   // std::cerr << response;
   char c;
   while(c != '\n' && read(clientSd, &c, sizeof(c)) > 0) { 
      std::cerr << c;
   }
   close(clientSd); // Close socket
}