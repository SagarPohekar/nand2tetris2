#include "ArgParser.hpp"

using namespace std;

int main(int argc, char* argv[])
{
	char* argvt[] = {
		"-c",
		"-p",
		"path",
		"-h",
		"-d",
		"filepath",
	};
	ArgParser ap{ argc, argvt };
	ap.addArg(ArgParser::ArgInfo{ ArgType::PositionalWithoutVal, "-c", "--compile", "Help"});
	ap.addArg(ArgParser::ArgInfo{ ArgType::PositionalWithVal, "-p", "--path", "Help"});
	ap.addArg(ArgParser::ArgInfo{ ArgType::OptionalWithoutVal, "-h", "--help", "Help"});
	ap.addArg(ArgParser::ArgInfo{ ArgType::OptionalWithVal, "-d", "--filepath", "Help"});

	try {
		ap.parse();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
	}
	return 0;
}
