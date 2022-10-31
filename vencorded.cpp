// Vencorded.cpp : Defines the entry point for the application.
//

#include <sstream>
#include "curses.h"
#include "ui/options.h"
#include "ui/optionsmenu.h"
#include "directories/discord.h"
#include "installer/installer.h"
#include "downloader/downloader.h"
#include "git2.h"

using vencorded::option_t;
using vencorded::options_t;
using vencorded::OptionFactory;
using vencorded::ExtraInfoOptionFactory;
using vencorded::OptionsMenu;

using vencorded::DiscordDir;
using vencorded::discord_directories;
using vencorded::discord_dirs_t;

using vencorded::install;
using vencorded::uninstall;

using vencorded::download_vencord;
using vencorded::update_vencord;
using vencorded::nuke_vencord;
using vencorded::vencord_downloaded;

enum class Menu
{
	main,
	download,
	update,
	nuke,
	install,
	uninstall
};

bool should_exit = false;
Menu current_menu = Menu::main;

void press_any_key_to_continue()
{
	printw("\n\nPress any key to continue...");
	refresh();
	getch();
	current_menu = Menu::main;
}

void menu_main()
{
	enum options
	{
		update,
		download,
		nuke,
		install,
		uninstall,
		exit
	};

	options_t options;
	if (vencord_downloaded())
	{
		options.push_back(OptionFactory(update, "Update"));
		options.push_back(OptionFactory(nuke, "Nuke"));
		options.push_back(OptionFactory(install, "Install"));
	} else
	{
		options.push_back(OptionFactory(download, "Download"));
	}
	options.push_back(OptionFactory(uninstall, "Uninstall"));
	options.push_back(OptionFactory(exit, "Exit"));

	OptionsMenu* menu = new OptionsMenu("Welcome to Vencorded! Please select the action you want to perform:", options);
	option_t result = menu->selection_prompt();
	delete menu;
	
	switch (result->id)
	{
	case download:
		current_menu = Menu::download;
		break;
	case update:
		current_menu = Menu::update;
		break;
	case nuke:
		current_menu = Menu::nuke;
		break;
	case install:
		current_menu = Menu::install;
		break;
	case uninstall:
		current_menu = Menu::uninstall;
		break;
	case exit:
		should_exit = true;
		break;
	default:
		break;
	}
}

void menu_install()
{
	const discord_dirs_t dirs = discord_directories();
	options_t choose_path_options;
	for (int i = 0; i < dirs.size(); i++)
	{
		DiscordDir discord_dir = dirs[i];
		std::string display_name;
		if (discord_dir.patched)
		{
			display_name += "[MODIFIED] ";
		}
		display_name += discord_dir.display_name;
		choose_path_options.push_back(ExtraInfoOptionFactory(i, display_name, discord_dir.display_path));
	}
	OptionsMenu* choose_path_menu = new OptionsMenu("Please choose your Discord installation:", choose_path_options);
	option_t result = choose_path_menu->selection_prompt();
	delete choose_path_menu;

	printw("Installing...");
	refresh();

	std::string discord_path = dirs[result->id].actual_path;
	bool installed = install(discord_path);

	clear();

	if (installed)
	{
		printw("Successfully installed!");
	} else
	{
		printw("Failed to install.");
	}
	press_any_key_to_continue();
}

void menu_uninstall()
{
	const discord_dirs_t dirs = discord_directories();
	options_t choose_path_options;
	for (int i = 0; i < dirs.size(); i++)
	{
		DiscordDir discord_dir = dirs[i];
		if (discord_dir.patched)
		{
			choose_path_options.push_back(ExtraInfoOptionFactory(i, discord_dir.display_name, discord_dir.display_path));
		}
	}
	OptionsMenu* choose_path_menu = new OptionsMenu("Please choose your Vencord installation:", choose_path_options);
	option_t result = choose_path_menu->selection_prompt();
	delete choose_path_menu;

	printw("Uninstalling...");
	refresh();

	std::string discord_path = dirs[result->id].actual_path;
	bool uninstalled = install(discord_path);

	clear();
	if (uninstalled)
	{
		printw("Successfully uninstalled!");
	} else
	{
		printw("Failed to uninstall.");
	}
	press_any_key_to_continue();
}

void menu_download()
{
	printw("Downloading Vencord... s");
	refresh();
	bool status = download_vencord();
	if (status)
	{
		printw("Successfully downloaded!");
	}
	else
	{
		printw("Failed to download.");
	}
	press_any_key_to_continue();
}

void menu_update()
{
	printw("Updating Vencord... ");
	refresh();
	bool status = update_vencord();
	if (status)
	{
		printw("Successfully updated!");
	} else
	{
		printw("Failed to update.");
	}
	press_any_key_to_continue();
}

void menu_nuke()
{
	printw("Nuking Vencord... ");
	refresh();
	nuke_vencord();
	printw("Vencord nuked!");
	press_any_key_to_continue();
}

int main()
{
	git_libgit2_init();

	initscr();
	keypad(stdscr, true);
	noecho();
	cbreak();

	use_default_colors();
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_WHITE);

	menu_main();

	while (!should_exit)
	{
		clear();
		refresh();
		switch (current_menu)
		{
		case Menu::main:
			menu_main();
			break;
		case Menu::download:
			menu_download();
			break;
		case Menu::update:
			menu_update();
			break;
		case Menu::nuke:
			menu_nuke();
			break;
		case Menu::install:
			menu_install();
			break;
		case Menu::uninstall:
			menu_uninstall();
			break;
		}
	}

	endwin();

	git_libgit2_shutdown();
	return 0;
}
