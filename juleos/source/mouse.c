typedef struct{
	word x;
	word y;
	byte right;
	byte down;
}mouse_inf;
	

void mouse_init(ovid)
{
	asm mov ah,0xc2
	asm mov al,0
	asm mov bh,0x01
	int 0x15
	

}

mouse_inf mouse_update()
{
	mouse_inf mouse;
	bytes t;
	word w1,w2,w3,w4;
	asm pop w1
	asm pop w2
	asm pop w3
	asm pop w4
	if (w4<>0){
		asm push w1
		asm push w2
		asm push w3
		asm push w4}else{
	mouse.x=w2;
	mouse.y=w3;
	if (w1&0x
	

}



}







