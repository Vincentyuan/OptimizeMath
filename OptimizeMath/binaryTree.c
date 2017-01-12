#include "stdio.h"
typedef enum { false, true } bool;//define the bool type variable
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


//tools
void ArrayPointerCopy(int * , int * ,int);// array pointer copy used at binary tree
void printfBinaryNode(struct node *);
void printfBinaryTree(struct node *);
void printAllFeasibleSolution(struct node *);


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
	/*
	printf("the optimized result is ：\nthe value for the variable is :\n");
	printfIntArray(maxNode->decisionArray,numberOfVariable);
	printf("\nthe max z  is %f\n",maxNode->z);
	*/
	printf("optimized finished , here is all the feasible solutions :\n");
	printAllFeasibleSolution(maxNode);

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
	// if get the greater value;
	if((currentNode->left == NULL) &&
		(currentNode->right == NULL) && 
		(currentNode->z > maxNode->z)&&
		checkFeasible(currentNode)){

		*maxNode = *currentNode;
	}else if((currentNode->left == NULL) && // if get the same value add to the link
		(currentNode->right == NULL) && 
		(currentNode->z == maxNode->z)&&
		checkFeasible(currentNode)){
		maxNode->right = currentNode;
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
void printAllFeasibleSolution(struct node * node){
	//i++；
	if(node != NULL){
		printf("the value of this solution is  :\n");
		printfIntArray(node->decisionArray,node->numberOfVariable);
		//printf("the deep of this node is %d\n",node->deep );
		printf("\nthe z value of this node is %f\n",node->z );
		printf("the theta array is :\n");
		printfDoubleArray(node->theta,2);
		printf("\n\n");
	}
	if(node->right != NULL){
		printAllFeasibleSolution(node->right);
	}
	//return *i;
}
