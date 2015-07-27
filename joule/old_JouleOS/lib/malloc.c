//See COPYRIGHT.txt for info on copyright
//512b of overhead per page
/*malloc header
512 bytes saying what bytes are used; if all 0 then page is free()'ed
1 dword(ptr) pointing to the next page


kmalloc core page info:
all ptrs pointing toward pages[used by the application]
the last ptr space is used for pointer to the next kmalloc core page, or 0 if this is the last
*/

typedef struct{
	unsigned int used[112];
//	void *next;
	unsigned char data[3984];
}km_page;
typedef struct{
	void *ptrs[4092];
	void *next;
}km_page_info;






/*
km_page_info **kmalloc_ptr=0; //a pointer to allocated memory which points to pages of allocated memory
unsigned int kmalloc_count;
void *kmalloc(unsigned int size){
	char *ptr;signed int i;

	if (kmalloc_ptr==0){ //this is the first call of kmalloc then
	kmalloc_ptr=AllocCore(1);
	if(kmalloc_ptr==0){ return 0;}//return 0 kmalloc_ptr is not valid
	kmalloc_ptr[0]=AllocCore(1);
	if (kmalloc_ptr[0]==0){return 0;}
	}
	while (i
	i=find_0_bit(*kmalloc_ptr->*used);
     if (i==-1){

}

*/
//some handy little functions to make things simpler
//void km_add_cpage(){



/*
void kmc_push(void *adr){
	if (kmalloc_count%4092==0){
		if(next==0){km_add_cpage()}
	(unsigned int)*kmalloc_ptr=;

	}

}
*/

