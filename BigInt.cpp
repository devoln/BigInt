#include "BigInt.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <assert.h>

BigInt::BigInt(Value val)
{
	arr.push_back(val);
	negative = false;
}

BigInt::BigInt(SignedValue val)
{
	arr.push_back(abs(val));
	negative = (val<0);
}

BigInt::BigInt(LongValue val)
{
	arr.push_back((Value)val);
	arr.push_back((Value)(val >> (sizeof(Value)*8)));
	negative = false;
}

BigInt::BigInt(SignedLongValue val)
{
	auto abss = abs(val);
	arr.push_back((Value)abss);
	arr.push_back((Value)abs(SignedValue(abss >> (sizeof(Value)*8))));
	negative = (val<0);
}

BigInt::BigInt(const std::string& s)
{
	assert(!s.empty());
	*this = 0;
	size_t i=0, l=0;
	bool neg = false;
	if(s[0]=='-') neg = true, i = 1;
	if(s[0]=='+') i = 1;
	size_t temp = 0;
	for(; i<s.length(); i++, l++)
	{
		if(!isdigit(s[i])) break;
		temp = temp*10+(s[i]-'0');
		if(l%9==8)
		{
			*this = *this*1000000000+temp;
			temp=0;
		}
	}
	Value pow10=1;
	for(size_t j=0; j<l%9; j++) pow10*=10;
	*this = *this*pow10+temp;
	negative = neg;
}

BigInt::BigInt(const BigInt& rhs): arr(rhs.arr), negative(rhs.negative) {}
BigInt::BigInt(BigInt&& rhs): arr(move(rhs.arr)), negative(rhs.negative) {}


BigInt BigInt::operator+(const BigInt& rhs) const
{
	if(negative==rhs.negative)
	{
		BigInt result = abs_add(rhs);
		if(!negative) return result;
		result.negative = !result.negative;
		return result;
	}
	if(abs_compare(rhs)>0)
	{
		BigInt result = abs_subtract(rhs);
		if(!negative) return result;
		result.negative = !result.negative;
		return result;
	}
	BigInt result = rhs.abs_subtract(*this);
	if(!rhs.negative) return result;
	result.negative = !result.negative;
	return result;
}

BigInt BigInt::operator*(const BigInt& rhs) const
{
	BigInt result;
	result.negative = (negative!=rhs.negative);
	size_t lsize = arr.size();
	size_t rsize = rhs.arr.size();
	while(lsize>1 && arr[lsize-1]==0) lsize--;
	while(rsize>1 && rhs.arr[rsize-1]==0) rsize--;
	const size_t sumsize = lsize+rsize;
	result.arr.resize(sumsize);
	for(size_t i=0; i<lsize; i++)
	{
		for(size_t j=0; j<rsize; j++)
		{
			LongValue product = (LongValue)arr[i]*rhs.arr[j];
			result.arr[i+j] += (Value)product;
			LongValue remainder = (product >> (sizeof(Value)*8));
			if(result.arr[i+j] < (Value)product) remainder++;
			result.arr[i+j+1] += (Value)remainder;
			if(result.arr[i+j+1] < remainder)
				result.inc_digit(i+j+2);
		}
	}
	result.trim_size();
	return result;
}

BigInt BigInt::Divide(Value rhs, Value* mod) const
{
	assert(rhs!=0);
	int cmp = abs_compare(rhs);
	if(cmp<0)
	{
		if(mod!=nullptr) *mod = arr[0];
		return 0;
	}
	if(cmp==0)
	{
		if(mod!=nullptr) *mod = 0;
		return negative==(rhs<0)? 1: -1;
	}

	BigInt result;
	result.negative = (negative!=(rhs<0));
	result.arr.resize(arr.size());
	LongValue temp = 0;
	for(intptr_t i = intptr_t(arr.size()-1); i>=0; i--)
	{
		temp = (temp << (sizeof(Value)*8))|arr[i];
		result.arr[i] = (Value)(temp/rhs);
		temp = temp%rhs;
	}
	result.trim_size();
	if(mod!=nullptr) *mod = (Value)temp;
	return result;
}

BigInt BigInt::Divide(const BigInt& rhs, BigInt* mod) const
{
	if(rhs.arr.size()==1)
	{
		if(mod!=nullptr) *mod = 0;
		auto result = Divide(rhs.arr[0], mod!=nullptr? &mod->arr[0]: nullptr);
		result.negative = (negative!=rhs.negative);
		return result;
	}

	int cmp = abs_compare(rhs);
	if(cmp<0)
	{
		if(mod!=nullptr) *mod = *this;
		return 0;
	}
	if(cmp==0)
	{
		if(mod!=nullptr) *mod = 0;
		return negative==rhs.negative? 1: -1;
	}

	BigInt result, remainder;
	result.arr.resize(arr.size());
	for(intptr_t i = intptr_t(arr.size()-1); i>=0; i--)
	{
		remainder.arr.insert(remainder.arr.begin(), arr[i]);
		Value x = 0;
		SignedLongValue l = 0;
		SignedLongValue r = 1ll << 32;
		while(l<=r)
		{
			SignedLongValue m = (l+r)/2;
			BigInt cur = rhs*m;
			if(cur<=remainder)
			{
				x = (Value)m;
				l = m+1;
			}
			else r = m-1;
		}
		result.arr[i]=x;
		remainder -= rhs*x;
	}
	result.trim_size();

	if(mod!=nullptr) *mod = remainder;
 
	result.negative = (negative!=rhs.negative);
	return result;
}

