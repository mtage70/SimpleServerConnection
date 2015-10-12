#include <iostream>
#include <cstdlib>
#include <string>
#include <cstdio>
#include <cmath>
#include <winsock.h>


using namespace std;

//server IP and port
//convert IP for inet_addr()?
#define DEST_DNS "cs5700.ccs.neu.edu"
#define DEST_PORT 27993
#define BACKLOG 10
//max transfer size is 256 bytes
#define MAXDATASIZE 256

int main(int argc, char *argv[]){
	//socket requirements for windows
	WSAData wsaData;

	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
		fprintf(stderr, "WSAStartup failed.\n");
		exit(1);
	}
	//create a buffer
	char buf[MAXDATASIZE];
	//change given server DNS into IP
	struct hostent *remoteHost;
	remoteHost = gethostbyname(DEST_DNS);

	//create a socket to listen on MYPORT
	int sockfd;
	int sin_size;
	int numbytes;
	struct sockaddr_in dest_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	//socket() error check
	if (sockfd == -1) {
		fprintf(stderr, "sock() failed.\n");
	}
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(DEST_PORT);
	dest_addr.sin_addr.s_addr = inet_addr(remoteHost->h_name);
	memset(&(dest_addr.sin_zero), '/0', 8);

	//connect to the server
	//connect will figure out our local port # and the server will get that info

	connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr));

	//send HELLO message
	const char* hellomsg = "cs5700fall2015 HELLO 001176499\n";
	send(sockfd, hellomsg, strlen(hellomsg), 0);

	string recvstring;
	bool communicating = true;
	while (communicating) {
		if ((numbytes = recv(sockfd, buf, MAXDATASIZE - 1, 0)) == -1) {
			perror("recv");
			exit(1);
		}
		recvstring = buf;
		if (recvstring.find("BYE")){
			//if the server says bye then it's time to shut down
			communicating = false;
		}
		else {
			cout << recvstring << "\n";
			//get arguments from string
			// domath();
		}
	}


	//when done...
	closesocket(sockfd);



	WSACleanup();
}


int domath(int arg1, char* op, int arg2) {
	if (op == "+") {
		return arg1 + arg2;
	}
	else if (op == "-") {
		return arg1 - arg2;
	}
	else if (op == "*") {
		return arg1 * arg2;
	}
	else return floor(arg1 / arg2);

}

//use closesocket() instead of close()
//select() only works with socket descriptors not file descriptors (0 for stdin)
//server runs on cs5700f15.ccs.neu.edu and listens for requests on a TCP socket bound to port 27993.
//HELLO cs5700fall2015 HELLO [ your NEU ID ] \ n
//use SOCK_STREAM


//wondering...
//listen/accept unecessary??
//listen(sockfd, BACKLOG);
//sin_size = sizeof(struct sockaddr_in);
//newfd = accept(sockfd, (struct sockaddr *)&dest_addr, &sin_size);
