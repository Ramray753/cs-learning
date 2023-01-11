# DataLab

# 1. `copyLSB`

描述：Replicate the least significant bit of $x$ across the entire word.

```c
/*
 * copyLSB - set all bits of result to least significant bit of x
 *   Examples:
 *     copyLSB(5L) = 0xFFFFFFFFFFFFFFFFL,
 *     copyLSB(6L) = 0x0000000000000000L
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
long copyLSB(long x) {
    return (x << 63) >> 63;
}

int main(void){
    printf("copyLSB(5L): %lx\n", copyLSB(5L));
    printf("copyLSB(6L): %lx\n", copyLSB(6L));
}
```

输出：

```shell
ubuntu@c-test-node:~/cmu15213/lab1/test$ ./bits
copyLSB(5L): ffffffffffffffff
copyLSB(6L): 0
```

思路：

* 先将最右的比特移到最左边。
* 然后进行算数右移，会自动复制最右边的比特。

# 2. `dividePower2`

描述：Return $\frac{x}{2^n}$ for $0 \leq n \leq 62$, rounding towards 0.

```c
/*
 * dividePower2 - Compute x/(2^n), for 0 <= n <= 62
 *  Round toward zero
 *   Examples: dividePower2(15L,1L) = 7L, dividePower2(-33L,4L) = -2L
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
long dividePower2(long x, long n) {
    long sign = x >> 63;
    long last_n = x << (64 + ~n + 1);
    return x >> n + !!(sign & last_n);
}

int main(void){
    printf("dividePower2(15L,1L): %li\n", dividePower2(15L,1L));
    printf("dividePower2(-33L,4L): %li\n", dividePower2(-33L,4L));
}
```

结果：

```c
dividePower2(15L,1L): 7
dividePower2(-33L,4L): -2
```

思路：

* 对于正数，只需要右移`n`位，会自动截断小数点。
* 对于负数，需要判断最后的`n`位是否含1，如果含有，说明右移后会产生小数，在右移`n`位后需要加上1才能Round to 0。
* 只要最后`n`位有1，而且为负数，那么`!!(sign & last_n) = 1`。

# 3. `distinctNegation`

描述：Returns 1 if $x \neq-x$.

```c
/*
 * distinctNegation - returns 1 if x != -x.
 *     and 0 otherwise
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 5
 *   Rating: 2
 */
long distinctNegation(long x) {
    long neg = ~x + 1;
    return !!(x ^ neg); // 也可以 return  ~((x ^ neg) >> 63) + 1;
}

int main(void){
    printf("distinctNegation(0L): %li\n", distinctNegation(0L));
    printf("distinctNegation(1L << 63): %li\n", distinctNegation(1L << 63));
    printf("distinctNegation(1L): %li\n", distinctNegation(1L));
}
```

结果：

```shell
distinctNegation(0L): 0
distinctNegation(1L << 63): 0
distinctNegation(1L): 1
```

思路：

* 先求出Negation。
* 与原`x`做XOR，如果相同，则全为0，否则MSB一定是1。
* 之后：
  * 可以算数右移63位，得到0或-1，再做一个Negation即可。
  * 也可以先求`!`，如果之前XOR是0，得到1，反之得到0。再求一次`!`得到最终答案啊。

# 4. `anyEvenBit`

描述：Return 1 if any even-numbered bit in word is set to 1. Bits are numbered from 0 (LSB) to 63 (MSB).

```c
/*
 * anyEvenBit - return 1 if any even-numbered bit in word set to 1
 *   where bits are numbered from 0 (least significant) to 63 (most significant)
 *   Examples anyEvenBit(0xAL) = 0L, anyEvenBit(0xEL) = 1L
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 14
 *   Rating: 2
 */
long anyEvenBit(long x) {
    long mask = 0x55555555L | (0x55555555L << 32);
    return !!(mask & x);
}

int main(void){
    printf("anyEvenBit(0xAL): %li\n", anyEvenBit(0xAL));
    printf("anyEvenBit(0xEL): %li\n", anyEvenBit(0xEL));
}
```

结果：

```c
ubuntu@c-test-node:~/cmu15213/lab1/test$ ./bits
anyEvenBit(0xAL): 0
anyEvenBit(0xEL): 1
```

思路：

