/***************************************************************/
/*                                                             */
/*   MIPS-32 Instruction Level Simulator                       */
/*                                                             */
/*   SCE212 Ajou University                                    */
/*   parse.c                                                   */
/*   Adapted from Computer Architecture@KAIST                  */
/*                                                             */
/***************************************************************/

#include <stdio.h>

#include "util.h"
#include "parse.h"

int text_size;
int data_size;

instruction parsing_instr(const char *buffer, const int index)
{
	/** Implement this function */
    instruction instr;
	char temp[33];
	int i; // for loop

	for (i = 0; i < 6; i++) 
		temp[i] = buffer[i];
	temp[i] = '\0';
	instr.opcode = fromBinary(temp);

	// TYPE J
	if (instr.opcode == 0x2 || instr.opcode == 0x3) {
		for (i = 6; i < 32; i++)
			temp[i - 6] = buffer[i];
		temp[i-6] = '\0';

		instr.r_t.target = fromBinary(temp);

		return instr;
	}

	for (i = 6; i < 11; i++)
		temp[i - 6] = buffer[i];
	temp[i-6] = '\0';
	instr.r_t.r_i.rs = fromBinary(temp);

	for (i = 11; i < 16; i++)
		temp[i - 11] = buffer[i];
	temp[i-11] = '\0';
	instr.r_t.r_i.rt = fromBinary(temp);

	// TYPE R
	if (instr.opcode == 0x0) {
		for (i = 16; i < 21; i++)
			temp[i - 16] = buffer[i];
		temp[i-16] = '\0';
		instr.r_t.r_i.r_i.r.rd = fromBinary(temp);

		for (i = 21; i < 26; i++)
			temp[i - 21] = buffer[i];
		temp[i-21] = '\0';
		instr.r_t.r_i.r_i.r.shamt = fromBinary(temp);

		for (i = 26; i < 32; i++)
			temp[i - 26] = buffer[i];
		temp[i-26] = '\0';
		instr.func_code = fromBinary(temp);

		return instr;
	}
	// TYPE I
	else {
		for (i = 16; i < 32; i++)
			temp[i - 16] = buffer[i];
		temp[i-16] = '\0';
		instr.r_t.r_i.r_i.imm = fromBinary(temp);

		return instr;
	}
}

void parsing_data(const char *buffer, const int index)
{
	/** Implement this function */
}

void print_parse_result()
{
    int i;
    printf("Instruction Information\n");

    for(i = 0; i < text_size/4; i++)
    {
        printf("INST_INFO[%d].value : %x\n",i, INST_INFO[i].value);
        printf("INST_INFO[%d].opcode : %d\n",i, INST_INFO[i].opcode);

	    switch(INST_INFO[i].opcode)
        {
            //Type I
            case 0x9:		//(0x001001)ADDIU
            case 0xc:		//(0x001100)ANDI
            case 0xf:		//(0x001111)LUI	
            case 0xd:		//(0x001101)ORI
            case 0xb:		//(0x001011)SLTIU
            case 0x23:		//(0x100011)LW
            case 0x2b:		//(0x101011)SW
            case 0x4:		//(0x000100)BEQ
            case 0x5:		//(0x000101)BNE
                printf("INST_INFO[%d].rs : %d\n",i, INST_INFO[i].r_t.r_i.rs);
                printf("INST_INFO[%d].rt : %d\n",i, INST_INFO[i].r_t.r_i.rt);
                printf("INST_INFO[%d].imm : %d\n",i, INST_INFO[i].r_t.r_i.r_i.imm);
                break;

            //TYPE R
            case 0x0:		//(0x000000)ADDU, AND, NOR, OR, SLTU, SLL, SRL, SUBU  if JR
                printf("INST_INFO[%d].func_code : %d\n",i, INST_INFO[i].func_code);
                printf("INST_INFO[%d].rs : %d\n",i, INST_INFO[i].r_t.r_i.rs);
                printf("INST_INFO[%d].rt : %d\n",i, INST_INFO[i].r_t.r_i.rt);
                printf("INST_INFO[%d].rd : %d\n",i, INST_INFO[i].r_t.r_i.r_i.r.rd);
                printf("INST_INFO[%d].shamt : %d\n",i, INST_INFO[i].r_t.r_i.r_i.r.shamt);
                break;

            //TYPE J
            case 0x2:		//(0x000010)J
            case 0x3:		//(0x000011)JAL
                printf("INST_INFO[%d].target : %d\n",i, INST_INFO[i].r_t.target);
                break;

            default:
                printf("Not available instruction\n");
                assert(0);
        }
    }

    printf("Memory Dump - Text Segment\n");
    for(i = 0; i < text_size; i+=4)
        printf("text_seg[%d] : %x\n", i, mem_read_32(MEM_TEXT_START + i));
    for(i = 0; i < data_size; i+=4)
        printf("data_seg[%d] : %x\n", i, mem_read_32(MEM_DATA_START + i));
    printf("Current PC: %x\n", CURRENT_STATE.PC);
}
