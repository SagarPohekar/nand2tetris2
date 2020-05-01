// JackAnalyzer.cpp : Defines the entry point for the application.
//

#include "JackAnalyzer.h"
#include "DebugUtils.hpp"

using namespace std;
namespace fs = std::filesystem;
static const std::string kXMLExtn{ ".xmlt" };

int main(int argc, char* argv[])
{
	if (argc < 2) {
		std::cerr << "Source file/directory not provided" << '\n'
			<< "usage : JackAnalyzer <path to directory containing .jack files>/<source>.jack" << '\n';
		std::exit(-1);
	}

	fs::path sourcePath{ argv[1] };
	const bool isDir = fs::is_directory(sourcePath);
	const bool curDir = sourcePath.stem().string() == ".";

	if (!isDir && !fs::exists(sourcePath)) {
		std::cerr << "File: " << sourcePath.c_str() << " does not exits" << '\n';
		std::exit(-1);
	}
	else if (isDir && (fs::is_empty(sourcePath) || !fs::exists(sourcePath))) {
		std::cerr << "Directory: " << sourcePath.c_str() << " does not exits or is empty" << '\n';
		std::exit(-1);
	}

	std::vector<fs::path> sourceFilePaths{};
	for (auto const& dir : fs::recursive_directory_iterator(sourcePath)) {
		fs::path path{ dir };
		if (fs::is_regular_file(path) && path.has_extension() && path.extension() == ".jack")
			sourceFilePaths.push_back(path);
	}

	if (sourceFilePaths.empty()) {
		std::cerr << "Directory: " << sourcePath.c_str() << " does not contain .jack files" << '\n';
		std::exit(-1);
	}

	auto getSourceFileFullPath = [=](const auto& sourcePath) {
		fs::path sourceFilePath{ sourcePath };

		if (curDir) {
			sourceFilePath = fs::current_path() / sourceFilePath.filename();
		}
		return sourceFilePath;
	};

	for (const auto& sourceFilePath : sourceFilePaths) {
		fs::path sourceFileFullPath{ getSourceFileFullPath(sourceFilePath) };
		Debug::Log(sourceFileFullPath);
		/*JackTokenizer jk{ sourceFileFullPath };
		while (jk.hasMoreTokens()) {
			jk.advance2();
			const auto& [token, tokentype] = jk.getCurrentToken();
			if (!token.empty() && JackTokenizer::TokenType::Invalid != tokentype) {
				std::clog << std::setw(15) << std::left << token << tokentype << '\n';
			}
		}*/
		CompilationEngine ce{ sourceFileFullPath };
	}
	

	return 0;
}
