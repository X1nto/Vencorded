#include "installer.h"
#include <filesystem>
#include <fstream>
#include <format>

namespace filesystem = std::filesystem;

bool install(std::string& discord_dir, std::string& vencord_dir)
{
	const filesystem::path discord_resources_dir_path(discord_dir);
	const filesystem::path vencord_dir_path(vencord_dir);
	
	const filesystem::path patcher_path = vencord_dir_path / "dist" / "patcher.js";

	const filesystem::path discord_app_dir_path = discord_resources_dir_path / "app";
	if (filesystem::exists(discord_app_dir_path))
	{
		filesystem::remove_all(discord_app_dir_path);
	}
	filesystem::create_directory(discord_app_dir_path);

	const filesystem::path index_path = discord_app_dir_path / "index.js";
	const filesystem::path package_path = discord_app_dir_path / "package.json";

	std::string patcher_path_str = filesystem::canonical(patcher_path).string();
#ifdef _WIN32
	std::replace(patcher_path_str.begin(), patcher_path_str.end(), '\\', '/');
#endif

	const std::string index_content = std::format(R"cock(require("{0}"); require("../app.asar");)cock", patcher_path_str);
	const std::string package_content = R"({"name":"discord","main":"index.js"})";

	std::ofstream index(index_path);
	index << index_content;
	index.close();

	std::ofstream package(package_path);
	package << package_content;
	package.close();

	return true;
}

bool uninstall(std::string& discord_path)
{
	filesystem::path discord_resources_dir_path(discord_path);
	filesystem::remove_all(discord_resources_dir_path / "app");
	return true;
}
