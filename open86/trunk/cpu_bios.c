#include "_base.h"

typedef struct{
	unsigned short offset;
	unsigned short segment;
}__attribute__((packed))
far_ptr;

typedef struct{
	unsigned char ascii;
	unsigned char scancode;
}__attribute__((packed))
keyboard_entry;

typedef struct{
	unsigned char column;
	unsigned char row;
}__attribute__((packed))
cursor;

//Wow! what a huge struct!
//sorry for the long names, but I wanted it to be fairly readable
typedef struct{
	unsigned short COM_base_address[4];
	unsigned short LPT_base_address[4];
	unsigned short equipment_list;
	unsigned char reserved1;
	unsigned short memory_size;
	unsigned short reserved2;
	unsigned char shift_status;
	unsigned char extended_shift;
	unsigned char alternate_keypad;
	unsigned short keyboard_head_ptr;
	unsigned short keyboard_tail_ptr;
	keyboard_entry keyboard_buffer[16];
	unsigned char diskette_recalibrate_status;
	unsigned char diskette_motor_status;
	unsigned char motor_off_counter;
	unsigned char last_diskette_operation_status;
	unsigned char diskette_drive_status[7];
	unsigned char display_mode;
	unsigned short display_columns;
	unsigned short display_page_size;
	unsigned short display_start_buffer;
	cursor cursor_pos[8];
	unsigned char cursor_end_scanline;
	unsigned char cursor_start_scanline;
	unsigned char current_page;
	unsigned short CRT_base_address;
	unsigned char current_mode_controller_register;
	unsigned char current_color_select_register;
	unsigned char reserved3[5];
	unsigned int timer_counter;
	unsigned char timer_overflow;
	unsigned char break_key_state;
	unsigned char reset_state_flag;
	unsigned char last_fixed_operation_status;
	unsigned char number_of_fixed_disks;
	unsigned char reserved4;
	unsigned char reserved5;
	unsigned char LPT_timeout_values[4];
	unsigned char COM_timeout_values[4];
	unsigned short keyboard_buffer_start_ptr;
	unsigned short keyboarD_buffer_end_ptr;
	unsigned char display_rows; //minus one
	unsigned short char_height;
	unsigned short reserved6;
	unsigned short reserved7;
	unsigned char diskette_media_control;
	unsigned char fixed_disk_controller_status;
	unsigned char fixed_disk_error_status;
	unsigned char fixed_disk_int_control;
	unsigned char reserved8;
	unsigned char diskette_media_state[2]; //this contains drive0 first, then drive 1
	unsigned short reserved9;
	unsigned char diskette_cylinder[2]; //this has drive 0 first, then drive 1
	unsigned char keyboard_status_flags;
	unsigned char keyboard_led_status;
	far_ptr ptr_wait_complete_flag;
	unsigned int user_wait_count;
	unsigned char wait_active_flag;
	unsigned char reserved10[7];
	far_ptr ptr_video_parameters;
	unsigned char reserved11[44];
	unsigned char ICA[16]; //intra-application communcation area
	unsigned char print_screen_status;
	unsigned char reserved12[3];
	unsigned char phantom_drive_flag;
	unsigned char reserved13[250];
}__attribute__((packed))BIOS_low_memory_struct;

typedef struct{
	far_ptr post_jump;
	unsigned char reserved1; //dunno...says the post jump should somehow have 5 bytes!?
	unsigned char release_date[8]; //this is the date in ASCII
	unsigned short reserved2;
	unsigned char system_model;
}__attribute__((packed))BIOS_high_memory_struct;


far_ptr *interrupts;
BIOS_low_memory_struct *low_memory;
BIOS_high_memory_struct *high_memory;

void init_bios_vars(){
	interrupts=core+((0<<4)+0); //interrupts start at 0:0
	low_memory=core+0x400;
	high_memory=core+((0xF000<<4)+0xFFF0);
}

void BIOS_post(){
	init_bios_vars();
	gregs16[AX]=0;
	gregs16[BX]=0;
	gregs16[CX]=0;
	gregs16[DX]=0;
	gregs16[SI]=0;
	gregs16[DI]=0;
	gregs16[BP]=0;
	gregs16[SP]=0x100;
	sregs[DS]=0x500;
	sregs[ES]=0x500;
	sregs[SS]=0x400;
	memset(low_memory,0,105+250);
	high_memory->post_jump.segment=0xF000;
	high_memory->post_jump.offset=0;
	strcpy(high_memory->release_date,"00/00/07");
	high_memory->system_model=0xFE;


	if(bin_loaded==1){
		push(0x500);
		push(bin_ip);
		return;
	}



}


//port 0xFE
unsigned int BIOS_port(unsigned int out,unsigned char in_or_out,unsigned char size){
	switch(out){
		case 0: //0 is post
		BIOS_post();
		break;
	}

}








