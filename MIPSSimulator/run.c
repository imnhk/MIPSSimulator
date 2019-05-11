/***************************************************************/
/*                                                             */
/*   MIPS-32 Instruction Level Simulator                       */
/*                                                             */
/*   SCE212 Ajou University                                    */
/*   run.c                                                     */
/*   Adapted from Computer Architecture@KAIST                  */
/*                                                             */
/***************************************************************/

#include <stdio.h>

#include "util.h"
#include "run.h"

/***************************************************************/
/*                                                             */
/* Procedure: get_inst_info                                    */
/*                                                             */
/* Purpose: Read insturction information                       */
/*                                                             */
/***************************************************************/
instruction* get_inst_info(uint32_t pc)
{
    return &INST_INFO[(pc - MEM_TEXT_START) >> 2];
}

/***************************************************************/
/*                                                             */
/* Procedure: process_instruction                              */
/*                                                             */
/* Purpose: Process one instrction                             */
/*                                                             */
/***************************************************************/
void process_instruction()
{
	/** Implement this function */
	instruction* instr = get_inst_info(CURRENT_STATE.PC);
	
	if (OPCODE(instr) == 0x0) {

		// TYPE dd
		switch (FUNC(instr)) {
		case 0x21:	// ADD U

			break;
		case 0x24:	// AND
			printf("AND :%d = %d & %d \n", RD(instr), RS(instr), RT(instr));
			RD(instr) = RS(instr) & RT(instr);
			break;
		case 0x27:	// NOR

			break;
		case 0x25:	// OR

			break;
		case 0x2b:	// SLT U

			break;
		case 0x00:	// SLL

			break;
		case 0x02:	// SRL

			break;
		case 0x23:	// SUB U

			break;
		default:
			printf("ERROR: Check process_instruction() TYPE R func_code\m");
		}
		CURRENT_STATE.PC += 4;
	}
	else {
		switch (OPCODE(instr)) {

			// TYPE I
			case 0x9:		//(0x001001)ADDIU

				break;
			case 0xc:		//(0x001100)ANDI

				break;
			case 0xf:		//(0x001111)LUI	

				break;
			case 0xd:		//(0x001101)ORI

				break;
			case 0xb:		//(0x001011)SLTIU

				break;
			case 0x23:		//(0x100011)LW

				break;
			case 0x2b:		//(0x101011)SW

				break;
			case 0x4:		//(0x000100)BEQ

				break;
			case 0x5:		//(0x000101)BNE

				break;
			
			// TYPE J
			case 0x2:		//(0x000010)J

				break;
			case 0x3:		//(0x000011)JAL

				break;

			default:
				printf("ERROR: Check process_instruction() TYPE I,, J opcode\n");
		}
	}
}
