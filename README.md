Documentation

Retriever
Methods:
Main: Accepts the remote host to connect to, the file to request, and the remote port number as command line arguments. Creates a socket that connects to the remote host and writes an HTTP GET request to it. It then reads the server response character-by-character and prints the server reply to the console.
Key Variables:
ServerName: domain name or IP of the remote host; first command line argument;
FileName: name of the file to request that will be added to the HTTP Get  request; second command line argument.
ServerPort: port number of the remote server to connect with; third command line argument.
Compilation:
Compile using the makefile as described in the “Makefile” section.
Execution:
To run individually, use ./retriever [serverName] [fileName] [serverPort]

Server
Methods:
SendFileContent: Accepts a string reference as a response to send to the client, the name of the file to send, and the server socket descriptor value as parameters. If the response does not have an error code, it adds the 200 OK reply and any file content to send to the string, writes it to the socket, then closes the socket.
ServerThreadFunction: Reads the client request and interprets the appropriate response, (i.e. whether to throw an error). It then calls SendFileContent to send the appropriate response back to the client.
Main: Opens a socket on the port specified in the command line argument. Listens for incoming requests and forwards them to a new thread. Accepts up to 50 concurrent connections.
Key Variables:
ServerPort: The port to listen on for incoming requests; specified as a command line argument.
Compilation:
	Compile using the makefile as described in the “Makefile” section.
Execution:
	To run individually, use ./server [serverPort]

Makefile and Test Script:
Makefile:
Compiles the server and retriever source files, Server.cpp and Retriever.cpp, using g++ with the following flags: -std=c++14 -g -Wall -Wextra for both. The Server.cpp is compiled with the additional -pthread flag to allow multithreading. The executables are titled  “server” and “retriever''.” 
Execution: 
Enter “make” in the console to run.

Test Script:
The bash test script is titled “tests.sh.” For each test, it writes the output of the program executed in each test to a separate text file titled test<number>resultl.txt. It performs six tests, numbered 2-7: (2) tests for a 200 OK reply from a real web server (in this case http://www.baidu.com), (3) tests for a 200 OK reply when requesting an actual file that is stored by the provided Server program, (4) tests for 401 Unauthorized response when trying to access a real file called “SecretFile.html” that is stored by the provided server program, (5) tests for a 403 Forbidden response when attempting to navigate out of the current directory by requesting “../../../etc/passwd,” (6) tests for 404 Not Found response when requesting a file that does not exist on the Server, and (7) tests for a 400 Bad Request response when sending a syntactically incorrect HTTP request (in this case by sending part of the request as the file name).
Execution:  
First, ensure the server program is listening on a port by executing it as described in the “Server'' section above. On a different machine, enter “bash tests.sh” in the console to run.

