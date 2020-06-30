#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h> // close(int fd)


using namespace std;

int main(int argc, char** argv) {
    if ( argc != 4 ) {
        cerr << "Usage: ./client <client_id> <client_name> <server_name:port>" << endl;
        exit(EXIT_FAILURE);
    }

    int socketfd;



    if ( (socketfd = socket(AF_INET, SOCK_STREAM, 0) < 0)) {
        cout << "socket creation failed: " << strerror(errno) << endl;
        close(socketfd);
        exit(EXIT_FAILURE);
    }



    close(socketfd);
    return 0;
}