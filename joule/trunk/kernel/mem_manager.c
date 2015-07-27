#include <k_JouleOS.h>
//#include <string.h>




//this override functions let you alloc or free a range of addresses..
unsigned int override_AllocCore(AddressSpace *space, unsigned int base,unsigned int number){
    if((base+number)>space->limit){return 0;}
    SetDataBits(space->address,base,number,1);
    return base;
}
unsigned int FreeCore(AddressSpace *space,unsigned int base,unsigned int number){
    if((base+number)>space->limit){return 0;}
    SetDataBits(space->address,base,number,0);
    return base;
}


unsigned int AllocCore(AddressSpace *space, unsigned int number){
    return SetFreeBits(space->address,0,space->limit,number,1); //yea simple..
}
void SetupAddressSpace(AddressSpace *space,void *address,unsigned int limit,unsigned char uses_paging,void *owner){
    space->address=address;

    memset(space->address,0,(limit/32)+1);

    //putc_tmp('2');
    space->limit=limit;
    space->page_addressing=uses_paging;
    space->owner=owner;
    AllocCore(space,1); //allocate first bit..
}
unsigned int virtual_address[32768];
unsigned int physical_address[32768];

AddressSpace physical_memory;
AddressSpace virtual_memory;

void InitMemMan(){
    //putc_tmp("h");
    //I 0xFFFF FFFF isn't right!?
    SetupAddressSpace(&physical_memory,physical_address,0xFFFFF,1,0); //eventually change the limit to the size of physical memory..
    SetupAddressSpace(&virtual_memory,virtual_address,0xFFFFF,1,0);
    //__asm("hlt");
    putc_tmp('x');
    override_AllocCore(&physical_memory,0,0x20000/4096); //allocate first 2 megs

}
















