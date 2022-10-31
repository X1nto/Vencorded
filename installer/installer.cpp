#include "installer.h"
#include <filesystem>
#include <fstream>
#include <format>
#include "../directories/vencorded.h"

namespace filesystem = std::filesystem;

bool vencorded::install(std::string& discord_dir)
{
	const filesystem::path discord_resources_dir_path(discord_dir);
	
	const filesystem::path discord_app_dir_path = discord_resources_dir_path / "app";
	if (filesystem::exists(discord_app_dir_path))
	{
		filesystem::remove_all(discord_app_dir_path);
	}
	filesystem::create_directory(discord_app_dir_path);

	const filesystem::path index_path = discord_app_dir_path / "index.js";
	const filesystem::path package_path = discord_app_dir_path / "package.json";

	std::string patcher_path = filesystem::canonical(vencorded::vencorded_vencord_patcher_path()).string();
#ifdef _WIN32
	std::replace(patcher_path.begin(), patcher_path.end(), '\\', '/');
#endif

	const std::string index_content = std::format(R"cock(require("{0}"); require("../app.asar");)cock", patcher_path);
	const std::string package_content = R"({"name":"discord","main":"index.js"})";

	std::ofstream index(index_path);
	index << index_content;
	index.close();

	std::ofstream package(package_path);
	package << package_content;
	package.close();

	return true;
}

bool vencorded::uninstall(std::string& discord_path)
{
	filesystem::path discord_resources_dir_path(discord_path);
	filesystem::remove_all(discord_resources_dir_path / "app");
	return true;
}
