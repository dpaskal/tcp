#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h> // close(int fd)

using namespace std;


/*
Client code should take client_ID, client_name, server_name/ip, port_number as arguments and in the same sequence.
Example: ./client X Alice remote01.cs.binghamton.edu12000
		 ./client Y Bob remote01.cs.binghamton.edu 12000
*/

int main(int argc, char** argv) {

	struct sockaddr_in serv_addr;
	struct hostent *hn; // for translating ip addresses
	char buffer[1024];
	int socketfd, newsockfd, port, opt = 1;

	// Check input
	if ( argc != 5 ) {
		cerr << "Usage: ./client <client_id> <client_name> <server_name> <port>" << endl;
		exit(EXIT_FAILURE);
	}

	string client_ID = argv[1], client_name = argv[2], server_name = argv[3];

	// Validate port number
	port = atoi(argv[4]);
	if ((port <= 0) || port > 65535) {
		cerr << "port invalid" << endl;
		exit(EXIT_FAILURE);
	}

	// Create a TCP socket
	if ( (socketfd = socket(AF_INET, SOCK_STREAM, 0) < 0)) {
		cout << "socket creation failed: " << strerror(errno) << endl;
		close(socketfd);
		exit(EXIT_FAILURE);
	}

	// Set socket options o reuse address and port
	if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		cerr << "setsockopt failed" << strerror(errno) << endl;
		close(socketfd);
		exit(EXIT_FAILURE);
	}


	// Translate input to hostent struct
	// https://man7.org/linux/man-pages/man3/gethostbyname.3.html
	if ((hn = gethostbyname(argv[1])) == NULL )
		return 1; // error checking gethostbyname

	// Fill server info
	serv_addr.sin_family = AF_INET;							// IPv4
	memcpy(&serv_addr.sin_addr, hn->h_addr, hn->h_length);	// IP address
	serv_addr.sin_port = htons(port);						// port

	close(socketfd);
	return 0;
}