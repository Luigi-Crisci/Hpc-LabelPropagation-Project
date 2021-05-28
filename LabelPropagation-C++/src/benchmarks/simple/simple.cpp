#include<iostream>
#include<omp.h>


int main(){
	
	int sum = 0;

	#pragma omp parallel for reduction(+:sum)
	for (int i = 0; i < 100000; i++)
	{
		sum += 1;
	}

	std::cout<<sum<<std::endl;	

	exit(0);
}