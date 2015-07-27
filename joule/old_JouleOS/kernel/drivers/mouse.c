//See COPYRIGHT.txt for info on copyright
/*Since there is no irq for mouse update it every 100ms*/
#include <k_JouleOS.h>
#include <mytypes.h>
#include <event_drive.h>
#include <mouse.h>

volatile byte mouse_cycle=0;    //unsigned char
volatile sbyte mouse_byte[3];    //signed char
volatile sbyte mouse_x=0;        //signed char
volatile sbyte mouse_y=0;        //signed char


//global api vars
volatile MouseInfo Mouse1;

//Mouse functions
void mouse_handler() //struct regs *a_r (not used but just there)
{
  switch(mouse_cycle)
  {
    case 0:
      mouse_byte[0]=inportb(0x60);
      mouse_cycle++;
      break;
    case 1:
      mouse_byte[1]=inportb(0x60);
      mouse_cycle++;
      break;
    case 2:
      mouse_byte[2]=inportb(0x60);
      mouse_x=mouse_byte[1];
      mouse_y=mouse_byte[2];
      mouse_cycle=0;
      //mouse handler is now done so we can act
      //bit4=x bit5=y
      //cwin->width is the current screen width and same for cwin->height
      //mouse_x and mouse_y is the movement data, Mouse1.mouse_x and such is what the X for the mouse is
      if(get_bit(5,mouse_byte[0])==0){
      	Mouse1.mouse_x=Mouse1.mouse_x+mouse_x;
          if(Mouse1.mouse_x>CurrentConsole->width){Mouse1.mouse_x=CurrentConsole->width-2;}
      }else{
      	Mouse1.mouse_x=Mouse1.mouse_x-mouse_x;
          if(Mouse1.mouse_x>CurrentConsole->width){Mouse1.mouse_x=1;} //no sign bit
     }
     //mouse_y=mouse_y;
     //mouse_x=mouse_x/4;
     if(get_bit(6,mouse_byte[0])==0){ //0 is not signed anythignn else is signed
     	Mouse1.mouse_y=Mouse1.mouse_y-mouse_y;
          if(Mouse1.mouse_y>CurrentConsole->height){Mouse1.mouse_y=1;}
     }else{
     	Mouse1.mouse_y=Mouse1.mouse_y+mouse_y;
          if(Mouse1.mouse_y>CurrentConsole->height){Mouse1.mouse_y=CurrentConsole->height-2;}
     }
     Mouse1.mouse_click=mouse_byte[0]&7; //clear all but click data
     CallEvent(MOUSE_CHANGE,9,&Mouse1);
     break;
  }
}

inline void mouse_wait(byte a_type) //unsigned char
{
  dword _time_out=100000; //unsigned int
  if(a_type==0)
  {
    while(_time_out--) //Data
    {
      if((inportb(0x64) & 1)==1)
      {
        return;
      }
    }
    return;
  }
  else
  {
    while(_time_out--) //Signal
    {
      if((inportb(0x64) & 2)==0)
      {
        return;
      }
    }
    return;
  }
}

inline void mouse_write(byte a_write) //unsigned char
{
  //Wait to be able to send a command
  mouse_wait(1);
  //Tell the mouse we are sending a command
  outportb(0x64, 0xD4);
  //Wait for the final part
  mouse_wait(1);
  //Finally write
  outportb(0x60, a_write);
}

byte mouse_read()
{
  //Get's response from mouse
  mouse_wait(0);
  return inportb(0x60);
}

void mouse_install()
{
  byte _status;  //unsigned char

  //Enable the auxiliary mouse device
  mouse_wait(1);
  outportb(0x64, 0xA8);

  //Enable the interrupts
  mouse_wait(1);
  outportb(0x64, 0x20);
  mouse_wait(0);
  _status=(inportb(0x60) | 2);
  mouse_wait(1);
  outportb(0x64, 0x60);
  mouse_wait(1);
  outportb(0x60, _status);

  //Tell the mouse to use default settings
  mouse_write(0xF6);
  mouse_read();  //Acknowledge

  mouse_write(0xE8);
  mouse_read();
  mouse_write(0);
  mouse_read();

  //Enable the mouse
  mouse_write(0xF4);
  mouse_read();  //Acknowledge

  //Setup the mouse handler
  //irq_install_handler(12, mouse_handler);
}















