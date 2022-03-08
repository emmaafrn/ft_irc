#include <sys/socket.h>
#include <functional>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <poll.h>
#include <string.h>

int	main(){
	int				sockfd, new_socket;
	sockaddr_in		address;
	int				ns;
	struct pollfd	fds[200];
	int				nfds = 1, current_size = 0;
	bool			end_server = false;

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
	if (listen(sockfd, 1) < 0){
		std::cout << "Error, failed to listen on socket\n";
		exit(EXIT_FAILURE);
	}
	int addrlen = sizeof(address);
	memset(fds, 0 , sizeof(fds));
	fds[0].fd = sockfd;
	fds[0].events = POLLIN;
	while (!end_server){
		if ((new_socket = accept(sockfd, (struct sockaddr *)&address, 
			(socklen_t*)&addrlen)) < 0){
			std::cout << "Error, failed to grab connection\n";
			exit(EXIT_FAILURE);
		}
		ns = poll(fds, nfds, 3 * 60 * 1000);
		if (ns < 0){
			std::cout << "Poll failed\n";
			break;
		}
		else if (ns == 0){
			std::cout << "Poll timed out\n";
			break;
		}
		current_size = nfds;
		for (int i = 0 ; i < current_size ; i++){
			if(fds[i].revents != POLLIN)
			{
				printf("  Error! revents = %d\n", fds[i].revents);
				end_server = true;
				break;
			}
			if(fds[i].revents != POLLIN)
			{
				printf("  Error! revents = %d\n", fds[i].revents);
				end_server = true;
				break;
			}

		}
		char		buffer[1024];
		int			val_read;

		fcntl(new_socket, F_SETFL, O_NONBLOCK);
		val_read = read(new_socket, buffer, 1024);
		std::cout << "THE MESSAGE : " << buffer << std::endl;
		std::string str = "thank's for the talk\n";
		send(new_socket, str.data(), str.size(), 0);
	}

	close(new_socket);
	close(sockfd);

	return 0;
}