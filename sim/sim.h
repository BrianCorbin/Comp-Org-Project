/**
	@file
	@author Andrew D. Zonenberg
	@brief Main program include file
 */

#ifndef sim_h
#define sim_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// System headers

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <linux/elf.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Opcode table

enum opcodes
{
	OP_RTYPE	= 0x00,
    OP_BGEZ     = 0x01, //done rt = 00001
    OP_BGEZAL   = 0x01, //done rt = 10001
    OP_BLTZ     = 0x01, //done rt = 00000
    OP_BLTZAL   = 0x01, //done rt = 10000
    OP_J        = 0x02, //done
    OP_JAL      = 0x03, //done
    OP_BEQ      = 0x04, //done
    OP_BNE      = 0x05, //done
    OP_BLEZ     = 0x06, //done
    OP_BGTZ     = 0x07, //done
    OP_ADDI     = 0x08, //done
    OP_ADDIU    = 0x09, //done
    OP_SLTI     = 0x0a, //done
    OP_SLTIU    = 0x0b, //done
    OP_ANDI     = 0x0c, //done
    OP_ORI      = 0x0d, //done
    OP_XORI     = 0x0e, //done
    OP_LUI      = 0x0f, //done
    OP_LB       = 0x20, //done
    OP_LW       = 0x23, //done
    OP_SB       = 0x28, //done
	OP_SW		= 0x2b  //done
};

enum functions
{
    FUNC_SLL        = 0x00, //done
    FUNC_SRL        = 0x02, //done
    FUNC_SRA        = 0x03, //done
    FUNC_SLLV       = 0x04, //done
    FUNC_SRLV       = 0x05, //done
	FUNC_JR			= 0x08, //done
	FUNC_SYSCALL	= 0x0c,
    FUNC_MFHI       = 0x10, //done
    FUNC_MFLO       = 0x11, //done
    FUNC_MULT       = 0x18, //done
    FUNC_MULTU      = 0x19, //done
    FUNC_DIV        = 0x1a, //done
    FUNC_DIVU       = 0x1b, //done
    FUNC_ADD        = 0x20, //done
	FUNC_ADDU		= 0x21, //done
    FUNC_SUB        = 0x22, //done
    FUNC_SUBU       = 0x23, //done
    FUNC_AND        = 0x24, //done
	FUNC_OR			= 0x25, //done
    FUNC_XOR        = 0x26, //done
    FUNC_SLT        = 0x2a, //done
    FUNC_SLTU       = 0x2b  //done
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Magic register IDS

enum regids
{
	REGID_ZERO = 0,
	REGID_A0 = 4,
	REGID_SP = 29,
	REGID_RA = 31
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A MIPS instruction

//This union allows easily pulling bitfields out from instructions
union mips_instruction
{
	//R-type format
	struct
	{
		unsigned int func:6;
		unsigned int shamt:5;
		unsigned int rd:5;
		unsigned int rt:5;
		unsigned int rs:5;
		unsigned int opcode:6;
	} rtype;
	
	//I-type format
	struct
	{
		unsigned int imm:16;
		unsigned int rt:5;
		unsigned int rs:5;
		unsigned int opcode:6;
	} itype;
	
	//J-type format
	struct
	{
		unsigned int addr:26;
		unsigned int opcode:6;
	} jtype;
	
	//Write to this to load an instruction as a 32-bit word
	uint32_t word;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Virtual memory

/**
	@brief One contiguous region of virtual memory (corresponds to an ELF program header).
 */
struct virtual_mem_region
{
	uint32_t vaddr;
	uint32_t len;
	uint32_t* data;
	struct virtual_mem_region* next;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPU context

/**
	@brief All CPU registers
 */
struct context
{
	uint32_t pc;
	uint32_t regs[32];
    uint32_t HI;
    uint32_t LO;
};

enum mips_regids
{
	zero,
	at,
	v0, v1,
	a0, a1, a2, a3,
	t0, t1, t2, t3, t4, t5, t6, t7, t8, t9,
	s0, s1, s2, s3, s4, s5, s6, s7, s8,
	fp,
	k0, k1,
	gp,
	sp,
	ra
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Startup

void ReadELF(const char* fname, struct virtual_mem_region** memory, struct context* ctx);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Simulator core

void RunSimulator(struct virtual_mem_region* memory, struct context* ctx);

uint32_t FetchWordFromVirtualMemory(uint32_t address, struct virtual_mem_region* memory);
void StoreWordToVirtualMemory(uint32_t address, uint32_t value, struct virtual_mem_region* memory);

int SimulateInstruction(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
int SimulateRtypeInstruction(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
int SimulateBswitch(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
int SimulateSyscall(uint32_t callnum, struct virtual_mem_region* memory, struct context* ctx);
int SimulateSyscall(uint32_t callnum, struct virtual_mem_region* memory, struct context* ctx);

// Simulate specific instructions
void simBGEZ(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simBGEZAL(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simBLTZ(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simBLTZAL(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simJ(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simJAL(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simBEQ(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simBNE(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simBLEZ(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simBGTZ(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simADDI(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simADDIU(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simSLTI(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simSLTIU(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simANDI(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simORI(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simXORI(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simLUI(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simLB(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simLW(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simSB(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simSW(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simSLL(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simSRL(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simSRA(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simSLLV(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simSRLV(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simJR(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simMFHI(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simMFLO(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simMULT(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simMULTU(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simDIV(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simDIVU(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simADD(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simADDU(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simSUB(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simSUBU(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simAND(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simOR(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simXOR(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simSLT(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
void simSLTU(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
#endif
