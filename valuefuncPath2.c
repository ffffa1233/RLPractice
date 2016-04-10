#include <stdio.h>
#include <stdlib.h>

int main(void){
	FILE *fp;
	double *value_function;
	int numActions = 11;
	int numStates = 51;
	int numVelocity = 31;

	value_function = (double*)calloc(numActions*numStates*numVelocity, sizeof(double));
	printf("load policy \t  state, velocity, action, Q value\n");

	fp = fopen("results.dat","rb");
	fread(value_function, sizeof(double), numActions*numStates*numVelocity, fp);
	fclose(fp);

	int i;

	for(i=0;i<numActions*numStates*numVelocity;i++)
		printf("%lf\n",value_function[i]);
	return 0;
}
