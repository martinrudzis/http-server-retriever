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
#include <fstream>
#include <sstream>
#include <string>
#include <pthread.h>

struct thread_data {
   int sd;
};

bool sendFileContent(std::string& content, const std::string& fileName, int sd) {
   std::string line;
   std::ifstream fileStream;
   fileStream.open(fileName);
   if (fileStream.is_open()) {
      // if content string is empty, then an error code wasn't added
      if (content.length() == 0) {
         content = "HTTP/1.1 200 OK\n\n";
      }
      // Send file
      std::cout << "Successfully opened file" << std::endl;
      while (getline(fileStream, line)) {
         content += line + "\n";
      }
      fileStream.close();
      write(sd, content.c_str(), strlen(content.c_str()));
      close(sd);
      return true;
   }
   return false;
}

void *serverThreadFunction(void *data_param) {
   struct thread_data *data = static_cast<thread_data*>(data_param);
   char buffer[1350];
   read(data->sd, buffer, 1350);
   std::string fileName;
   std::string request(buffer);
   std::string content;

   // Check for valid request
   std::istringstream input(request); 
   std::string firstLine;
   std::getline(input,firstLine);
   int get = firstLine.find("GET ");
   int version = firstLine.find(" HTTP/1.1");
   // std::size_t firstLine = request.find("\n");
   int fName = get + 5;
   // Check that "GET" comes before "HTTP/1.1", that a file is requested, 
   // and that "HTTP/1.1 is on the same line as "GET"
   if (get != -1 && version != -1 && fName < version - 8) {
      // Parse name of requested file
      for (int i = fName; !isspace(request[i]); i++) {
         fileName.push_back(request[i]);
      }
      std::cout << "The file requested is " << fileName << std::endl;
      if (static_cast<int>(fileName.find("../")) != -1) {
         content = "HTTP/1.1 403 Forbidden\n\n";
         sendFileContent(content, "403.html", data->sd); // 403 Forbidden! Bad!
      }
      else if (fileName != "SecretFile.html") {
         // Try to send requested file; if it can't be sent, it doesn't exist
         if (!sendFileContent(content, fileName, data->sd)) {
            content = "HTTP/1.1 404 Not Found\n\n";
            sendFileContent(content, "404.html", data->sd); // 404 Not found
         }
      }
      else {
         content = "HTTP/1.1 401 Unauthorized\n\n";
         sendFileContent(content, "401.html", data->sd); // 401 Unauthorized
      }
   }
   else {
      content = "HTTP/1.1 400 Bad Request\n\n";
      sendFileContent(content, "400.html", data->sd);  // 400 Bad request
   }
   free(data);
   return NULL;
}

int main(int argc, char **argv) {
   // Program arguments (specified in command line)
   (void)argc; // Remove unused parameter warning
   std::string serverPort = argv[1]; // Use port 1041 on linux lab machines
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

   struct sockaddr_storage clientAddr;
   socklen_t clientAddrSize = sizeof(clientAddr);

   while (1) {
      // Accept incoming connection
      int newSd = accept(serverSd, (struct sockaddr *)&clientAddr, &clientAddrSize);
      // Create a new thread to handle connection
      pthread_t newThread;
      struct thread_data *data = new thread_data;
      data->sd = newSd;
      pthread_create(&newThread, NULL, serverThreadFunction, (void*) data);
   }
   return 0;
}