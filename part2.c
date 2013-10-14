#include <stdio.h> // for stderr
#include <stdlib.h> // for exit()
#include "mips.h" // for execute_syscall()
#include "types.h"

void execute_instruction(Instruction instruction,Processor* processor,Byte *memory) {
    //Word temp;
    /* YOUR CODE HERE: COMPLETE THE SWITCH STATEMENTS */
    switch(instruction.opcode) {
        case 0x0: // opcode == 0x0(SPECIAL)
            switch(instruction.rtype.funct) {
                case 0x00: // funct == 0x0 (SLL)
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rt] << instruction.rtype.shamt;
                    processor->PC += 4;
                    break;
                case 0x02: // funct == 0x02 (SRL)
			    if ((processor->R[instruction.rtype.rt] & 0x10000) != 0){ //< didn't work for some reason
		              processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rt] * -1;
			    }
			    else{
		               processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rt];
			    }
		      processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rd] >> instruction.rtype.shamt;
                    processor->PC += 4;
                    break;
                case 0x03: // funct == 0x03 (SRA)
		      {
		      //printf("%d\n", processor->R[instruction.rtype.rt]);
		      int i;
		      for (i=0;i<instruction.rtype.shamt;i++){
			    if (i==0){
		              processor->R[instruction.rtype.rd] = (sWord)(sHalf)processor->R[instruction.rtype.rt] >> 1;
			    }
			    else{
		               processor->R[instruction.rtype.rd] = (sWord)(sHalf)processor->R[instruction.rtype.rd] >> 1;
			    }
		      }
                    processor->PC += 4;
                    break;
		      }
                case 0x08: // funct == 0x08 (JR)
                    processor->PC = processor->R[instruction.rtype.rs];
                    break;                    
                case 0x09: // funct == 0x09 (JALR)
		      {
		      Word temp;
                    temp  = processor->PC + 4;
                    processor->PC = processor->R[instruction.rtype.rs];
                    processor->R[instruction.rtype.rd] = temp;
                    break;
 		      }
                case 0xc: // funct == 0xc (SYSCALL)
                    execute_syscall(processor);
                    processor->PC += 4;
                    break;
                case 0x10: // funct == 0x10 (MFHI)
                    processor->R[instruction.rtype.rd] = processor->RHI;
                    processor->PC += 4;
                    break;
                case 0x12: // funct == 0x12 (MFLO)
                    processor->R[instruction.rtype.rd] = processor->RLO;
                    processor->PC += 4;
                    break;
                case 0x18: // funct == 0x18 (MULT)
		      {
		      sDouble temp;
                    temp = ((sWord) processor->R[instruction.rtype.rs]) * (sWord) processor->R[instruction.rtype.rt];
                    processor->RLO = (sWord)temp;
                    processor->RHI = (sWord)(temp >> 32);
		      printf("%d\n",processor->RLO);
		      printf("%d\n",processor->RHI);
                    processor->PC += 4;
                    break;
		      }
                case 0x19: // funct == 0x19 (MULTU)
		      {
		      Double temp;
                    temp = (Double) processor->R[instruction.rtype.rs] * (Double) processor->R[instruction.rtype.rt];
                    processor->RLO = (Word)temp;
                    processor->RHI = (Word)(temp >> 32);
                    processor->PC += 4;
                    break;
		      }
                case 0x21: // funct == 0x21 (ADDU)
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] + processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                case 0x23: // funct == 0x23 (SUBU)
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] - processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                case 0x24: // funct == 0x24 (AND)
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] & processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                case 0x25: // funct == 0x25 (OR)
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] | processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                case 0x26: // funct == 0x26 (XOR)
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] ^ processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                case 0x27: // funct == 0x27 (NOR)
                    processor->R[instruction.rtype.rd] = ~(processor->R[instruction.rtype.rs] & processor->R[instruction.rtype.rt]);
                    processor->PC += 4;
                    break;
                case 0x2a: // funct == 0x2a (SLT)
                    processor->R[instruction.rtype.rd] = (sWord) processor->R[instruction.rtype.rs] < (sWord) processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                case 0x2b: // funct == 0x2b (SLTU)
                    processor->R[instruction.rtype.rd] = (Word) processor->R[instruction.rtype.rs] < (Word) processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                default: // undefined funct
                    fprintf(stderr,"%s: pc=%08x,illegal function=%08x\n",__FUNCTION__,processor->PC,instruction.bits);
                    exit(-1);
                    break;
            }
            break;
        case 0x2: // opcode == 0x2 (J)
            processor->PC = ((processor->PC+4) & 0xf0000000) | (instruction.jtype.addr << 2);
            break;
        case 0x3: // opcode == 0x3 (JAL)
            processor->R[31] = processor->PC + 4;
            processor->PC = ((processor->PC+4) & 0xf0000000) | (instruction.jtype.addr << 2);
            break;
        case 0x04: // opcode == 0x04 (BEQ)
            if (processor->R[instruction.itype.rs] == processor->R[instruction.itype.rt]){
                processor->PC += 4 + (sWord)(sHalf)instruction.itype.imm*4;
		  break;
            }
            processor->PC += 4; 
            break;
        case 0x05: // opcode == 0x05 (BNE)
            if (processor->R[instruction.itype.rs] != processor->R[instruction.itype.rt]){
                processor->PC += 4 + (sWord)(sHalf)instruction.itype.imm*4;
		  break;
            }
            processor->PC += 4;
            break;
        case 0x09: // opcode == 0x09 (ADDIU)
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] + (sWord)(sHalf)instruction.itype.imm;
            processor->PC += 4; 
            break;
        case 0x0a: // opcode == 0x0a (SLTI)
            processor->R[instruction.itype.rt] = (sWord)processor->R[instruction.itype.rs] < (sWord)(sHalf)instruction.itype.imm;
            processor->PC += 4; 
            break;
        case 0x0b: // opcode == 0x0b (SLTIU)
            processor->R[instruction.itype.rt] = (sWord) processor->R[instruction.itype.rs] < instruction.itype.imm; ///////DIFFF
            processor->PC += 4; 
            break;
        case 0x0c: // opcode == 0x0c (ANDI)
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] & instruction.itype.imm;
            processor->PC += 4; 
            break;
        case 0xd: // opcode == 0xd (ORI)
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] | instruction.itype.imm;
            processor->PC += 4;
            break;
        case 0x0e: // opcode == 0x0e (XORI)
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] ^  instruction.itype.imm;
            processor->PC += 4;
            break;
        case 0x0f: // opcode == 0x0f (LUI)  
            processor->R[instruction.itype.rt] = instruction.itype.imm << 16;
            processor->PC += 4;
            break;

        case 0x20: // opcode == 0x20 (LB)
            processor->R[instruction.itype.rt] = (sWord) load(memory, processor->R[instruction.itype.rs] + (sWord)(sHalf)instruction.itype.imm, LENGTH_BYTE);
            processor->PC += 4;
            break;
        case 0x21: // opcode == 0x21 (LH)
            processor->R[instruction.itype.rt] = (Word) load(memory, processor->R[instruction.itype.rs] + (sWord)(sHalf)instruction.itype.imm, LENGTH_HALF_WORD);
            processor->PC += 4;
            break;
        case 0x23: // opcode == 0x23 (LW)
            processor->R[instruction.itype.rt] = load(memory, processor->R[instruction.itype.rs] + (sWord)(sHalf)instruction.itype.imm, LENGTH_WORD);
            processor->PC += 4;
            break;
        case 0x24: // opcode == 0x24 (LBU)
            processor->R[instruction.itype.rt] = load(memory, processor->R[instruction.itype.rs] + (sWord)(sHalf)instruction.itype.imm, LENGTH_BYTE);
            processor->PC += 4;
            break;
        case 0x25: // opcode == 0x25 (LHU) 
            processor->R[instruction.itype.rt] = load(memory, processor->R[instruction.itype.rs] + (sWord)(sHalf)instruction.itype.imm, LENGTH_HALF_WORD);
            processor->PC += 4;
            break;
        case 0x28: // opcode == 0x28 (SB)
            store(memory, processor->R[instruction.itype.rs] + (sWord)instruction.itype.imm,  LENGTH_BYTE, processor->R[instruction.itype.rt]);
            processor->PC += 4;
            break;
        case 0x29: // opcode == 0x029 (SH)
            store(memory, processor->R[instruction.itype.rs] + (sWord)instruction.itype.imm,  LENGTH_HALF_WORD, processor->R[instruction.itype.rt]);
            processor->PC += 4;
            break;
        case 0x2b: // opcode == 0x2b (SW)
            store(memory, processor->R[instruction.itype.rs] + (sWord)instruction.itype.imm,  LENGTH_WORD, processor->R[instruction.itype.rt]);
            processor->PC += 4;
            break;
        default: // undefined opcode
            fprintf(stderr,"%s: pc=%08x,illegal instruction: %08x\n",__FUNCTION__,processor->PC,instruction.bits);
            exit(-1);
            break;
    }
}


