// Vencorded.cpp : Defines the entry point for the application.
//

#include <sstream>
#include "curses.h"
#include "ui/options.h"
#include "ui/optionsmenu.h"
#include "installer/directories.h"
#include "installer/installer.h"
#include "util/casts.h"

using vencorded::option_t;
using vencorded::options_t;
using vencorded::OptionFactory;
using vencorded::ExtraInfoOptionFactory;
using vencorded::OptionsMenu;

using vencorded::DiscordDir;
using vencorded::get_discord_directories;
using vencorded::discord_dirs_t;

int main()
{
	initscr();
	keypad(stdscr, true);
	noecho();
	cbreak();

	use_default_colors();
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_WHITE);

	const options_t install_uninstall_options {
		OptionFactory("install", "Install"),
		OptionFactory("uninstall", "Uninstall"),
		OptionFactory("exit", "Exit"),
	};
	OptionsMenu* install_uninstall_menu = new OptionsMenu("Welcome to Vencorded! Please select the action you want to perform:", install_uninstall_options);
	option_t result = install_uninstall_menu->selection_prompt();
	delete install_uninstall_menu;

	const discord_dirs_t dirs = get_discord_directories();
	if (result->id == "install")
	{
		options_t choose_path_options;
		for (DiscordDir discord_dir : dirs)
		{
			std::string display_name;
			if (discord_dir.patched)
			{
				display_name += "[MODIFIED] ";
			}
			display_name += discord_dir.display_name;
			choose_path_options.push_back(ExtraInfoOptionFactory(discord_dir.actual_path, display_name, discord_dir.display_path));
		}
		OptionsMenu* choose_path_menu = new OptionsMenu("Please choose your Discord installation:", choose_path_options);
		option_t result = choose_path_menu->selection_prompt();
		delete choose_path_menu;

		std::string input;
		bool should_exit_path_enter = false;
		while (!should_exit_path_enter)
		{
			printw("Please enter your Vencord path:\n");
			printw("%s", input.c_str());
			refresh();
			int c = getch();
			switch (c)
			{
			case 10:
				should_exit_path_enter = true;
				break;
			case '\b':
			case 127:
			case KEY_DC:
			case KEY_BACKSPACE:
				input.pop_back();
				break;
			default:
				input += c;
				break;
			}
			clear();
		}
		clear();

		cast_to<vencorded::ExtraInfoOption*>(result.get(), [&](vencorded::ExtraInfoOption* c) {
			printw("Installing...");
			refresh();
			std::string id = c->id;
			bool installed = install(id, input);
			clear();
			if (installed)
			{
				printw("Successfully installed!");
			}
			else 
			{
				printw("Failed to install.");
			}
			refresh();
			});
	}
	else if (result->id == "uninstall")
	{
		options_t choose_path_options;
		for (DiscordDir discord_dir : dirs)
		{
			if (discord_dir.patched)
			{
				choose_path_options.push_back(ExtraInfoOptionFactory(discord_dir.actual_path, discord_dir.display_name, discord_dir.display_path));
			}
		}
		OptionsMenu* choose_path_menu = new OptionsMenu("Please choose your Vencord installation:", choose_path_options);
		option_t result = choose_path_menu->selection_prompt();
		delete choose_path_menu;
		cast_to<vencorded::ExtraInfoOption*>(result.get(), [&](vencorded::ExtraInfoOption* c) {
			printw("Uninstalling...");
			refresh();
			std::string id = c->id;
			bool uninstalled = uninstall(id);
			clear();
			if (uninstalled)
			{
				printw("Successfully uninstalled!");
			}
			else
			{
				printw("Failed to uninstall.");
			}
			refresh();
			});
	}

	if (result->id != "exit")
	{
		printw("\n\nPress any key to exit...");
		refresh();
		getch();
	}

	endwin();
	return 0;
}
