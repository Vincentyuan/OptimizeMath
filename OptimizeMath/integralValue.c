// contains the split method and convert to binary tree solutions 
//#include "stdio.h"
//
struct splitNode
{
	double ** matrix;
	int rows;
	int columns;
	int nonIntegralRow;
	int nonIntegralVariable;
	double z;
	bool isAllIntegral;
	struct splitNode * left ;
	struct splitNode * right ;
};
struct MatrixNode {
	double ** matrix;
	int rows;
	int columns;
};


void calculateSplit(double ** , int , int );
struct splitNode * calculateSplitTree(struct splitNode * ,
	struct MatrixNode * , struct MatrixNode * ,int );
struct MatrixNode * getNewSimplexMatrixConstraint(struct MatrixNode * , struct splitNode* ,int );
struct splitNode* getSplitNodeFromSimplexResultMatrix(double ** , int , int ,struct MatrixNode *);
void findIntegralOptimizedNodeFromTree(struct splitNode *  ,struct splitNode * );
bool isValidBranch(struct MatrixNode * ,struct splitNode* ,int );
int getFirstNonIntegralIndex(double ** , int , int ,struct MatrixNode * );
double getFirstNonIntegralValue(double ** , int  ,int );
double getZ(double ** , int , int );
bool isAllIntegral(double ** , int  , int );
bool isIntegral(double);
void printfSplitMethodResult(struct splitNode *,int);
void printfSplitNode(struct splitNode * );


// construct a tree untill all the method that all branch get the integral value 
void calculateSplit(double ** matrix, int rows, int columns){
	//printf("\n\nsdafsdfasdfasdfffaaaddddddddddddddddddd\n\n");
	struct MatrixNode *  originalMatrix = malloc(sizeof(struct MatrixNode));
	originalMatrix->matrix = matrix;
	originalMatrix->rows = rows;
	originalMatrix->columns = columns;
	
	double ** afterSimplex = calculateSimplex(matrix,rows,columns);

	int ResultRows = rows;
	int ResultColumns = (columns - 2)+(rows-1)+3;
	struct splitNode * integralInitalNode = getSplitNodeFromSimplexResultMatrix(afterSimplex,ResultRows,ResultColumns,originalMatrix);
	//printf("the after calcuate\n");
	//printfAllDataInArray(integralInitalNode->matrix,ResultRows,ResultColumns);
	
	//if get result 
	if(afterSimplex != NULL){
		// check variable here 
		if(integralInitalNode->isAllIntegral){
			//if yes print the solutions here 
			printfSimplexSolution(afterSimplex,ResultRows,ResultColumns, originalMatrix->columns-2);
		}else{
			//struct splitNode * optimizedNode = malloc(sizeof(struct splitNode));

			//struct splitNode * intialNode = getSplitNodeFromMatrix(matrix,rows,columns);
			struct splitNode * root = calculateSplitTree(integralInitalNode,originalMatrix,originalMatrix,-1);
			//solveSplit(initialNode, optimizedNode);
			struct splitNode *optimizedNode;
			optimizedNode->z = 0;
			//printf("to find the optimzed node \n");
			findIntegralOptimizedNodeFromTree(root, optimizedNode);

			printfSplitMethodResult(optimizedNode,originalMatrix->columns-2);
		}	
	}else{
		// no solution here 
		printf("since there is no solution for simplex, there is no solution for split \n");
	}

} 

