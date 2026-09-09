#include<stdio.h>
#include <stdint.h>
//#include <stdlib.h>
#include <elfutils/libdwfl.h>

// Global DWARF handle
static Dwfl *dwfl = NULL;

/*int init_dwarf(const char *executable) {
    // Initialize Dwfl (DWARF Frame Library)
    dwfl = dwfl_begin(&Dwfl_Callbacks);
    if (!dwfl) return -1;
    
    // Report the executable
    dwfl_report_offline(dwfl, executable, executable, -1);
    dwfl_report_end(dwfl, NULL, NULL);
    
    return 0;
}
	*/




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

	static const Dwfl_Callbacks callbacks = {
		.find_elf = dwfl_linux_proc_find_elf,
		.find_debuginfo = dwfl_standard_find_debuginfo,
	};
	dwfl = dwfl_begin(&callbacks);
	//dwfl_linux_proc_report(dwfl, getpid());

	dwfl_report_end(dwfl, NULL, NULL);
	
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
        
		printf("...[#%d] Frame: %p, Prev-Ptr: %p, Return address: %p,  Function: %s\n", 
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
