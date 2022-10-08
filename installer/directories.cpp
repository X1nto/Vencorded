#include "directories.h"
#include <string>
#include <filesystem>

namespace filesystem = std::filesystem;

vencorded::discord_dirs_t get_windows_dirs();
vencorded::discord_dirs_t get_linux_dirs();
vencorded::discord_dirs_t get_osx_dirs();

vencorded::discord_dirs_t vencorded::get_discord_directories()
{
#ifdef _WIN32
	return get_windows_dirs();
#elif __linux__
	return get_linux_dirs();
#elif __MACH__
	return get_osx_dirs();
#endif // _WIN32
}

vencorded::discord_dirs_t get_windows_dirs()
{
	vencorded::discord_dirs_t dirs;
	const std::vector<std::string> discord_dir_names{
		"Discord",
		"DiscordPTB",
		"DiscordCanary",
		"DiscordDevelopment"
	};
	const std::string localappdata = std::getenv("localappdata");
	const filesystem::path localappdata_dir = filesystem::path(localappdata);
	for (const std::string& discord_dir_name : discord_dir_names)
	{
		const filesystem::path discord_dir_path = localappdata_dir / discord_dir_name;

		if (!filesystem::exists(discord_dir_path))
		{
			continue;
		}

		const filesystem::directory_iterator discord_dir_iter = filesystem::directory_iterator(discord_dir_path);
		const filesystem::directory_iterator discord_version_dir_iter = std::find_if(
			filesystem::begin(discord_dir_iter),
			filesystem::end(discord_dir_iter),
			[](const filesystem::directory_entry& entry) {
				return entry.path().filename().string().starts_with("app");
			}
		);
		if (discord_version_dir_iter == filesystem::end(discord_dir_iter))
		{
			continue;
		}

		const filesystem::path discord_appdir_path = discord_version_dir_iter->path();
		const filesystem::path resources_dir_path = discord_appdir_path / "resources";
		if (!filesystem::exists(resources_dir_path / "app.asar"))
		{
			continue;
		}

		const bool installed = filesystem::exists(resources_dir_path / "app");

		std::string dir_display_name;
		if (discord_dir_name == "Discord")
		{
			dir_display_name = "Stable";
		}
		else {
			dir_display_name = discord_dir_name.substr(7);
		}

		const vencorded::DiscordDir discord_dir_s{
			dir_display_name,
			filesystem::canonical(discord_dir_path).string(),
			filesystem::canonical(resources_dir_path).string(),
			installed
		};
		dirs.push_back(discord_dir_s);
	}
	return dirs;
}

vencorded::discord_dirs_t get_osx_dirs()
{
	vencorded::discord_dirs_t dirs;
	std::vector<std::string> discord_dirs{
		"Discord.app",
		"Discord PTB.app",
		"Discord Canary.app",
		"Discord Development.app"
	};
	return dirs;
}