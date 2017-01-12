#include  "stdio.h"
#include "stdlib.h"
#include "string.h"
//#include "math.h"
typedef enum { false, true } bool;//define the bool type variable 
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



// calculate router 
void calculateByType(char);

//1 simplex module;
void calculateSimplex(double ** ,int ,int );
int getSimplexType(double **, int ,int ); 
	//simplex phases 1
double ** SimplyToNormalSimplexMatrix(double **, int ,int );
double ** initialSimplexPhases1Matrix(double ** ,int ,int );
int getArtificialVariableNumber(double ** , int , int );
double * getBaseVariable(double ** , int , int );
double ** solveSimplexPhases1Matrix();
double ** formatPhases1Matrix();
	//normal simplex
double ** NormalizeSimplexMatrix(double ** , int ,int );
void solveNormalSimplex(double ** ,int , int );
	//functions to solve normal simplex
void calculateBA(int ,int ,int ,double  ** );//calculate B/A to find the index to operate
int getMinBAIndex(double ** , int ,int );// find the min positive value index in B/A;
void calculateMatrixByOptiParameter(int ,int ,int ,int ,double ** );//based on the calculate calculate matrix;
void calculateMatrixLineByParameter(double ** ,double ,int ,int ,int );// calculate each line for matrix

//2 cutting and spliting 


// 3 olve binary tree
void calculateBinary(double ** , int , int );
struct node * constructBinaryTrees(int ,int ,int * );
struct node * constructOneNodeWithParameters(int  ,int  , int ,int * ,int );//create tree node 
void tuningDecisionArray(int * ,int ,int , int );
void solveTheBinaryTree(struct node *  ,int, double ** , int , int );
void calculateForEachNode(struct node * ,struct node * ,int,double ** , int , int );
double calculateZValueByDecision(int * ,int,double ** , int , int );
double * calculateThetaValueByDecision(int * ,int,int , double ** , int , int );
void findOptimizedNodeFromTree(struct node *,struct node * ,struct node * );
bool checkFeasible(struct node *  );

// tool functions 
void ArrayPointerCopy(int * , int * ,int);// array pointer copy used at binary tree
void printfAllDataInArray(double ** ,int ,int);
//void printfBinaryTree(struct node * );
void printfIntArray(int *,int );
void printfDoubleArray(double *,int);
void printfBinaryNode(struct node *);
void printfBinaryTree(struct node *);


//node for binary tree
struct node
{
	int * decisionArray;// the array than contains the choice of decision
	int numberOfVariable;//should equal to  deepth because the deep of root is 0 and it's leat node means the first variable equal to 0 and so on
	int deep;
	double z;
	double  * theta;// how many inequations.
	int numberOfInequations;
	struct node * left;
	struct node * right;
};

