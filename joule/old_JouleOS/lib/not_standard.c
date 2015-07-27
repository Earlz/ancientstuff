//See COPYRIGHT.txt for info on copyright
#include <stdio.h>
#include <mytypes.h>
char hex_digits[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void print_hex(unsigned int dat){
     unsigned int i=7; //7
     unsigned char tmp[2]={'\0',0};
     printf("0x");
     //waitkey();
     while(((dat&(0xF<<(i*4)))>>(i*4))==0){
         if(i==0){break;}
         i--;
     }
     for(i=i;i>0;i--){
          tmp[0]=hex_digits[(dat&(0xF<<(i*4)))>>(i*4)];
         // waitkey();
          printf(tmp);
          //wait(2000);
          if(i==0){break;}
     }
     tmp[0]=hex_digits[(dat&(0xF))];
     printf(tmp);

}

void special_print_hex(unsigned int dat){
     unsigned int i=7;
     BOOL not_zero=0;
     unsigned char tmp[2]={'\0',0};
     //waitkey();
     for(i=7;i>0;i--){
         if(((dat&(0xF<<(i*4)))>>(i*4))==0 && not_zero==0){}else{
            not_zero=1;
          tmp[0]=hex_digits[(dat&(0xF<<(i*4)))>>(i*4)];
         // waitkey();
          printf(tmp);
          //wait(2000);
         }
          if(i==0){if(not_zero==0){printf("0");}break;}

     }
     tmp[0]=hex_digits[(dat&(0xF))];
     printf(tmp);

}

void k_special_print_hex(unsigned int dat){
     unsigned int i=7;
     unsigned char tmp[2]={'\0',0};
     //waitkey();
    while(((dat&(0xF<<(i*4)))>>(i*4))==0){
         if(i==0){break;}
         i--;
     }
     for(i=7;i>0;i--){
          tmp[0]=hex_digits[(dat&(0xF<<(i*4)))>>(i*4)];
         // waitkey();
          k_printf(tmp);
          //wait(2000);
          if(i==0){break;}
     }
     tmp[0]=hex_digits[(dat&(0xF))];
     k_printf(tmp);

}


char dec_digits[]={'0','1','2','3','4','5','6','7','8','9'};


void print_decu(unsigned int dat){
     unsigned int i,j;
     char buffer[15];
     char tmp[2];
     unsigned int rem=0;
     tmp[1]=0;
     for(i=1;i<11;i++){ //10 digits
          if(dat==0){buffer[i]=dec_digits[0];}
          rem=dat%10;
          //print_hex(rem);
          buffer[i]=dec_digits[rem];
          if(dat<10){
               buffer[i+1]=dec_digits[dat/10];
               break;
          }else{

               dat=dat/10;
          }
     }
     //waitkey();
     for(j=i;j>=1;j--){
          //print_hex(j);
          //waitkey();
          tmp[0]=buffer[j];
          tmp[1]=0;
          printf(tmp);
          if(j==0){break;}
     }
}
void k_print_decu(unsigned int dat){
     unsigned int i,j;
     char buffer[15];
     char tmp[2];
     unsigned int rem=0;
     tmp[1]=0;
     for(i=1;i<11;i++){ //10 digits
          if(dat==0){buffer[i]=dec_digits[0];}
          rem=dat%10;
          //print_hex(rem);
          buffer[i]=dec_digits[rem];
          if(dat<10){
               buffer[i+1]=dec_digits[dat/10];
               break;
          }else{

               dat=dat/10;
          }
     }
     //waitkey();
     for(j=i;j>=1;j--){
          //print_hex(j);
          //waitkey();
          tmp[0]=buffer[j];
          tmp[1]=0;
          k_printf(tmp);
          if(j==0){break;}
     }
}


void print_decs(signed int dat){
     unsigned int i,j,t;
     char buffer[15];
     char tmp[2];
     unsigned int rem=0;
     tmp[1]=0;
     t=1;
     if(dat<0){t=2;}
     for(i=t;i<11;i++){ //10 digits
         // if(dat&80000000!=0 && i==1){dat=dat&80000000;buffer[1]='-';i=2;}
          if(dat==0){buffer[i]=dec_digits[0];}
          rem=dat%10;
          //print_hex(rem);
          buffer[i]=dec_digits[rem];
          if(dat<10){
               buffer[i+1]=dec_digits[dat/10];
               break;
          }else{

               dat=dat/10;
          }
     }
     //waitkey();
     for(j=i;j>=1;j--){
          //print_hex(j);
          //waitkey();
          tmp[0]=buffer[j];
          tmp[1]=0;
          printf(tmp);
          if(j==0){break;}
     }
}






void k_print_hex(unsigned int dat){
     unsigned int i;
     unsigned char tmp[2]={'\0',0};
     k_printf("0x");
     //waitkey();
     for(i=7;i>0;i--){
          tmp[0]=hex_digits[(dat&(0xF<<(i*4)))>>(i*4)];
         // waitkey();
          k_printf(tmp);
          //wait(2000);
          if(i==0){break;}
     }
     tmp[0]=hex_digits[(dat&(0xF))];
     k_printf(tmp);

}














