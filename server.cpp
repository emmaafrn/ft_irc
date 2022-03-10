#include "server.hpp"


int	main(){
	int				listen_fd, new_socket;
	sockaddr_in		address;
	int				ns;
	struct pollfd	fds[200];
	int				nfds = 1, current_size = 0, i;
	bool			end_server = false;
	char			buffer[512];
	int				val_read;


	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		std::cout << "Error\n";
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(8080); // from little endian to big endian
	if (bind(listen_fd, (struct sockaddr *)&address, sizeof(address)) == -1){
		std::cout << "Error, failed to bind\n";
		exit(EXIT_FAILURE);
	}
	if (listen(listen_fd, 1) < 0){
		std::cout << "Error, failed to listen on socket\n";
		exit(EXIT_FAILURE);
	}
	int addrlen = sizeof(address);
	memset(fds, 0 , sizeof(fds));
	fds[0].fd = listen_fd;
	fds[0].events = POLLIN|POLLOUT;
	while (!end_server){
		ns = poll(fds, nfds, 20 * 1000);
		if (ns < 0){
			std::cout << "Poll failed\n";
			break;
		}
		else if (ns == 0){
			std::cout << "Poll timed out\n";
			break;
		}
		current_size = nfds;
		for (i = 0 ; i < current_size ; i++){
			if(fds[i].revents == 0)
				continue;
			if(!(fds[i].revents & (POLLIN|POLLOUT)))
			{
				std::cout << "Error, revents : " << fds[i].revents << std::endl;
				end_server = true;
				break;
			}
			if (fds[i].fd == listen_fd){
				while (new_socket != -1){
					if ((new_socket = accept(listen_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0){
						std::cout << "Error, failed to grab connection\n";
						exit(EXIT_FAILURE);
					}
					fds[i].fd = new_socket;
					fds[i].events = POLLIN|POLLOUT;
					i++;
				}
			}
			else {

			}
		}
		fcntl(new_socket, F_SETFL, O_NONBLOCK);
		val_read = read(new_socket, buffer, 512);
		std::cout << "THE MESSAGE : " << buffer << std::endl;
		std::string str = "thank's for the talk\n";
		send(new_socket, str.data(), str.size(), 0);
	}

	close(new_socket);
	close(listen_fd);

	return 0;
}