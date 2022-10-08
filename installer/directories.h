#pragma once

#include <vector>
#include <string>

namespace vencorded
{
	
	struct DiscordDir
	{
		const std::string display_name;
		const std::string display_path;
		const std::string actual_path;
		const bool patched;
	};
	using discord_dirs_t = std::vector<DiscordDir>;

	discord_dirs_t get_discord_directories();
}