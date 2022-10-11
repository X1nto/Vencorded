#include "options.h"
#include <format>

vencorded::Option::Option(const std::string& id, const std::string& display_name)
	: id(id), display_name(display_name) {}
vencorded::Option::~Option() = default;

std::shared_ptr<vencorded::Option> vencorded::OptionFactory(
	const std::string& id, 
	const std::string& display_name)
{
	return std::make_shared<vencorded::Option>(id, display_name);
}

std::string vencorded::Option::get_display_name() const
{
	return display_name;
}

bool vencorded::Option::handle_selection() const
{
	return true;
}

vencorded::ExtraInfoOption::ExtraInfoOption(
	const std::string& id,
	const std::string& display_name,
	const std::string& display_extra) 
	: Option(id, display_name), display_extra(display_extra) {}
std::shared_ptr<vencorded::ExtraInfoOption> vencorded::ExtraInfoOptionFactory(
	const std::string& id,
	const std::string& display_name,
	const std::string& display_extra)
{
	return std::make_shared<vencorded::ExtraInfoOption>(id, display_name, display_extra);
}
std::string vencorded::ExtraInfoOption::get_display_name() const { 
	return Option::get_display_name() + std::format(" ({})", display_extra);
};