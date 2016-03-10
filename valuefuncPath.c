#include <stdio.h>
#include <stdlib.h>

int main(void){
	FILE *fp;
	double *value_function;
	value_function = (double*)calloc(11*51, sizeof(double));
	printf("load policy\n");

	fp = fopen("results.dat","rb");
	fread(value_function, sizeof(double), 11*51, fp);
	fclose(fp);

	int i,j, num;
	double max;
	for(i=0;i<51;i++){
		max=-10;
		num=0;
		for(j=0;j<11;j++){
			if(max < value_function[i*11+j]){
				max = value_function[i*11+j];
				num = j;
			}
		}
		printf("%d : %d, %lf\n",i,num,max);
	}
	return 0;
}

