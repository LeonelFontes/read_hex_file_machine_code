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
	//tratar informação para novo array com informação-> adress, record type, op code
//	unsigned char simple_loop[]={
//
//	0x7f,0x00,
//
//	     0xbf,0x0a,0x00,
//
//	0x50,0x03,
//
//	0x0f,
//
//	0x80,0xf8,
//
//	0x22};
	//when find record type 00 -> put for the array, when find \n go forward 2 index for write correct next opcode
	//avanço sempre para a 9 posição vejo se a anterior é 1 ou 0, caso seja 1 o record type terminou, se for 0 contiuo a ler
	//falta ver a condição de paragem
	char simple_loop_txt[50]={0};
	int j=0;
	i=9;
	while(machine_code_content[i]!='\0'){
		if(machine_code_content[i]=='\n'){
			j-=2;
			i+=9;//to record type
			if(machine_code_content[i]=='1'){//record type -> End Of File
				//end while
				//clean check sum
				simple_loop_txt[j]=0;
				simple_loop_txt[++j]=0;
				break;
			}
			++i; //go to next op code
		}
		simple_loop_txt[j]=machine_code_content[i];
		++j;
		++i;

	}
	puts(simple_loop_txt);
	return 0;
}


