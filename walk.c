#include<stdio.h>
#include <stdint.h>
//#include <stdlib.h>
#include <elfutils/libdwfl.h>
#include <sys/types.h>
#include <unistd.h>

// Global DWARF handle
static Dwfl *dwfl = NULL;

int init_dwarf(void) {
    static const Dwfl_Callbacks callbacks = {
        .find_elf = dwfl_linux_proc_find_elf,
        .find_debuginfo = dwfl_standard_find_debuginfo,
        .debuginfo_path = NULL,
    };
    
    dwfl = dwfl_begin(&callbacks);
    if (!dwfl) {
        fprintf(stderr, "ERROR: dwfl_begin failed\n");
        return -1;
    }
  
    if (dwfl_linux_proc_report(dwfl, getpid()) != 0) {
        fprintf(stderr, "ERROR: dwfl_linux_proc_report failed\n");
        dwfl_end(dwfl);
        dwfl = NULL;
        return -1;
    }
    
    dwfl_report_end(dwfl, NULL, NULL);
    printf("DWARF initialized successfully\n");
    return 0;
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
					//printf("Func name stored: %s\n", name);
                if (name) func_name = name;
            }
 
		}
		printf("...[#%d] Frame: %p, Prev-Ptr: %p, Return address: %p,  Function: %s\n", 
                level, sf,sf->prev_ptr, sf->return_addr,func_name);
                // Move to previous frame
        	sf = sf->prev_ptr;
        	level++;
        
        	// Safety: don't walk too far
        	if (level > 20) break;
        	}

		printf("Total frames: %d\n", level);
	}
    
    /// Toy Functions

	void func_a(int y)
{
	walk_stack("a");
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

int main()
{
	init_dwarf();
	printf("Basic Stack Call\n");
	func_c();
	//walk_stack("main");
	return 0;
//-----------------------------------------

	

}
