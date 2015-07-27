


void *linked_GetObject(void *data,void *prev,void *next,signed int number){
    signed int tmp;
    unsigned int diff_next=(next-data);
    unsigned int diff_prev=(prev-data);
    void **ptr;
    tmp=number;
    if(number==0){return data;}
    if(number>0){
        for(;tmp!=0;tmp--){
            data=data+diff_next;
            ptr=data;
            data=*ptr;

        }
    }else{ //tmp is less than 0, or negative
        for(;tmp!=0;tmp++){
            data=data+diff_prev;
            ptr=data;
            data=*ptr; //C doesn't allow you to jsut specify void **, have to make a whole new variable!
        }
    }
    return data;
}






