#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h> // memset
#include <unistd.h> // close(int fd)

using namespace std;

#define BACKLOG	3
#define DONE_MSG "Sent acknowledgment to both X and Y"

int main(int argc, char **argv) {
	char firstBuffer[1024], secondBuffer[1024], returnBuffer[1024];
	int socketfd, firstfd, secondfd, port, opt = 1;
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
	if ((firstfd = accept(socketfd, (struct sockaddr *)&hint, &addrlen)) < 0) {
		cerr << "first accept failed: " << strerror(errno) << endl;
	}

	// Read from first socket fd
	if ((bytesRead = read(firstfd, firstBuffer, 1024)) < 0) {
		cerr << "first read failed: " << strerror(errno) << endl;
	}
	firstBuffer[bytesRead] = '\0';
	// cout << "server's firstBuffer after first read: " << firstBuffer << endl; // debug

	// Accept second call
	if ((secondfd = accept(socketfd, (struct sockaddr *)&hint, &addrlen)) < 0) {
		cerr << "second accept failed: " << strerror(errno) << endl;
	}

	// Read from second socket fd
	if ((bytesRead = read(secondfd, secondBuffer, 1024)) < 0) {
		cerr << "second read failed: " << strerror(errno) << endl;
	}
	secondBuffer[bytesRead] = '\0';
	// cout << "server's secondBuffer after second read: " << secondBuffer << endl; // debug

	strcpy(returnBuffer, firstBuffer);
	strcat(returnBuffer, " received before ");
	strcat(returnBuffer, secondBuffer);
	// Respond to first call
	send(firstfd, returnBuffer, 1024, 0);

	// Respond to second call
	send(secondfd, returnBuffer, 1024, 0);

	//Close the closet
	close(socketfd);
	return 0;
}