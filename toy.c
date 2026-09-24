#include"trace.h"



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

void recurse(int n)
{
		walk_stack("walk_stack");
		if (n>0){
			printf("Trace %d\n",n); //why does recursion print multiple stacks
			recurse(n-1);
		}

		return;


}

// main
int main()
{
	//init_dwarf();
	printf("Basic Stack Call\n");
	//func_c();
	recurse(3);
	//walk_stack("main");
	return 0;
//-----------------------------------------


}