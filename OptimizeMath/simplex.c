#include "stdio.h"
double ** calculateSimplex(double ** ,int ,int );
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
double ** solveNormalSimplex(double ** ,int , int ,int);
	//functions to solve normal simplex
void calculateBA(int ,int ,int ,double  ** );//calculate B/A to find the index to operate
int getMinBAIndex(double ** , int ,int );// find the min positive value index in B/A;
void calculateMatrixByOptiParameter(int ,int ,int ,int ,double ** );//based on the calculate calculate matrix;
void calculateMatrixLineByParameter(double ** ,double ,int ,int ,int );// calculate each line for matrix

void printfAllDataInArray(double ** ,int ,int);
void printfSimplexBase(double ** ,int , int , int);
void printfSimplexSolution(double **, int ,int, int);


double ** calculateSimplex(double ** matrix,int rows, int columns){
	//printf("hello" );
	char* optimizedType = *(matrix[rows-1])==1?"min":"max";
	printf("your optimizedType is :%s\n",optimizedType );
	int type = getSimplexType(matrix,rows,columns);
	int numberOfVariable = columns-2;
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
		printf("the initial matrix here is:\n");
		printfAllDataInArray(simplexMatrix,NormalSimplexRows,NormalSimplexColumns);
		return solveNormalSimplex(simplexMatrix,NormalSimplexRows,NormalSimplexColumns,numberOfVariable);

	}else{
		printf("%s\n", "there is no solution for your equations");
		return NULL;
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
		printf("\nthe result after finished the phases 1 without artificial variable:\n");
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
		//printf("\nsthe number of the artificial columns is from %d to %d\n",columns-2 +1 +rows -1,columns-2+1+rows-1+numberAV );
		for(; j <columns - 2 + 1 +rows -1 + numberAV ;j++ ){
			//printf("\nsthe number of the artificial columns is from %d to %d\n",columns-2 +1 +rows -1,columns-2+1+rows-1+numberAV );
			if(*(initalMatrixWithArtificialVariable[i]+j - (rows - 1 - i)) < 0 ){
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
	printf("after sovlved with artificial variable");
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
			*(SimplexMatrix[i]) = (columns-1)+i;//initialize the left columns
			//printf("the position is %d and %d  columns :%d\n",i,j ,columns-2+j);
			for(j = 1 ; j<columns-1;j++) {//initiallize the factor of variable
				*(SimplexMatrix[i]+j) = *(matrix[i]+j-1);
			}
			for(;j<=(columns - 2 + rows - 1);j++){//initialzie the slack;
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
double ** solveNormalSimplex(double ** matrix, int rows, int columns,int numberOfVariable){
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
            printfAllDataInArray(matrix,rows,columns);
		}
	}
	//
	printfSimplexSolution(matrix,rows,columns,numberOfVariable);

	return matrix;
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
	
	if(*(matrix[baseRow]+baseColumn)  != 1.0 && *(matrix[baseRow]+baseColumn) != 0.0){
		factor = (1-*(matrix[baseRow]+baseColumn))/(*(matrix[baseRow]+baseColumn));
		
	}else {
		factor = 0;
		//calculateMatrixLineByParameter(matrix,factor,columns,baseRow,baseRow);
	}
	calculateMatrixLineByParameter(matrix,factor,columns,baseRow,baseRow);
	
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
void printfSimplexSolution(double ** matrix, int rows, int columns,int numberOfVariable){
	double result  = -(*(matrix[rows-1] +columns-2));
	printf("\nfind the opitmized result is : %f \n\n", result);
	printfSimplexBase(matrix,rows,columns,numberOfVariable);
	//out put the base 

	printf("\n%s", "the final matrix is ");
	//printf("%s\n","after calculate" );
	printfAllDataInArray(matrix,rows,columns);
}

void printfSimplexBase(double ** matrix,int rows, int columns,int numberOfVariable){
	printf("the base is : \n");
	int i = 0;
	printf("the numberOfVariable is %d\n",numberOfVariable );
	for(; i< rows -1 ; i++){
		if( (int) *(matrix[i]) <= numberOfVariable){
			printf("the variable x%d should be %f\n", (int) *(matrix[i]),*(matrix[i]+columns-2) );
		}else{
			printf("the variable e%d should be %f\n", (int) *(matrix[i])  - numberOfVariable,*(matrix[i]+columns-2) );
		}	
		
	}
	printf("other variable shoud be 0 \n");
	

}
