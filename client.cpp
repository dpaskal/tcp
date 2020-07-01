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
	if ( argc != 4 ) {
		cerr << "Usage: ./client <client_id> <client_name> <server_name:port>" << endl;
		exit(EXIT_FAILURE);
	}

	char buffer[1024];
	int socketfd, newsockfd, port;



	if ( (socketfd = socket(AF_INET, SOCK_STREAM, 0) < 0)) {
		cout << "socket creation failed: " << strerror(errno) << endl;
		close(socketfd);
		exit(EXIT_FAILURE);
	}



	close(socketfd);
	return 0;
}