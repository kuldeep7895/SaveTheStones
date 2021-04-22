// Client side C/C++ program to demonstrate Socket programming
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#define PORT 8080

using namespace std;

int main(int argc, char const *argv[])
{
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char *hello = "Hello from client";
	char buffer[1024] = {0};
	bool isExit = false;
    int bufsize = 1024;
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}
	send(sock , hello , strlen(hello) , 0 );
	printf("Hello message sent\n");
	valread = read( sock , buffer, 1024);
	printf("%s\n",buffer );
	
	 cout << "=> Awaiting confirmation from the server..." << endl; //line 40
	    recv(sock, buffer, bufsize, 0);
	    cout << "=> Connection confirmed, you are good to go...";

	    cout << "\n\n=> Enter # to end the connection\n" << endl;
	
	do {
		cout << "Client: ";
		do {
		    cin >> buffer;
		    send(sock, buffer, bufsize, 0);
		    if (*buffer == '#') {
		        send(sock, buffer, bufsize, 0);
		        *buffer = '*';
		        isExit = true;
		    }
		} while (*buffer != 42);

		cout << "Server: ";
		do {
		    recv(sock, buffer, bufsize, 0);
		    cout << buffer << " ";
		    if (*buffer == '#') {
		        *buffer = '*';
		        isExit = true;
		    }

		} while (*buffer != 42);
		cout << endl;

	    } while (!isExit);


    cout << "\n=> Connection terminated.\nGoodbye...\n";

    close(sock);
    return 0;
	return 0;
}

