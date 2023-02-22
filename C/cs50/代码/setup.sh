export CC="clang"
export CFLAGS="-fsanitize=signed-integer-overflow -fsanitize=undefined -ggdb3 -O0 -std=c11 -Wall -Werror -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow"
export LDLIBS="-lcrypt -lcs50 -lm"
export PATH=/home/ubuntu/C/*/:$PATH
function run { make $1 && echo "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *" ; ./$1 ;}
export -f run