* 先创建一个Even位全是1，Odd位全是0的Mask，使用16进制创建即可。可以先创建前32位，再使用左移然后按位或即可。
* 该Mask与`x`进行AND操作，如果Even位有1，那么结果不为0。
* 使用两次`!`操作即可。

# 5. `isLessOrEqual`

描述：Return 1 if $x \leq y$, else return 0。

```c
/*
 * isLessOrEqual - if x <= y  then return 1, else return 0
 *   Example: isLessOrEqual(4L,5L) = 1L.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
long isLessOrEqual(long x, long y) {
    long a = x >> 63;
    long b = y >> 63;
    long c = (y + ~(x) + 1) >> 63;
    return !!((~b & a) | (~(a ^ b) & ~c));
  	// 或者 return ((~b & a) & 1L) | (((~(a ^ b)) & 1L) & (~c & 1L));
}

int main(void){
    printf("isLessOrEqual(4L,5L): %li\n", isLessOrEqual(4L,5L));
    printf("isLessOrEqual(5L,4L): %li\n", isLessOrEqual(5L,4L));
    printf("isLessOrEqual(3L,3L): %li\n", isLessOrEqual(3L,3L));
    printf("isLessOrEqual(-4L,5L): %li\n", isLessOrEqual(-4L,5L));
    printf("isLessOrEqual(5L,-4L): %li\n", isLessOrEqual(5L,-4L));
}
```

结果：

```shell
ubuntu@c-test-node:~/cmu15213/lab1/test$ ./bits 
isLessOrEqual(4L,5L): 1
isLessOrEqual(5L,4L): 0
isLessOrEqual(3L,3L): 1
isLessOrEqual(-4L,5L): 1
isLessOrEqual(5L,-4L): 0
```

思路：

* 如果不同号而且`x < 0, y >= 0`时返回1，当且仅当`~b & a`全为1（或者`(~b & a) & 1L`为1）。
* 如果符号相同，当且仅当`a ^ b`全为0，即`~(a ^ b)`全为1（或者`(~(a ^ b)) & 1L`为1）。
  * 符号相同的话，两数相减不会出现Overflow，因此若`y >= x`，当且仅当`c`全为0，即`~c`全为1（或者`~c & 1L`为1）。

# 6. `replaceByte`

描述：Replace byte $n$ in $x$ with $c$。

```c
/*
 * replaceByte(x,n,c) - Replace byte n in x with c
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: replaceByte(0x12345678,1,0xab) = 0x1234ab78
 *   You can assume 0 <= n <= 7 and 0 <= c <= 255
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 3
 */
long replaceByte(long x, long n, long c) {
    long mask_c = c << (n << 3);
    long mask_1 = ~(0xffL << (n << 3));
    return (x & mask_1) | mask_c;
}

int main(void){
    printf("replaceByte(0x12345678,0,0xab): 0x%lx\n", replaceByte(0x12345678,0,0xab));
    printf("replaceByte(0x12345678,1,0xab): 0x%lx\n", replaceByte(0x12345678,1,0xab));
    printf("replaceByte(0x12345678,2,0xab): 0x%lx\n", replaceByte(0x12345678,2,0xab));
    printf("replaceByte(0x12345678,3,0xab): 0x%lx\n", replaceByte(0x12345678,3,0xab));
}
```

思路：

* 先把非替换字节的比特筛选出来，替换字节的比特位用0代替，例如`0x12340078`。
* 把替换后字节的比特移到对应的比特位，其余比特用0代替，例如`0x0000ab00`。
* 这两个数字做`|`操作即可。

# 7. `conditional`

描述：Evaluate the expression `x ? y : x`。

```c
/*
 * conditional - same as x ? y : z
 *   Example: conditional(2,4L,5L) = 4L
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
long conditional(long x, long y, long z) {
    long mask = ~(!!(x)) + 1L;
    return (mask & y) | (~mask & z);
}

int main(void){
    printf("conditional(2,4L,5L): %li\n", conditional(2,4L,5L));
    printf("conditional(0,4L,5L): %li\n", conditional(0,4L,5L));
    printf("conditional(-2,4L,5L): %li\n", conditional(-2,4L,5L));
}
```

结果：

```shell
ubuntu@c-test-node:~/cmu15213/lab1/test$ ./bits 
conditional(2,4L,5L): 4
conditional(0,4L,5L): 5
conditional(-1,4L,5L): 4
```

