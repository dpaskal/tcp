#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h> // close(int fd)
#include <arpa/inet.h> // inet_pton()


using namespace std;


/*
Client code should take client_ID, client_name, server_name/ip, port_number as arguments and in the same sequence.
Example: ./client X Alice remote01.cs.binghamton.edu12000
		 ./client Y Bob remote01.cs.binghamton.edu 12000
*/

int main(int argc, char** argv) {

	struct sockaddr_in serv_addr;
	struct hostent *hn; // for translating ip addresses
	char sendBuffer[1024], recvBuffer[1024];
	int socketfd, port, opt = 1, bytes_read;

	// Check input
	if ( argc != 5 ) {
		cerr << "Usage: ./client <client_id> <client_name> <server_name> <port>" << endl;
		exit(EXIT_FAILURE);
	}

	char *client_ID = argv[1], *client_name = argv[2], *server_name = argv[3];

	// Validate port number
	port = atoi(argv[4]);
	if ((port <= 0) || port > 65535) {
		cerr << "port invalid" << endl;
		exit(EXIT_FAILURE);
	}

	// Create a TCP socket
	if ( (socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		cout << "socket creation failed: " << strerror(errno) << endl;
		close(socketfd);
		exit(EXIT_FAILURE);
	}

	// Set socket options to reuse address and port
	if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		cerr << "setsockopt failed: " << strerror(errno) << endl;
		close(socketfd);
		exit(EXIT_FAILURE);
	}


	// // Translate input to hostent struct
	// // https://man7.org/linux/man-pages/man3/gethostbyname.3.html
	if ((hn = gethostbyname(server_name)) == NULL )
		exit(EXIT_FAILURE);
	memcpy(&serv_addr.sin_addr, hn->h_addr, hn->h_length);	// IP address


	// Fill server info
	serv_addr.sin_family = AF_INET;							// IPv4
	serv_addr.sin_port = htons(port);						// port

	// Convert IP addresses from text to binary
	// if ((inet_pton(AF_INET, server_name, &serv_addr.sin_addr)) < 0) {
	// 	cerr << "inet_pton failed: " << strerror(errno) << endl;
	// 	exit(EXIT_FAILURE);
	// }

	// Connect to socket
	if (connect(socketfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		cerr << "connect failed: " << strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}

	// Set message
	strcpy(sendBuffer, client_ID);
	strcat(sendBuffer, ": ");
	strcat(sendBuffer, client_name);
	cout << sendBuffer << endl;
	// Send message
	if (!(send(socketfd, sendBuffer, strlen(sendBuffer), 0))) {
		cerr << "send failed: " << strerror(errno) << endl;
	}

	// Receive response
	if (!(bytes_read = read(socketfd, recvBuffer, sizeof(recvBuffer)))) {
		cerr << "read failed: " << strerror(errno) << endl;
	}
	recvBuffer[bytes_read] = '\0';

	// Print the message sent to the server, followed by the reply received from the server
	cout << endl;
	cout << client_ID << " sent message: " << sendBuffer << endl;
	cout << client_ID << " received message: " << recvBuffer << endl;

	close(socketfd);
	return 0;
}