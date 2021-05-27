#include<iostream>
#include<omp.h>


int main(){
	
	int size = 1000000000;
	int *vet = (int*) calloc(size,sizeof(int*));

	// #pragma omp simd
	// {	
		for (int i = 0; i < size; i++)
		{
			vet[i] = i;
		}
	// }

	// for (int i = 0; i < size; i++)
	// {
	// 	std::cout<<vet[i]<<" ";
	// }
	// std::cout<<std::endl;
	free(vet);
	return 0;
}