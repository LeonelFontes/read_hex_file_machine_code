/*
 * main.cpp
 *
 *  Created on: 11/05/2018
 *      Author: aet
 */

#include <stdio.h>
#include <stdlib.h>



int main(){

	printf("Start program!\n");
	FILE *fp;
	char *machine_code_content, ch;
	int array_msb_hex[16]={0,16,32,48,64,80,96,112,128,144,160,176,192,208,224,240};

	fp=fopen("machine_code.txt", "r");

	if(fp==NULL){
		perror("Open file error: ");
	}
	fseek(fp,0,SEEK_END);//position at end for know file size with ftell(fp)
	machine_code_content=(char*)malloc(ftell(fp)+1);//allocate memory for temporary array with txt machine code -> + 1 for the '\0'
	rewind(fp);
	int i=0;
	while((ch=fgetc(fp))!=EOF){
		machine_code_content[i]=ch;
		++i;
	}
	machine_code_content[i]='\0';
	puts(machine_code_content);
	// test check sum -> latter for put in on new function -> doesn't work yet
	int check_sum=0;
	i=1; // i starts 1 because first char is ':'
	while(machine_code_content[i]!='\0'){
		//when machine code has just one line at the end go to else or is the end line '\n', doesn't add checksum
		if(machine_code_content[i]!='\n' && machine_code_content[i+2]!='\0' && machine_code_content[i+2]!='\n'){
			if(i%2!=0){//odd position MSB
				if(machine_code_content[i]>='A' && machine_code_content[i]<='F'){
					//go to correspond position array for value MSB hex A...F
					check_sum+=array_msb_hex[machine_code_content[i]-55];
				}
				else{
					//go to correspond position array for value MSB hex 1...9
					check_sum+=array_msb_hex[machine_code_content[i]-48];
				}
			}
			if(i%2==0){//even position LSB
				if(machine_code_content[i]>='1' && machine_code_content[i]<='9'){
					check_sum+=machine_code_content[i]-48; // convert char to int decimal
				}
				if(machine_code_content[i]>='A' && machine_code_content[i]<='F'){
					check_sum+=machine_code_content[i]-55; // convert hex char to respective int decimal
				}
			}
			++i;
		}
		else{
//			//verify checkSum
//			//in this moment the check_sum has also add the checksum
//			--i; // position at MSB checksum and subtract -> before the position was LSB checksum
//			if(machine_code_content[i]>='A' && machine_code_content[i]<='F'){
//				//go to correspond position array for value MSB hex A...F
//				check_sum-=array_msb_hex[machine_code_content[i]-55];
//			}else{
//				//go to correspond position array for value MSB hex 1...9
//				check_sum-=array_msb_hex[machine_code_content[i]-48];
//			}
//			++i;//position at LSB checksum and subtract
//			if(machine_code_content[i]>='1' && machine_code_content[i]<='9'){
//				check_sum-=machine_code_content[i]-48; // convert char to int decimal
//			}else if(machine_code_content[i]>='A' && machine_code_content[i]<='F'){
//				check_sum-=machine_code_content[i]-55; // convert hex char to respective int decimal
//			}
			i+=3; // in this else we are positioned at '\n' and the next position is ':' -> so increment 3 times
		}

	}

	printf("checksum=%X\n", check_sum);
	//when find record type 00 -> copy for the array, when find \n go to 2 line for write correct next opcode
	//check record if record type LSB = '1' when change line
	char simple_loop_txt[50]={0};
	int j=0;
	i=9;
	int flag_mcode_one_line=1;
	while(machine_code_content[i]!='\0'){
		if(machine_code_content[i]=='\n'){
			flag_mcode_one_line=0;
			j-=2;//go back for copy again in position of record type
			i+=9;//to record type
			if(machine_code_content[i]=='1'){//record type LSB -> End Of File
				//end while
				//clean check sum for the array
				simple_loop_txt[j]=0;
				simple_loop_txt[++j]=0;
				break;
			}
			++i; //go to next op code
		}
		simple_loop_txt[j]=machine_code_content[i];
		++j;//increment simple_loop_txt
		++i;//increment position code machine

	}
	if(flag_mcode_one_line==1){//for delete checksum, when machine code has just one lie
		j-=2;//position in checksum
		//clean check sum for the array
		simple_loop_txt[j]=0;
		simple_loop_txt[++j]=0;
	}
	puts(simple_loop_txt);
	return 0;
}


