//See COPYRIGHT.txt for info on copyright
/*ideas go here since this thing isnt made yet
do not want to use the stack allocation thing



Physical memory used by kernel:
0-1mb are reserved for special things
interrupt vectors are saved;
01000h-01200 is used for temporary int 13h stuff





*/

unsigned int pages[32768]; //128k tells which pages are in use
//unsigned int superpages[32];

void *FAllocCore(void *page,unsigned int num_pages);

void init_memman(){
	memset(pages,0,32768*4);
	//FAllocCore(0xFF800000,300); //gives us 300 pages for kernel
	FAllocCore(0,16); //reserves first meg-- bios, video memory and such
	FAllocCore(0x100000,300); //gives us 300 pages for kernel and oter assorted things
}
//This lets you allocate a range of pages; should only be used by kernel
void *FAllocCore(void *page_ptr,unsigned int num_pages){
	unsigned int index;unsigned int page=page_ptr;
	page=page/4096; //convert the address to a page number
	num_pages+=page;
	while(num_pages>page){
		index=page/32; //get what the index for pages is
		pages[index]=pages[index]|(1<<(page%32)); //set the bit
		page++;
	}
}

unsigned int is_free_bits(unsigned int *data,unsigned int start_bit,unsigned int num,unsigned int len){
	unsigned int i,j=0,endbit,total,counter; //i bet i could've used less stack but anyway
	endbit=num+start_bit; //make it the end bit
	total=0;
	counter=0;
	for(i=start_bit;total<=endbit;i++,total++){
		if((counter/8)==(len+1)){return 2;} //bits go past length
		counter++;
		if((data[j]&(1<<i))!=0){return 4+total;} //the bits are not free-- subtract 4 and you got number of the set bit
		if(i==31 && total<endbit){i=0;start_bit=0;j++;} //bits are free here but need to check next dword
	}
	return 1; //the bits are free
}


unsigned int num_set_bits(unsigned int *data,unsigned int start_bit,unsigned int len){
     unsigned int i,j=0,endbit,total,counter; //i bet i could've used less stack but anyway
	endbit=len*8+start_bit; //make it the end bit
	total=0;
	counter=0;
	for(i=start_bit;total<=endbit;i++,total++){
		if(counter/8==len+1){return 1;} //bits go past length --have to use signed stuff for this
		counter++;
		if((data[j]&(1<<i))==0){return total+4;} //the bits are not free-- subtract 4 and you got number of the set bit
		if(i==31 && total<endbit){i=0;start_bit=0;j++;} //bits are free here but need to check next dword
	}
	return 0; //the bits are free
}

unsigned int* set_bits(unsigned int *data,unsigned int start_bit,unsigned int end_bit,unsigned int len){
	unsigned int i,counter,j,total;
	j=0;
	counter=0;
	total=0;
	for(i=start_bit;total<=end_bit;i++,total++){
		if(counter==len+1){return 0;} //goes past len
		counter++;
		data[j]=data[j]|(1<<i);
		if(i==31 && total<end_bit){i=0;start_bit=0;j++;}
	}
	return data;
}

unsigned int* clear_bits(unsigned int *data,unsigned int start_bit,unsigned int end_bit,unsigned int len){
	unsigned int i,counter,j,total;
	j=0;
	total=0;
	counter=0;
	for(i=start_bit;total<=end_bit;i++,total++){ //i gets roundoff every 32 cycles so we need one that don't
		if(counter==len+1){return 0;} //goes past len
		counter++;
		data[j]=data[j]&(0xFFFFFFFF^(1<<i));
		if(i==31 && total<end_bit){i=0;start_bit=0;j++;}
	}
return data;
}

/*******My Debug Printf Format******
C means check and then a number
I means if or case
L means loop
D means data and there will be data after the number
T means test and then data
************************************/