std::string BigInt::ToString() const
{
	BigInt temp = *this;
	std::string result;
	result.reserve(((arr.size()*1233) >> 7)+1);

	char tempstr[10];
	tempstr[9]=0;
	do
	{
		Value mod;
		temp = temp.Divide(1000000000, &mod);

		char* tempstrIter = tempstr;
		do *tempstrIter++ = '0'+mod%10; while(mod/=10);
		while(tempstrIter!=tempstr+9) *tempstrIter++='0';

		result += tempstr;
	}
	while(temp!=0);

	auto beg = result.begin();
	auto end = result.end()-1;
	while(end>beg && *end=='0') end--;
	result.resize(end+1-beg);

	if(negative && !(arr.size()==1 && arr[0]==0)) result.push_back('-');

	beg = result.begin();
	end = result.end()-1;
	while(beg<end) std::swap(*beg++, *end--);

	return result;
}

std::ostream& operator<<(std::ostream& stream, const BigInt& val)
{
	return stream << val.ToString();
}

std::istream& operator>>(std::istream& stream, BigInt& val)
{
	val = 0;
	bool wasdigit = false;
	bool neg = false;
	for(;;)
	{
		char c;
		stream.get(c);
		if(isdigit(c))
		{
			val = val*10+(c-'0');
			wasdigit = true;
			continue;
		}
		if(!wasdigit)
		{
			if(c=='-')
			{
				neg = !neg;
				continue;
			}
			if(c=='+' || isspace(c)) continue;
		}

		if(neg) val.Negate();
		return stream;
	}
}

int BigInt::abs_compare(Value rhs) const
{
	for(size_t i = arr.size()-1; i>=1; i--)
	{
		const Value leftElement = arr.size()>i? arr[i]: 0;
		if(leftElement>0) return  1;
	}
	return (arr[0]>rhs)-(arr[0]<rhs);
}

int BigInt::abs_compare(const BigInt& rhs) const
{
	for(size_t i = std::max(arr.size(), rhs.arr.size())-1; i>=1; i--)
	{
		const Value leftElement = arr.size()>i? arr[i]: 0;
		const Value rightElement = rhs.arr.size()>i? rhs.arr[i]: 0;
		if(leftElement<rightElement) return -1;
		if(leftElement>rightElement) return  1;
	}
	return (arr[0]>rhs.arr[0])-(arr[0]<rhs.arr[0]);
}

int BigInt::Compare(const BigInt& rhs) const
{
	if(this->arr.size()==1 && rhs.arr.size()==1 && rhs.arr[0]==this->arr[0]) return 0; //+0 == -0
	if(negative && !rhs.negative) return -1;
	if(!negative && rhs.negative) return 1;

	const int sign = negative? -1: 1;
	if(arr.size()<rhs.arr.size()) return -sign;
	if(arr.size()>rhs.arr.size()) return sign;
	return sign*abs_compare(rhs);
}

int BigInt::Compare(Value rhs) const
{
	if(arr.size()==1 && arr[0]==rhs) return 0; //+0 == -0
	if(negative && rhs>0) return -1;
	if(!negative && rhs<0) return 1;

	const int sign=negative? -1: 1;
	if(arr.size()>1) return sign;
	return sign*abs_compare(rhs);
}

BigInt BigInt::abs_add(const BigInt& rhs) const
{
	const size_t lsize = arr.size();
	const size_t rsize = rhs.arr.size();
	const size_t maxsize = std::max(lsize, rsize);
	BigInt result;
	result.arr.resize(maxsize);
	bool carryOut=0;
	for(size_t i=0; i<maxsize; i++)
	{
		const Value left = i<lsize? arr[i]: 0;
		const Value right = i<rsize? rhs.arr[i]: 0;
		const Value sum = left+right+carryOut;
		carryOut = (sum<left || carryOut && sum==left);
		result.arr[i] = sum;
	}
	if(carryOut) result.arr.push_back(1);
	return result;
}

BigInt BigInt::abs_subtract(const BigInt& rhs) const
{
	assert(abs_compare(rhs)>0);
	bool carryOut=0;
	const size_t lsize = arr.size();
	const size_t rsize = rhs.arr.size();
	const size_t maxsize = std::max(lsize, rsize);
	BigInt result;
	result.arr.resize(maxsize);
	for(size_t i=0; i<maxsize; i++)
	{
		const Value left = i<lsize? arr[i]: 0;
		const Value right = i<rsize? rhs.arr[i]: 0;
		const Value diff = left-right-carryOut;
		carryOut = (diff>left || diff==left && carryOut);
		result.arr[i]=diff;
	}
	assert(!carryOut);
	result.trim_size();
	return result;
}

void BigInt::trim_size()
{
	auto size=arr.size();
	while(size>1 && arr[size-1]==0) size--;
	arr.resize(size);
}




BigInt pow(BigInt x, size_t y)
{
	BigInt z=1;
    while(y!=0)
	{
		if(y&1) z*=x;
		x*=x;
		y/=2;
	}
	return z;
}

BigInt factorial(size_t n)
{
	BigInt result=1;
	size_t val=1;
	for(size_t i=2; i<=n; i++)
	{
		if(((int64_t)val*i) >> 32)
			result*=val, val=1;
		val*=i;
	}
	result *= val;
	return result;
}

BigInt sqrt(const BigInt& a)
{
	assert(!a.negative);
	if(a.arr.size()<=1) return (BigInt::Value)sqrt((double)a.arr[0]);

	BigInt x;
	x.arr.resize(a.arr.size()/2);
	x.arr.back() = (1 << (sizeof(BigInt::Value)*8/2)); //initial approximation

	for(int i=0; ; i++)
	{
		BigInt xnext = (x+a/x)/2;
		if(xnext>=x && i>2) return x;
		x = std::move(xnext);
	}
}
