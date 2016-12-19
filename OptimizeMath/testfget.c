#include  "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
int M = 100;
int N = 100;
void getOriginalData(char *,double (*)[]);
void  saveToArray(char *, double (*)[],int,int);
int main(){
/*
char * line =NULL;
FILE * fp;
size_t len =0;
ssize_t read;
if((fp=fopen("math.txt","r"))==NULL){
	printf("can't open file\n");
}else{
	while ((read = getline(&line, &len, fp)) != -1) {
        printf("Retrieved line of length %zu :\n", read);
        printf("%s", line);
    }
}
fclose(fp);
*/
double data[M][N];
//initalization 
int i =0,j=0;
for(;i<M;i++)
	for(;j<N;j++)
		data[i][j]=-300;
//		printf("%f",data[i][j]);
char * path = "math.txt";
char * testend = "testEnd.txt";

getOriginalData(path,data);
printf("\n");
for(i=0;i<4;i++){
	printf("\n");
    for(j =0;j<5;j++)
        printf(" %f ",data[i][j]);
}
return 0;

}
void  getOriginalData(char* filePath,double (* dataArray)[N]){
	double *  data[100];
	char * line  = NULL;
	FILE * fp;
	size_t len = 0;
	ssize_t read;
	int row = 0;
	if((fp=fopen(filePath,"r"))==NULL){
		printf("can't open file\n");
	}else{
		while((read = getline(&line,&len,fp))!=-1){
		//	printf("%s",*(line+3)-*(line));
		//	str_split(line," ");
			printf("%s",line);
			printf("%d",len);
			printf("-%s-", (line+9));
			saveToArray(line,dataArray,row,len);
			row++;				
		}
	}
	fclose(fp);
	
}
void  saveToArray(char * charArray, double (*dataArray)[N],int row,int len){
	//get two space and splite get the value.
	int dataIndex = 0;
	int startIndex = 0;
	int endIndex = 0;
	int index = 0;
	//while( strncmp((charArray+index),'\0',1) ==0)
	int i =0;
//  while(*(charArray+index)!= '\0')
	while(index<len)
	{
//printf("index = %d and len is %d the string here -%s-",index,len,(charArray+index));
     //   printf("in loop");
		//if(strncmp((charArray+index),'\0',1) ==0)
        if(*(charArray+index)!= ' '&&*(charArray+index)!='\n'&&*(charArray+index)!='.')
        {
			index++;
		}else{
			endIndex = index;
			//get each item and then multiply;
			int length = endIndex - startIndex;
			double number = 0;//stored a number 
//			printf("start index is %d, and the end index is %d the current index is %d\n",startIndex,endIndex,index);
			while(startIndex < endIndex){
				number += atof((charArray+startIndex))*pow(10,length-1);
				startIndex++;
				length -- ;				
			}
			startIndex++;//ensure that the start point to the non null field
			*(dataArray[row]+dataIndex++)= number;
            printf("number : %f data at array row :%d, index :%d value :%F\n",number,row,dataIndex-1,*(dataArray[row]+dataIndex-1));
			index++;
		}
	}
    //printf("out of loop");

}
void printfAllDataInOriginal(double (*data)[] ){

	int i = 0,j = 0;
	for( ;i<M;i++){`
		printf("the row %d",i);
		for(;j<N;j++){
			printf(" %f ",data[i][j]);				
		}
		printf("\n");
	}
}
