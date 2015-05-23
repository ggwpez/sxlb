#include "test.hpp"
#include "keyboard.hpp"
#include "video.hpp"
#include "textmode.hpp"

namespace user
{
	void move_cursor();
	uint16_t sX = 0, sY = 0;
	uint8_t x = 0, y = 0, oX = 0, oY = 0;
	uchar_t input;
	bool pressed, spaced;
	
	void start()
	{
		hw::keyboard::init();		
		ui::video::get_size(sX, sY);
		ui::video::draw_pixel(x,y, ui::video::video_color::VC_BLUE);	//draw the cursor		
		ui::video::update();
		
		while (true)
		{
			input = hw::keyboard::getc();
			
			if (input == ' ')
			{ oX = x; oY = y; }
			
			move_cursor();			//move the cursor according to the users input
			ui::video::clear_screen(ui::video::bg_color);
			ui::video::draw_line(y,x, oY, oX, ui::video::video_color::VC_BLUE);	//draw the cursor
			ui::video::update();			
		}
	};
	
	void move_cursor()
	{
		if (input == hw::keyboard::Keys::UP)
			if (x > 0) x--;
		
		if (input == hw::keyboard::Keys::DOWN)
			if (x < sX) x++;
			
		if ( input == hw::keyboard::Keys::LEFT)
			if (y > 0) y--;
			
		if (input == hw::keyboard::Keys::RIGHT)
			if (y < sY) y++;
	};
}