思路：

* 如果`x`含有`1`比特，那么`!!x`返回1，否则返回0。
* 根据是否含有`1`比特，创建全为1或者全为0的Mask，然后和`y`和`z`做与操作，后合并即可。

# 8. `bitMask`

描述：Generate a mask consisting of all 1's between $l$ and $h$.

```c
/*
 * bitMask - Generate a mask consisting of all 1's
 *   between lowbit and highbit
 *   Examples: bitMask(5L,3L) = 0x38L
 *   Assume 0 <= lowbit < 64, and 0 <= highbit < 64
 *   If lowbit > highbit, then mask should be all 0's
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
long bitMask(long highbit, long lowbit) {
    long mask1 = (1L << 63) >> (63 + ~lowbit + 1);
    long mask2 = ((1L << 63) >> (63 + ~highbit + 1)) << 1;
    return (mask1 ^ mask2) & ((lowbit - highbit + ~1L + 1) >> 63);
}

int main(void){
    printf("bitMask(5L,3L): 0x%lx\n", bitMask(5L,3L));
    printf("bitMask(3L,3L): 0x%lx\n", bitMask(3L,3L));
    printf("bitMask(63L,3L): 0x%lx\n", bitMask(63L,3L));
    printf("bitMask(3L,5L): 0x%lx\n", bitMask(3L,5L));
}
```

结果：

```shell
ubuntu@c-test-node:~/cmu15213/lab1/test$ ./bits 
bitMask(5L,3L): 0x38
bitMask(3L,3L): 0x8
bitMask(63L,3L): 0xfffffffffffffff8
bitMask(3L,5L): 0x0
```

思路：

* 分别创建右侧`l`位和右侧`h + 1`位全为0的Mask，两个Mask做XOR即可得到结果。
* 如果`l > h`，那么`(lowbit - highbit - 1) >> 63`将全为0，否则全为1，将这个结果与原结果相与即可。

# 9. `bitCount`

描述：Return the 1 bits in `x`.

```c
long bitCount(long x) {
  long mask_1 = 0x55555555L | 0x55555555L << 32;
  long mask_2 = 0x33333333L | 0x33333333L << 32;
  long mask_4 = 0x0f0f0f0fL | 0x0f0f0f0fL << 32;
  long mask_8 = 0x00ff00ffL | 0x00ff00ffL << 32;
  long mask_16 = 0x0000ffffL | 0x0000ffffL << 32;
  long mask_32 = 0x00000000ffffffffL;
  x = (x&mask_1) + ((x>>1)&mask_1);
  x = (x&mask_2) + ((x>>2)&mask_2);
  x = (x&mask_4) + ((x>>4)&mask_4);
  x = (x&mask_8) + ((x>>8)&mask_8);
  x = (x&mask_16) + ((x>>16)&mask_16);
  x = (x&mask_32) + ((x>>32)&mask_32);
  return x;
}

int main(void){
    printf("bitCount(0x3f3fL): %li\n", bitCount(0x3f3fL));
    printf("bitCount(0xfffL): %li\n", bitCount(0xffffL));
    printf("bitCount(0x0000L): %li\n", bitCount(0x0000L));
}
```

结果：

```shell
ubuntu@c-test-node:~/cmu15213/lab1/test$ ./bits 
bitCount(0x3f3fL): 12
bitCount(0xfffL): 16
bitCount(0x0000L): 0
```

思路：

* 类似于自下而上的分治法。依次统计每2/4/8/16/32/64位有多少个1，把结果储存在2/4/8/16/32/64的比特块中。
* 要计算2N位中有多少个，假设前N位的1的个数已经记录在前N位，后N位的1的个数已经记录在后N位，那么总个数只需要将前N位和后N位做`+`运算即可。结果最大最大不会超过2N位（其实是N + 1）位，所以不会造成Overflow。

# 10. `bitReverse`

描述：Return the bits of `x` in reversed order.

