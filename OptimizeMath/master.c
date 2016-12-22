#include  "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
//support at most for 98 parameters and less than 99 inequality
int M = 100;
int N = 100;
double initialValueForArray = -300.99;// to check the original maxtrix value;

//next three varable should not be changed
int realRows;// how many rows in the array.
int realColumns;// how many columns in the array.
double ** formatData; // the data matrix to be dealwith. with pointer you can refer the r row c columns with *(formatData[r]+s)

//read data from file module 
void getDataFromFile(char *,double (*)[]);// read data to the arraay
void saveToArray(char *, double (*)[],int,int);//used to save data called by getOriginaldata.
int getSpaceNumbers(char *,int);

//initialization module 
void initialization(char * );
int getRowNumber(double (*)[]);
int getColumnNumber(double (*)[]);
double  ** getFormatData(double(*)[]);

void printfAllDataInArray(double ** ,int ,int);

// calculate module 
void calculateByType(char);


int main(){

	//initalization arrays, suppose if there is no value then with -300.99
	//char * path = "/Users/yuanping/Documents/code/xcode/OptimizeMath/OptimizeMath/math.txt";
    char * path = "math.txt";// xcode please with full path
	initialization(path);

	//output the format data to user
	printf("\n%s\n","please check your input data here :" );
	printfAllDataInArray(formatData,realRows,realColumns);
    
    //let user to choose operation
	//read choose from the console;
	char typeOfOperation;
	printf("%s\n", "please input 1 for the simplex. 2 for cutting. 3 for binary tree");
	typeOfOperation = getc(stdin);


	//calculate the result :
	calculateByType(typeOfOperation);
    
	//output result here or output during the calculate progress
	printf("the result is : \n");
	return 0;

}

void  getDataFromFile(char* filePath,double (* dataArray)[N]){
	//double *  data[100];
	char * line  = NULL;
	FILE * fp;
	size_t len = 0;
	ssize_t read;
	int row = 0;
	if((fp=fopen(filePath,"r"))==NULL){
		printf("can't open file\n");
	}else{
		while((read = getline(&line,&len,fp))!=-1){
			//printf(" line data is :%s\n", line );
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
	//int getNumbers = getSpaceNumbers(charArray,len)+1;// the numbers should be the space in the array plus one;
	//printf("the number of the row :%d is %d\n",row,getNumbers );
	//int i =0;
	//int numberParserd = 0;

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
				//printf("start at %s the length is %d\n", charArray+startIndex,length);
				strncpy(item,charArray+startIndex,length);
				//printf("the iterm is :%s  the value is %f:\n",item ,atof(item));
				number = atof(item);
				*(dataArray[row]+dataIndex++)= number;

	            //printf("number : %f data at array row :%d, index :%d value : %F endindex:%d \n"  ,number,row,dataIndex-1,*(dataArray[row]+dataIndex-1),endIndex);
				startIndex = endIndex+1;
			}
			index++;
			
		}
	}
   
}
//check space number to get the whole value
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
//initialize  the two dimension array and the row number and column number
void initialization(char * filePath){

	double data[M][N]; 
	int i =0,j=0;
	for(i=0;i<M;i++)
		for(j=0;j<N;j++)
			data[i][j]=initialValueForArray;
	/*
     //the default matix
	printf("the  matix is :\n");
	for(i=0;i<13;i++){
		printf("\n");
	    for(j =0;j<13;j++)
	        printf(" %f ",data[i][j]);
	}
	*/
	
	getDataFromFile(filePath,data);
	/*
     //output inital matix
	printf("the initil matix is :\n");
	for(i=0;i<13;i++){
		printf("\n");
	    for(j =0;j<13;j++)
	        printf(" %f ",data[i][j]);
	}
	*/

	// get useful data
	realRows = getRowNumber(data);
	realColumns = getColumnNumber(data);
	formatData = getFormatData(data);

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
double ** getFormatData(double (*original) [N]){
	
	int rowNumber = getRowNumber(original);

	int columnNumber = getColumnNumber(original);

	double ** returnData = malloc((N+M)*sizeof(double));
	
    //initial new arrays
	int i =0, j=0;
	double * rows;
	for( i = 0; i<rowNumber;i++){
		rows = malloc(N*sizeof(double));
		*(returnData+i) = rows; //new double [columnNumber];
	}
	//fulfile the real data
	for (i = 0; i < rowNumber; ++i)
	{
		for (j = 0; j < columnNumber; ++j)
		{	
			if(*(original[i]+j) == initialValueForArray){
				returnData[i][j] = 0.0;
            }else{
                returnData[i][j] = *(original[i]+j);
            }
		}
	}
	return returnData;
}
//output the two dimension array with the row number and column number
void printfAllDataInArray(double **afterData ,int row, int column){
	int i=0,j=0;
    for( i=0;i<row;i++){
		printf("the %dth row :",i);
		for(j=0;j<column;j++){
			printf(" %f ",*(afterData[i]+j));//*(afterData[i]+j));				
		}
		printf("\n");
	}
}
// router for different operation. and the progress can be output into files or just directly out at console
void calculateByType(char typeOfOperation){
	printf("%s\n", "here should be some functions here:");
	printf("%s\n","your choose is : " );
	fputc(typeOfOperation ,stdout);
	printf("%s\n","" );
	if(typeOfOperation == '1'){
		// call function simplex
		printf("%s\n", "you have choose to use simplex");
	}else if(typeOfOperation == '2'){
		// call function for cuttings
		printf("%s\n", "you have choose to use cutting");
	}else if(typeOfOperation == '3'){
		// call function for binary tree
		printf("%s\n", "you have choose to use binary");
	}else if(typeOfOperation == '9'){//9 means finished the operation
		return ;
	}else{
		printf("%s\n","please recheck value you entered" );
	}
}
