#pragma once

#include <string>
#include <memory>

namespace vencorded
{
class Option
{
public:
	const int id;
	const std::string display_name;

	Option(const int id, const std::string& display_name);
	virtual ~Option();

	virtual std::string get_display_name() const;
	virtual bool handle_selection() const;
};
std::shared_ptr<Option> OptionFactory(const int id, const std::string& display_name);

class ExtraInfoOption : public Option
{
public:
	const std::string display_extra;

	ExtraInfoOption(
		const int id,
		const std::string& display_name,
		const std::string& display_extra);
	virtual std::string get_display_name() const override;
};
std::shared_ptr<ExtraInfoOption> ExtraInfoOptionFactory(const int id, const std::string& display_name, const std::string& display_extra);

}