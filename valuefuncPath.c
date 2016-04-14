#include <stdio.h>
#include <stdlib.h>

int main(void){
	FILE *fp;
	double *value_function;
	int numActions = 11;
	int numStates = 51;
	int numVelocity = 101;

	value_function = (double*)calloc(numActions*numStates*numVelocity, sizeof(double));
	printf("load policy \t  state, velocity, action, Q value\n");

	fp = fopen("results.dat","rb");
	fread(value_function, sizeof(double), numActions*numStates*numVelocity, fp);
	fclose(fp);

	int i,j,k, num;
    int enter;
	double max;
	for(i=0;i<numStates;i++){
        enter = 0;
		for(j=0;j<numVelocity;j++){
			max = -100;
			num = 0;
			for(k=0;k<numActions;k++){
				if(max < value_function[i*numActions*numVelocity + j*numActions + k]){
					max = value_function[i*numActions*numVelocity + j*numActions + k];
					num = k;
				}
			}
            if(max!=0){
			    printf("%d, %d : %d, %lf\t",i, j, num, max);
                enter++;
                if(enter%5 == 4)
                    printf("\n");
            }
		}
		printf("\n----------------------------------------------------------------------\n");
	}
/*
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
*/
	return 0;
}

