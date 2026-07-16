#include <stdio.h>
#include <stdlib.h>

static unsigned int counter = 0;

unsigned int retrieve(void){
	return counter;
}

void increment(void){
	counter++;
}

int main (int argc, char* argv[]){
	for(int i = 0; i < 5; i++){
		increment();
		printf("%d ", retrieve());
	}
	return EXIT_SUCCESS;
}