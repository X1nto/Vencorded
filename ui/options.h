#pragma once

#include <string>
#include <memory>

namespace vencorded
{
	class Option
	{
	public:
		const std::string id;
		const std::string display_name;

		Option(const std::string& id, const std::string& display_name);
		virtual ~Option();
	};
	std::shared_ptr<Option> OptionFactory(const std::string& id, const std::string& display_name);

	class ExtraInfoOption : public Option
	{
	public:
		const std::string display_extra;
		
		ExtraInfoOption(
			const std::string& id, 
			const std::string& display_name, 
			const std::string& display_extra);
	};
	std::shared_ptr<ExtraInfoOption> ExtraInfoOptionFactory(const std::string& id, const std::string& display_name, const std::string& display_extra);
}