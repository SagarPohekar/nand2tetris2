#include "HackAssembler.h"

using namespace std;
/*
int main(int argc, char* argv[])
{

	Parser parser{ "Pong.asm" };
	try {
		parser.parse();
		//parser.dump();
		parser.writeToBinary("Pong");
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		throw;
	}
	return 0;
}
*/


int main(int argc, char* argv[])
{
	if (argc < 3) {
		puts("invalid inputs: source/output file not specified");
		return -1;
	}
	Parser parser{ argv[1] };
	try {
		parser.parse();
		parser.writeToBinary(argv[2]);
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		throw;
	}
	return 0;
}
