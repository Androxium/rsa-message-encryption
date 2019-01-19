#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
void printArray(int A[], int size) { 
    int i; 
    for (i=0; i < size; i++) 
        printf("%d ", A[i]); 
    printf("\n"); 
} 

/* 	m = message you want are sending/receiving
   	e = exponent you raise m to
	n = the modulo number  */

/*
int main (void){
	printf("%d\n", rsa(47, 27, 55));
	return 0;
}
*/
