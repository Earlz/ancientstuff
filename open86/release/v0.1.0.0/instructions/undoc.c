//yea.. I might just try to emulate a few undocumented instructions

#include "../_base.h"



/**salc -0xD6- this is set al on carry -no arguments
log:
    if cf=0 then al=0 else al=FF
**/
void salc(){ //set al on carry
    if(flags->cf==0){*gregs8[AL]=0;
    }else{*gregs8[AL]=0xFF;
    }

}




















