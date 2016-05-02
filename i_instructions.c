#include <stdint.h>
#include <stdio.h>
#include "cpu.h"
#include "i_instructions.h"

#define MASK 0x0000001f
#define I_MASK 0x0000ffff
#define IH_MASK 0x000000ff
#define RS_SHIFT 21
#define RT_SHIFT 16
#define SIGN_MASK 0xffff0000


// RS_FIELD() extracts the value of the RS field bits (bits 21-25)
// RT_FIELD() extracts the value of the RT field bits (bits 16-20)
// RD_FIELD() extracts the value of the RD field bits (bits 11-15)
#define RS_FIELD(instruction) (uint8_t)(MASK & (instruction >> RS_SHIFT))
#define RT_FIELD(instruction) (uint8_t)(MASK & (instruction >> RT_SHIFT))
#define I_FIELD(instruction) (uint16_t)(I_MASK & instruction)


uint32_t signExt(uint16_t instruction)
{
  uint16_t imm = I_FIELD(instruction);
  if((1<<15 & imm)>>15)
    return (uint32_t) ((SIGN_MASK | imm));
  else
    return (uint32_t) ((0<<31) | imm);
}




void addi(uint32_t instruction)
{
  uint8_t rs = RS_FIELD(instruction);
  uint8_t rt = RT_FIELD(instruction);
  uint16_t imm = I_FIELD(instruction);
  registers[rt] = (uint32_t) ((int32_t)registers[rs] + (int32_t)signExt(imm));

}

void addiu(uint32_t instruction)
{
  uint8_t rs = RS_FIELD(instruction);
  uint8_t rt = RT_FIELD(instruction);
  uint16_t imm = I_FIELD(instruction);
  registers[rt] = (uint32_t) ((uint32_t) registers[rs] +  signExt(imm));

}


void andi(uint32_t instruction)
{
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rs = RS_FIELD(instruction);
  uint16_t imm = I_FIELD(instruction);
  registers[rt] =(uint32_t) (registers[rs] &  imm);

}


void ori(uint32_t instruction)
{
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rs = RS_FIELD(instruction);
  uint16_t imm = I_FIELD(instruction);
  registers[rt] =(uint32_t) (registers[rs] |  imm);

}


void xori(uint32_t instruction)
{
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rs = RS_FIELD(instruction);
  uint16_t imm = I_FIELD(instruction);
  registers[rt] =(uint32_t) (registers[rs] ^  imm);

}


void slti(uint32_t instruction)
{
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rs = RS_FIELD(instruction);
  uint16_t imm = I_FIELD(instruction);
  if ((int32_t)registers[rs] < (int32_t)signExt(imm))
    registers[rt] = (uint32_t)1;
  else
    registers[rt] = (uint32_t)0;

}


void sltiu(uint32_t instruction)
{
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rs = RS_FIELD(instruction);
  uint16_t imm = I_FIELD(instruction);
  if(registers[rs] < signExt(imm))
    registers[rt] = (uint32_t) 1;
  else
    registers[rt] = (uint32_t) 0;

}


void beq(uint32_t instruction)
{
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rs = RS_FIELD(instruction);
  uint16_t imm = I_FIELD(instruction);
  if(registers[rs] == registers[rt])
    npc = pc + 4 + (uint32_t) ((signExt(imm)) << 2);

}


void bne(uint32_t instruction)
{
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rs = RS_FIELD(instruction);
  uint16_t imm = I_FIELD(instruction);
  if(registers[rs] != registers[rt])
    npc = pc + 4 + (uint32_t) ((signExt(imm)) << 2);

}


void lb(uint32_t instruction)
{
  uint8_t rs = RS_FIELD(instruction);
  uint8_t rt = RT_FIELD(instruction);
  uint16_t imm = I_FIELD(instruction);
  uint32_t maddress = registers[rs] + signExt(imm);
  uint8_t m = memory[maddress];
  
  if (((1 << 7) & m) >> 7)
    registers[rt] = (uint32_t)(0xffffff00 | m);
  else
    registers[rt] = (uint32_t)(0x00000000 | m);

}


void lbu(uint32_t instruction)
{
  
  uint8_t rs = RS_FIELD(instruction);
  uint8_t rt = RT_FIELD(instruction);
  uint16_t imm = I_FIELD(instruction);
  uint32_t maddress = (uint32_t)(registers[rs] + signExt(imm));
  registers[rt] = (uint32_t)memory[maddress];

}


void lh(uint32_t instruction)
{
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rs = RS_FIELD(instruction);
  uint16_t imm = I_FIELD(instruction);
  registers[rt] = signExt( *(uint16_t *) &(memory[registers[rs] + signExt(imm)]));

}


void lhu(uint32_t instruction)
{
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rs = RS_FIELD(instruction);
  uint16_t imm = I_FIELD(instruction);
  registers[rt] = (uint32_t)(*(uint16_t *) &(memory[registers[rs] + signExt(imm)]));

}


void lw(uint32_t instruction)
{
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rs = RS_FIELD(instruction);
  uint16_t imm = I_FIELD(instruction);
  registers[rt] = (uint32_t)(*((uint32_t*) &memory[ registers[rs] + signExt(imm)]));
}


void lui(uint32_t instruction)
{    
  uint8_t rt = RT_FIELD(instruction);
  uint16_t imm = I_FIELD(instruction);
  registers[rt]=(uint32_t)(imm << 16);
}


// define mask to extract the lowest byte of [rt]
#define MASK_SB 0x000000ff
void sb(uint32_t instruction)
{    
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rs = RS_FIELD(instruction);
  uint16_t imm = I_FIELD(instruction);
  memory[registers[rs] + signExt(imm)] = (uint8_t)(MASK_SB & registers[rt]);
}


// define mask to extract the lower half word of [rt]
#define MASK_SH 0x0000ffff
void sh(uint32_t instruction)
{    
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rs = RS_FIELD(instruction);
  uint16_t imm = I_FIELD(instruction);
  *(uint16_t*) &(memory[registers[rs] + signExt(imm)]) = (uint16_t)(MASK_SH & registers[rt]);
}


void sw(uint32_t instruction)
{    
  uint8_t rt = RT_FIELD(instruction);
  uint8_t rs = RS_FIELD(instruction);
  uint16_t imm = I_FIELD(instruction);
  *(uint32_t*) &(memory[registers[rs] + signExt(imm)]) = registers[rt];
}