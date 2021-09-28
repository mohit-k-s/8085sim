#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <string.h>
#include <stdint.h>

enum rg
{
	b=0x00,c,d,e,h,l,m,a
};

/*
	PATTERN:
	0 	1 	2 	3 	4 	5 	6 	7
	B 	C 	D 	E 	H 	L 	M 	A
	0 	0	1 	1	2 	2	3 	3
	
	BDHM=2*index
	CELA=2*index+1
*/
enum flag
{
	cy=0,b1,p,b3,ac,b5,z,s
};
/*
flag order
7  6  5  4  3  2  1  0
s  z	 ac	   p	 cy

s-sign
z-zero
ac-auxillary carry
p-parity
c-carry flag
*/

typedef struct Simulator
{
	//8085 parts
	uint8_t registers[8];			//in order of b,c,d,e,h,l
	uint8_t IR;						//instruction registers
	uint16_t PC;					//program counter
	uint16_t SP;					//stack pointer
	uint8_t flags;					//flag register
	uint8_t memory[UINT16_MAX];		//raw memory

	//for simulation purpose
	uint16_t temp16;				//for data transfer etc
	uint8_t  temp8;					//temp for 8 bits
	uint16_t start;					//start location of program(main)
	uint16_t maxstack;				//max stack size(#stack directive)
	int state;						//state of simulator

}Simulator;

//basic operators
int swap(uint8_t* a,uint8_t* b)
{
	*a=*a^*b;
	*b=*a^*b;
	*a=*a^*b;
	return 0;
}

//operators
//data transfer
int mov(uint8_t* r1,uint8_t* r2)
{
	*r1=*r2;
	return 0;
}

int mvi(uint8_t* r,uint8_t data)
{
	*r=data;
	return 0;
}

int lxi(uint8_t* r,uint16_t address)
{
	memset(r,address,sizeof(uint16_t));
	return 0;
}

int lda(uint8_t* r,uint8_t data)
{
	*r=data;
	return 0;
}

int sta(uint8_t* address,uint8_t data)
{
	*address=data;
	return 0;
}

int lhld(uint8_t* r,uint16_t address)
{
	memset(r,address,sizeof(uint16_t));
	return 0;
}

int shld(uint8_t* address,uint8_t* hl)
{
	memset(address,*(hl+1),sizeof(uint8_t));	//writing L
	memset(address+1,*(hl),sizeof(uint8_t));	//writing H
	return 0;
}

//arithmetic
int add(uint8_t* acc,uint8_t data,uint8_t* flags)
{
	//set flag if carry is out of limits
	uint8_t acc_temp=*acc;
	*acc+=data;
	//carry flag
	if(*acc<acc_temp)		//overflowed
	{
		*flags|=(0x01<<cy);	//last bit is set
	}
	//if sign negative: last bit set
	if(*acc>>7==0x01)		
	{
		*flags|=(0x01<<s);
	}
	//if zero
	if(*acc==0x00)
	{
		*flags|=(0x00<<z);
	}
	//source: gfg
	//parity
	int y = *acc^(*acc>>1);
		y = y^(y>>2);
		y = y^(y>>4);
	*flags|=((y&0x01)<<p);		//last bit determines parity
	//not using auxiliary flag as it is in bcd system
	return 0;
}

void init(Simulator* sim,uint16_t start)
{
	//set all to zero and stack pointer to max
	sim->IR=0x00;
	sim->PC=start;
	sim->SP=0xFFFF;
	sim->flags=0x00;
	//set registers to 0x00
	memset(&(sim->registers),sizeof(uint8_t)*8,0);
	sim->state=0;
	return;
}

void cleanmemory(Simulator* sim)
{
	for(unsigned int i=0;i<UINT16_MAX;i++)
	{
		sim->memory[i]=0x00;
	}
	return;
}

