#include  "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "functions.h"
//support at most for 99 parameters and less than 99 variable

int main(){
//get file path from the parameter
 // store the data.
//initalization  suppos there is no value -300
double data[M][N];
realRows = getRowNumber(data);
realColumns = getColumnNumber(data);
int i =0,j=0;
for(;i<M;i++)
	for(;j<N;j++)
		data[i][j]=initialValueForArray;
//		printf("%f",data[i][j]);
char * path = "math.txt";
char * testend = "testEnd.txt";

getOriginalData(path,data);

printf("the initil matix is :\n");
for(i=0;i<13;i++){
	printf("\n");
    for(j =0;j<13;j++)
        printf(" %f ",data[i][j]);
}
double ** a = getReadedData(data);
//let user to choose operation

printf("\n%s\n","after analysis" );
printfAllDataInOriginal(a,data);
return 0;

}