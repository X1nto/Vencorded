#pragma once

#include <type_traits>

template<typename CastTo, typename CastFrom, typename Action>
inline void cast_to(CastFrom value, Action block)
{
	CastTo casted_ptr = dynamic_cast<CastTo>(value);
	if (casted_ptr != nullptr)
	{
		block(casted_ptr);
	}
}
