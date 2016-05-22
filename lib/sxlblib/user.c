#include "user.h"
#include "/home/vados/Code/Projects/sxlb/src/system/syscall_defs.hpp"
//#include "/home/vados/Code/Projects/sxlb/src/ui/textmode.hpp"

void set_fc_clr(char clr)
{
	SYSCALL1(SYSCNUM_UI_TEXT_SET_FC, clr);
}

void set_bc_clr(char clr)
{
	SYSCALL1(SYSCNUM_UI_TEXT_SET_BC, clr);
}
