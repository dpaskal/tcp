#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h> // memset
#include <unistd.h> // close(int fd)

using namespace std;


int main(int argc, char **argv) {
	int socketfd, port;
	struct sockaddr_in hint;

	// Check input
	if ( argc != 2 ) {
		cerr << "Usage: ./server <port>" << endl;
		exit(EXIT_FAILURE);
	}

	// Validate port number
	port = atoi(argv[1]);
	if ((port <= 0) || port > 65535) {
		cerr << "port invalid" << endl;
		exit(EXIT_FAILURE);
	}

	// Create a TCP socket
	if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		cout << "socket creation failed: " << strerror(errno) << endl;
		close(socketfd);
		exit(EXIT_FAILURE);
	}

	// Bind the socket to ip:port
	memset(&hint, 0, sizeof(hint));
	hint.sin_family = AF_INET;			// IPv4
	hint.sin_port = htons(port);		// port
	hint.sin_addr.s_addr = INADDR_ANY;	// localhost

	if (bind(socketfd, (const struct sockaddr *)&hint, sizeof(hint)) < 0) {
		cerr << "bind failed: " << strerror(errno) << endl;
		close(socketfd);
		exit(EXIT_FAILURE);
	}

	// Listen on socket

	// Accept a call

	//Close the closet
	close(socketfd);
	return 0;
}