```c
long bitReverse(long x) {
  long mask_1 = 0x55555555L | 0x55555555L << 32;
  long mask_2 = 0x33333333L | 0x33333333L << 32;
  long mask_4 = 0x0f0f0f0fL | 0x0f0f0f0fL << 32;
  long mask_8 = 0x00ff00ffL | 0x00ff00ffL << 32;
  long mask_16 = 0x0000ffffL | 0x0000ffffL << 32;
  long mask_32 = 0x00000000ffffffffL;
  x = ((x&mask_1)<<1) | ((x>>1)&mask_1);
  x = ((x&mask_2)<<2) | ((x>>2)&mask_2);
  x = ((x&mask_4)<<4) | ((x>>4)&mask_4);
  x = ((x&mask_8)<<8) | ((x>>8)&mask_8);
  x = ((x&mask_16)<<16) | ((x>>16)&mask_16);
  x = ((x&mask_32)<<32) | ((x>>32)&mask_32);
  return x;
}

int main(void){
    printf("bitReverse(0xa6L): 0x%lx\n", bitReverse(0xa6L));
    printf("bitReverse(0x6F0F0123c480F0F6L): 0x%lx\n", bitReverse(0x6F0F0123c480F0F6L));
}
```

结果：

```shell
ubuntu@c-test-node:~/cmu15213/lab1/test$ ./bits 
bitReverse(0xa6L): 0x6500000000000000
bitReverse(0x6F0F0123c480F0F6L): 0x6f0f0123c480f0f6
```

思路：

* 类似于`bitCount`，只不过计算逻辑不一样。
* 假设前N位和后N位已经做了局部Reverse，那么只需要交换着前后N位即可。

# 11. `isPalindrome`

描述：Return 1 if bit pattern in $x$ is equal to its mirror image.

```c
/*
 * isPalindrome - Return 1 if bit pattern in x is equal to its mirror image
 *   Example: isPalindrome(0x6F0F0123c480F0F6L) = 1L
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 70
 *   Rating: 4
 */
long isPalindrome(long x) {
  long mask_1 = 0x55555555L | 0x55555555L << 32;
  long mask_2 = 0x33333333L | 0x33333333L << 32;
  long mask_4 = 0x0f0f0f0fL | 0x0f0f0f0fL << 32;
  long mask_8 = 0x00ff00ffL | 0x00ff00ffL << 32;
  long mask_16 = 0x0000ffffL | 0x0000ffffL << 32;
  long mask_32 = 0x00000000ffffffffL;
  long origin = x;
  x = ((x&mask_1)<<1) | ((x>>1)&mask_1);
  x = ((x&mask_2)<<2) | ((x>>2)&mask_2);
  x = ((x&mask_4)<<4) | ((x>>4)&mask_4);
  x = ((x&mask_8)<<8) | ((x>>8)&mask_8);
  x = ((x&mask_16)<<16) | ((x>>16)&mask_16);
  x = ((x&mask_32)<<32) | ((x>>32)&mask_32);
  return !(x ^ origin); 
}

int main(void){
    printf("isPalindrome(0x6F0F0123c480F0F6L): %li\n", isPalindrome(0x6F0F0123c480F0F6L));
    printf("isPalindrome(0xffL): %li\n", isPalindrome(0xffL));
}
```

结果：

```c
ubuntu@c-test-node:~/cmu15213/lab1/test$ ./bits 
isPalindrome(0x6F0F0123c480F0F6L): 1
isPalindrome(0xffL): 0
```

思路：

* 先求出倒序，然后做XOR运算即可。

# 12. `trueFiveEighths`

描述：Return $\frac{5}{8} x$, rounding towards 0 and avoiding errors due to overflow.

```c
/*
 * trueFiveEighths - multiplies by 5/8 rounding toward 0,
 *  avoiding errors due to overflow
 *  Examples:
 *    trueFiveEighths(11L) = 6L
 *    trueFiveEighths(-9L) = -5L
 *    trueFiveEighths(0x3000000000000000L) = 0x1E00000000000000L (no overflow)
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 20
 *  Rating: 4
 */
long trueFiveEighths(long x) {
    long sign = x >> 63;
    long last_1 = x << 63;
    long last_3 = x << 61;
    long overflow = (last_1 & last_3) >> 63;
    return (x >> 1) + (x >> 3) + !!(sign & last_3) + !!(overflow);
}

int main(void){
    printf("trueFiveEighths(11L): %li\n", trueFiveEighths(11L));
    printf("trueFiveEighths(-9L): %li\n", trueFiveEighths(-9L));
    printf("trueFiveEighths(0x3000000000000000L): 0x%lx\n", trueFiveEighths(0x3000000000000000L));
}
```

结果：

