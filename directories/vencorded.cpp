#include "vencorded.h"

namespace filesystem = std::filesystem;

inline filesystem::path _vencorded_path_windows();

std::filesystem::path vencorded::vencorded_path()
{
#if _WIN32
	return _vencorded_path_windows();
#endif
}

std::filesystem::path vencorded::vencorded_vencord_path()
{
	return vencorded_path() / "repo";
}
std::filesystem::path vencorded::vencorded_vencord_patcher_path()
{
	return vencorded_vencord_path() / "dist" / "patcher.js";
}

inline filesystem::path _vencorded_path_windows()
{
	const std::string appdata = std::getenv("appdata");
	const filesystem::path appdata_path = filesystem::path(appdata);
	const filesystem::path vencorded_path = appdata_path / "Vencorded";
	if (!filesystem::exists(vencorded_path))
	{
		filesystem::create_directories(vencorded_path);
	}

	return vencorded_path;
}