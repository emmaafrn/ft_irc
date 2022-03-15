#ifndef SERVER_HPP
# define SERVER_HPP

#include <sys/socket.h>
#include <functional>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <poll.h>
#include <string>
#include <sys/poll.h>
#include <vector>

void	msg_parser(std::string msg);


#endif