```shell
ubuntu@c-test-node:~/cmu15213/lab1/test$ ./bits 
trueFiveEighths(11L): 6
trueFiveEighths(-9L): -5
trueFiveEighths(0x3000000000000000L): 0x1e00000000000000
```

思路：

* `5/8`可以分解为`1/2 + 1/8`。
* 看小数部分相加有没有Overflow，如果有，结果加上1。
* 如果是负数，而且小数部分存在1，那么再加上1，用于Rouding to 0。不存在Overflow成整数的情况。

# 13. `logicalNeg`

描述：Implement the ! operator, without using it.

```c
/*
 * logicalNeg - implement the ! operator, using all of
 *              the legal operators except !
 *   Examples: logicalNeg(3L) = 0L, logicalNeg(0L) = 1L
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 */
long logicalNeg(long x) {
    long mask_1 = 1L;
    long mask_2 = ~((1L << 63) >> 61);
    long mask_4 = ~((1L << 63) >> 59);
    long mask_8 = ~((1L << 63) >> 55);
    long mask_16 = ~((1L << 63) >> 47);
    long mask_32 = ~((1L << 63) >> 31);
    x = (x&mask_32) | ((x>>32)&mask_32);
    x = (x&mask_16) | ((x>>16)&mask_16);
    x = (x&mask_8) | ((x>>8)&mask_8);
    x = (x&mask_4) | ((x>>4)&mask_4);
    x = (x&mask_2) | ((x>>2)&mask_2);
    x = (x&mask_1) | ((x>>1)&mask_1);
    return ~x + 2L;
}

// 方法2:
long logicalNeg(long x) {
    return (((~x + 1) | x) >> 63) + 1L;
}
```

结果：

```shell
ubuntu@c-test-node:~/cmu15213/lab1/test$ ./bits 
logicalNeg(3L): 0
logicalNeg(-3L): 0
logicalNeg(0L): 1
```

思路：

* 只有0的相反数是0，MSB还是0，否则MSB一定发生反转。
* 去相反数前后的MSB做`|`操作，如果是1则为非0数，否则原数为0。

# 14. `bitXor`

描述：$x|| y$ using only \& and $\sim$.

```c
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  return ~(~(~x & y) & ~(x & ~y));
}

int main(void){
    printf("bitXor(0xaL, 6L): %i\n", bitXor(0xaL, 6L));
}
```

结果：

```shell
ubuntu@c-test-node:~/cmu15213/lab1/self-test$ ./bits
bitXor(0xaL, 6L): 12
```

思路：

* 对于`~x & y`和`~x & y`，如果本来比特不同，那么这两额结果中至少有一个为1，不然结果一定都为0.
* 将上面的结果取NOT，然后相与，如果本来比特不同，那么结果为0，否则为1。
* 最后再做一次NOT即可。

# 15. `tmin`

描述：Smallest two's complement integer.

```c
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  return 1L << 31;
}

int main(void){
    printf("tmin(): %i\n", tmin());
}
```

结果：

```shell
ubuntu@c-test-node:~/cmu15213/lab1/self-test$ ./bits
tmin(): -2147483648
```

# 16. `isTmax`

描述：True only if $x$ largest two's comp. integer.

```c
int isTmax(int x) {
  return !(x ^ (~(x + 1)) | (!(~x)));
}

int main(void){
    printf("isTmax(-1): %i\n", isTmax(-1));
    printf("isTmax(1): %i\n", isTmax(1));
    printf("isTmax(-1): %i\n", isTmax(~(1 << 31)));
}
```

结果：

```shell
ubuntu@c-test-node:~/cmu15213/lab1/self-test$ ./bits
isTmax(-1): 0
isTmax(1): 0
isTmax(-1): 1
```

思路：

* Tmax的特性是`x == ~(x + 1)`，因此`x ^ (~(tmax + 1))`的结果应该全为0。
* 但是`-1`也满足这个条件，所以要排除这个情况。

# 17. `allOddBits`

描述：True only if all odd-numbered bits in `x` set to 1.

```c
int allOddBits(int x) {
  int mask = 0xaaaa | 0xaaaa << 16;
  return !((x & mask) ^ mask);
}

int main(void){
    printf("allOddBits(0xFFFFFFFD): %i\n", allOddBits(0xFFFFFFFD));
    printf("allOddBits(0xAAAAAAAA): %i\n", allOddBits(0xAAAAAAAA));
}
```

