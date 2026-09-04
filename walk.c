#include<stdio.h>
#include <stdint.h>
#include <stdlib.h>


void func_a()
{
	printf("Hello World\n");	
}

void func_b()
{
	func_a();
}

void func_c()
{
	func_b();
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
	
	struct stack_frame* sf=(struct stack_frame*)get_current_ptr();
	int level=0;
	
	while(sf && (uintptr_t)sf>0x1000)
	{
		printf("...[#%d] Frame: %p, Return address: %p\n", 
                level, sf, sf->return_addr);
                // Move to previous frame
        	sf = sf->prev_ptr;
        	level++;
        
        	// Safety: don't walk too far
        	if (level > 20) break;
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