void dump(Simulator* sim,const char* filename)
{
	FILE* fp=fopen(filename,"wb");
	if(fp==NULL)
	{
		fprintf(stderr,"error creating dump file\n");
		return;
	}
	//register dump
	fwrite(&(sim->registers[0]),sizeof(uint8_t),1,fp);
	fwrite(&(sim->registers[1]),sizeof(uint8_t),1,fp);
	fwrite(&(sim->registers[2]),sizeof(uint8_t),1,fp);
	fwrite(&(sim->registers[3]),sizeof(uint8_t),1,fp);
	fwrite(&(sim->registers[4]),sizeof(uint8_t),1,fp);
	fwrite(&(sim->registers[5]),sizeof(uint8_t),1,fp);
	fwrite(&(sim->registers[6]),sizeof(uint8_t),1,fp);
	fwrite(&(sim->registers[7]),sizeof(uint8_t),1,fp);
	fwrite(&(sim->IR),sizeof(uint8_t),1,fp);
	fwrite(&(sim->PC),sizeof(uint16_t),1,fp);
	fwrite(&(sim->SP),sizeof(uint16_t),1,fp);

	//memory dump
	for(unsigned int i=0;i<UINT16_MAX;i++)
	{
		fwrite(&(sim->memory[i]),sizeof(uint8_t),1,fp);
	}
	fclose(fp);
	return;
}

