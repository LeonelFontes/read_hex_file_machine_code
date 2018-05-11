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
	int hex= 0x2 +32;
	printf("hex=%d\n", hex);
	char chex= 'A'-0x31;
	printf("hex=%x\n", chex);
	FILE *fp;
	char *machine_code_content;
	char ch;

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
	i=1; // i starts 1 becuase first char is ':'
	while(machine_code_content[i]!='\0'){
		if(machine_code_content[i]!='\n'){
			if(i%2!=0){//odd position MSB
				if(machine_code_content[i]>='A' && machine_code_content[i]<='F'){
					if(machine_code_content[i]=='A'){
						check_sum+=160;
					}
					if(machine_code_content[i]=='B'){
						check_sum+=176;
					}
					if(machine_code_content[i]=='C'){
						check_sum+=192;
					}
					if(machine_code_content[i]=='D'){
						check_sum+=208;
					}
					if(machine_code_content[i]=='E'){
						check_sum+=224;
					}
					if(machine_code_content[i]=='F'){
						check_sum+=240;
					}
				}
				else{
					if(machine_code_content[i]=='1'){
						check_sum+=16;
					}
					if(machine_code_content[i]=='2'){
						check_sum+=32;
					}
					if(machine_code_content[i]=='3'){
						check_sum+=48;
					}
					if(machine_code_content[i]=='4'){
						check_sum+=64;
					}
					if(machine_code_content[i]=='5'){
						check_sum+=80;
					}
					if(machine_code_content[i]=='6'){
						check_sum+=96;
					}
					if(machine_code_content[i]=='7'){
						check_sum+=112;
					}
					if(machine_code_content[i]=='8'){
						check_sum+=128;
					}
					if(machine_code_content[i]=='9'){
						check_sum+=144;
					}
				}
			}
			if(i%2==0){//even position LSB
				if(machine_code_content[i]>='1' && machine_code_content[i]<='9'){
					check_sum+=machine_code_content[i]-48; // convert char to int decimal
				}
				if(machine_code_content[i]>='A' && machine_code_content[i]<='F'){
					check_sum+=machine_code_content[i]-0x31; // convert hex char to respective int decimal
				}
			}
			++i;
		}
		else{
			i+=3; // in this else we are positioned at '\n' and the next position is ':' -> so increment 3 times
		}
	}

	printf("checksum=%d\n", check_sum);
	//when find record type 00 -> copy for the array, when find \n go to 2 line for write correct next opcode
	//check record if record type LSB = '1' when change line
	char simple_loop_txt[50]={0};
	int j=0;
	i=9;
	while(machine_code_content[i]!='\0'){
		if(machine_code_content[i]=='\n'){
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
	puts(simple_loop_txt);
	return 0;
}


