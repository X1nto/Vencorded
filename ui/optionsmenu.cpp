#include "optionsmenu.h"
#include "options.h"
#include "curses.h"
#include "../util/casts.h"

vencorded::OptionsMenu::OptionsMenu(const std::string& title, const options_t& options) 
	: title(title), options(options), options_s(options.size()), last_i(options_s - 1)
{
	this->current_i = 0;
	refresh();
}

vencorded::OptionsMenu::~OptionsMenu()
{
	clear();
}

std::shared_ptr<vencorded::Option> vencorded::OptionsMenu::selection_prompt()
{
	int c;
	bool should_exit = false;

	while (!should_exit)
	{
		print_menu();
		c = getch();
		switch (c)
		{
		case KEY_UP:
			current_i--;
			if (current_i < 0)
			{
				current_i = (int)last_i;
			}
			break;
		case KEY_DOWN:
			current_i++;
			if (current_i > last_i)
			{
				current_i = 0;
			}
			break;
		case KEY_CLOSE:
			should_exit = true;
			break;
		case 10:
			return options[current_i];
		}
		clear();
	}
}

void vencorded::OptionsMenu::print_menu()
{
	printw("%s\n\n", title.c_str());
	for (int i = 0; i < options_s; i++)
	{
		bool is_selected = i == current_i;

		option_t option = options[i];

		printw("  ");
		if (is_selected)
		{
			attron(COLOR_PAIR(1));
		}

		printw("[%i] %s", i + 1, option->display_name.c_str());

		if (is_selected)
		{
			cast_to<ExtraInfoOption*>(option.get(), [](ExtraInfoOption* c) {
				printw(" (%s)", c->display_extra.c_str());
				});
		}

		printw("\n");

		if (is_selected)
		{
			attroff(COLOR_PAIR(1));
		}
	}
	printw("\nUse the arrow keys to navigate; Press Enter to select.");
	refresh();
}