int process(Simulator* sim)
{
	//decode instruction
	//and execute it
	if(sim->IR==0x76)	//HLT
	{
		//sanity check
		return -1;
	}
	//MOV isntructions
	else if((sim->IR)&(0x70)==0x70)	//of opcode with high==7 (M location and A location)
	{
		return ((0x70)^(sim->IR)<(0x08))? 
		mov((uint8_t*)&(sim->memory[sim->registers[h]<<4+sim->registers[l]]),	//0 to 7 indicates M[HL] location
			(uint8_t*)&(sim->registers[(uint8_t)0x70^sim->IR])):
		mov((uint8_t*)&(sim->registers[a]),(uint8_t*)&(sim->registers[(uint8_t)(0x70^(sim->IR-0x08))]));
	}
	else if((sim->IR)&(0x60)==0x60)
	{
		return ((0x60)^(sim->IR)<(0x08))? 
		mov((uint8_t*)&(sim->registers[h]),(uint8_t*)&(sim->registers[(uint8_t)0x60^sim->IR])):
		mov((uint8_t*)&(sim->registers[l]),(uint8_t*)&(sim->registers[(uint8_t)(0x60^(sim->IR-0x08))]));
	}
	else if((sim->IR)&(0x50)==0x50)
	{
		return ((0x50)^(sim->IR)<(0x08))? 
		mov((uint8_t*)&(sim->registers[d]),(uint8_t*)&(sim->registers[(uint8_t)0x50^sim->IR])):
		mov((uint8_t*)&(sim->registers[e]),(uint8_t*)&(sim->registers[(uint8_t)(0x50^(sim->IR-0x08))]));
	}
	else if((sim->IR)&(0x40)==0x40)
	{
		return ((0x40)^(sim->IR)<(0x08))? 
		mov((uint8_t*)&(sim->registers[b]),(uint8_t*)&(sim->registers[(uint8_t)0x40^sim->IR])):
		mov((uint8_t*)&(sim->registers[c]),(uint8_t*)&(sim->registers[(uint8_t)(0x40^(sim->IR-0x08))]));
	}
	//MVI instructions
	else if((sim->IR)&(0x06)==0x06 && (sim->IR>>4)<0x04)
	{	//BDHM pattern here=0,1,2,3 for bdhm pattern 2*(0 to 3)
		sim->temp8=sim->memory[++sim->PC];
		return mvi((uint8_t*)&(sim->registers[(sim->IR>>4)*2]),sim->temp8);
	}
	else if((sim->IR)&(0x0E)==0x0E && (sim->IR>>4)<0x04)
	{	//CELA pattern here=0,1,2,3 for cela pattern 2*(0 to 3)+1
		sim->temp8=sim->memory[++sim->PC];
		return mvi((uint8_t*)&(sim->registers[((sim->IR>>4)*2)+1]),sim->temp8);
	}
	//LXI instruction
	else if((sim->IR)&(0x01)==0x01 && (sim->IR>>4)<0x04)
	{	//BDH SP pattern
		sim->temp16=0x0000;
		sim->temp16=sim->memory[++sim->PC];			//load low address
		sim->temp16+=sim->memory[++sim->PC]<<8;		//load high address
		if((sim->IR>>4)<0x03)	//register pairs
		{
			return lxi((uint8_t*)&(sim->registers[(sim->IR>>4)*2]),sim->temp16);
		}
		else	//SP
		{
			return sim->SP=sim->temp16;
		}
	}
	//LDA
	else if(sim->IR==0x3A)
	{	
		sim->temp16=0x0000;
		sim->temp16=sim->memory[++sim->PC];			//load low address
		sim->temp16+=sim->memory[++sim->PC]<<8;		//load high address
		return lda((uint8_t*)&(sim->registers[a]),sim->memory[sim->temp16]);
	}
	//STA
	else if(sim->IR==0x32)
	{
		sim->temp16=0x0000;
		sim->temp16=sim->memory[++sim->PC];			//load low address
		sim->temp16+=sim->memory[++sim->PC]<<8;		//load high address
		return sta((uint8_t*)&(sim->memory[sim->temp16]),sim->registers[a]);
	}
	//LHLD
	else if(sim->IR==0x2A)
	{
		sim->temp16=0x0000;
		sim->temp16=sim->memory[++sim->PC];			//load low address
		sim->temp16+=sim->memory[++sim->PC]<<8;		//load high address
		return lhld((uint8_t*)&(sim->registers[h]),sim->temp16);
	}
	//SHLD
	else if(sim->IR==0x22)
	{
		sim->temp16=0x0000;
		sim->temp16=sim->memory[++sim->PC];			//load low address
		sim->temp16+=sim->memory[++sim->PC]<<8;		//load high address
		return shld((uint8_t*)&(sim->memory[sim->temp16]),&sim->registers[h]);
	}
	//LDAX
	else if(sim->IR&0x0A==0x0A && sim->IR>>4<0x02)
	{
		//ldax is loading from register
		sim->temp16=0x0000;
		//BD pattern so index*2
		sim->temp16=sim->memory[sim->IR>>4*2+1];		//load low address
		sim->temp16+=sim->memory[sim->IR>>4*2]<<8;		//load high address
		return lda((uint8_t*)&(sim->registers[a]),sim->memory[sim->temp16]);
	}
	//STAX
	else if(sim->IR&0x02==0x02 && sim->IR>>4<0x02)
	{
		//stax is storing accumulator to register address
		sim->temp16=0x0000;
		//BD pattern so index*2
		sim->temp16=sim->memory[sim->IR>>4*2+1];		//load low address
		sim->temp16+=sim->memory[sim->IR>>4*2]<<8;		//load high address
		return sta((uint8_t*)&(sim->memory[sim->temp16]),sim->registers[a]);
	}
	//XCHG
	else if(sim->IR==0xDE)
	{
		//exchange HL and DE pairs
		return swap(&sim->registers[h],&sim->registers[d])
		&& swap(&sim->registers[l],&sim->registers[e]);
	}

	//Arithmetic group
	//ADD r or ADD M
	else if(sim->IR&0x80==0x80)
	{
		if(sim->IR&0x06==0x06)	//M
		{
			//load from HL pair
			sim->temp16=sim->registers[l];
			sim->temp16+=sim->registers[h]<<8;
			sim->temp8=sim->memory[sim->temp16];
		}
		else
		{	//register pair respectively
			sim->temp8=sim->registers[sim->IR^0x80];
		}
		return add(sim->registers[a],sim->temp8,sim->flags);
	}
	return 0;
}

void run(Simulator* sim)
{
	//fetch decode execute
	sim->IR=sim->memory[sim->PC];
	sim->state=1;
	int result=0;
	while(result!=-1 && sim->IR!=0x76)
	{
		result=process(sim);
		sim->PC++;
	}
	sim->state=0;
	return;
}

#endif //SIMULATOR_H