// Server side C/C++ program to demonstrate Socket programming
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#define PORT 8080

using namespace std;

int main(int argc, char const *argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	bool isExit = false;
	int bufsize = 1024;
	char *hello = "Hello from server";
	
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	
	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
												&opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	
	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address,
								sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
					(socklen_t*)&addrlen))<0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	valread = read( new_socket , buffer, 1024);
	printf("%s\n",buffer );
	send(new_socket , hello , strlen(hello) , 0 );
	printf("Hello message sent\n");
	
	int clientCount = 1;
	
	strcpy(buffer, "=> Server connected...\n");
		send(new_socket, buffer, bufsize, 0);
		cout << "=> Connected with the client #" << clientCount << ", you are good to go..." << endl;
		cout << "\n=> Enter # to end the connection\n" << endl;


		cout << "Client: ";
		do {
		    recv(new_socket, buffer, bufsize, 0);
		    cout << buffer << " ";
		    if (*buffer == '#') {
		        *buffer = '*';
		        isExit = true;
		    }
		} while (*buffer != '*');

		do {
		    cout << "\nServer: ";
		    do {
		        cin >> buffer;
		        send(new_socket, buffer, bufsize, 0);
		        if (*buffer == '#') {
		            send(new_socket, buffer, bufsize, 0);
		            *buffer = '*';
		            isExit = true;
		        }
		    } while (*buffer != '*');

		    cout << "Client: ";
		    do {
		        recv(new_socket, buffer, bufsize, 0);
		        cout << buffer << " ";
		        if (*buffer == '#') {
		            *buffer == '*';
		            isExit = true;
		        }
		    } while (*buffer != '*');
		} while (!isExit);

		
		cout << "\n\n=> Connection terminated with IP " << inet_ntoa(address.sin_addr);
		close(new_socket);
		cout << "\nGoodbye..." << endl;
		isExit = false;
		exit(1);
	    
	
	return 0;
}

