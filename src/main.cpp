#include <network/Parsing.hpp>
#include <network/Msg_manager.hpp>

bool	onlyDigits(std::string port){
	size_t i = 0;
	for (; i < port.size() && (port[i] >= '0' && port[i] <= '9'); i++);
	return i == port.size();
}

bool	checkCRLF(std::string password){
	return password.find("\r\n") == std::string::npos;
}

int	main(int argc, char **argv){
	if (argc != 3 || !onlyDigits(argv[1]) || !checkCRLF(argv[2])){
		std::cerr << "Error : wrong argument(s)\n";
		exit(EXIT_FAILURE);
	}
	msg_manager		manager(argv[2]);
	manager.set_connection(argv[1]);
	manager.connections_manager();
	return 0;
}