int check(Address address,Alignment alignment) {
    if (address < 1 || address >= MEMORY_SPACE){
    return 0;
    }
    if (address + alignment >= MEMORY_SPACE){ //not enough space
    return 0;
    }
    if (alignment != LENGTH_BYTE && alignment != LENGTH_HALF_WORD && alignment != LENGTH_WORD){
    return 0;
    }
    if ((address % alignment) != 0){
    return 0;
    }
    return 1;
}

void store(Byte *memory,Address address,Alignment alignment,Word value) {
    if(!check(address,alignment)) {
        fprintf(stderr,"%s: bad write=%08x\n",__FUNCTION__,address);
        exit(-1);
    }
    
    /* YOUR CODE HERE */
/*
    int i;
    for (i=0;i<alignment;i++){
        *(Word*)(memory + address + i) = (Byte)(value & 0xFF); 
       value = value >> 8; //chop off last 8 bits (1 byte)
   }
*/
    if (alignment == LENGTH_BYTE){
        *(Byte*)(memory+address) = value;
    }
    else if (alignment == LENGTH_HALF_WORD){
        *(Half*)(memory+address) = value;
    }
    else{
       *(Word*)(memory+address) = value;
    }


}

Word load(Byte *memory,Address address,Alignment alignment) {
    if(!check(address,alignment)) {
        fprintf(stderr,"%s: bad read=%08x\n",__FUNCTION__,address);
        exit(-1);
    }
    
    /* YOUR CODE HERE */
    //if (alignment != LENGTH_WORD){
    //Word return_thing = 0;
    //int i;
    //Word chunk =0;
    //Word mask = 0xff;
    //for(i=alignment-1;i>=0;i--){
	//chunk = (*(Byte*) (memory + address + i) & mask);
        //return_thing =  return_thing + chunk;
	//mask = mask << 8;
        //return_thing = (Word)return_thing << 8;
    //}
    //return return_thing;
    //}
    // incomplete stub to let "simple" execute
    // (only handles size == SIZE_WORD correctly)
    // feel free to delete and implement your own way
    if (alignment == LENGTH_BYTE){
        return *(Byte*)(memory+address);
    }
    else if (alignment == LENGTH_HALF_WORD){
        return *(Half*)(memory+address);
    }
    else {
    	return *(Word*)(memory+address);
    }
}
