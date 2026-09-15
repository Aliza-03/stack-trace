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


// main
int main()
{
	//init_dwarf();
	printf("Basic Stack Call\n");
	func_c();
	//walk_stack("main");
	return 0;
//-----------------------------------------


}