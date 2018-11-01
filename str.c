#include "str.h"


uint32_t tobinary(uint32_t x){
	if(x==0) return 0;
	if(x==1) return 1;
	return (x%2 + 10*tobinary(x/2));
}

int h1_hash(uint32_t n){				//////////////////////////////////
	return (n & (1 << h1)-1);  // shift left	//				//
}							//      an h1=4 kai h2=6	//
							//				//
int h2_hash(uint32_t n){				//	1001 1101 1111 0011	//
	n = n >> h1;					//	       ^     ^ ^  ^	//
	return (n & (1 << h2)-1);  // shift right	//             < h2  > <h1>	//
}							//////////////////////////////////

// struct relation* create_relation(){

// }

void create_histogram(histogram* histogram, relation *rel){
	int i, h_val;
	int hist_rows = (int)pow(2,h1);
	uint32_t n;
	
	//initialize histogram
	for (i = 0; i < hist_rows; i++){
		histogram[i].value = 0;
		histogram[i].sum = 0;
	}
	
	for(i = 0; i < rel -> num_tuples; i++){
		n = tobinary((uint32_t)rel->tuples[i].payload);
		h_val = h1_hash(n);
		histogram[h_val].value = '0'+ h_val;
		histogram[h_val].sum++;
	}

	printf(" Histogram %d tuples\n",rel->num_tuples);
	for (i = 0; i < hist_rows; i++){
		printf("%c| %d\n", histogram[i].value, histogram[i].sum);		
	}
}

void create_psum(histogram* psum,histogram * histogram, relation *rel){
	int i, h_val;
	int hist_rows = (int)pow(2,h1);
	uint32_t n;

	//initialize psum
	for (i = 0; i < hist_rows; i++){
		psum[i].value = 0;
		psum[i].sum = 0;
	}

	psum[0].value = histogram[0].value; //first element
	for (i = 1; i < hist_rows; i++){

		psum[i].value = histogram[i].value;
		psum[i].sum = psum[i-1].sum + histogram[i-1].sum;
	}

	//Print Psum
	printf(" PSUM\n");
	for (int i = 0; i < hist_rows; i++){
		printf("%c | %d\n", psum[i].value, psum[i].sum);
	}
}

void reorder(relation * ord_rel, relation *rel, histogram* hist, histogram* psum){
	int i,j,val,n,pos,next_bucket;
	int tuples = rel->num_tuples;
	ord_rel -> num_tuples = tuples;

	int hist_rows = (int)pow(2,h1);

	ord_rel -> tuples = (struct tuple*)malloc(tuples*sizeof(struct tuple));
	if(ord_rel->tuples == NULL){
		printf("Error! memory not allocated\n");
		exit(0);
	}

	for (i = 0; i < tuples; i++){
		ord_rel -> tuples[i].key = i+1;
		ord_rel -> tuples[i].payload = -1;
	}


	for(i = 0; i < tuples; i++){
		n = tobinary(rel -> tuples[i].payload);
		val = h1_hash(n); 

		pos = psum[val].sum;
		next_bucket = pos + hist[val].sum;

		for(j = pos; j < next_bucket; j++){
			if(ord_rel->tuples[j].payload == -1){				
				// ord_rel->tuples[j].payload == n; 
				ord_rel->tuples[j].key = i;
				ord_rel->tuples[j].payload = rel -> tuples[i].payload; 
				break;
			}
		}
	}

	printf(" ORDERED \n");
	for (i = 0; i < tuples; i++){
		printf("%d | %c\n", ord_rel->tuples[i].key,ord_rel->tuples[i].payload);
	}

}

/*Radix Hash Join*/
// result* RadixHashJoin(relation *relR, relation* relS){}

