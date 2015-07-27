#include "_base.h"
#include "Open86_Device.h"
#include <stdio.h>
/**This file contains all things related to config files**/

void SetMemSize(unsigned int size){
    printf("mem is now set to %i",size);
}

void SetCpu(char *arch){



}

void Randomize_Memory(){
	unsigned int i;
	unsigned char *ptr=core;
	for(i=0;i<0x100000;i++){
		ptr[i]=rand();
	}
}





/**This is the master parser of the config file, this function basically will
figure out what [configuration] functions to call with what arguments from the practically
unformatter configuration file**/
void ParseConfigFile(unsigned char *cfile){
    FILE *config_file;
    FILE *test_file;
    volatile unsigned char tmp_stor=0;
    unsigned char *tmp,*tmp2,*tmp3;
    void *tmp_ptr,*tmp2_ptr;
    unsigned char comments=0;
    unsigned int i1=0,i2=0;
    unsigned char got=0; //this is set when a cycle completed successful, if this is 0 at it will give a syntax error
    unsigned int buf_size;
    unsigned char use=1; //this is 1 if we actually do a command or 0 if we don't(for commenting and such)
    unsigned char quotes=0;
    unsigned char skipping=0; //this is 1 if were to ignore a byte for this "cycle"
    unsigned char in_it=0; //this is 1 if were in a command yet or 0 if were at the start of the stirng
    config_file=fopen(cfile,"r");
    if(config_file==NULL){panic("Error opening config file\n");}
    tmp=malloc(1024*64);
    if(tmp==NULL){panic("Error allocating temporary memory\n");}
    tmp2=malloc(512);
    if(tmp2==NULL){panic("Error allocating temporary memory\n");}
    tmp_ptr=tmp;
    tmp2_ptr=tmp2; //these store the original pointers so we can free them later..
    GetConfigDir(cfile);
    while(!feof(config_file)){
        if(comments==1){skipping=1;i1--;} //parse out comments
        tmp[i1]=fgetc(config_file);
        if(skipping==0 && tmp[i1]=='\n'){
            tmp[i1]=0;
            comments=0;
            quotes=0;
            in_it=0;
        }
        if(skipping==0 && tmp[i1]=='\r'){
            tmp[i1]=0;
            comments=0;
            quotes=0;
            in_it=0;
        }
        if(skipping==0 && comments==0 && quotes==0 && tmp[i1]==':'){ tmp[i1]=0;in_it=0;} //convert :'s to null termination
        if(skipping==0 && comments==0 && tmp[i1]=='\"'){
            skipping=1;
            i1--; //do this to remove quotes
            quotes=quotes^1; //flip flop the quotes thing
        }


        if(quotes==0 && skipping==0 && comments==0 && in_it==0 && tmp[i1]==' '){skipping=1;i1--;} //skip this byte
        if(skipping==0 && comments==0 && in_it==0 && tmp[i1]=='\t'){skipping=1;i1--;}
        if(skipping==0 && tmp[i1]=='#'){tmp[i1]=0;comments=1; quotes=0;in_it=0;
			while((tmp_stor!='\r') || (tmp_stor!='\n') || (tmp_stor==0xFF)){
				tmp_stor=fgetc(config_file);
				if(tmp_stor=='\n'){break;}
				if(tmp_stor=='\r'){break;}
				if(tmp_stor==0xFF){break;}
				//printf("bah: %c\n",tmp_stor);
				//system("pause");
			}
			i1--;
			comments=0;
			skipping=0;
			in_it=1;
        }
        if(tmp[i1]==0xFF){tmp[i1]=0;break;}
        i1++;
        skipping=0;

    } //now done with the parsing of comments and new lines
    //this completely works as expected!!
    buf_size=i1; //save the size

    printf("\n\nDebug output for config_files.c\n\n");

    //now parse for actual commands!!
    tmp2=tmp;
    while(1){
        if(tmp2>=(tmp+buf_size)){break;} //if were at the end of the buffer
        //now then.. add all of our commands/keywords
        if(strlen(tmp2)==0){got=1;}
        //these are "this os only" prefixes
        if(strcmp(tmp2,"unix_only")==0){
            #ifndef __unix__
            use=0;
            #endif
            tmp2=tmp2+strlen(tmp2)+1; //set it to the next command
            got=1;
        }
        if(strcmp(tmp2,"win32_only")==0){
            #ifndef WIN32
            use=0;
            #endif
            tmp2=tmp2+strlen(tmp2)+1; //set it to the next command
            got=1;
        }

        //actual commands/keywords
        if(strcmp(tmp2,"mem")==0){
            tmp2=tmp2+strlen(tmp2)+1;
            if(use==1){SetMemSize(atoi(tmp2));} //we only actually execute the command if use is 1
            got=1;
        }
        if(strcmp(tmp2,"load_bios")==0){
            tmp2=tmp2+strlen(tmp2)+1;
            if(use==1){LoadBios(tmp2);} //we only actually execute the command if use is 1
            got=1;
        }
        if(strcmp(tmp2,"cpu")==0){
            tmp2=tmp2+strlen(tmp2)+1;
            if(use==1){SetCpu(tmp2);}
            got=1;
        }
        if(strcmp(tmp2,"load_bin_file")==0){
            tmp2=tmp2+strlen(tmp2)+1;
            test_file=fopen(tmp2,"r");
            if(test_file!=NULL){if(use==1){fclose(test_file);LoadBinFile(tmp2);}got=1;}
            else{
                *tmp_filename='\0';
                strcat(tmp_filename,config_dir);
                printf("config_dir=%s\n",config_dir);
                strcat(tmp_filename,tmp2);
                //system("cd");
                if(use==1){LoadBinFile(tmp_filename);}
                got=1;
            }

        }
        if(strcmp(tmp2,"load_com_file")==0){
            tmp2=tmp2+strlen(tmp2)+1;
            test_file=fopen(tmp2,"r");
            if(test_file!=NULL){if(use==1){fclose(test_file);LoadComFile(tmp2);}got=1;}
            else{
                *tmp_filename='\0';
                strcat(tmp_filename,config_dir);
                printf("config_dir=%s\n",config_dir);
                strcat(tmp_filename,tmp2);
                //system("cd");
                if(use==1){LoadComFile(tmp_filename);}
                got=1;
            }
        }
        if(strcmp(tmp2,"randomize_memory")==0){
            Randomize_Memory();
            got=1;
        }
		if(strcmp(tmp2,"load_device")==0){
            tmp2=tmp2+strlen(tmp2)+1;
            strcpy(tmp_filename,tmp2);
            tmp3=tmp_filename;
            tmp3=tmp3+strlen(tmp_filename)-1;
            if(*tmp3=='*'){ //add the predefined OS specific extension
            	*tmp3=0;
            	#ifdef WIN32
            	strcat(tmp_filename,"dll");
            	#else
            	strcat(tmp_filename,"so");
            	#endif
            }
            test_file=fopen(tmp_filename,"r");
            if(test_file!=NULL){if(use==1){fclose(test_file);InitExLib(tmp_filename);}got=1;}
            else{
                *tmp_filename='\0';
                strcat(tmp_filename,config_dir);
                printf("config_dir=%s\n",config_dir);
                strcat(tmp_filename,tmp2);
                tmp3=tmp_filename;
				tmp3=tmp3+strlen(tmp_filename)-1;
				if(*tmp3=='*'){
					*tmp3=0;
					#ifdef WIN32
					strcat(tmp_filename,"dll");
					#else
					strcat(tmp_filename,"so");
					#endif
				}
                //system("cd");
                if(use==1){InitExLib(tmp_filename);}
                got=1;
            }
        }




        if(got==0){printf("Error parsing config file!\nError keyword is ");printf(tmp2);printf("\n");printf("strlen=%i",strlen(tmp2));panic("");}

        tmp2=tmp2+strlen(tmp2)+1;
        use=1;
        got=0;
    }
    printf("\n\nNow ending debug output for config_files.c\n\n");
    free(tmp_ptr);
    free(tmp2_ptr);






}
