#include <stdio.h>
#include <stdlib.h>
#define N 27
#define data_t int
#define IDENT 1
#define OP *

typedef struct{
    size_t len;
    data_t *data;
} vec;

typedef vec* vec_ptr;

int get_vec_element(vec *v, size_t idx, data_t *val){
    if (idx >= v -> len){
        return 0;
    }
    *val = v -> data[idx];
    return 1;
}

size_t vec_length(vec_ptr v){
    return v -> len;
}

data_t* get_vec_start(vec_ptr v){
    return v -> data;
} 

void combine(vec_ptr v, data_t *dest){
    long int i;
    *dest = IDENT;
    size_t length = vec_length(v);
    size_t limit = length - 1;
    data_t *d = get_vec_start(v); 
    data_t x0 = IDENT;
    data_t x1 = IDENT;
    for (i = 0; i < limit; i += 2) {
        x0 = x0 OP d[i];
        x1 = x1 OP d[i+1]; 
    }
    for (; i < length; i++) {
        x0 = x0 OP d[i];
    }
    *dest = x0 OP x1;
} 

int main(void){
    vec *test_data = malloc(sizeof(vec));
    test_data -> len = 1L << N;
    test_data -> data = malloc(sizeof(data_t) * 1L << N);
    data_t dest;
    for (int i; i < 10; i++){
        combine(test_data, &dest);
    }
    free(test_data -> data);
    free(test_data);
}