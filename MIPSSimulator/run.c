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
	uint32_t PC_addr; // for Jump

	CURRENT_STATE.PC += 4;


	if (OPCODE(instr) == 0 && TARGET(instr) == 0) {
		RUN_BIT = FALSE;
		CURRENT_STATE.PC -= 4;
		return;
	}

	if (OPCODE(instr) == 0x0) {
		// TYPE R
		switch (FUNC(instr)) {
		case 0x21:  // ADD U
			//printf("ADDU :$%d = $%d + $%d \n", RD(instr), RS(instr), RT(instr));
			CURRENT_STATE.REGS[RD(instr)] = CURRENT_STATE.REGS[RS(instr)] + CURRENT_STATE.REGS[RT(instr)];
			break;
		case 0x24:  // AND
			//printf("AND :$%d = $%d & $%d \n", RD(instr), RS(instr), RT(instr));
			CURRENT_STATE.REGS[RD(instr)] = CURRENT_STATE.REGS[RS(instr)] & CURRENT_STATE.REGS[RT(instr)];
			break;
		case 0x27:  // NOR
			//printf("NOR :$%d = ~($%d | $%d) \n", RD(instr), RS(instr), RT(instr));
			CURRENT_STATE.REGS[RD(instr)] = ~(CURRENT_STATE.REGS[RS(instr)] | CURRENT_STATE.REGS[RT(instr)]);
			break;
		case 0x25:  // OR
			//printf("OR :$%d = $%d | $%d \n", RD(instr), RS(instr), RT(instr));
			CURRENT_STATE.REGS[RD(instr)] = CURRENT_STATE.REGS[RS(instr)] | CURRENT_STATE.REGS[RT(instr)];
			break;
		case 0x2b:  // SLT U
			//printf("SLTU :$%d = ($%d < $%d) ? 1:0 \n", RD(instr), RS(instr), RT(instr));
			CURRENT_STATE.REGS[RD(instr)] = (CURRENT_STATE.REGS[RS(instr)] < CURRENT_STATE.REGS[RT(instr)]) ? 1 : 0;
			break;
		case 0x00:  // SLL
			//printf("SLL :$%d = $%d << ($%d)shamt \n", RD(instr), RT(instr), SHAMT(instr));
			CURRENT_STATE.REGS[RD(instr)] = CURRENT_STATE.REGS[RT(instr)] << SHAMT(instr);
			break;
		case 0x02:  // SRL
			//printf("SRL :$%d = $%d >> ($%d)shamt \n", RD(instr), RT(instr), SHAMT(instr));
			CURRENT_STATE.REGS[RD(instr)] = CURRENT_STATE.REGS[RT(instr)] >> SHAMT(instr);
			break;
		case 0x23:  // SUB U
			//printf("SUBU :$%d = $%d - $%d \n", RD(instr), RS(instr), RT(instr));
			CURRENT_STATE.REGS[RD(instr)] = CURRENT_STATE.REGS[RS(instr)] - CURRENT_STATE.REGS[RT(instr)];
			break;
		case 0x08:  //JR
			//printf("JR : PC = 0x%x \n", CURRENT_STATE.REGS[RS(instr)]);
			CURRENT_STATE.PC = CURRENT_STATE.REGS[RS(instr)];
			break;
		default:
			//printf("ERROR: Check process_instruction() TYPE R func_code\n");
			//RUN_BIT = FALSE;
			//CURRENT_STATE.PC -= 4;
			break;
		}
	}
	else {
		switch (OPCODE(instr)) {
			// TYPE I
		case 0x9:       //(0x001001)ADDIU
			//printf("ADDIU :$%d = $%d + %d \n", RT(instr), RS(instr), IMM(instr));
			CURRENT_STATE.REGS[RT(instr)] = CURRENT_STATE.REGS[RS(instr)] + IMM(instr);
			break;
		case 0xc:       //(0x001100)ANDI
			//printf("ANDI :$%d = $%d & $%d \n", RD(instr), RS(instr), IMM(instr));
			CURRENT_STATE.REGS[RT(instr)] = CURRENT_STATE.REGS[RS(instr)] & IMM(instr);
			break;
		case 0xf:       //(0x001111)LUI, Load Upper Imm.
			//printf("LUI :%d = %d \n", RT(instr), IMM(instr));
			CURRENT_STATE.REGS[RT(instr)] = IMM(instr) * 65536; // 16^4
			break;
		case 0xd:       //(0x001101)ORI
			//printf("ORI :%d = %d or %d \n", RT(instr), RS(instr), IMM(instr));
			CURRENT_STATE.REGS[RT(instr)] = CURRENT_STATE.REGS[RS(instr)] | IMM(instr);
			break;
		case 0xb:       //(0x001011)SLTIU
			//printf("SLTIU :$%d = ($%d < $%d) ? 1:0 \n", RT(instr), RS(instr), IMM(instr));
			CURRENT_STATE.REGS[RT(instr)] = (CURRENT_STATE.REGS[RS(instr)] < IMM(instr)) ? 1 : 0;
			break;
		case 0x23:      //(0x100011)LW
			//printf("LW :$%d = M[0x%8x + %d] \n", RT(instr), CURRENT_STATE.REGS[RS(instr)], IMM(instr));
			CURRENT_STATE.REGS[RT(instr)] = mem_read_32(CURRENT_STATE.REGS[RS(instr)] + IMM(instr));
			break;
		case 0x2b:      //(0x101011)SW
			//printf("SW :M[0x%8x + %d] = $%d \n", CURRENT_STATE.REGS[RS(instr)], IMM(instr), RT(instr));
			mem_write_32(CURRENT_STATE.REGS[RS(instr)] + IMM(instr), CURRENT_STATE.REGS[RT(instr)]);
			break;
		case 0x4:       //(0x000100)BEQ
			//printf("BEQ :if($%d == $%d) goto PC + %d(*4) \n", RS(instr), RT(instr), IMM(instr));
			if (CURRENT_STATE.REGS[RS(instr)] == CURRENT_STATE.REGS[RT(instr)])
				CURRENT_STATE.PC += 4 * IMM(instr);
			break;
		case 0x5:       //(0x000101)BNE
			//printf("BNE :if($%d != $%d) goto PC + %d(*4) \n", RS(instr), RT(instr), IMM(instr));
			if (CURRENT_STATE.REGS[RS(instr)] != CURRENT_STATE.REGS[RT(instr)])
				CURRENT_STATE.PC += 4 * IMM(instr);
			break;
			// TYPE J
		case 0x2:       //J
			//printf("J :PC = PC[31:28] strcat [0x%x(imm) * 4] \n", TARGET(instr));
			PC_addr = CURRENT_STATE.PC;
			PC_addr = PC_addr & 0xf0000000; //PC[31:28]
			CURRENT_STATE.PC = PC_addr + TARGET(instr) * 4;
			break;
		case 0x3:       //JAL
			//printf("JAL :R[31]=PC+4, J to 0x%x(imm)*4 \n", TARGET(instr));
			CURRENT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
			PC_addr = CURRENT_STATE.PC;
			PC_addr = PC_addr & 0xf0000000; //PC[31:28]
			CURRENT_STATE.PC = PC_addr + TARGET(instr) * 4;
			break;
		default:
			//printf("ERROR: Check process_instruction() TYPE I,, J opcode\n");
			//RUN_BIT = FALSE;
			//CURRENT_STATE.PC-= 4;
			break;
		}
	}
}
