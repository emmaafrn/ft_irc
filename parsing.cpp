#include "server.hpp"
#include <vector>

void	msg_parser(std::string msg){
	size_t						i = 0;
	size_t						start = 0;
	std::string					cmd;
	std::vector<std::string>	params;
	int							len = msg.size();

	while (i < len){
		while (i < len && msg[i] == ' ')
			i++;
		start = i;
		if (i < len && msg[i] == ':' && cmd.size() > 0){
			while (i < len)
				i++;
		}
		else {
			while (i < len && msg[i] != ' ')
				i++;
		}
		if (i > start && cmd.size() == 0)
			cmd = msg.substr(start, i - start);
		else if (i > start)
			params.push_back(msg.substr(start, i - start));
	}
}