#include <stdio.h>
#include <stdlib.h>

typedef void (*pt)(void);

void ff(void){
	printf("ff\n");
}
void sf(void){
	printf("sf\n");
}
void tf(void){
	printf("tf\n");
}

void invoke_function(unsigned int index, pt *functions){
	if(index > 2) printf("Invalid index\n");
	else functions[index]();
}

int main(int argc, char* argv[]){
	pt functions[3];
	
	functions[0] = ff;
	functions[1] = sf;
	functions[2] = tf;
	
	for(int i = 0; i < 3; ++i)
		invoke_function(i, functions);
	
	return EXIT_SUCCESS;
}