#include <stdio.h>
#include <stdlib.h>

int main(void){
	FILE *fp;
	double *value_function;
	int numActions = 11;
	int numStates = 51;
	value_function = (double*)calloc(numActions*numStates, sizeof(double));
	printf("load policy\n");

	fp = fopen("results.dat","rb");
	fread(value_function, sizeof(double), numActions*numStates, fp);
	fclose(fp);

	int i,j, num;
	double max;
	for(i=0;i<numStates;i++){
		max=-100;
		num=0;
		for(j=0;j<numActions;j++){
			if(max < value_function[i*numActions+j]){
				max = value_function[i*numActions+j];
				num = j;
			}
		}
		printf("%d : %d, %lf\n",i,num,max);
	}
	return 0;
}

