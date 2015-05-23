#include "keyboard.hpp"

#include "hwaccess.hpp"

namespace hw
{
namespace keyboard
{
	bool shift = false;

	void init()
	{
		//clear buffer
		while (asm_inb(0x64) & 1)
			asm_inb(0x60);
	};

	uint8_t get_key(bool& pressed)
	{
		while (!(asm_inb(0x64) & 1)) { }
		uint8_t code = asm_inb(0x60), key = code & 0x7f;

		uchar_t port_value = asm_inb(0x61);	//Let the keyboard know, that we got it
		asm_outb(0x61, port_value | 0x80);	// 0->1
		asm_outb(0x61, port_value &~0x80);	// 1->0

		pressed = !(code & B(10000000));		//sing bit indicates if the key was pressed or released

		if (key == Keys::RLEFT_SHIFT || key == Keys::RRIGHT_SHIFT)
		{
			shift = !shift;					//toggle shift
			return 0;
		}
		
		return key;
	};

	char getc()
	{
		bool was_pressed;
		uint8_t c = get_key(was_pressed);

		while (true)
		if (was_pressed && c != 0)
			break;
		else
			c = get_key(was_pressed);

		if (shift)
			return asciiShift[c];
		else
			return asciiNonShift[c];
	};
}
}
