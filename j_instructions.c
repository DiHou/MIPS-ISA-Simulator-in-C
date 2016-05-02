
#include <stdio.h>
#include <stdint.h>
#include "cpu.h"
#include "j_instructions.h"

#define J_FIELD(instruction)  (0x03ffffff & instruction)


void j(uint32_t instruction)
{
  uint32_t address = J_FIELD(instruction);
  npc = ((pc + 4) & 0xf0000000) | (address << 2);
}


void jal(uint32_t instruction)
{
  uint32_t address = J_FIELD(instruction);
  registers[31] = pc + 4;
  npc = ((pc + 4) & 0xf0000000) | (address << 2);
}