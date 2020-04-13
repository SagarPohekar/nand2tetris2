// VMTranslatorI.cpp : Defines the entry point for the application.
//

#include "VMTranslatorI.h"
#include <string>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
	if (argc < 2) {
		std::cerr << "Source file not provided" << '\n'
							<< "usage : VMTranslatorI <source>.vm" << '\n'
							<< "secify -d to dump each command" << '\n';
		std::exit(-1);
	}

	bool dumpCommands{ false };
	if (argc >= 3 && argv[argc - 1] == "-d") {
		dumpCommands = true;
	}

	fs::path sourceFilePath{ argv[1] };
	
	if (!fs::exists(sourceFilePath)) {
		std::cerr << sourceFilePath.c_str() << " does not exits" << '\n';
		std::exit(-1);
	}
	
	Parser parser{ sourceFilePath };

	if (!parser.readSourceFile()) {
		std::cerr << "Unable to read " << sourceFilePath << '\n';
		std::exit(-1);
	}

	fs::path outputFilePath{ sourceFilePath };
	outputFilePath = outputFilePath.replace_extension(".asm");
	
	CodeWriter codeWriter{ outputFilePath };
	codeWriter.setDumpCommands(dumpCommands);

	while (parser.hasMoreCommands()) {
		auto currentVMInstruction = parser.advance();
		auto type = parser.getCommandType();
		std::vector<std::string> cmdTokens{};

		if (dumpCommands)
			std::clog << currentVMInstruction << '\n';
		try {
			switch (type)
			{
			case CommandType::Invalid:
				break;
			case CommandType::ArithmeticLogical:
				codeWriter.writeArithmetic(currentVMInstruction);
				break;
			case CommandType::MemoryAccess:
				cmdTokens = parser.getCommandTokens();
				codeWriter.writePushPop(cmdTokens);
				break;
			case CommandType::Branching:
				break;
			case CommandType::Functional:
				break;
			default:
				break;
			}
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << '\n'
								<< "Failed to process instruction: " << currentVMInstruction << '\n'
								<< "exiting..." << '\n';
			exit(-1);
		}
		
	}

	return 0;
}
