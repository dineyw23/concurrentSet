#include "set.h"

#include <string.h>
int compare(void* data1, void* data2)
{	
	int int1 = *(int*)data1;
	int int2 = *(int*)data2;
	return (int1 - int2);
}
int main(){
	set_t* newSet = new_set(compare);

	int intTest = 323;
	int* intP = malloc(sizeof(int));
    memcpy(intP, &intTest, sizeof(int));
	int intTest2 = 2;
	int* intP2 = malloc(sizeof(int));
	memcpy(intP2, &intTest2, sizeof(int));
	printf("%d\n", set_add(newSet,intP));
	printf("%d\n", set_add(newSet,intP2));
	printf("%d\n", set_add(newSet,intP2));
	printf("%d\n", set_contains(newSet,&intTest2));	
	printf("%d\n", set_del(newSet,intP));
	printf("%d\n", set_contains(newSet,intP));
}