结果：

```shell
ubuntu@c-test-node:~/cmu15213/lab1/self-test$ ./bits
allOddBits(0xFFFFFFFD): 0
allOddBits(0xAAAAAAAA): 1
```

思路：

* 与`anyEvenBits`类似。

# 18. `isAsciDigit`

描述：True if `0x30 <= x <= 0x39`.

```c
int isAsciiDigit(int x) {
  return (!((x >> 4) ^ 0x3)) & ((!((x >> 3) ^ 0x6)) | (!((x >> 1) ^ 0x1c)));
}

int main(void){
    printf("isAsciiDigit(0x2f): %i\n", isAsciiDigit(0x2f));
    printf("isAsciiDigit(0x30): %i\n", isAsciiDigit(0x30));
    printf("isAsciiDigit(0x31): %i\n", isAsciiDigit(0x31));
    printf("isAsciiDigit(0x32): %i\n", isAsciiDigit(0x32));
    printf("isAsciiDigit(0x33): %i\n", isAsciiDigit(0x33));
    printf("isAsciiDigit(0x34): %i\n", isAsciiDigit(0x34));
    printf("isAsciiDigit(0x35): %i\n", isAsciiDigit(0x35));
    printf("isAsciiDigit(0x36): %i\n", isAsciiDigit(0x36));
    printf("isAsciiDigit(0x37): %i\n", isAsciiDigit(0x37));
    printf("isAsciiDigit(0x38): %i\n", isAsciiDigit(0x38));
    printf("isAsciiDigit(0x39): %i\n", isAsciiDigit(0x39));
    printf("isAsciiDigit(0x3a): %i\n", isAsciiDigit(0x3a));
}
```

结果：

```c
ubuntu@c-test-node:~/cmu15213/lab1/self-test$ ./bits
isAsciiDigit(0x2f): 0
isAsciiDigit(0x30): 1
isAsciiDigit(0x31): 1
isAsciiDigit(0x32): 1
isAsciiDigit(0x33): 1
isAsciiDigit(0x34): 1
isAsciiDigit(0x35): 1
isAsciiDigit(0x36): 1
isAsciiDigit(0x37): 1
isAsciiDigit(0x38): 1
isAsciiDigit(0x39): 1
isAsciiDigit(0x3a): 0
```

思路：

* `0x30`是`0011 0000`，`0x39`是`0011 1001`。
* 对于4-7位比特必须是`0x3`：`(x >> 4) ^ 0x3`必须是0。
* 对于0-3位比特：
  * 如果第3位是`0`，那么一定成立：`(x >> 3) ^ 0x6`等于0。
  * 如果第4位是`1`，那么1-2位必须是0：`(x >> 1) ^ 0x1c`。

# 19. `greatestBitPos`

描述：返回为1的最高比特位。

```c
int greatestBitPos(int x) {
  int n = 0;
  int mask = (~0) << (n + 16);
  n += ((!!(x & mask)) << 4);
  mask = (~0) << (n + 8);
  n += ((!!(x & mask)) << 3);
  mask = (~0) << (n + 4);
  n += ((!!(x & mask)) << 2);
  mask = (~0) << (n + 2);
  n += ((!!(x & mask)) << 1);
  mask = (~0) << (n + 1);
  n += (!!(x & mask));
  return n;
}
```

结果：

```shell
ubuntu@c-test-node:~/cmu15213/lab1/self-test$ ./bits
greatestBitPos(0xf00): 11
```

思路：

* 二分思想。
* 先判断前16位中有没有1，如果有，判断前八位，如果没有，判断8-15位，以此类推。
* 注意每个Mask是动态生成的，基于上一次的判断结果。

# 20. `howManyBits`

描述：Min. no. of bits to represent $x$ in two's comp.

