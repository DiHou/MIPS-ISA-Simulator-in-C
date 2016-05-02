#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "cpu.h"
#include "register_names.h"


void syscall(uint32_t instruction)
{
  char src[100000];
  

  switch(registers[2])
  { 
      case 1:
	printf("\n%d", registers[4]);
	break;
      case 2:
	printf("Error: print_float not supported.");
	cpu_exit(1);
	break;
      case 3:
	printf("Error: print_double not supported.");
	cpu_exit(1);
	break;
      case 4:
	printf("\n%s", &memory[registers[4]]);
	break;
      case 5:
	scanf("\n%d",&registers[2]);
	break;
      case 6:
	printf("Error: read_float not supported.");
	cpu_exit(1);
	break;
      case 7:
	printf("Error: read_double not supported.");
	cpu_exit(1);
	break;
      case 8:	
        scanf("\n%s", src);
	strncpy((char*) &memory[registers[4]], src, registers[5]);
	break;
      case 9:
	printf("Error: sbrk not supported.");
	cpu_exit(1);
	break;
      case 10:
	cpu_exit(0);
	break;
      default:
	printf("Error: Unsupported Operation.");
	cpu_exit(1);
	break;
    } 
}