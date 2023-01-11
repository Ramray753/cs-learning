int loop(int x){
    int out = 0;
    for(int i = 0; i < x; i++){
        out += x;
    }
    return out;
}

int main(void){
    return loop(100);
}