int main(){

	//initalization arrays, suppose if there is no value then with -300.99
	char * path = "/Users/yuanping/Documents/code/xcode/OptimizeMath/OptimizeMath/math.txt";

	//here 
    //char * path = "/Users/yuanping/Documents/code/xcode/OptimizeMath/OptimizeMath/binary.txt";
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
	typeOfOperation = '1';

	//calculate the result :
	calculateByType(typeOfOperation);
    
	//output result here or output during the calculate progress
	//printf("the result is : \n");
	return 0;

}
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
	char* optimizedType = *(matrix[rows-1])==1?"min":"max";
	int type = getSimplexType(matrix,rows,columns);
	double ** simplexMatrix ; 
	printf("type of simplex is %d \n",type );
	if(type == 2){
		printf("%s\n","this is two phases simplex" );
		simplexMatrix = SimplyToNormalSimplexMatrix(matrix, rows, columns);
	}else{
		printf("%s\n","this is normal simplex" );
		simplexMatrix = NormalizeSimplexMatrix(matrix,rows,columns);
	}
	
	if(simplexMatrix != NULL){
		int NormalSimplexRows = rows;
		int NormalSimplexColumns = (columns - 2) +(rows - 1) +3;
		printf("your optimizedType is :%s\n",optimizedType );
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
double **  SimplyToNormalSimplexMatrix(double ** matrix, int rows, int columns){
	
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
//initial matrix for the first step simplex
double ** initialSimplexPhases1Matrix(double ** matrix,int rows,int columns){
	// there exist some cases :
	//  <= b1 b1 greater than 0
	//  >= b2 b2 less than 0 then reverse and add one artificial variable 
	//  =  b3 add one artificial variable
	int i = 0, j = 0 , typeFactor;
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
			typeFactor = 1; 
			if(*(matrix[i]) == 1 ){
				typeFactor = -1;
			}
			if( i == rows - 1){
				*(initalMatrixWithArtificialVariable[i]+j) = *(matrix[i]+j) * typeFactor;//for target rows
				*(initalMatrixWithArtificialVariable[i]) = 2; // always convert the max
			}else{
				*(initalMatrixWithArtificialVariable[i]+j) = *(matrix[i]+j-1);
			}
			
		}
		//initial the slack for each row;
		for(; j < columns - 2 + 1 + rows-1 ; j++){
			if(j != columns -2 + 1 + i ){ 
				*(initalMatrixWithArtificialVariable[i]+j) = 0;
			}else if(*(matrix[i]+columns-2) == 3 ){ //<= 
				*(initalMatrixWithArtificialVariable[i]+j) = 1;
			}else if(*(matrix[i]+columns-2) == 5){  // >=
				*(initalMatrixWithArtificialVariable[i]+j) = -1;
			}else{ // =
				*(initalMatrixWithArtificialVariable[i]+j) = 0;
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
// deal with the result matrix from the first phases and convert into normal simplex matrix
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
double ** NormalizeSimplexMatrix(double ** matrix, int rows,int columns){
	//table method 
	int i = 0 , j = 0 , factor;
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
			factor = 1;
			if(*(matrix[i]) == 1){
				factor = -1;
			}
			for(j=1;j<columns;j++){
				*(SimplexMatrix[i]+j) = *(matrix[i]+j) * factor;
				*(SimplexMatrix[i]) = 2;
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
	printf("\nfind the opitmized result is : %f \n\n", result);
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

void calculateBinary(double ** matrix, int rows, int columns){
	int numberOfVariable = columns - 2; //can be used to initial the choice array;
	int numberOfInequations = rows - 1;
	int * initialDecisionArray= malloc(numberOfVariable*(sizeof(int)));
	// initial the arrays with -1
	int i = 0;
	for(;i < numberOfVariable;i++){
		*(initialDecisionArray+i) =  -1;
	}
	//construct the binary tree
	struct node * root = constructBinaryTrees(numberOfVariable,numberOfInequations,initialDecisionArray);
	//printf("the root element ? is \n");
	//printfBinaryNode(root);
	solveTheBinaryTree(root, numberOfVariable,matrix,rows,columns);

}

struct node * constructBinaryTrees(int numberOfVariable,int numberOfInequations,int * path){
	struct node * root  = constructOneNodeWithParameters(0,numberOfVariable,numberOfInequations,path,-1);

	return root;
}
// when path the path should pass a new pointer so that we can directly assign to the data 
// if the node is left then the position should be 0 otherwise should be 1
struct node * constructOneNodeWithParameters(int deep ,int numberOfVariable , int numberOfInequations,int * pathOfParent,int positionOfNode){
	//printf("the number of variable is %d the deep is %d",numberOfVariable,deep);

	//printf("%s\n","" );
	if(deep <= numberOfVariable ){
		int * currentPath = malloc(numberOfVariable*sizeof(int));

		// create a new array fromt the old decision array
		ArrayPointerCopy(currentPath,pathOfParent,numberOfVariable);
		// reconstruct the decisionArray
		tuningDecisionArray(currentPath,numberOfVariable,deep,positionOfNode);

		//construct the new node
		struct node * currentNode = (struct node*)malloc(sizeof(struct node));
		currentNode->decisionArray = currentPath;
		currentNode->numberOfVariable = numberOfVariable;
		currentNode->deep = deep;
		currentNode->z = 0;
		currentNode->theta = malloc(numberOfInequations*sizeof(double));// how many inequations.
		currentNode->numberOfInequations = numberOfInequations;
		currentNode->left = constructOneNodeWithParameters(deep + 1 ,numberOfVariable, numberOfInequations, currentPath,0);
		currentNode->right = constructOneNodeWithParameters(deep + 1 ,numberOfVariable, numberOfInequations, currentPath,1);

		return currentNode;
	}else{
		return NULL;
	}
}
void tuningDecisionArray(int * currentPath,int numberOfVariable,int deep, int positionOfNode){
	int i = 0;
	for(i = deep ; i < numberOfVariable ; i++){
		*(currentPath+i) = -1;
	}
	if(deep > 0 ){ // if not the root node; if the root node do nothing for the current path
		if(positionOfNode == 0){
			*(currentPath+deep - 1 ) = 0;
				//printf("the index is %d, the  value is %d\n", deep ,*(currentPath+deep - 1 ));
		}else {
			*(currentPath+deep - 1 ) = 1;
				//printf("the index is %d, the  value is %d\n", deep ,*(currentPath+deep - 1 ));
		}
	}
	//*(intArray + deep -1 )
}
void solveTheBinaryTree(struct node * root ,int numberOfVariable, double ** matrix, int rows, int columns){
	
	calculateForEachNode(NULL,root,numberOfVariable, matrix,rows,columns);
	
	struct node * maxNode  = malloc(sizeof(struct node));
	maxNode->z = 0;
	//printfBinaryNode(root);

	//printfBinaryNode(maxNode);
	//printfBinaryTree(root);
	//tail recurision
	findOptimizedNodeFromTree(NULL,root,maxNode);
	//print the result 
	
	printf("the optimized result is ：\nthe value for the variable is :\n");
	printfIntArray(maxNode->decisionArray,numberOfVariable);
	printf("\nthe max z  is %f\n",maxNode->z);
}

void calculateForEachNode(struct node * pathOfParent,struct node * currentNode,int numberOfVariable,double ** matrix, int rows, int columns){
	// calculate current node
	if(pathOfParent != NULL){
		//calcuate the z and the theta variable 

		//calculate the z value :
		currentNode->z = calculateZValueByDecision(currentNode->decisionArray,numberOfVariable,matrix,rows,columns);

		// calculate the theta arrays;
		currentNode->theta = calculateThetaValueByDecision(currentNode->decisionArray,numberOfVariable,currentNode->numberOfInequations,matrix,rows,columns);

	}else{
		//this is root node 
		//calculate the z value :
		currentNode->z = 0;//calculateZValueByDecision(currentNode->decisionArray,numberOfVariable,matrix,rows,columns);

		// calculate the theta arrays;
		currentNode->theta = calculateThetaValueByDecision(currentNode->decisionArray,numberOfVariable,currentNode->numberOfInequations,matrix,rows,columns);
	}


	if(currentNode->left != NULL){
		//printf("left\n");
		calculateForEachNode(currentNode,currentNode->left,numberOfVariable,matrix,rows,columns);
	}

	if(currentNode->right != NULL){
		//printf("right\n");
		calculateForEachNode(currentNode,currentNode->right,numberOfVariable,matrix,rows,columns);
	}
}
double calculateZValueByDecision(int * decisionArray,int numberOfVariable, double ** matrix, int rows, int columns){
	double z = 0;
	int i = 0 , j = 0;

	//initial z
	for( i = 1;i < columns-1 ;i++){

		if(*(matrix[rows-1] + i ) > 0){
			z += *(matrix[rows-1]+i);
		}
	}
	i = 0; j = 1;
	for(;i < numberOfVariable;i++){
		if(*(decisionArray + i ) != -1 && *(decisionArray + i) == 0){
			if(*(matrix[rows-1]+j)>0){
				z -= *(matrix[rows-1]+j);
			}

		}else if(*(decisionArray + i ) != -1 && *(decisionArray + i) == 1){
			// if decision  == 1 and the factor is less than 0
			if(*(matrix[rows-1]+j) < 0){
				z += *(matrix[rows-1]+j);
			}
		}
		
		j++;
	}
	
	return z;

}
double * calculateThetaValueByDecision(int * decisionArray,int numberOfVariable,int numberOfInequations, double ** matrix, int rows, int columns){
	double * theta = malloc(numberOfInequations *sizeof(double));
	int i = 0 , j = 0 ;
	// initial 
	for(i = 0 ; i < numberOfInequations ; i++)
		*(theta+i) = *(matrix[i]+columns-1);
	for(i = 0; i< rows-1;i++){
		for( j = 0 ; j< numberOfVariable ;j++ ){
			if(*(matrix[i]+j) < 0){
				//printf("\nexist the negative? \n");
				*(theta+i) -= *(matrix[i]+j);
			}
		}
	}

	for(i = 0 ; i < numberOfInequations; i++){
		
		for( j = 0; j < numberOfVariable ; j++){
			if(*(decisionArray + j) != -1 && *(decisionArray + j) == 0){
				if(*(matrix[i]+j)<=0){
					*(theta+i) += *(matrix[i]+j);
				}
			}else if(*(decisionArray + j ) != -1 && *(decisionArray + j) == 1){
				if(*(matrix[i]+j) > 0){
					*(theta+i) -= *(matrix[i]+j);
				}
			}
		}
		
	}

	return theta;

}
//central left right. just need to check the leaf node.
void findOptimizedNodeFromTree(struct node * parentNode,struct node * currentNode,struct node * maxNode){
	/*
	printf("\nthe z in current node is %f, the current node is :\n",node->z);
	printfIntArray(node->decisionArray,node->numberOfVariable);
	printf("\n");
	*/
	
	if((currentNode->left == NULL) &&(currentNode->right == NULL) && (currentNode->z >= maxNode->z)&&checkFeasible(currentNode)){
		*maxNode = *currentNode;
	}
	
	
	if(currentNode->right != NULL){
		findOptimizedNodeFromTree(currentNode,currentNode->right,maxNode);
		
	}


	if(currentNode->left != NULL){
		findOptimizedNodeFromTree(currentNode,currentNode->left,maxNode);
	}

}
// if all variable in theta is positive then return true;
bool checkFeasible(struct node * node ){
	bool result = true;
	int i = 0;
	for(;i < node->numberOfInequations;i++){
		if(*((node->theta)+i) < 0){
			result = false;
		}
	}
	return result;
}
void ArrayPointerCopy(int * newArray, int * oldArray,int length){
	int i = 0;
	for(;i < length ; i++){
		*(newArray+i) = *(oldArray+i);
	}
}
void printfIntArray(int * intArray,int length){
	//printf("%s\n"," the path is :" );
	int i = 0; 
	
	for(;i<length;i++){
		printf("%d",*(intArray+i) );
	}
}
void printfDoubleArray(double * doubleArray,int length){
	//printf("%s\n"," the path is :" );
	int i = 0; 
	
	for(;i<length;i++){
		printf("%f ",*(doubleArray+i) );
	}
	
}
void printfBinaryTree(struct node * node){
	//struct node * indexPoint = root;
	printf("the path of this node is  :\n");
	printfIntArray(node->decisionArray,node->numberOfVariable);
	printf("the deep of this node is %d\n",node->deep );
	printf("the z value of this node is %f\n",node->z );
	printf("the theta array is :\n");
	printfDoubleArray(node->theta,2);
	printf("\n\n");
	if(node->left != NULL){
		printfBinaryNode(node->left);
	}

	if(node->right != NULL){
		printfBinaryNode(node->right);
	}
	
}

void printfBinaryNode(struct node * node){
	//struct node * indexPoint = root;
	printf("the path of this node is  :\n");
	printfIntArray(node->decisionArray,node->numberOfVariable);
	printf("the deep of this node is %d\n",node->deep );
	printf("the z value of this node is %f\n",node->z );
	printf("the theta array is :\n");
	printfDoubleArray(node->theta,2);
	printf("\n\n");

	
}
