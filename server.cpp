#include <sys/socket.h>
#include <functional>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int	main(){
	int			sockfd, new_socket;
	sockaddr_in	address;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		std::cout << "Error\n";
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(8080); // from little endian to big endian
	if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) == -1){
		std::cout << "Error, failed to bind\n";
		exit(EXIT_FAILURE);
	}
	if (listen(sockfd, 3) < 0){
		std::cout << "Error, failed to listen on socket\n";
		exit(EXIT_FAILURE);
	}
	int addrlen = sizeof(address);
	if ((new_socket = accept(sockfd, (struct sockaddr *)&address, 
		(socklen_t*)&addrlen)) < 0){
		std::cout << "Error, failed to grab connection\n";
		exit(EXIT_FAILURE);
	}
	char		buffer[1024];
	int			val_read;
	
	val_read = read(new_socket, buffer, 1024);
	std::cout << "THE MESSAGE : " << buffer << std::endl;
	std::string str = "thank's for the talk\n";
	send(new_socket, str.data(), str.size(), 0);

}