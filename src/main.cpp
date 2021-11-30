#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <regex>
#include <filesystem>
#include <windows.h>

int main()
{
	if (!std::filesystem::exists("osu!.exe")) {
		std::cout << "Can't find osu! executable." << std::endl;
		return 0;
	}

	int sWidth = GetSystemMetrics(SM_CXSCREEN);
	int sHeight = GetSystemMetrics(SM_CYSCREEN);

	const std::regex config_regex("(osu!.[a-zA-Z]+.cfg)");
	std::smatch match;

	for (const auto& entry : std::filesystem::directory_iterator(".")) {
		std::string path(entry.path().filename().string());

		if (std::regex_match(path, match, config_regex)) {
			std::ssub_match filename_sub = match[1];
			std::string filename = filename_sub.str();

			std::ifstream ifs(filename);
			std::string config((std::istreambuf_iterator<char>(ifs)),
				(std::istreambuf_iterator<char>()));

			const std::regex height_regex("Height = [0-9]+");
			const std::regex width_regex("Width = [0-9]+");

			const std::regex heightf_regex("HeightFullscreen = [0-9]+");
			const std::regex widthf_regex("WidthFullscreen = [0-9]+");
	
			std::string result;

			result = std::regex_replace(config, height_regex, "Height = " + std::to_string(sHeight));
			result = std::regex_replace(result, width_regex, "Width = " + std::to_string(sWidth));

			result = std::regex_replace(config, heightf_regex, "HeightFullscreen = " + std::to_string(sHeight));
			result = std::regex_replace(result, widthf_regex, "WidthFullscreen = " + std::to_string(sWidth));

			std::ofstream ofs(path);
			ofs << result;
			ofs.close();
		}
	}

	ShellExecute(NULL, "open", "osu!.exe", NULL, NULL, SW_SHOWDEFAULT);

	return 0;
}