#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>

using namespace std;

class BigInt{
public:
	int d[200]; // the actual number
	int len; // number of digits
	BigInt(){
		memset(d,0,sizeof(d));
		//flag=true;
		len=0;
	}//无参数时的构造函数
	BigInt(const char str []); // constructor which takes in an array of string
	BigInt(const BigInt& num, int start, int end);
	bool operator==(const BigInt & operand2);
	BigInt & operator=(const BigInt & operand);
	BigInt operator+(const BigInt & operand2);
	BigInt operator-(const BigInt & operand2);
	void printBigInt();//打印大整数
};

BigInt::BigInt(const char str[]){
	len=strlen(str); // gets the number of digits
	for(int i=0;i<len;++i){
		d[i]=str[len-1-i]-'0'; // convert each char to int, invert order to make addition easier
	}
}

BigInt::BigInt(const BigInt& num, int start, int end) {
	len = end - start;
	int index = 0;
	for (int i=start; i<end; i++) {
		d[index] = num.d[i];
		index++;
	}
	while(len > 1 && d[len-1]==0){
		len--; //get rid of leading zeros
	}
	memset(d+len, 0, (200-len)*sizeof(int)); // set the unnecessary digits to 0
}

bool BigInt::operator ==(const BigInt & operand2){
	if(len==operand2.len){
		for(int i=len-1;i>=0;--i){
			if(d[i]!=operand2.d[i])
				return false;//从高位往低位比较，只要有一位不等，则数不等
		}
		return true;//两数相等
	}else
		return false;
}

BigInt & BigInt::operator =(const BigInt & operand){
	if(operand.d==this->d)
		return *this;//防止a=a这样的情况
	this->len=operand.len;
	for(int i=0;i<operand.len;++i){
		this->d[i]=operand.d[i];
	}
	return *this;
}

BigInt BigInt::operator +(const BigInt & operand2){
	BigInt result;
	int carry=0;//carry是进位
	for(int i=0;i<len||i<operand2.len;++i){//以较长的为界限
		int temp=carry+d[i]+operand2.d[i];//两个对应位与进位相加
		carry=temp/10;//去掉最后一位之后就是新的进位
		result.d[result.len++]=temp%10;//留下最后一位作为该位的结果
	}
	if(carry!=0)//若最后进位不为0，则直接赋给结果的最高位
		result.d[result.len++]=carry;
	return result;
}
 
BigInt BigInt::operator -(const BigInt & operand2){
	BigInt result;
	BigInt minuend;//被减数
	BigInt subtrahend;//减数
	// if(*this<operand2){
	// 	result.flag=false;
	// 	minuend=operand2;
	// 	subtrahend=*this;//如果被减数小于减数，则将结果置负并将两数对换
	// }else{
	minuend=*this;
	subtrahend=operand2;
	// }
	for(int i=0;i<minuend.len||i<subtrahend.len;i++){//以较长的为界限
		if(minuend.d[i]<subtrahend.d[i]){//如果不够减
			minuend.d[i+1]--;//向高位借位
			minuend.d[i]+=10;//当前位加10
		}
		result.d[result.len++]=minuend.d[i]-subtrahend.d[i];//减法结果为当前位结果
	}
	while(result.len > 1 && result.d[result.len-1]==0){
		result.len--;//去除高位的0，同时至少保留一位最低位
	}
	return result;
}

void BigInt::printBigInt(){
	for(int i=len-1;i>=0;--i){
		printf("%d",d[i]);
	}
	cout << endl;
}

/*
Do a multiplication function, with many different situations:
1. when both are single digits
2. when only one of them is single digits
3. when both are not single digit
*/
BigInt simple_mult(int n1, const BigInt & n2) {
	BigInt result;
	int temp, carry=0;
	for (int i=0; i<n2.len; i++) {
		temp = n1*n2.d[i] + carry;
		carry = temp/10;
		result.d[result.len++] = temp % 10;
	}
	if (carry != 0) {
		result.d[result.len++] = carry;
	}
	return result;
}

// dir = 1, shift right; dir = 0, shift left
void shift(int f, BigInt & input, bool sign) {
	if (sign) {
		for (int i=input.len-1+f; i>=f; i--) {
			input.d[i] = input.d[i-f];
		}
		memset(input.d, 0, f*sizeof(int));
		input.len += f;
	} else {
		for (int i=0; i<input.len-f; i++) {
			input.d[i] = input.d[i+f];
		}
		memset(input.d+(input.len-f), 0, f*sizeof(int));
		input.len -= f;
	}
}

BigInt karatsuba(const BigInt & number1, const BigInt & number2) {
	// copy the inputs
	BigInt num1, num2;
	num1 = number1;
	num2 = number2;
	// base case: when one of the operand has only a single digit
	if (num1.len == 1) {
		int n1 = num1.d[0];
		return simple_mult(n1, num2);
	}
	if (num2.len == 1) {
		int n2 = num2.d[0];
		return simple_mult(n2, num1);
	}

	// handles the case with unequal lengths
	int length, shorter, remove;
	if (num1.len == num2.len) {
		length = num1.len;
		remove = 0;
	} else {
		length = max(num1.len, num2.len);
		shorter = min(num1.len, num2.len);
		remove = length-shorter;
		if (num1.len < length) {
			shift(remove, num1, true);
		}
		if (num2.len < length) {
			shift(remove, num2, true);
		}
	}

	// get each of the a b c d element
	int len_l, len_r;
	len_l = length/2;
	len_r = length - len_l;
	BigInt a(num1, len_r, length);
	BigInt b(num1, 0, len_r);
	BigInt c(num2, len_r, length);
	BigInt d(num2, 0, len_r);

	/* apply karatsuba multiplication */

	// get the 3 coefficients
	BigInt ac = karatsuba(a, c); // coeff 1
	BigInt bd = karatsuba(b, d); // coeff 3
	BigInt anb = a+b;
	BigInt cnd = c+d;
	BigInt third_term = karatsuba(anb, cnd);
	BigInt temp = third_term - ac;
	BigInt middle = temp - bd; // coeff 2

	// get the multiplitiers, handles the case where number of digits is odd
	int factor;
	if (length%2 == 0) {
		factor = length;
	} else {
		factor = length + 1;
	}

	// combine together
	shift(factor, ac, true);
	shift(factor/2, middle, true);
	BigInt ans1, ans2;
	ans1 = ac+middle;
	ans2 = ans1 + bd;
	if (remove != 0) {
		shift(remove, ans2, false);
	}
	// ans1 shift by factor, ans2 shift by factor/2
	return ans2;
}

int main() {
	char str1[100];
	char str2[100];
	gets(str1);
	gets(str2);
	BigInt a(str1);
	BigInt b(str2);
	// BigInt c=a-b;
	BigInt c = karatsuba(a, b);
	c.printBigInt();

	/* test for shift */
	// int factor = 4;
	// shift(factor, a, false);
	// shift(factor/2, b, false);
	// a.printBigInt();
	// b.printBigInt();
}