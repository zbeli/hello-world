#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "str.h"



int main(void){

// uint64_t numTuples|uint64_t numColumns|uint64_t T0C0|
// uint64_t T1C0|..|uint64_t TnC0|uint64_t T0C1|..|
// uint64_t TnC1|..|uint64_t TnCm
/*
	//Table R
int r_size = 10;
	char R[10][3] = {'a','a','a','a','d','d','c','b','a','a'};
int s_size = 108;
	char S[108] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		//'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		//'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		//'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','e','t','u','v','w','x','y','z',
		'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','e','t','u','v','w','x','y','z'
	};
*/

/*
	//Table R
int r_size = 16;
	char R[16][3] = {'0','1','2','3','4','5','6','7','8','9',':',';','<', '=','>','?'};
int s_size = 16;
	char S[16] = {'9','8','7','6','5','4','3','2','1','0',':',';','<','=','>','?'};

*/
/*
int r_size = 92;
	char R[r_size];
	R[0] = '!';
	for(int i=1 ; i<r_size ; i++){
		R[i] = R[i-1]+1;
	} 
	for(int i=0 ; i<r_size ; i++){
		printf("%c ",R[i] );
	}
	printf("\n");

int s_size = 92;
	char S[s_size];
	S[s_size-1] = '!';
	for(int i=s_size-2 ; i>=0 ; i--){
		S[i] = S[i+1]+1;
	} 
	for(int i=0 ; i<s_size ; i++){
		printf("%c ",S[i]) ;
	}
	printf("\n");
*/

int r_size = 32;
	char R[r_size];
	for(int i=0 ; i<r_size ; i++){
		R[i] = 'A' + (random()%26);
	} 
	for(int i=0 ; i<r_size ; i++){
		printf("%c ",R[i] );
	}
	printf("\n");

int s_size = 32;
	char S[s_size];
	for(int i=0 ; i<s_size ; i++){
		S[i] = 'A' + (random()%26);
	} 
	for(int i=0 ; i<s_size ; i++){
		printf("%c ",S[i]) ;
	}
	printf("\n");



//////////////////////////////////
	//   test
//////////////////////////////////

/////////////////////////////////////////////////////
	/////////////////////////////////////////////
	//test
	/////////////////////////////////////////////
/////////////////////////////////////////////////////





////////////   ///////////





	// int numTuples = 10;
	// int numColumns = 3;
	// int rowldRcolumns = 2;
	
	int i,j;
	// int **rowldR;
    
    /////////////////////////////////////////
    /////////////////////////////////////////
    struct relation relR;
    relR.num_tuples = r_size;
    relR.tuples = (struct tuple*)malloc(relR.num_tuples*sizeof(struct tuple));
    for (i = 0; i < relR.num_tuples; i++){
    	relR.tuples->key = i+1;
    	relR.tuples[i].payload = R[i];
    }

 /*   printf("1.___________ Relation R _____________\n");
    for (i = 0; i < relR.num_tuples; i++){
    	printf("%d | %c \n", i+1, relR.tuples[i].payload);
    }
	*/
    struct relation relS;
    relS.num_tuples = s_size;
    relS.tuples = (struct tuple*)malloc(relS.num_tuples*sizeof(struct tuple));
    for (i = 0; i < relS.num_tuples; i++){
    	relS.tuples->key = i+1;
    	relS.tuples[i].payload = S[i];
    }

/*    printf("2.___________ Relation S _____________\n");
    for (i = 0; i < relS.num_tuples; i++){
    	printf("%d | %c \n", i+1, relS.tuples[i].payload);
    }*/

    RadixHashJoin(&relR, &relS);


    printf("End of Program.\n");
	return 0;
}