struct splitNode * calculateSplitTree(struct splitNode * initialNode,
	struct MatrixNode * transformMatrix, struct MatrixNode * initialMatrix,int status){

	//printfSplitNode(initialNode);
	//printf("\nthe matrix to be solved is :");
	printfAllDataInArray(transformMatrix->matrix,transformMatrix->rows,transformMatrix->columns);
	//if(initialNode->matrix != NULL&&isValidBranch(initialMatrix,initialNode,status)){ // check variable still in the valid area with the inital matrix
		double ** simplexMatrix = calculateSimplex(transformMatrix->matrix,transformMatrix->rows,transformMatrix->columns);

		struct splitNode * result =  malloc(sizeof(struct splitNode));
		result->matrix = simplexMatrix;
		result->rows = transformMatrix->rows;
		result->columns = (transformMatrix->columns - 2) +(transformMatrix->rows - 1) +3;
		printfAllDataInArray(simplexMatrix,result->rows,result->columns);
		result->nonIntegralRow = getFirstNonIntegralIndex(simplexMatrix,result->rows,result->columns,initialMatrix);

		//printf("sdfasdfasdgasdafdsfa the value of non inegervariable is L \n");
		result->nonIntegralVariable = (int)*(simplexMatrix[result->nonIntegralRow]);
		//printf("\nthe non integral row is %d  the value is %d\n",result->nonIntegralRow ,result->nonIntegralVariable);
		result->z = getZ(simplexMatrix,transformMatrix->rows,transformMatrix->columns);

		result->isAllIntegral= result->nonIntegralRow < 0?true:false;

//printf("the unber of nonIntegralRow is %d\n", result->nonIntegralRow);

		if(result->nonIntegralRow > -1) {
			
			struct MatrixNode * right = getNewSimplexMatrixConstraint(transformMatrix,result,1);
			if(right != NULL)
				initialNode->right = calculateSplitTree(result,right,initialMatrix,1); 


			//printf("right finished\n");
			struct MatrixNode * left = getNewSimplexMatrixConstraint(transformMatrix,result,0);
			//printf("here ?\n");
			if(left != NULL)
				initialNode->left = calculateSplitTree(result,left,initialMatrix,0); 
			//printf("left finished\n");
			
			//printf("\ncheck where the exception is the non result is %d\n",result->nonIntegralRow );

		}else{
			initialNode->left = NULL;
			initialNode->right = NULL;
		}
		//printf("");
		printfAllDataInArray(result->matrix,result->rows,result->columns);
		return result;
	//}else{
	//	return NULL;
	//}


}
bool isValidConstraint(struct MatrixNode* initialMatrix, struct splitNode * node, int status){
	// for the variable 
	int numberOfVariableTobeHandle = node->nonIntegralVariable;
	double baseVariable = status==0? (int)*((node->matrix)[node->nonIntegralRow]+node->columns-2):(int)*((node->matrix)[node->nonIntegralRow]+node->columns-2)+1;

	//printf("the new constraint is numberOfVariableTobeHandle is %d, the operate is %d, the reuslt is %f\n",
	//numberOfVariableTobeHandle,status,baseVariable );

	int numberOfVariable = initialMatrix->columns - 2;
	int i = 0,j = 0, k = 0;
	bool isValid = true;

	for(i = 0; i< numberOfVariable ; i++){
		// loop the whole matrix to find the exist constraint;

		int rowIndexOfVariable = -1 ;
		//printf("here before loop?\n");

		for(j = 0 ; j < initialMatrix->rows-1;j++ ){
			int nonZeroNumber = 0;
			for(k = 0 ; k< initialMatrix->columns;k++){
				if(*((initialMatrix->matrix)[j]+k) != 0){
					nonZeroNumber++;
				}
			}
			//printf("the %d rows the numbe of nonZeroNumber is %d and the variable of %d row , %d column is %f\n", 
			//	i, nonZeroNumber,j,i,*((initialMatrix->matrix)[j]+i) );
			if(nonZeroNumber ==  3 && *((initialMatrix->matrix)[j]+i) == 1){
			//	printf("should get the row index of variabl %d \n",j);
				rowIndexOfVariable = j;
			}
		}
		///printf("here1 ?%d \n",rowIndexOfVariable);
		if(rowIndexOfVariable != -1&&status ==0 && *((initialMatrix->matrix)[rowIndexOfVariable]+initialMatrix->columns-2) == 5){// 
			//printf("here get\n");
			if(baseVariable <*((initialMatrix->matrix)[rowIndexOfVariable]+initialMatrix->columns-1)){
				//return false;
				isValid = false;
			}
		}
		//printf("here2 ?%d \n",rowIndexOfVariable);
		if(rowIndexOfVariable != -1&&status ==0 && *((initialMatrix->matrix)[rowIndexOfVariable]+initialMatrix->columns-2) == 3){
			if(baseVariable <*((initialMatrix->matrix)[rowIndexOfVariable]+initialMatrix->columns-1)){
				//return false;
				//return true;
				*((initialMatrix->matrix)[rowIndexOfVariable]+initialMatrix->columns-1) = baseVariable;
				//*((node->matrix)[node->nonIntegralRow]+node->columns-2)
				//return true;
			}else{
				//return false;
				isValid = false;
			}
		}
		//printf("do you come here for visit? rowIndexOfVariable %d\n",rowIndexOfVariable);
		if(rowIndexOfVariable > -1)
			//printf("the baseVariable is %f and the original value is  %f\n",baseVariable,*((initialMatrix->matrix)[rowIndexOfVariable]+initialMatrix->columns-1));
		if(rowIndexOfVariable != -1&&status ==1 && *((initialMatrix->matrix)[rowIndexOfVariable]+initialMatrix->columns-2) == 3){
			
			if(baseVariable > *((initialMatrix->matrix)[rowIndexOfVariable]+initialMatrix->columns-1)){
				//return false;
				isValid = false;
			}
		}
		if(rowIndexOfVariable != -1&&status ==1 && *((initialMatrix->matrix)[rowIndexOfVariable]+initialMatrix->columns-2) == 5){
			if(baseVariable > *((initialMatrix->matrix)[rowIndexOfVariable]+initialMatrix->columns-1)){
				//return false;
				*((initialMatrix->matrix)[rowIndexOfVariable]+initialMatrix->columns-1) = baseVariable;
				return true;
			}else{
				//return false;
				isValid = false;
			}
		}
	}

	//return isValid;
	return true;
}
// status means the upper or the lower value . 0 means the lower value , 1 means the upper value 
struct MatrixNode * getNewSimplexMatrixConstraint(struct MatrixNode * initialMatrix , struct splitNode* node,int status){
	//printf("start the new simple matr\n");
	if(initialMatrix!= NULL&&isValidConstraint(initialMatrix,node,status)){
		
		struct MatrixNode * result = malloc(sizeof(struct MatrixNode));
		result->matrix = malloc((initialMatrix->rows+1)*sizeof(double));
		result->rows = initialMatrix->rows + 1;
		result->columns = initialMatrix->columns;

		double * rowData ;
		int i = 0, j = 0;
		for( i = 0; i < result->rows ;i++){
			rowData = malloc((initialMatrix->columns+1)*sizeof(double));
			*((result->matrix)+i) = rowData; //new double [columnNumber];
		}
	//copy original constraint
		for(i = 0; i < initialMatrix->rows - 1;i++){
			for(j = 0 ; j < initialMatrix->columns;j++){
				*((result->matrix)[i]+j) = *((initialMatrix->matrix)[i]+j);
			}
			for(; j  == initialMatrix->columns;j++){
				*((result->matrix)[i]+j) = 0;
			}
			for(; j < result->columns;j++){
				*((result->matrix)[i]+j) = *((initialMatrix->matrix)[i]+j-1);
			}
		}
	// add a new constraint
		//for(; i == initialMatrix->rows - 1;i++){
		for(j = 0; j < initialMatrix->columns;j++ ){
			//initial with the non integer rows just for the x variable 
			if( j == node->nonIntegralVariable - 1){
				*((result->matrix)[i]+j) = 1;
			}else if(j == initialMatrix->columns - 2 &&status == 0){
				*((result->matrix)[i]+j) = 3;
			}else if(j == initialMatrix->columns - 2 && status == 1){
				*((result->matrix)[i]+j) = 5;
			}else if(j == initialMatrix->columns-1 && status == 1){
				//printf(" the index of non integral is %d , the right part is %f ",node->nonIntegralRow,*((node->matrix)[node->nonIntegralRow]+node->columns-1));
				*((result->matrix)[i]+j) = (int)*((node->matrix)[node->nonIntegralRow]+node->columns-2) + 1;
			}else  if(j  == initialMatrix->columns-1 && status == 0){
				*((result->matrix)[i]+j) = (int)*((node->matrix)[node->nonIntegralRow]+node->columns-2);
			}else{
				*((result->matrix)[i]+j) = 0;
			}
		}
		//}
	//add original target optimized 
		//for(; i == node->rows -1;i++){
		for(j = 0;j < node->columns;j++){
			*((result->matrix)[result->rows -1 ]+j) = *((initialMatrix->matrix)[initialMatrix->rows-1]+j);
		}
		//}

		return result;
	}else{
		//printf("new constraint null ?\n");
		return NULL;
	}

}
/*
struct splitNode* getSplitTreeFromMatrix(double ** matrix, int rows, int columns){
	
	return getSplitNode(matrix);

}

struct splitNode * getSplitNode(double ** matrix, int rows, int columns){

	if(){ // check variable still in the valid area
		struct splitNode * result =  malloc(sizeof(struct splitNode));
		result->matrix = matrix;
		result->rows = rows;
		result->columns = columns;
		result->nonIntegralRow = getFirstNonIntegralIndex(matrix,rows,columns);
		result->z = getZ(matrix,rows, columns);
		result->isAllIntegral= splitNode->nonIntegralRow < 0 ？true；false;
		if(result->nonIntegralRow > -1) {
			result->left = getSplitNode(result); 
			result->right = getSplitNode(result);
		}else{
			result->left = NULL;
			result->right = NULL;
		}
	}else{
		return NULL;
	}



}
*/
// with no following node
struct splitNode* getSplitNodeFromSimplexResultMatrix(double ** matrix, int rows, int columns,struct MatrixNode * initialMatrix){

