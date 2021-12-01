#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <regex>
#include <filesystem>
#include <windows.h>

int main()
{
	if (!std::filesystem::exists("osu!.exe")) 
	{
		std::cout << "Can't find osu! executable." << std::endl;
		return 0;
	}

	int sWidth = GetSystemMetrics(SM_CXSCREEN);
	int sHeight = GetSystemMetrics(SM_CYSCREEN);

	const std::regex config_regex("(osu!.[a-zA-Z]+.cfg)");
	const std::string filename = "osu!." + std::string(getenv("username")) + ".cfg";

	if (std::filesystem::exists(filename))
	{
		std::ifstream ifs(filename);
		std::string config((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

		const std::regex height_regex("Height = [0-9]+");
		const std::regex width_regex("Width = [0-9]+");

		const std::regex heightf_regex("HeightFullscreen = [0-9]+");
		const std::regex widthf_regex("WidthFullscreen = [0-9]+");

		std::string result;

		result = std::regex_replace(config, height_regex, "Height = " + std::to_string(sHeight));
		result = std::regex_replace(result, width_regex, "Width = " + std::to_string(sWidth));

		result = std::regex_replace(result, heightf_regex, "HeightFullscreen = " + std::to_string(sHeight));
		result = std::regex_replace(result, widthf_regex, "WidthFullscreen = " + std::to_string(sWidth));

		std::ofstream ofs(filename);
		ofs << result;
		ofs.close();
	}

	ShellExecute(NULL, "open", "osu!.exe", NULL, NULL, SW_SHOWDEFAULT);
	return 0;
}