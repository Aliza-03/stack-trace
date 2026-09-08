#include<stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <elfutils/libdwfl.h>

// Global DWARF handle
static Dwfl *dwfl = NULL;

/*int init_dwarf(const char *executable) {

    // Open the ELF file
    Elf *elf = elf_begin(-1, ELF_C_READ, NULL);
    if (!elf) return -1;
    
    // Initialize DWARF
    Dwarf = dwarf_begin_elf(elf, DWARF_C_READ, NULL);
    if (!dwarf) return -1;
    
    return 0;
}*/



void func_a(int y)
{
	printf("%d\n",y);	
}

void func_b(int y)
{
	y*=2;
	func_a(y);
}

void func_c()
{
	func_b(2);
}
//--------------------------------------------
// Stack Walk Logic
struct stack_frame
{
	struct stack_frame* prev_ptr; //points to the previous frame
	void* return_addr; // uhhh we'll get to that
	
};

//to get current ptr

void* get_current_ptr()
{
	void* fp; 
	__asm__ volatile("mov %%rbp, %0" : "=r"(fp)); //bad idea. fix this to be more efficient
    return fp;
}

void walk_stack(const char* p){
	
	printf("Strolling from %s\n",p);
	const char *func_name = "unknown";
	
	struct stack_frame* sf=(struct stack_frame*)get_current_ptr();
	int level=0;
	
	while(sf && (uintptr_t)sf>0x1000)
	{
		void *ret_addr = sf->return_addr;
		func_name="unknown";
		
		if (dwfl) {
            		Dwfl_Module *mod = dwfl_addrmodule(dwfl, (Dwarf_Addr)ret_addr);
            	if (mod) {
                	const char *name = dwfl_module_addrname(mod, (Dwarf_Addr)ret_addr);
                if (name) func_name = name;
            }
        
		printf("...[#%d] Frame: %p, Prev-Ptr: %p, Return address: %p,  Function: %c\n", 
                level, sf,sf->prev_ptr, sf->return_addr,func_name);
                // Move to previous frame
        	sf = sf->prev_ptr;
        	level++;
        
        	// Safety: don't walk too far
        	if (level > 20) break;
        	}
	}
    
       printf("Total frames: %d\n", level);
}

int main()
{

	printf("Basic Stack Call\n");
	func_c();
	walk_stack("main");
	return 0;
//-----------------------------------------

	

}
