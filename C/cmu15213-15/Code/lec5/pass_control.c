long mult2(long a, long b){
    a = a + 100;
    return b;
}

void multstore(long x, long y, long *dest){
    long t = mult2(x, y);
    *dest = x;
}