unsigned volatile char debug_tmp[33];
void *FreeCore(void *page_ptr,unsigned int num_pages);
void *AllocCore(unsigned int num_pages){
	unsigned int i=0,t,j;unsigned int page,super_page;
	unsigned char *tmp;
	unsigned int index,index2,j2;
	/*
	while(superpages[i]==0xFFFFFFFF){
		if(i>=32){return 0;} //error! couldnt find a free superpage
		i++;
	}
	super_page=find_0_bit(superpages[i]);
	super_page=super_page+(i*32);
	i=super_page*1024; //this is the page number where the super page begins*/

	if (num_pages>1){ //use a special allocation method
          while(pages[i]==0xFFFFFFFF){
		if(i>=32768){return 0;}
		//k_printf("C1");
		i++;
          }
          index=i;
          t=find_0_bit(pages[i]);
          /*
          k_printf("T1:");
          num2bstr(0,debug_tmp);
          k_printf(debug_tmp);
          k_printf("\n");

          k_printf("T2:");
          num2bstr(1,debug_tmp);
          k_printf(debug_tmp);
          k_printf("\n");

          k_printf("T3:");
          num2bstr(0xFFFFFFFF,debug_tmp);
          k_printf(debug_tmp);
          k_printf("\n");

          k_printf("T4:");
          num2bstr(0x2CA80D1F ,debug_tmp);
          k_printf(debug_tmp);
          k_printf("\n");

          k_printf("D1:");
          num2bstr(t,debug_tmp);
          k_printf(debug_tmp);
          */
          j=t;
          for(;;){
          	//k_printf("L1");
          	j=is_free_bits(&pages[index],t,num_pages,32768*4-index*4);
          	if(j==1){
          	//	k_printf("I1");
          		if(set_bits(&pages[index],t,num_pages+t,32768*4-index*4)==0){return 0;} //means page table is full or there isn't enough room
                 //   k_printf("C3");
                    return (index+(t*32))*4096; //this and above means that the bits were free
               }
               if(j==2){k_printf("I2");return 0;} //goes past length
               if(j>=4){ //some bits weren't free
                //k_printf("I3");
                    j=j-4; //we added 4 to it for error code
                    index=index+j/32; //advances index if past a dword
                    t=(j%32)+1;
                  //  k_printf("C4");
                    t=num_set_bits(&pages[index],t,32768*4-index*4);
                    //k_printf("C5");
                    //if t is 0 then we just continue
                    if(t==1){k_printf("I4");return 0;} //goes beyond, full
                    t=t-4; //we added 4 on this one too
                    index=index+t/32; //advances index if past a dword
                    t=(t%32)+1;
                    //k_printf("C6");


                     //idk who would need 32 pages but anyway
                    //and now reset and will begin the loop again
               }
          }	}else{ //if num_pages==1
     //k_printf("I5");

	}
	while(pages[i]==0xFFFFFFFF){
		if(i>=32768){return 0;}
		//k_printf("t3");
		i++;
	}

	page=find_0_bit(pages[i]);

	pages[i]=pages[i]|(1<<page);
	page=page+(i*32);
	page=page*4096;
	//if (i==0){	k_printf("t3");}
     t=i;
     return page;
}

void *FreeCore(void *page_ptr,unsigned int num_pages){
		unsigned int index;unsigned int page=page_ptr;
	page=page/4096; //convert the address to a page number
	num_pages+=page;
	while(num_pages>page){
		index=page/32; //get what the index for pages is
		pages[index]=pages[index]&(0xFFFFFFFF^(1<<(page%32))); //CHECK HERE;unset the bit
		page++;
	}
}


typedef struct{ //only 4 bytes of overhead!
	unsigned int free;
	unsigned char data[4088];
}sbrk_page;


sbrk_page *spage;
void *sbrk(unsigned int size){
	void *rety;
	if (spage==0){
		spage=AllocCore(1);
          if (spage==0){return 0;}
          spage->free=0;
	}
	if (spage->free==0x1000){spage=AllocCore(1);spage->free=4;}
	if (4096-spage->free>=size){spage=AllocCore(1);spage->free=4;}
	rety=spage->free;
	return (unsigned int)rety+(unsigned int)spage;
}
void _alloc43a(unsigned int size){//Its a copy!
SetESP(GetESP()+size);
}
/*
typedef struct{
	unsigned int free[8];
	unsigned char data[3844];
}kmalloc_page;

typedef struct{
	kmalloc_page *list[1023];
	void *next;
}kmalloc_list;

volatile kmalloc_list *klist=0;


void add_kmalloc_page(){
	if



void *kmalloc(unsigned int size){

     if (klist==0){
     	klist=AllocCore(1);
     	if (klist==0){return 0;}
     	klist->list[0]=AllocCore(1);
     	if (klist->list[0]==0){return 0;}
     	memset(klist->list[0],0,256); //zero the free listing
     }
     //size=size/2; //because i half the overhead by using 2 byte groupings instead of 1 we must do this
     if (size%2!=0){size++;}
     size=size/2;


}
*/
