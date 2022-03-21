#include "server.hpp"
#include <algorithm>
#include <sstream>
#include <vector>
#include <map>
#include <utility>


std::string find_msg(std::map<int, content > *mamap, int fd, char *msg){
	int									i = 0;
	std::string							tmp(msg);
	std::map<int, content >::iterator	it = mamap->find(fd);
	int									size = tmp.size();

	while (i < size){
		if ((tmp[i] == '\r' && tmp[i + 1] && tmp[i + 1] == '\n')
			|| (tmp[i] == '\n' && it != mamap->end() && it->second.r)){
			if (it == mamap->end())
				return tmp;
			it->second.buff += tmp.substr(0, i);
			tmp = it->second.buff;
			return tmp;
		}
		i++;
	}
	if (it == mamap->end())
		mamap->insert(std::make_pair(fd, content()));
	it = mamap->find(fd);
	it->second.buff += tmp.substr(0, size);
	if (tmp[size - 1] == '\r')
		it->second.r = true;
	else
		it->second.r = false;
	return "";
}

int	main(){
	int					listen_fd, new_socket;
	sockaddr_in			address;
	int					ns;
	char				buffer[4097];
	std::map<int, struct content > mamap;

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
	if (listen(listen_fd, 100) < 0){
		std::cout << "Error, failed to listen on socket\n";
		exit(EXIT_FAILURE);
	}
	int addrlen = sizeof(address);
	std::vector<pollfd> poll_fds;
	poll_fds.push_back((pollfd){.fd = listen_fd, .events = POLLIN|POLLOUT});
	while (42){
		// check for potential read/write/accept
		ns = poll(poll_fds.data(), poll_fds.size(), 100);
		if (ns < 0){
			std::cout << "Poll failed\n";
			break;
		}
		else if (ns == 0)
			continue;
		else if (poll_fds[0].revents & POLLIN){
			new_socket = accept(listen_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
			if (new_socket < 0){
				std::cout << "Error, failed to grab connection\n";
				exit(EXIT_FAILURE);
			}
			poll_fds.push_back((pollfd){.fd = new_socket, .events = POLLIN|POLLOUT, .revents = 0});
		}
		std::vector<pollfd>::iterator it = poll_fds.begin() + 1;
		while (it != poll_fds.end()){
			if (it->revents & POLLIN){
				int result = 1;
				memset(buffer, 0, 4097);
				result = read(it->fd, buffer, 4096);
				std::string str;
				if (!(str = find_msg(&mamap, it->fd, buffer)).empty())
					msg_parser(str, it->fd);
				std::cout << buffer << std::endl;
				if (result < 0)
					std::cout << "couldn't read from socket\n";
				else if (result == 0){
					close(it->fd);
					it = poll_fds.erase(it);
					continue;
				}
				else if (result && it->revents & POLLOUT){
					std::string str = "thank's for the talk\n";
					send(it->fd, str.data(), str.size(), 0);
				}
			}
			it++;
		}
	}
	return 0;
}
