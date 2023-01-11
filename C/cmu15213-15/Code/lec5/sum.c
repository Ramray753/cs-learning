long plus(long x, long y){
    return x + y;
};

void sumstore(long x, long y, long *dest){
    long t = plus(x, y);
    *dest = t;
}

int main(void){
    long a = 100;
    long b = 200;
    long c;
    sumstore(a, b, &c);
}