#include  "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
//support at most for 99 parameters and less than 99 variable
int M = 100;
int N = 100;
//next three varable should not be changed
int realRows;// how many rows in the array.
int realColumns;// how many columns in the array.
double ** formatData; // the data matrix to be dealwith. with pointer you can refer the r row c columns with *(formatData[r]+s)


double initialValueForArray = -300.99;// to check the original maxtrix value;
void getOriginalData(char *,double (*)[]);// read data to the arraay
void  saveToArray(char *, double (*)[],int,int);//used to save data called by getOriginaldata.
void printfAllDataInOriginal(double ** ,int ,int);
double  ** getReadedData(double(*)[]);
int getRowNumber(double (*)[]);
int getColumnNumber(double (*)[]);
void initialization(char * );
void calculateByType();
int getSpaceNumbers(char *,int);
int main(){
//get file path from the parameter
// store the data.
//initalization  suppos there is no value -300
//initial arrays
char * path = "testEnd.txt";
//char * path = "testEnd.txt";
initialization(path);


//let user to choose operation


//output the format data
printf("\n%s\n","after analysis" );
printfAllDataInOriginal(formatData,realRows,realColumns);
//read choose from the console;
char typeOfOperation;
printf("%s\n", "please input 1 for the simplex. 2 for cutting. 3 for binary tree");
typeOfOperation = getc(stdin);
printf("%s\n","your choose is : " );
fputc(typeOfOperation ,stdout);
printf("%s\n","" );

//calculate the result :
calculateByType();
//out put here 
printf("the result is : \n");
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
	int getNumbers = getSpaceNumbers(charArray,len)+1;// the numbers should be the space in the array plus one;
	printf("the number of the row :%d is %d\n",row,getNumbers );
	int i =0;
	int numberParserd = 0;

	while(index<len)
	//while(numberParserd<getNumbers)
	{

        if(*(charArray+index)!= ' '&&*(charArray+index)!='\n'&&*(charArray+index)!='.')
        {
			index++;
		}else{
			endIndex = index;
			if (endIndex >= startIndex+1&&strlen(charArray+startIndex)>0)
			{
				//get each item and then multiply;
				int length = endIndex - startIndex;
				double number = 0;//stored a number 
	//			printf("start index is %d, and the end index is %d the current index is %d\n",startIndex,endIndex,index);
				//printf("start index is %d length is %d\n",startIndex,length );
				/*
				while(startIndex<endIndex){
					//number += ((int)(atof(charArray+startIndex)/pow(10,length-1)))*pow(10,length-1);
					number = atof(charArray+startIndex);
					printf("startIndex is : %d factor: %f base :%d power: %d\n",startIndex,atof(charArray+startIndex),10,length-1);
					startIndex++;
					length -- ;				
				}
				startIndex++;//ensure that the start point to the non null field
				*/
				char item [10] = "";
				printf("start at %s the length is %d\n", charArray+startIndex,length);
				strncpy(item,charArray+startIndex,length);
				printf("the iterm is :%s  the value is %f:\n",item ,atof(item));
				number = atof(item);
				*(dataArray[row]+dataIndex++)= number;

	            printf("number : %f data at array row :%d, index :%d value : %F endindex:%d \n"  ,number,row,dataIndex-1,*(dataArray[row]+dataIndex-1),endIndex);
				startIndex = endIndex+1;
			}
			index++;
			
		}
	}
   
}
void printfAllDataInOriginal(double **afterData ,int row, int column){
	int i=0,j=0;
	//int row = getRowNumber(original),column = getColumnNumber(original);
	for( i=0;i<row;i++){
		printf("the row %d",i);
		for(j=0;j<column;j++){
			printf(" %f ",*(afterData[i]+j));//*(afterData[i]+j));				
		}
		printf("\n");
	}
}
double ** getReadedData(double (*original) [N]){
	
	int rowNumber = getRowNumber(original);

	int columnNumber = getColumnNumber(original);

	double ** returnData = malloc((N+M)*sizeof(double));
	//returnData = nprintf("here");

	int i =0, j=0;
	double * rows;
	for( i = 0; i<rowNumber;i++){
		//double rows[N] ;
		rows = malloc(N*sizeof(double));
		//printf("row number  :%d columns number : %d\n",rowNumber,columnNumber);
		//printf("rows is :%f returnData : %f\n",rows,*(returnData+i) );
		//printf("\n%s %f \n","hi" ,*(rows+i));
		*(returnData+i) = rows; //new double [columnNumber];

	}
	
	for (i = 0; i < rowNumber; ++i)
	{
		for (j = 0; j < columnNumber; ++j)
		{	
			if(*(original[i]+j) == initialValueForArray){
				returnData[i][j] = 0.0;
			}else
				returnData[i][j] = *(original[i]+j);
		}
	}
	return returnData;
}
int getRowNumber(double (*original)[N]){
	//get row number
	int rowNumber = 0;
	
	while(*(original[rowNumber]) != initialValueForArray && (*(original[rowNumber])!=0.0)){
		rowNumber++;

	}
	/*
	for (int i = 0; i < N; ++i)
	{
		if(*(original[rowNumber+i]) != initialValueForArray && (*(original[rowNumber+i])!=0.0)){
			rowNumber++;
			printf("%d\n",rowNumber);
		}
	}
	*/

	
	return rowNumber;
}
int getColumnNumber(double (*original)[N]){
	//get max columns number for each row
	int columnNumber = 0;
	
	while(*(original[0]+columnNumber) != initialValueForArray && (*(original[0]+columnNumber)!=0.0)){
		columnNumber++;
	}

	return columnNumber;
}
void initialization(char * filePath){

	double data[M][N]; 
	int i =0,j=0;
	for(i=0;i<M;i++)
		for(j=0;j<N;j++)
			data[i][j]=initialValueForArray;
	/*
	printf("the  matix is :\n");
	for(i=0;i<13;i++){
		printf("\n");
	    for(j =0;j<13;j++)
	        printf(" %f ",data[i][j]);
	}
	*/
	//set files
	//char * path = "math.txt";
	//char * testend = "testEnd.txt";
	// parser original data 
	getOriginalData(filePath,data);

	printf("the initil matix is :\n");
	for(i=0;i<13;i++){
		printf("\n");
	    for(j =0;j<13;j++)
	        printf(" %f ",data[i][j]);
	}


	// get useful data

	realRows = getRowNumber(data);
	realColumns = getColumnNumber(data);
	formatData = getReadedData(data);

}
void calculateByType(){
	printf("%s\n", "here should be some functions here:");
}
int getSpaceNumbers(char * line,int len){
	int index = 0;
	int count = 0;
	while(index<len) {
		if (*(line+index) == ' ')
		{
			count++;
		}
		index++;
	}

	return count;
}