```c
int greatestBitPos(int x) {
  x = (x << 1) ^ x;
  int n = 0;
  int mask = (~0) << (n + 16);
  n += ((!!(x & mask)) << 4);
  mask = (~0) << (n + 8);
  n += ((!!(x & mask)) << 3);
  mask = (~0) << (n + 4);
  n += ((!!(x & mask)) << 2);
  mask = (~0) << (n + 2);
  n += ((!!(x & mask)) << 1);
  mask = (~0) << (n + 1);
  n += (!!(x & mask));
  return n + 1;
}

int main(void){
    printf("howManyBits(12): %i\n", howManyBits(12));
    printf("howManyBits(298): %i\n", howManyBits(298));
    printf("howManyBits(-5): %i\n", howManyBits(-5));
    printf("howManyBits(0): %i\n", howManyBits(0));
    printf("howManyBits(-1): %i\n", howManyBits(-1));
    printf("howManyBits(0x80000000): %i\n", howManyBits(0x80000000));
}
```

结果：

```shell
ubuntu@c-test-node:~/cmu15213/lab1/self-test$ ./bits
howManyBits(12): 5
howManyBits(298): 10
howManyBits(-5): 4
howManyBits(0): 1
howManyBits(-1): 1
howManyBits(0x80000000): 32
```

思路：

* 找到`s`位外，最高为`1`的位数，加上1位需要的非`s`位的个数，最后天加一个`s`位。
* 为了避免`s`位的影响，我们只需要异或相邻的数`x = x ^ (x<<1)`，找出为`1`的最高位在哪一位，最后加1（`s`位）即可。

# 21. `floatScale2`

描述：Return bit-level equiv. of $2 * \pm$ for f.p. $\arg . f$.

```c
unsigned floatScale2(unsigned uf) {
  int exp_ = (uf & 0x7f800000) >> 23;
  int s_ = uf & 0x80000000;
  if (exp_ == 0){
    return (uf << 1) | s_;
  }
  if (exp_ == 255){
    return uf;
  }
  exp_++;
  if (exp_ == 255){
    return 0x7f800000 | s_;
  }
  return (uf & 0x807fffff) | (exp_ << 23);
}
```

思路：

* 先抽取`exp`和`s`位。
* 分类讨论：
  * 如果`exp`为0，说明为非规格化小数或`0`只需要左移1位，然后补上`s`。
  * 如果`exp`为255，说明为无限大，乘以2不改变结果。
  * 要乘以2那么`exp`需要自增1。
    * 如果自增后为`255`，返回无限（`0x7f800000`），修改符号。
    * 否则近修改`exp`部分即可。

# 22. `floatPower2`

描述：Return bit-level equiv. of $2.0^{\wedge} \mathrm{x}$ for integer $\mathrm{x}$.

```c
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x) {
    int exp = x + 127;
    if(exp <= 0) return 0;
    if(exp >= 255) return 0x7f800000;
    return exp << 23;
}
```

思路：

* 考虑特殊情况，指数`exp<-127`和 `exp>128`，分别返回`0`和`0x7f800000`。

# 23. `floatFloat2Int`

描述：Return bit-level equiv. of (int) $f$ for f.p. arg. $f$.

```c
int floatFloat2Int(unsigned uf) {
  int s_    = uf>>31;
  int exp_  = ((uf&0x7f800000)>>23)-127;
  int frac_ = (uf&0x007fffff)|0x00800000; 
  if(!(uf&0x7fffffff)) return 0;
  
  if(exp_ > 31) return 0x80000000;
  if(exp_ < 0) return 0;
  
  if(exp_ > 23) frac_ <<= (exp_-23);
  else frac_ >>= (23-exp_);
  
  if(!((frac_>>31)^s_)) return frac_;
  else if(frac_>>31) return 0x80000000;
  else return ~frac_+1;
}
```

思路：

* 先将浮点数分成三段：
  * 符号部分`s_ = uf>>31`
  * 指数大小`exp_ = ((uf&0x7f800000)>>23)-127`
  * 获取小数部分，并补上浮点数缺省的`1`，`frac_ = (uf&0x007fffff)|0x00800000`。
* 处理特殊情况全为`0`是返回`0`，若指数大于`31`，整数无法表示溢出返回`0x80000000`。若指数小于`0`，该数`0<x<1`返回`0`。
* 若指数部分大于`23`则将小数部分向左移动`frac_ <<= (exp_ - 23)` ，`exp_`代表指数大小。
* 若指数部分小于`23`则将小数部分向右移动`frac_ >>= (23 - exp_)` ，`exp_`代表指数大小。
* 考虑最后符号，正数转换为负数不会产生溢出。若`frac_`为正数，则根据`s_`调整正负输出即可。
* 若`frac_`为负数，唯一正确情况为`0x80000000`。











