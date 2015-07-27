/*this contains index control commands*/

byte flush_index(byte drive,byte index_num){
	byte error;
	byte buffer[512];
	word i;
	index_num=index_num*512;
	while (i<=512){
		buffer[i]=indexb[index_num+i];
	}
	error=abswriteN(drive,1,4,&buffer); /*the 4 is to change*/
	return error;
}

byte write_entry(byte drive,word num,char* data){
	byte ret;
	byte* ptr;
	byte i;
	num=num*16;
	ptr=indexb;
	ptr+num;
	while (i<=16){
		*ptr=*data;
		data++;
		ptr++;
		i++;
	}
	ret=flush_index(drive,0);
	return ret;
}


	