	struct splitNode * result =  malloc(sizeof(struct splitNode));
	result->matrix = matrix;
	result->rows = rows;
	result->columns = columns;
	result->nonIntegralRow = getFirstNonIntegralIndex(matrix,rows,columns,initialMatrix);
	result->z = getZ(matrix,rows, columns);
	//printf("the non integral row is  : %d" , result->nonIntegralRow);
	result->isAllIntegral= result->nonIntegralRow < 0?true:false;

	result->left = NULL;
	result->right = NULL;

	return result;

}
void findIntegralOptimizedNodeFromTree(struct splitNode * currentNode ,struct splitNode * optimizedNode){

	if(currentNode->right != NULL){
		findIntegralOptimizedNodeFromTree(currentNode,optimizedNode);
		
	}


	if(currentNode->left != NULL){
		findIntegralOptimizedNodeFromTree(currentNode,optimizedNode);
	}

	if((currentNode->left == NULL) &&
		(currentNode->right == NULL) && 
		(currentNode->z > optimizedNode->z)&&
		currentNode->isAllIntegral){

		*optimizedNode = *currentNode;
	}

}
// as the same status = 0 means the lower bounder status = 1 means the upper bounder
bool isValidBranch(struct MatrixNode * initialMatrix,struct splitNode* currentNode,int status){
	bool result = true;

	return result;
}
/*
double  ** solveSplit(struct splitNode * node  ){
	if(!isSplitStop(node)){
		int nonIntegralIndex = getFirstNonIntegralIndex(node);
		double nonIntegralValue = getFirstNonIntegralValue(node);
		int upperValue = (int)nonIntegralValue + 1;
		int lowerValue = (int)nonIntegralValue;
		double upperOptimizedResult = solveSimplex();
		double lowerOptimizedResult = solveSimplex();

		if(upperOptimizedResult > lowerOptimizedResult){
			//run upper side 

		}

		if(upperOptimizedResult < lowerOptimizedResult){
			// run the lower side 
		}
	}
}
*/


