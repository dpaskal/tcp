#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h> // memset
#include <unistd.h> // close(int fd)

using namespace std;

#define BACKLOG	3

int main(int argc, char **argv) {
	char buffer[1024];
	int socketfd, newsockfd, port, opt = 1;
	struct sockaddr_in hint;
	ssize_t bytesRead;

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

	// Set socket options to reuse address and port
	if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		cerr << "setsockopt failed" << strerror(errno) << endl;
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
	if ((listen(socketfd, BACKLOG)) < 0) {
		cerr << "listen failed: " << strerror(errno) << endl;
	}


	// Accept first call
	socklen_t addrlen = sizeof(hint); // (socklen_t *)sizeof(hint)
	if ((newsockfd = accept(socketfd, (struct sockaddr *)&hint, &addrlen)) < 0) {
		cerr << "accept failed: " << strerror(errno) << endl;
	}

	// Accept second call

	// Read from first socket fd
	if ((bytesRead = read(newsockfd, buffer, 1024)) < 0) {
		cerr << "read failed: " << strerror(errno) << endl;
	}
	buffer[bytesRead] = '\0';
	cout << "server's buffer: " << buffer << endl; // debug

	// Read from second socket fd


	// Respond to first call
	strcpy(buffer, "rest");
	send(newsockfd, buffer, 1024, 0);

	// Respond to second call

	//Close the closet
	close(socketfd);
	return 0;
}