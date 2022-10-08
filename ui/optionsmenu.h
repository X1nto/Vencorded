#pragma once

#include <vector>
#include <memory>
#include <string>
#include "options.h"

namespace vencorded
{
	using option_t = std::shared_ptr<Option>;
	using options_t = std::vector<option_t>;
	class OptionsMenu
	{
		const std::string title;
		const options_t options;

		const size_t options_s;
		const size_t last_i;

		int current_i;

		void print_menu();
	public:
		OptionsMenu(const std::string& title, const options_t& options);
		~OptionsMenu();

		option_t selection_prompt();
	};
}