int getFirstNonIntegralIndex(double ** matrix, int rows, int columns,struct MatrixNode * initialMatrix){
	int i = 0;
	printfAllDataInArray(matrix, rows,columns);
	for(  i = 0; i < rows - 1;i++){
		//printf(" i is %d, the number of variable is %f the value is %f the numberOfVariable is %d, is integral or not %d\n",i,
		//	*(matrix[i]), *(matrix[i] +columns - 2),initialMatrix->columns-2, isIntegral(*(matrix[i] +columns - 2)));
		if((*(matrix[i]) <= initialMatrix->columns-2)&&!isIntegral(*(matrix[i] +columns - 2))){ // if there exist one not  integer directly return false 
			//isAllIntegral = ;
		//	printf("the non integral rows in the function is %d\n",i);
			return i;
		}
	}
	return -1;
}
double getFirstNonIntegralValue(double ** matrix, int rows ,int columns){
	return *(matrix[rows]+columns - 2);
}
double getZ(double ** matrix, int rows, int columns){
	return *(matrix[rows - 1] + columns-2);
}
bool isAllIntegral(double ** matrix, int rows , int columns){
	//bool isAllIntegral = true;
	int i = 0 ;
	for(  i = 1; i < rows - 1;i++){
		if(!isIntegral(*(matrix[0] +columns - 2))) // if there exist one not  integer directly return false 
			//isAllIntegral = ;
			return false;
	}
	return true;
}
bool isIntegral(double value ){
	if( 0 == (value - (int)value)) {
			//isAllIntegral = false;
		return true;
	}else{
		return false;
	}
}
void printfSplitMethodResult(struct splitNode *optimizedNode,int numberOfVariable){
	printfSimplexSolution(optimizedNode->matrix,optimizedNode->rows, optimizedNode->columns,numberOfVariable);
}
void printfSplitNode(struct splitNode * result){
	printf("\nthe node of split is :\n");
	printfAllDataInArray(result->matrix,result->rows,result->columns);
	printf("is this a all integral matrix ? %d\n",result->isAllIntegral );
	printf("the z is  %f\n",result->z );
	printf("the nonIntegralRow is %d \n",result->nonIntegralRow);
}
