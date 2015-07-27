



int liballoc_lock(){
	stopints();
	return 0;
}

int liballoc_unlock(){
	startints();
	return 0;
}
void* liballoc_alloc(int num){
     unsigned char *tmp_liballoc;
	tmp_liballoc=AllocCore(num);
	if(tmp_liballoc==0){printf("Out of memory!");waitkey();} //temporary
	return tmp_liballoc;
}
int liballoc_free(void* ptr,int num){
	FreeCore(ptr,num);
	return 0;
}
