#include "server.hpp"
#include <vector>

void	msg_parser(std::string msg){
	size_t						i = 0;
	size_t						start = 0;
	std::string					cmd;
	std::vector<std::string>	params;
	int							len = msg.size();

	if (msg[i] == ':'){
		while (i < len && msg[i] != ' ')
			i++;
		while (i < len && msg[i] == ' ')
			i++;
	}
	start = i;
	while (i < len && msg[i] != ' ')
		i++;
	cmd = msg.substr(start, i - start);
	while (i < len){
		while (i < len && msg[i] == ' ')
			i++;
		start = i;
		while (i < len && msg[i] != ' ')
			i++;
		if (i > start)
			params.push_back(msg.substr(start, i - start));
	}
}