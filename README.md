# 8085sim

## Intel 8085

The Intel 8085 ("eighty-eighty-five") is an 8-bit microprocessor produced by Intel and introduced in March 1976. It is a software-binary compatible with the more-famous Intel 8080 with only two minor instructions added to support its added interrupt and serial input/output features. However, it requires less support circuitry, allowing simpler and less expensive microcomputer systems to be built.

## Architecture

It consists of eight 8-bit general purpose registers along with a temporary 8-bit register
It has configuration of
It has the following configuration −

1. 8-bit data bus
2. 16-bit address bus, which can address upto 64KB
3. A 16-bit program counter
4. A 16-bit stack pointer
5. Six 8-bit registers arranged in pairs: BC, DE, HL

## Functional Units

1. **Accumulator**:             
It is an 8-bit register used to perform arithmetic, logical, I/O & LOAD/STORE operations. It is connected to internal data bus & ALU.

2. **General purpose register**:                               
There are 6 general purpose registers in 8085 processor, i.e. B, C, D, E, H & L. Each register can hold 8-bit data.				
These registers can work in pair to hold 16-bit data and their pairing combination is like B-C, D-E & H-L.

3. **Program counter**:                     
It is a 16-bit register used to store the memory address location of the next instruction to be executed. Microprocessor increments the program whenever an instruction is being executed, so that the program counter points to the memory address of the next instruction that is going to be executed.

4. **Stack pointer**:                  
It is also a 16-bit register works like stack, which is always incremented/decremented by 2 during push & pop operations.

5. **Temporary register**:                      
It is an 8-bit register, which holds the temporary data of arithmetic and logical operations.

6. **Flag register**:                      
It is an 8-bit register having five 1-bit flip-flops, which holds either 0 or 1 depending upon the result stored in the accumulator.
                                 
    These are the set of 5 flip-flops −
	1. Sign (S)
	2. Zero (Z)
	3. Auxiliary Carry (AC)
	4. Parity (P)
	5. Carry (C)

	Its bit position is shown in the following table −

	|D7	|D6	|D5	|D4	|D3	|D2	|D1	|D0|
	|---|---|---|---|---|---|---|--|
	|S	|Z	|	|AC	|P	|	|CY |  |

7. **Instruction register and decoder**:                           
It is an 8-bit register. When an instruction is fetched from memory then it is stored in the Instruction register. Instruction decoder decodes the information present in the Instruction register.

8. **Timing and control unit**:                      
It provides timing and control signal to the microprocessor to perform operations. Following are the timing and control signals, which control external and internal circuits 

Control Signals: READY, RD’, WR’, ALE
Status Signals: S0, S1, IO/M’
DMA Signals: HOLD, HLDA
RESET Signals: RESET IN, RESET OUT

9. **Interrupt control**:                                      
As the name suggests it controls the interrupts during a process. When a microprocessor is executing a main program and whenever an interrupt occurs, the microprocessor shifts the control from the main program to process the incoming request. After the request is completed, the control goes back to the main program.

	There are 5 interrupt signals in 8085 microprocessor:                                    
	1. INTR
	2. RST 7.5
	3. RST 6.5
	4. RST 5.5
	5. TRAP

10. **Serial Input/output control**:                         
It controls the serial data communication by using these two instructions: SID (Serial input data) and SOD (Serial output data).

11. **Address buffer and address-data buffer**:                                
The content stored in the stack pointer and program counter is loaded into the address buffer and address-data buffer to communicate with the CPU. The memory and I/O chips are connected to these buses; the CPU can exchange the desired data with the memory and I/O chips.

12. **Address bus and data bus**:                                 
Data bus carries the data to be stored. It is bidirectional, whereas address bus carries the location to where it should be stored and it is unidirectional. It is used to transfer the data & Address I/O devices.

