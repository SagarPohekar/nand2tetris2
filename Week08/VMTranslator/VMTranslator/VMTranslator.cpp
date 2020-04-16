// VMTranslator.cpp : Defines the entry point for the application.
//

#include "VMTranslator.h"
#include <string>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
	if (argc < 2) {
		std::cerr << "Source file/directory not provided" << '\n'
							<< "usage : VMTranslator <path to directory containing .vm files>/<source>.vm" << '\n'
							<< "secify -d to dump each command" << '\n';
		std::exit(-1);
	}
	
	bool dumpCommands{ false };
	if (argc >= 3 ) {
		std::string d{ argv[argc - 1] };
		if (d == "-d")
			dumpCommands = true;
	}
	
	bool isDir{ false };
	fs::path sourcePath{ argv[1] };
	isDir = fs::is_directory(sourcePath);


	if (!isDir && !fs::exists(sourcePath)) {
		std::cerr << "File: " << sourcePath.c_str() << " does not exits" << '\n';
		std::exit(-1);
	}
	else if (isDir && (fs::is_empty(sourcePath) || !fs::exists(sourcePath))) {
		std::cerr << "Directory: " << sourcePath.c_str() << " does not exits or is empty" << '\n';
		std::exit(-1);
	}

	fs::path outputFilePath{  };
	std::vector<fs::path> filePaths{};
	for (auto const& dir : fs::recursive_directory_iterator(sourcePath)) {
		fs::path path{ dir };
		if (fs::is_regular_file(path) && path.has_extension() && path.extension() == ".vm")
			filePaths.push_back(path);
	}

	// TO DO : if dir == . then retrieve parent directory name
	if (isDir) {
		outputFilePath = sourcePath  / fs::path{ sourcePath.stem().string() + ".asm" };
	}
	else {
		outputFilePath = sourcePath.replace_extension(".asm");
	}
	
	CodeWriter codeWriter{ outputFilePath };
	codeWriter.setDumpCommands(dumpCommands);
	codeWriter.writeInit();

	for (auto const& sourceFilePath : filePaths) {

		codeWriter.setCurrentSourceFile(sourceFilePath);

		Parser parser{ sourceFilePath };

		if (!parser.readSourceFile()) {
			std::cerr << "Unable to read " << sourceFilePath << '\n';
			std::exit(-1);
		}

		if (dumpCommands)
			std::clog << "Processing file starts: " << sourceFilePath << '\n';

		while (parser.hasMoreCommands()) {
			auto currentVMInstruction = parser.advance();
			auto type = parser.getCommandType();
			std::vector<std::string> cmdTokens{};
			std::string assemblyInstructions{ "" };

			if (dumpCommands)
				std::clog << "Processing...: " << currentVMInstruction << '\n';

			try {
				cmdTokens = parser.getCommandTokens();

				switch (type)
				{
				case CommandType::ArithmeticLogical:
					assemblyInstructions = codeWriter.writeArithmetic(cmdTokens);
					break;

				case CommandType::MemoryAccess:
					assemblyInstructions = codeWriter.writePushPop(cmdTokens);
					break;

				case CommandType::Branching:
					assemblyInstructions = codeWriter.writeBranching(cmdTokens);
					break;

				case CommandType::Functional:
					assemblyInstructions = codeWriter.writeFunctional(cmdTokens);
					break;

				case CommandType::Invalid:
				default:
					break;
				}

				if (dumpCommands)
					std::clog << assemblyInstructions << '\n';
			}
			catch (const std::exception& e) {
				std::cerr << e.what() << '\n'
					<< "Failed to process instruction: " << currentVMInstruction << '\n'
					<< "exiting..." << '\n';
				exit(-1);
			}
		}
		if (dumpCommands)
			std::clog << "Processing file ends: " << sourceFilePath << '\n';
	}
	return 0;
}
