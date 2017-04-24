#include <fstream>

static std::string read(const std::string &filepath) {

  std::string out;

	std::ifstream file(filepath);

	if (file.fail()) {
		perror(filepath.c_str());
		//fatalError("Couldn't open file in location " + filepath);
	}

	out = "";
	std::string line = "";

	while (std::getline(file, line))
		out += line + "\n";

	file.close();

  return out;
}
