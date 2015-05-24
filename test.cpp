#include "test.hpp"
#include "keyboard.hpp"
#include "video.hpp"
#include "textmode.hpp"
#include "syscall.hpp"

namespace user
{
	void start()
	{
		//((void(*)(const char))system::calls[0])('f');
		
		system::syscall(0, 'p', 'h', 'i', 'j', 'k');
		printf(" n ist: %i", system::syscall(system::call_number::TEST, 0,0,0,0,0));
	};
	
	uint32_t test () { return 987; };
}
