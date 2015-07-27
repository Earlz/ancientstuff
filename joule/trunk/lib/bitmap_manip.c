

inline unsigned char SetDataBit(void *data,unsigned int bit,unsigned int set){ //set should be 1 to set the bit to 1, or 0 to set the bit to 0
    unsigned int *ptr=data; //we do int for 32bit optimization
    ptr=ptr+(bit/32);
    if(set==0){
        *ptr=*ptr&(0xFFFFFFFF^(1<<(bit%32)));
    }else{
        *ptr=*ptr|(1<<(bit%32));
    }

    return *ptr;
}

inline unsigned char GetDataBit(void *data,unsigned int bit){
    unsigned int *ptr=data;
    ptr=ptr+(bit/32);
    return *ptr&(1<<(bit%32));
}

void SetDataBits(void *data,unsigned int base,unsigned int limit,unsigned int set){
    for(base=base;base<=limit;base++){
        SetDataBit(data,base,set);
    }
}

//set should be 1 if your searching for 0 bits and want to set them to 1 or 0 if your looking for 1 bits and want to set them to 0
// the 0 bit should be SET, as if it tries to set this bit and returns it, then it will make the caller think that their was no space
//otherwise though this returns 0 for no space, or the base bit set
unsigned int SetFreeBits(void *data,unsigned int base,unsigned int limit,unsigned int number,unsigned int set){ //set determines which way you want to set it..
    unsigned int tmp;
    unsigned int i;
    unsigned char base_for;
    unsigned char is_free=0;
    tmp=number;
    for(i=base;i<=limit;i++){
        if(GetDataBit(data,i)==(set^1)){
            if(is_free==0){base_for=i;}
            is_free=1;
            }else{
            base_for=0;
            is_free=0;
            }
        if(is_free==1){tmp--;}else{tmp=number;}
        if(tmp==0){
            SetDataBits(data,base_for,base_for+number,set);
            return base_for;
        }
    }
    return 0;
}
