gcc vector.c -o vector
echo 'Benchmark:'
time ./vector
gcc -O1 vector.c -o vector
echo 'Benchmark -O1:'
time ./vector