void RadixHashJoin(relation *relR, relation* relS){

//////////////
//  PART 1  //
//////////////	
	int hist_rows = (int)pow(2,h1);

	//  S  //
	printf("Relation S\n");
    for (int i = 0; i < relS->num_tuples; i++){
    	//printf("%d | %c \n", i+1, relS->tuples[i].payload);
    }
	histogram histS[hist_rows];
	create_histogram(histS, relS);

	histogram psumS[hist_rows];
	create_psum(psumS, histS, relS);

	struct relation orderedS[relS->num_tuples];
	reorder(orderedS, relS, histS, psumS);

printf("<------------------->\n");

	//  R  //
	printf("Relation R\n");
    for (int i = 0; i < relR->num_tuples; i++){
    	printf("%d | %c \n", i+1, relR->tuples[i].payload);
    }
	histogram histR[hist_rows];
	create_histogram(histR, relR);

	histogram psumR[hist_rows];
	create_psum(psumR, histR, relR);

	struct relation orderedR[relR->num_tuples];
	reorder(orderedR, relR, histR, psumR);
	
printf("<------------------->\n");
//////////////
//  PART 2  //
//////////////
	int num_h2 = (int)pow(2,h2);
// init bucket S
	struct relation S_buckets[hist_rows];
	for(int i=0 ; i<hist_rows ; i++){  // gia kathe ena S bucket
		S_buckets[i].num_tuples = histS[i].sum;
		S_buckets[i].tuples = (struct tuple*)malloc( (histS[i].sum)*sizeof(struct tuple));
		for(int j=0 ; j<S_buckets[i].num_tuples ; j++){
			S_buckets[i].tuples[j].key = orderedS->tuples[j+psumS[i].sum].key;
			S_buckets[i].tuples[j].payload = orderedS->tuples[j+psumS[i].sum].payload;
		}
	}

// arxikopoihsh chain kai bucket
	struct bucket buckets[hist_rows];
	for(int i=0 ; i<hist_rows ; i++){
		buckets[i].key = (int32_t*)malloc(num_h2*sizeof(int32_t));
		for(int j=0 ; j<num_h2 ; j++){
			buckets[i].key[j] = -1;
		}
	}

	struct chain chains[hist_rows];
	for(int i=0 ; i<hist_rows ; i++){
		chains[i].num_tuples = histS[i].sum;
		chains[i].key = (int32_t*)malloc( (chains[i].num_tuples) *sizeof(int32_t));
		for(int k=0; k<chains[i].num_tuples ; k++){
			chains[i].key[k] = -1;
		}
	}


int i;
int n=0;
int h_val=0;
int32_t* temp_key;

int32_t* temp_k;
int l=0;

int b=0 ;  // trexon bucket ths h1 ths S

int chain_count=0;

for( b=0 ; b<hist_rows ; b++){
	printf("\n --------------------- %d\n",b);
	printf("H1 S Bucket %d [%d]\n", b, S_buckets[b].num_tuples);

	for( chain_count = S_buckets[b].num_tuples-1 ; chain_count>=0 ; chain_count--){  // gia to bucket b ths h1
		n = tobinary((uint32_t)S_buckets[b].tuples[chain_count].payload);
		h_val = h2_hash(n);

		temp_key = & (buckets[b].key[h_val] );

		while( (*temp_key) != -1){
			temp_key = &( chains[b].key[(*temp_key)] );
		}
		*temp_key = chain_count;
	}

	for(int k=0 ; k<num_h2 ; k++){  // MONO GIA PRINT TOU BUCKET
		temp_k = & ( buckets[b].key[k] ) ;
		while( (*temp_k) != -1){
			l++;
			temp_k = &( chains[b].key[(*temp_k)] );
		}
		printf("\t H2 S Bucket %d [%d]\n", k, l);

		l=0;
		temp_k = & ( buckets[b].key[k] ) ;
		while( (*temp_k) != -1){
			l++;
			printf("(%d-%c)", *temp_k, S_buckets[b].tuples[(*temp_k)].payload);
			temp_k = &( chains[b].key[(*temp_k)] );
		}
		l=0;
		printf("\n");
	}


	printf("\n");
	printf("H1 R Bucket %d\n", b);
	for(int r=psumR[b].sum ; r<(psumR[b].sum)+(histR[b].sum) ; r++){
		n = tobinary((uint32_t)orderedR->tuples[r].payload);
		h_val = h2_hash(n);
		printf("(%d-%c)%d", orderedR->tuples[r].key, orderedR->tuples[r].payload,h_val);
	}
	printf("\n");


//////////////////     JOIN     ///////////////////

	printf("\n");
	printf("JOIN %d\n", b);
	for(int r=psumR[b].sum ; r<(psumR[b].sum)+(histR[b].sum) ; r++){
		n = tobinary((uint32_t)orderedR->tuples[r].payload);
		h_val = h2_hash(n);
		temp_k = & ( buckets[b].key[h_val] ) ;
		while( (*temp_k) != -1){
			if( S_buckets[b].tuples[*temp_k].payload &  orderedR->tuples[r].payload ){
				printf("R(%d %c)-S(%d %c) ", orderedR->tuples[r].key , orderedR->tuples[r].payload , S_buckets[b].tuples[(*temp_k)].key, S_buckets[b].tuples[(*temp_k)].payload);
			}
			temp_k = &( chains[b].key[(*temp_k)] );
		}
	}
	printf("\n");






}  // b

	printf("\nEnd of Radix Hash Join\n");


}



















