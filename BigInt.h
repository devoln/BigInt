#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <assert.h>
#include <stdint.h>

class BigInt
{
public:
	typedef uint32_t Value;
	typedef uint64_t LongValue;
	typedef int32_t SignedValue;
	typedef int64_t SignedLongValue;

private:
	std::vector<Value> arr;
	bool negative;

public:
	BigInt(Value val=0);
	BigInt(SignedValue val);
	BigInt(LongValue val);
	BigInt(SignedLongValue val);
	explicit BigInt(const std::string& s);

	BigInt(const BigInt& rhs);
	BigInt(BigInt&& rhs);


	BigInt operator+(const BigInt& rhs) const;

	BigInt operator-(const BigInt& rhs) const {return *this+(-rhs);}
	BigInt& operator+=(const BigInt& rhs) {return *this=*this+rhs;}
	BigInt& operator-=(const BigInt& rhs) {return *this=*this-rhs;}

	BigInt operator*(const BigInt& rhs) const;
	BigInt& operator*=(const BigInt& rhs) {return *this=*this*rhs;}

	BigInt divide(Value rhs, Value* mod) const;
	BigInt divide(const BigInt& rhs, BigInt* mod) const;



	BigInt operator/(Value rhs) const {return divide(rhs, nullptr);}
	BigInt& operator/=(Value rhs) {return *this=*this/rhs;}
	BigInt operator/(const BigInt& rhs) const {return divide(rhs, nullptr);}
	BigInt& operator/=(const BigInt& rhs) {return *this=*this/rhs;}

	Value operator%(Value rhs) const {Value mod; divide(rhs, &mod); return mod;}
	BigInt operator%(const BigInt& rhs) const {BigInt mod; divide(rhs, &mod); return mod;}
	BigInt& operator%=(Value rhs) {return *this=BigInt(*this%rhs);}
	BigInt& operator%=(const BigInt& rhs) {return *this=*this%rhs;}


	bool operator==(const BigInt& rhs) const {return Compare(rhs)==0;}
	bool operator!=(const BigInt& rhs) const {return Compare(rhs)!=0;}
	bool operator>=(const BigInt& rhs) const {return Compare(rhs)>=0;}
	bool operator<=(const BigInt& rhs) const {return Compare(rhs)<=0;}
	bool operator>(const BigInt& rhs) const {return Compare(rhs)>0;}
	bool operator<(const BigInt& rhs) const {return Compare(rhs)<0;}

	bool operator==(Value rhs) const {return Compare(rhs)==0;}
	bool operator!=(Value rhs) const {return Compare(rhs)!=0;}
	bool operator>=(Value rhs) const {return Compare(rhs)>=0;}
	bool operator<=(Value rhs) const {return Compare(rhs)<=0;}
	bool operator>(Value rhs) const {return Compare(rhs)>0;}
	bool operator<(Value rhs) const {return Compare(rhs)<0;}

	BigInt operator-() const {BigInt result=*this; result.negative=!negative; return result;}
	BigInt& operator++() {return *this = *this+1;}
	BigInt& operator--() {return *this = *this-1;}
	BigInt operator++(int) {auto result = *this; ++*this; return result;}
	BigInt operator--(int) {auto result = *this; --*this; return result;}

	BigInt& operator=(const BigInt& rhs) {arr=rhs.arr; negative=rhs.negative; return *this;}
	BigInt& operator=(BigInt&& rhs) {arr=std::move(rhs.arr); negative=rhs.negative; return *this;}

	void Negate() {negative = !negative;}
	

	Value ToUInt() const {return arr.size()!=0? arr[0]: 0;}

	SignedValue ToInt() const
	{
		SignedValue result = SignedValue(ToUInt());
		if(negative) return -result;
		return result;
	}

	std::string ToString() const;

	int Compare(const BigInt& rhs) const;
	int Compare(Value rhs) const;

private:
	void inc_digit(size_t i) {while(++arr[i]==0) i++;}

	BigInt abs_add(const BigInt& rhs) const;
	BigInt abs_subtract(const BigInt& rhs) const;
	int abs_compare(Value rhs) const;
	int abs_compare(const BigInt& rhs) const;

	void trim_size();


	friend BigInt sqrt(const BigInt& a);
	friend BigInt abs(BigInt x);
};

std::ostream& operator<<(std::ostream& stream, const BigInt& val);
std::istream& operator>>(std::istream& stream, BigInt& val);

inline BigInt operator*(BigInt::Value lhs, const BigInt& rhs) {return rhs*lhs;}
inline BigInt operator*(BigInt::SignedValue lhs, const BigInt& rhs) {return rhs*lhs;}
inline BigInt operator*(BigInt::LongValue lhs, const BigInt& rhs) {return rhs*lhs;}
inline BigInt operator*(BigInt::SignedLongValue lhs, const BigInt& rhs) {return rhs*lhs;}

inline BigInt operator-(BigInt::Value lhs, const BigInt& rhs) {return BigInt(lhs)-rhs;}
inline BigInt operator-(BigInt::SignedValue lhs, const BigInt& rhs) {return BigInt(lhs)-rhs;}
inline BigInt operator-(BigInt::LongValue lhs, const BigInt& rhs) {return BigInt(lhs)-rhs;}
inline BigInt operator-(BigInt::SignedLongValue lhs, const BigInt& rhs) {return BigInt(lhs)-rhs;}

inline BigInt operator+(BigInt::Value lhs, const BigInt& rhs) {return BigInt(lhs)-rhs;}
inline BigInt operator+(BigInt::SignedValue lhs, const BigInt& rhs) {return BigInt(lhs)-rhs;}
inline BigInt operator+(BigInt::LongValue lhs, const BigInt& rhs) {return BigInt(lhs)-rhs;}
inline BigInt operator+(BigInt::SignedLongValue lhs, const BigInt& rhs) {return BigInt(lhs)-rhs;}

inline BigInt operator/(BigInt::Value lhs, const BigInt& rhs) {return BigInt(lhs)/rhs;}
inline BigInt operator/(BigInt::SignedValue lhs, const BigInt& rhs) {return BigInt(lhs)/rhs;}
inline BigInt operator/(BigInt::LongValue lhs, const BigInt& rhs) {return BigInt(lhs)/rhs;}
inline BigInt operator/(BigInt::SignedLongValue lhs, const BigInt& rhs) {return BigInt(lhs)/rhs;}

inline BigInt operator%(BigInt::Value lhs, const BigInt& rhs) {return BigInt(lhs)%rhs;}
inline BigInt operator%(BigInt::SignedValue lhs, const BigInt& rhs) {return BigInt(lhs)%rhs;}
inline BigInt operator%(BigInt::LongValue lhs, const BigInt& rhs) {return BigInt(lhs)%rhs;}
inline BigInt operator%(BigInt::SignedLongValue lhs, const BigInt& rhs) {return BigInt(lhs)%rhs;}

inline BigInt abs(BigInt x) {x.negative = false; return x;}

BigInt pow(BigInt x, size_t y);
BigInt factorial(size_t n);
BigInt sqrt(const BigInt& a);
