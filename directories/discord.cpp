#include <filesystem>
#include "discord.h"

namespace filesystem = std::filesystem;

inline vencorded::discord_dirs_t _discord_windows_directories();

vencorded::discord_dirs_t vencorded::discord_directories()
{
#if _WIN32
	return _discord_windows_directories();
#endif
}

inline vencorded::discord_dirs_t _discord_windows_directories()
{
	vencorded::discord_dirs_t dirs;
	const std::vector<std::string> discord_dir_names {
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
		const auto find_predicate = [](const filesystem::directory_entry& entry) { 
			return entry.path().filename().string().starts_with("app");
		};
		const filesystem::directory_iterator discord_version_dir_iter = std::find_if(
			filesystem::begin(discord_dir_iter),
			filesystem::end(discord_dir_iter),
			find_predicate
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
		} else
		{
			dir_display_name = discord_dir_name.substr(7);
		}

		const vencorded::DiscordDir discord_dir_s {
			dir_display_name,
			filesystem::canonical(discord_dir_path).string(),
			filesystem::canonical(resources_dir_path).string(),
			installed
		};
		dirs.push_back(discord_dir_s);
	}
	return dirs;
}