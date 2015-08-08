#pragma once

#include "types.hpp"
namespace Font
{
	struct Font_info
	{
		uint8_t* mask;
		uint8_t  W, distW;
		uint8_t  H, distH;
	};

	extern struct Font_info Lucidia_Console;
};