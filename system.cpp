#include "system.hpp"

extern "C"
{
/*ASM call to test, if the CPU supports CPUID.*/
extern uint32_t CPU_CPUID_supported();
/*ASM call to for CPUID_CPU_COMPANY_NAME.*/
extern void CPU_CPUID_request_00000000h(char_t* string);
/*ASM call to for CPUID_CPU_COMPANY_NAME.*/
extern void CPU_CPUID_request_00000001h(char_t* flags_out);
}

void sxlb_system_halt(const char_t* error_msg)	//__attribute__((noreturn))
{
	cli
#if __CHECKS_NLPTR
	if (!error_msg)
		syshlt("Double Fault");	//man wtf, are you trying to troll me?
#endif
	
	ui::text::set_foreground_color(FC_LIGHTGRAY | FC_MOD_INTENSITY);
	ui::text::set_background_color(BC_LIGHTBLUE);
	ui::text::clear_screen();
	ui::text::set_cursor(0,0);

	ui::text::writeline("  *****************");
	ui::text::writeline("  *  FATAL ERROR  *");
	ui::text::writeline("  * SYSTEM HALTED *");
	ui::text::writeline("  *****************");
	
	printlf("\n\nInfo:\n");
	printlf(" Type: \n  %s\n", error_msg);

	printlf(" CPU state:\n");
	sxlb_system_dumb();
	ui::video::update();

	hlt
};

void sxlb_system_dumb()
{
	uint8_t tab = ui::text::get_tab_with();
	ui::text::set_tab_with(20);

	printf("  eax: %u", asm_get_register_eax()); printlf("\tesi: %u", asm_get_register_esi());
	printf("  ebx: %u", asm_get_register_ebx()); printlf("\tedi: %u", asm_get_register_edi());
	printf("  ecx: %u", asm_get_register_ecx()); printlf("\tesp: %u", asm_get_register_esp());
	printf("  edx: %u", asm_get_register_edx()); printlf("\tebp: %u", asm_get_register_ebp());
	printlf("\teip: %u", asm_get_register_eip());
	ui::text::new_line();
	printlf("  cr0: %u", asm_get_register_ctrl(0));
	printlf("  cr1: ????");							//not accessable
	printlf("  cr2: %u", asm_get_register_ctrl(2));	//faulting address in cr2
	printlf("  cr3: %u", asm_get_register_ctrl(3));
	printlf("  cr4: %u", asm_get_register_ctrl(4));

	ui::text::set_tab_with(tab);
};

void parse_00000001h(uint32_t* flags_in, char_t* string_out)
{
	bool apci_support	= 0;

	uint8_t Stepping	= 0x0000000f & flags_in[0];
	uint8_t Modell		= 0x000000f0 & flags_in[0];
	uint8_t Familie		= 0x00000f00 & flags_in[0];
	uint8_t Typ			= 0x0000f000 & flags_in[0] & B(00000011);	//intel only
	uint8_t Erw_Modell	= 0x000f0000 & flags_in[0];
	uint8_t Erw_Familie = 0x0ff00000 & flags_in[0];


	uint8_t Brand_ID	= 0x000000ff & flags_in[1];
	uint8_t CLFLUSH_S	= 0x0000ff00 & flags_in[1];
	uint8_t cpu_count	= 0x00ff0000 & flags_in[1];
	uint8_t APIC_ID		= 0xff000000 & flags_in[1];
};

static bool tested = false;
static bool result = false;
void sxlb_system_cpu_info(CPUID_REQUEST_TYPE request_test, char_t* buffer)
{
#if __CHECKS_NLPTR
	if (!buffer)
		return;
#endif

	if (!tested)	//at first, test compatibility
	{
		result = CPU_CPUID_supported();
		tested = true;
	}

	if (!result)
	{
		buffer[0] = 0;
		return;
	}
	else
	{
		switch (request_test)
		{
			case CPUID_CPU_COMPANY_NAME:
				CPU_CPUID_request_00000000h(buffer);
				break;

			case CPUID_CPU_INFO:
			{
				char_t parse_buffer[16];
				CPU_CPUID_request_00000001h(parse_buffer);
				parse_00000001h((uint32_t*)parse_buffer, buffer);
			} break;

			default:
				buffer[0] = 0;
				return;
				break;
		}
	}
};

extern uint32_t kernel_end;
uint32_t sxlb_system_kernel_end_address()
{
	return &kernel_end;
};
kk
