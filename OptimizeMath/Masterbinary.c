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

//simplex module;
void calculateSimplex(double ** ,int ,int );
int getSimplexType(double **, int ,int ); 
	//simplex phases 1
double ** SimplyToNormalSimplex(double **, int ,int );
double ** initialSimplexPhases1Matrix(double ** ,int ,int );
int getArtificialVariableNumber(double ** , int , int );
double * getBaseVariable(double ** , int , int );
double ** solveSimplexPhases1Matrix();
double ** formatPhases1Matrix();
	//normal simplex
double ** NormalSimplex(double ** , int ,int );
void solveNormalSimplex(double ** ,int , int );
	//functions to solve normal simplex
void calculateBA(int ,int ,int ,double  ** );//calculate B/A to find the index to operate
int getMinBAIndex(double ** , int ,int );// find the min positive value index in B/A;
void calculateMatrixByOptiParameter(int ,int ,int ,int ,double ** );//based on the calculate calculate matrix;
void calculateMatrixLineByParameter(double ** ,double ,int ,int ,int );// calculate each line for matrix

//for binary solution

double varibale[99][98][2] = {0};
void fff(int x);

void calculateBinary(double ** , int , int );







int main(){

	//initalization arrays, suppose if there is no value then with -300.99
	//char * path = "/Users/yuanping/Documents/code/xcode/OptimizeMath/OptimizeMath/math.txt";
    //char * path = "/Users/yuanping/Documents/code/xcode/OptimizeMath/OptimizeMath/simplex2.txt";
     char * path = "/Users/zhangxulong/Downloads/OptimizeMath-master/OptimizeMath/simplex2.txt";
    //char * path = "math.txt";// xcode please with full path
	initialization(path);

	//output the format data to user
	printf("\n%s\n","please check your input data here :" );
	printfAllDataInArray(formatData,realRows,realColumns);
    
    //let user to choose operation
	//read choose from the console;
	char typeOfOperation;
	printf("%s\n", "please input 1 for the simplex. 2 for cutting. 3 for binary tree");
	//typeOfOperation = getc(stdin);
	typeOfOperation = '3';

	//calculate the result :
	calculateByType(typeOfOperation);
    
	//output result here or output during the calculate progress
	//printf("the result is : \n");
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
				//printf("start index is %d, and the end index is %d the current index is %d\n",startIndex,endIndex,index);
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
	
	while(*(original[rowNumber]) != initialValueForArray ){ //&& (*(original[rowNumber])!=0.0)){
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
	
	while(*(original[0]+columnNumber) != initialValueForArray ){//&& (*(original[0]+columnNumber)!=0.0)){
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
	printf("\n" );
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
		printf("%s\n", "you have choose to use simplex ");
		calculateSimplex(formatData,realRows,realColumns);
		//printf("%s\n","finished" );
	}else if(typeOfOperation == '2'){
		// call function for cuttings
		printf("%s\n", "you have choose to use cutting");
	}else if(typeOfOperation == '3'){
		// call function for binary tree
		printf("%s\n", "you have choose to use binary");
		calculateBinary(formatData,realRows,realColumns);
	}else if(typeOfOperation == '9'){//9 means finished the operation
		return ;
	}else{
		printf("%s\n","please recheck value you entered" );
	}
}


void calculateSimplex(double ** matrix,int rows, int columns){
	//printf("hello" );
	int type = getSimplexType(matrix,rows,columns);
	double ** simplexMatrix ; 
	printf("type of simplex is %d \n",type );
	if(type == 2){
		printf("%s\n","this is two phases simplex" );
		simplexMatrix = SimplyToNormalSimplex(matrix, rows, columns);
	}else{
		printf("%s\n","this is normal simplex" );
		simplexMatrix = NormalSimplex(matrix,rows,columns);
	}
	
	if(simplexMatrix != NULL){
		int NormalSimplexRows = rows;
		int NormalSimplexColumns = (columns - 2) +(rows - 1) +3;
		printfAllDataInArray(simplexMatrix,NormalSimplexRows,NormalSimplexColumns);
		solveNormalSimplex(simplexMatrix,NormalSimplexRows,NormalSimplexColumns);
	}else{
		printf("%s\n", "there is no solution for your equations");
	}
	
		
}
//type 1 means directly call with the initial matrix.
//type 2 means with 2-step-simplex
int getSimplexType(double ** matrix, int row, int columns){
	int type = 1;
	int i = 0;

	for(i = 0;i<row-1;i++){
		//printf("%f\n",*(matrix[i]+columns-2) );
		if(*(matrix[i]+columns-2) == 5){//check the result only with 3 and 5
			type = 2;
		}
	}
	return type;
}
//phase 1 reduce to normal simplex.
double **  SimplyToNormalSimplex(double ** matrix, int rows, int columns){
	
	int numberAV =  getArtificialVariableNumber(matrix,rows,columns);
	int initialMatrixRows = rows + 1; //add one new row for U
	int initialMatrixColumns = columns - 2 + rows + numberAV + 2;// columns 

	//initial phase1 matrix
	double ** simplexPhase1Matrix = initialSimplexPhases1Matrix(matrix,rows,columns);
	//printfAllDataInArray(simplexPhase1Matrix,initialMatrixRows, initialMatrixColumns);
	//solve phase 1 matrix 
	//printf("%s\n","adafds" );
	double ** solvedPhases1Matrx = solveSimplexPhases1Matrix(simplexPhase1Matrix,initialMatrixRows, initialMatrixColumns,numberAV);
	//printf("dafsafgs");
	//format phase 1 matrix 
	if(solvedPhases1Matrx != NULL){
		double ** NormalSimplexMatrix = formatPhases1Matrix(solvedPhases1Matrx, initialMatrixRows,initialMatrixColumns,numberAV);
		printf("after NormalSimplexMatrix");
		printfAllDataInArray(NormalSimplexMatrix,initialMatrixRows-1, initialMatrixColumns-numberAV);
		return NormalSimplexMatrix;
	}else{
		return NULL;
	}
	
}
double ** initialSimplexPhases1Matrix(double ** matrix,int rows,int columns){
	// there exist some cases :
	//  <= b1 b1 greater than 0
	//  >= b2 b2 less than 0 then reverse and add one artificial variable 
	//  =  b3 add one artificial variable
	int i = 0, j = 0;
	int numberAV = getArtificialVariableNumber(matrix, rows, columns);
	int initialMatrixRows = rows + 1; //add one new row for U
	int initialMatrixColumns = columns - 2 + rows + numberAV + 2;// matrix with artificial variable 
	//create new matrix
	//printf(" the rows is %d and the columns is %d\n",initialMatrixRows,initialMatrixColumns );
	double ** initalMatrixWithArtificialVariable = malloc((initialMatrixRows+initialMatrixColumns)*sizeof(double));
	double * rowData ;
	for( i = 0; i<initialMatrixRows;i++){
		rowData = malloc(initialMatrixColumns*sizeof(double));
		*(initalMatrixWithArtificialVariable+i) = rowData; //new double [columnNumber];
	}
	for(i = 0;i<initialMatrixRows;i++)
		for(j=0;j<initialMatrixColumns;j++)
			*(initalMatrixWithArtificialVariable[i]+j) = 0;

	//initialize the matrix;
	for(i = 0 ; i < rows; i++){
		
		// the structure is :
		// feasiable variable + variable + slack + artificial variable + right + b/a

		//initial the variables matrix  
		for( j = 1 ; j < columns-2 + 1 ; j++){
			if( i == rows - 1){
				*(initalMatrixWithArtificialVariable[i]+j) = *(matrix[i]+j);//for target rows
				*(initalMatrixWithArtificialVariable[i]) = *(matrix[i]);
			}else{
				*(initalMatrixWithArtificialVariable[i]+j) = *(matrix[i]+j-1);
			}
			
		}
		//initial the slack for each row;
		for(; j < columns - 2 + 1 + rows-1 ; j++){
			if(j != columns -2 + 1 + i ){
				*(initalMatrixWithArtificialVariable[i]+j) = 0;
			}else if(*(matrix[i]+columns-2) == 3 ){
				*(initalMatrixWithArtificialVariable[i]+j) = 1;
			}else if(*(matrix[i]+columns-2) == 5){
				*(initalMatrixWithArtificialVariable[i]+j) = -1;
			}
		}
		//initial the artificial variable
		for(; j < columns - 2 + 1 +rows -1 + numberAV;j++ ){
			if(*(initalMatrixWithArtificialVariable[i]+j - (rows -1)) < 0 ){
				*(initalMatrixWithArtificialVariable[i]+j) = 1;// initial artificial value
				*(initalMatrixWithArtificialVariable[initialMatrixRows-1]+j) = -1;//initial U line 
			}else{
				*(initalMatrixWithArtificialVariable[i]+j) = 0;
			}
		}
		//initial the result part 
		*(initalMatrixWithArtificialVariable[i]+j) = *(matrix[i]+columns-1);

	}
	
	//should initial  the first columns to identify base;
	//? how to define the base variable? current take the last row - 1
	for(i = 0 ; i < initialMatrixRows-2;i++){
		*(initalMatrixWithArtificialVariable[i]) = initialMatrixColumns-2-numberAV - 1+i; 
	}

	printf("\nthe initial first step of simplex is :\n");
	printfAllDataInArray(initalMatrixWithArtificialVariable, initialMatrixRows,initialMatrixColumns);
	//sovle the artificial variable matrix with the rows number and the columns number 

	return initalMatrixWithArtificialVariable;
}
// get artificial variable number from initial matrix
int getArtificialVariableNumber(double ** matrix, int rows, int columns){
	int i = 0 , number = 0;
	for(i = 0 ;i<rows-1;i++){
		if(*(matrix[i]+columns-2)== 5.0){
			number++;
		}
	}
	return number;
}
//target all weight of non-base variables are negative or null in last line 
double ** solveSimplexPhases1Matrix(double ** matrix, int rows, int columns, int numberAV){
	//printf("the rows is %d, the columns is %d",rows,columns);
	//printf("dafsafgs");
	//solve matrix with simplex 
	//solveFirstStepSimplexMatrix(matrix, rows, columns,numberAV);
	//if the result is not valid then return null

	double  * baseVariable= getBaseVariable(matrix,rows,columns);
	//printf("%s\n","after" );
	//express U using non-base variable 
	int i = 0 , j = 0;
	//loop all the artificial variable , so that all the result of U is express by non-base variable 
	int StartIndexOfAV = columns - 2 - numberAV;
	int endIndexOfAV = columns - 2;
	for( i = StartIndexOfAV ; i < endIndexOfAV ; i++){
		int nonZeroRow = -1;
		for(j = 0; j< rows - 1 ; j++){
			if(*(matrix[j]+i) == 1.0){
				nonZeroRow = j;
			}
		}
		for(j = 1 ; j < columns - 1; j++){
			*(matrix[rows - 1]+j) += *(matrix[nonZeroRow]+j);
		}
	}
	//printf("%s\n","after add the variable" );
	//printfAllDataInArray(matrix,rows,columns);

	// all weight of non-base variables are negative or null in U.solve this with simplex
	 for( i = 0 ;i<columns-1;i++){
		if ((i < endIndexOfAV)&&(*(matrix[rows-1]+i) > 0))
		{//if there exist one positive value , then recalculate the matrix
			//how to choose?calculate 
			calculateBA(i,rows,columns,matrix);
			//printf("\n operation on the rows %d columns is %d %s\n",rows,i,"after calculate BA" );
			//printfAllDataInArray(matrix,rows,columns);
			int baseRow = getMinBAIndex(matrix,columns,rows);
			//printf("%s %d\n","the min BA index is :" , baseRow);
			calculateMatrixByOptiParameter(rows,columns,baseRow,i,matrix);
            //printf("%s the step %d\n","after calculate" ,i);
            //printfAllDataInArray(matrix,rows,columns);
		}
	}
	//??how to change base and determine that the result can't be reached 
	printf("after sovlved");
    printfAllDataInArray(matrix,rows,columns);
	return matrix;
}
// get base variable array from the matrix
double * getBaseVariable(double ** matrix, int rows, int columns){
	double * baseVariable = malloc((rows - 2) * sizeof(double));
	int i = 0 ;
	//printf("the number of the array is  %d\n",rows - 2 );
	for(i = 0; i< rows - 1 ; i++){
		*(baseVariable+i) = *(matrix[i]);
		//printf(" the base variable is %f the row is %d\n",*(matrix[i]),i );

	}

	return baseVariable;
}

double ** formatPhases1Matrix(double ** matrix, int rows, int columns , int numberAV){
	if(matrix != NULL){
		double ** matrixWithoutAV = malloc((rows - 1 + columns - 1)*(sizeof(double)));
		double * rowData ;
		int i = 0 , j = 0; 

		for( i = 0; i<rows;i++){
			rowData = malloc(columns*sizeof(double));
			*(matrixWithoutAV+i) = rowData; //new double [columnNumber];
		}
		for(i = 0;i<rows;i++)
			for(j=0;j<columns;j++)
				*(matrixWithoutAV[i]+j) = 0;
		
		for( i =0; i < rows - 1 ; i++){
			for(j = 0 ; j< columns ; j++){
				if(j<columns - 2 - numberAV){
					*(matrixWithoutAV[i] +j ) = *(matrix[i]+j);
				}else if( j >=  columns - 2 ){
					*(matrixWithoutAV[i] +j - numberAV ) = *(matrix[i]+j);
				}else{
					// do nothing 
				}
			}
		}

		return matrixWithoutAV ;
	}else{
		return NULL;
	}

}

//phase 2 . all the inequallent is less than 
//columns equals to the number of variable plus one sign and one result.
double ** NormalSimplex(double ** matrix, int rows,int columns){
	//table method 
	int i = 0 , j = 0;
	int columnsNumber = (columns - 2)+(rows-1)+3;
	double ** SimplexMatrix = malloc((rows+columnsNumber)*sizeof(double));
	double * rowData ;
	for( i = 0; i<rows;i++){
		rowData = malloc(columnsNumber*sizeof(double));
		*(SimplexMatrix+i) = rowData; //new double [columnNumber];
	}

	//initialzation
	
	for(i = 0;i<rows;i++)
		for(j=0;j<columnsNumber;j++)
			*(SimplexMatrix[i]+j) = 0;
	
	for(i =0;i<rows;i++){
		if(i == rows -1){
			//printf("the length is %d and %d\n",columnsNumber,rows );
			for(j=0;j<columns;j++){
				*(SimplexMatrix[i]+j) = *(matrix[i]+j);
			}
			for(;j<(columns - 2 + rows - 1);j++){//initialzie the slack;
				*(SimplexMatrix[i]+j) = 0;
			}
		}else{
			//printf("the position is %d and %d %d  , %f\n",i,j,columns-2 +i ,*(SimplexMatrix[i]));
			*(SimplexMatrix[i]) = (columns-2)+i;//initialize the left columns
			//printf("the position is %d and %d  columns :%d\n",i,j ,columns-2+j);
			for(j = 1 ; j<columns-1;j++) {//initiallize the factor of variable
				*(SimplexMatrix[i]+j) = *(matrix[i]+j-1);
			}
			for(;j<(columns - 2 + rows - 1);j++){//initialzie the slack;
				if(j == (columns-1+i)){
					*(SimplexMatrix[i]+j) = 1 ;
				}else{
					*(SimplexMatrix[i]+j) = 0 ;
				}
			}
			*(SimplexMatrix[i]+columnsNumber-2) = *(matrix[i]+columns - 1 );
			*(SimplexMatrix[i]+columnsNumber-1) = 0;

		}
		
	}

	return SimplexMatrix;
}
//calculate step by step to get the result
void solveNormalSimplex(double ** matrix, int rows, int columns){
	//check the last line to ensure that there doesn't exist one positive value 
	int i = 1;
	for(;i<columns-1;i++){
		if (*(matrix[rows-1]+i) > 0)
		{//if there exist one positive value , then recalculate the matrix
			//how to choose?calculate 
			calculateBA(i,rows,columns,matrix);
			//printf("\n operation on the rows %d columns is %d %s\n",rows,i,"after calculate BA" );
			//printfAllDataInArray(matrix,rows,columns);
			int baseRow = getMinBAIndex(matrix,columns,rows);
			//printf("%s %d\n","the min BA index is :" , baseRow);
			calculateMatrixByOptiParameter(rows,columns,baseRow,i,matrix);
            //printf("%s the step %d\n","after calculate" ,i);
            //printfAllDataInArray(matrix,rows,columns);
		}
	}
	double result  = -(*(matrix[rows-1] +columns-2));
	printf("find the opitmized result is : %f \n\n", result);
	printf("%s", "the final matrix is ");
	//printf("%s\n","after calculate" );
	printfAllDataInArray(matrix,rows,columns);
}
void calculateBA(int index,int rows,int columns,double  ** matrix){
	int i = 0 ;
	for(i=0;i<rows-1;i++){
		if(*(matrix[i]+index) == 0.0 || *(matrix[i]+index) < 0){
			*(matrix[i]+columns-1) = -1.0;

		}else{
			*(matrix[i]+columns-1) = *(matrix[i]+columns-2) / *(matrix[i]+index);
		}
	}
}
int getMinBAIndex(double ** matrix, int columns,int rows){
	int  index = -1, i = 0;
	for(i = 0; i < rows - 1 ;i++){
		if( ( *(matrix[i]+columns-1) > 0.0  )){
			index = i;
		}
	}
	//printf("%s %d\n","current index is :",index );
	for(i = 0; i < rows - 1 ;i++){
		//printf("\nthe line %d value is %f the base value is %f  the value of index is %d",i,*(matrix[i]+columns-1),*(matrix[i]+index),index);
		if ( ( *(matrix[i]+columns-1) > 0.0  ) &&( *(matrix[i]+columns-1) <= *(matrix[index]+columns-1))){
			index = i;
		}
	}
	return index;
}
//calculate the matrix 
void calculateMatrixByOptiParameter(int rows,int columns,int baseRow,int baseColumn,double ** matrix){

	//update the first columns
	*(matrix[baseRow]) = baseColumn;

	double factorArray[rows];
	int i = 0;
	double factor;
	factor = 0;
	for(i = 0 ; i< rows ;i++){
		//printf(" current rows :%d base rows  :%d  row data is %f \n", i,baseRow,*(matrix[i]+baseColumn));
		//calculate the factor except base row 
		//printf("\n%s %d\n","rows number is :",i );
		factor = 0;
		if (i!=baseRow){
			//printf(" current rows :%d base rows  :%d  %f \n", i,baseRow,*(matrix[i]+baseColumn));
			//calculate the factor 
			factor = 0;
			if(*(matrix[baseRow]+baseColumn) == 0.0){
				factor = 0;
			}else{
				//find the factor
				factor = - (*(matrix[i]+baseColumn))/(*(matrix[baseRow]+baseColumn));
			}
			//printf(" the factor is %f",factor);
			calculateMatrixLineByParameter(matrix,factor,columns,i,baseRow);
			//calculateMatrixLineByParameter(*(matrix[i]),factor,columns);
			
		}
	}
	
	//calculate the factor for the base row;
	
	if(*(matrix[baseRow]+baseColumn)  != 1.0 && *(matrix[baseRow]+baseColumn) != 0.0)
		factor = (1-*(matrix[baseRow]+baseColumn))/(*(matrix[baseRow]+baseColumn));
	else {
		factor = 0;
		//printf(" the factor is %f",factor);
		calculateMatrixLineByParameter(matrix,factor,columns,i,baseRow);
			//calculateMatrixLineByParameter(*(matrix[i]),factor,columns);
	}
}
//calculate every line.
void calculateMatrixLineByParameter(double ** matrix,double factor,int columns,int targetRow,int baseRow){
	if(factor != 0.0){
		int i = 1;
		for(i = 1;i<columns-1;i++){
			*(matrix[targetRow]+i) +=  factor *(*(matrix[baseRow]+i));
			//printf("the factor is %f and the result is %f\n",factor,factor*(*(matrix[baseRow]+i)) );
		}
	}
}



// binary tree

/*
typedef struct Tree{
    int deep; //deep
    double v0;
    double v1;
    struct Tree *left;
    struct Tree *right;
    
    
}Tree;


void CreatTree(Tree * T){
    int ch;
    
    scanf("%d",&ch);
    if (ch == ' ') {
        T = NULL;
    }
    else{
    T = (Tree *)malloc(sizeof(Tree));
        T->deep = 0;
        T->v0 = varibale0[0][0];
        T->v1 = varibale1[0][0];
        CreatTree(T->left);
        CreatTree(T->right);
    }

}

void PreOrder(Tree *T){
    if(T){
        PreOrder(T->left);
        PreOrder(T->right);
    
    }
    
}

 */



double a[2<<98];

//double getS(int s){
//    return varibale[N][M];
//}



//calculateBinary(formatData,realRows,realColumns)


void calculateBinary(double ** matrix, int rows, int columns){
	int numberOfVariable = columns - 2;
    int numberOfInequality = rows - 1;
    

    
    printfAllDataInArray(matrix, rows, columns);
 
    printf("colums = %d rows = %d\n",columns,rows);
    printf("numberOfVariable = %d numberOfInequality = %d\n",numberOfVariable,numberOfInequality);
    
    
    int r,n = 0;
    //stock values : parameters multiply by (variable = 0 or 1);

    //get all of values possible
    
    for (r = 0; r <= numberOfInequality; r++) {
        for (n = 0; n < numberOfVariable; n++) {
            varibale[r][n][0] = 0 * matrix[r][n];
            varibale[r][n][1] = 1 * matrix[r][n];
//            printf("%f \n",matrix[r][n]);
//            printf("%f  %f \n",varibale[r][n][0],varibale[r][n][1]);
        }
    }
    
    for (r = 0; r < numberOfInequality; r++) {
        for (n = numberOfVariable; n < columns; n++) {
            varibale[r][n][0] = 0 ;
            varibale[r][n][1] = 0 ;
        }
    }
    
    
    
    int i = 0,j,m=0,s;
    int x[11]={0};
    double sum[2<<10] = {0};
   


//for (x[m]=0; x[m]<=1; x[m]++,m++) {
//    
//    if (m == 2) {
//        i=pow(2, m);
//        sum[i] = varibale[0][0][x[0]] + varibale[0][1][x[1]];;
//    }
//    for (x[m]=0; x[m]<=1; x[m]++,m++) {
//        if (m==2) {
//            i=pow(2, m);
//            sum[i] = varibale[0][0][x[0]] + varibale[0][1][x[1]];
//        }
//
//        
//
//        }
//    }
    
//以下两层运行正常
  
//    for (x[0]=0; x[0]<=1; x[0]++) {
//        

//        for (x[1]=0; x[1]<=1; x[1]++,i++) {
//            sum[i] = varibale[0][0][x[0]] + varibale[0][1][x[1]];
//            
//            
//        }
//    }
//    
    
    

    


 
// sum of each line equality
//多层求和有问题
    for (x[0]=0; x[0]<=1; x[0]++) {
        for (x[1]=0; x[1]<=1; x[1]++) {
            for (x[2]=0; x[2]<=1; x[2]++) {
                for (x[3]=0; x[3]<=1; x[3]++) {
                    for (x[4]=0; x[4]<=1; x[4]++) {
                        for (x[5]=0; x[5]<=1; x[5]++) {
                            for (x[6]=0; x[6]<=1; x[6]++) {
                                for (x[7]=0; x[7]<=1; x[7]++) {
                                    for (x[8]=0; x[8]<=1; x[8]++) {
                                        for (x[9]=0; x[9]<=1; x[9]++,i++) {
                                            for(j=0; j<numberOfVariable; j++){
                                            
                                            sum[i] = sum[i] + varibale[0][j][x[j]];
                                                
                                            }
                                            
                                            
                                        }
                                        
                                    }
                                    
                                }
                                
                            }
                            
                        }
                        
                    }
                    
                }
                
            }
            
        }
    }
    

    
    
    
//                                        sum[i] = varibale[0][0][x[0]] + varibale[0][1][x[1]] + varibale[0][2][x[2]]+ varibale[0][3][x[3]] + varibale[0][4][x[4]] + varibale[0][5][x[5]] + varibale[0][6][x[6]] + varibale[0][7][x[7]] + varibale[0][8][x[8]] + varibale[0][9][x[9]] ;
    
   
//     sum[i] = varibale[0][0][x1] + varibale[0][0][x2] + varibale[0][0][x3]+ varibale[0][0][x4] + varibale[0][0][x5] + varibale[0][0][x6] + varibale[0][0][x7] + varibale[0][0][x8] + varibale[0][0][x9] + varibale[0][0][x10] ;
    

    
    
    
    
    int k=0;
    for (k=0; k < pow(2,numberOfVariable); k++) {
        printf("%f\n",sum[k]);
    }
    
    //calculate
    //getVaribale_01(varibale0);





    

}
