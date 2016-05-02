#include <stdio.h>
#include <stdint.h>
#include "cpu.h"
#include "r_instructions.h"

// set the last five bits of the MASK as 1s to extract rs, rt and rd fields
// RS_FIELD(instruction) extracts the value of the RS field bits (bits 21-25)
// RT_FIELD(instruction) extracts the value of the RT field bits (bits 16-20)
// RD_FIELD(instruction) extracts the value of the RD field bits (bits 11-15)
#define MASK 0x0000001f 
#define RS_SHIFT 21
#define RT_SHIFT 16
#define RD_SHIFT 11
// shift:right shift to the corresponding bits; cast:only low value bits will be count and high order bits will be discarded
#define RS_FIELD(instruction) (uint8_t)(MASK & (instruction >> RS_SHIFT)) 
#define RT_FIELD(instruction)  (uint8_t)(MASK & (instruction >> RT_SHIFT)) 
#define RD_FIELD(instruction)  (uint8_t)(MASK & (instruction >> RD_SHIFT))

// define shift and mask for multiplication
#define MUL_SHIFT 32
#define MUL_MASK 0x00000000ffffffff

// define SHAMT
#define SHAMT_SHIFT 6
#define SHAMT_FIELD(instruction) (uint8_t)(MASK & (instruction >> SHAMT_SHIFT))

void add(uint32_t instruction) 
{
  uint8_t rs = RS_FIELD(instruction);
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rd = RD_FIELD(instruction);
  registers[rd] = (uint32_t) ((int32_t)(registers[rs]) + (int32_t)(registers[rt]));
  
}


void addu(uint32_t instruction)
{
  uint8_t rs = RS_FIELD(instruction);
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rd = RD_FIELD(instruction);
  registers[rd] = registers[rs] + registers[rt];

}

void sub(uint32_t instruction)
{
  uint8_t rs = RS_FIELD(instruction);
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rd = RD_FIELD(instruction);
  registers[rd] = (uint32_t) ((int32_t) registers[rs] - (int32_t) registers[rt]);

}

void subu(uint32_t instruction)
{
  uint8_t rs = RS_FIELD(instruction);
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rd = RD_FIELD(instruction);
  registers[rd] = registers[rs] - registers[rt];

}

void mult(uint32_t instruction)
{
  uint8_t rs = RS_FIELD(instruction);
  uint8_t rt = RT_FIELD(instruction);
  int64_t result = (int64_t)((int32_t)registers[rs]) * (int64_t)((int32_t)registers[rt]);
  HI = (uint32_t) (result >> MUL_SHIFT);
  LO = (uint32_t) (result & MUL_MASK);

}

void multu(uint32_t instruction)
{
  uint8_t rs = RS_FIELD(instruction);
  uint8_t rt = RT_FIELD(instruction);
  uint64_t result = (uint32_t)registers[rs] * (uint64_t)registers[rt];
  HI = result >> MUL_SHIFT;
  LO = result & MUL_MASK;

}

void div(uint32_t instruction)
{
  uint8_t rs = RS_FIELD(instruction);
  uint8_t rt = RT_FIELD(instruction);
  LO = (uint32_t)((int32_t)registers[rs] / (int32_t)registers[rt]);
  HI = (uint32_t)((int32_t)registers[rs] % (int32_t)registers[rt]);
  
}

void divu(uint32_t instruction)
{
  uint8_t rs = RS_FIELD(instruction);
  uint8_t rt = RT_FIELD(instruction);
  LO = registers[rs] / registers[rt];
  HI = registers[rs] % registers[rt];  

}

void and(uint32_t instruction)
{
  uint8_t rs = RS_FIELD(instruction);
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rd = RD_FIELD(instruction);
  registers[rd] = registers[rs] & registers[rt];

}

void nor(uint32_t instruction)
{
  uint8_t rs = RS_FIELD(instruction);
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rd = RD_FIELD(instruction);
  registers[rd] = ~(registers[rs] | registers[rt]);  

}

void or(uint32_t instruction)
{
  uint8_t rs = RS_FIELD(instruction);
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rd = RD_FIELD(instruction);
  registers[rd] = registers[rs] | registers[rt];
  
}

void xor(uint32_t instruction)
{
  uint8_t rs = RS_FIELD(instruction);
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rd = RD_FIELD(instruction);
  registers[rd] = registers[rs] ^ registers[rt];

}

void sll(uint32_t instruction)
{
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rd = RD_FIELD(instruction);
  uint8_t shamt = SHAMT_FIELD(instruction);
  registers[rd] = registers[rt] << shamt;
  
}

void sllv(uint32_t instruction)
{
  uint8_t rs = RS_FIELD(instruction);
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rd = RD_FIELD(instruction);
  registers[rd] = registers[rs] << registers[rt];

}

void srl(uint32_t instruction)
{
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rd = RD_FIELD(instruction);
  uint8_t shamt = SHAMT_FIELD(instruction);
  registers[rd] = registers[rt] >> shamt;

}

void sra(uint32_t instruction)
{
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rd = RD_FIELD(instruction);
  uint8_t shamt = SHAMT_FIELD(instruction);
  registers[rd] = (uint32_t)((int32_t)registers[rt] >> shamt);

}

void srlv(uint32_t instruction)
{
  uint8_t rs = RS_FIELD(instruction);
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rd = RD_FIELD(instruction);
  registers[rd] = registers[rs] >> registers[rt];

}

void srav(uint32_t instruction)
{
  uint8_t rs = RS_FIELD(instruction);
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rd = RD_FIELD(instruction);
  uint8_t shamt = SHAMT_FIELD(instruction);
  registers[rd] = (uint32_t)((int32_t)registers[rs] >> (int32_t)registers[rt]);

}

void slt(uint32_t instruction)
{
  uint8_t rs = RS_FIELD(instruction);
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rd = RD_FIELD(instruction);
  
  if ((int32_t)registers[rs] < (int32_t)registers[rt])
    registers[rd] = (uint32_t)((int32_t) 1);
  else
    registers[rd] = (uint32_t)((int32_t) 0);

}

void sltu(uint32_t instruction)
{
  uint8_t rs = RS_FIELD(instruction);
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rd = RD_FIELD(instruction);
  
  if (registers[rs] < registers[rt])
    registers[rd] = (uint32_t) 1;
  else
    registers[rd] = (uint32_t) 0;

}

void jr(uint32_t instruction)
{
  uint8_t rs = RS_FIELD(instruction);
  npc = registers[rs];

}

void jalr(uint32_t instruction)
{
  uint8_t rs = RS_FIELD(instruction);
  registers[31] = pc + 4;
  npc = registers[rs];

}

void mfhi(uint32_t instruction)
{
  uint8_t rd = RD_FIELD(instruction);
  registers[rd] = HI;

}

void mflo(uint32_t instruction)
{
  uint8_t rd = RD_FIELD(instruction);
  registers[rd] = LO;

}