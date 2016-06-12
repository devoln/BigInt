#include "BigInt.h"
#include <string>

#define DLLCALL _declspec(dllexport)

extern "C"
{
	DLLCALL BigInt* BigInt_CreateFromString(const char* str) {return new BigInt(str);}
	DLLCALL BigInt* BigInt_CreateFromInt(long long ll) {return new BigInt(ll);}
	DLLCALL void BigInt_Delete(BigInt* bi) {delete bi;}
	DLLCALL BigInt* BigInt_Add(const BigInt* lhs, const BigInt* rhs) {return new BigInt(*lhs + *rhs);}
	DLLCALL BigInt* BigInt_Multiply(const BigInt* lhs, const BigInt* rhs) {return new BigInt(*lhs * *rhs);}
	DLLCALL BigInt* BigInt_Power(const BigInt* lhs, const BigInt* rhs) {return new BigInt(pow(*lhs, rhs->ToUInt()));}
	DLLCALL BigInt* BigInt_Subtract(const BigInt* lhs, const BigInt* rhs) {return new BigInt(*lhs - *rhs);}
	DLLCALL BigInt* BigInt_Divide(const BigInt* lhs, const BigInt* rhs, BigInt* mod) {return new BigInt(lhs->divide(*rhs, mod));}
	DLLCALL BigInt* BigInt_ChangeSign(const BigInt* val) {return new BigInt(-*val);}
	DLLCALL BigInt* BigInt_Sqrt(const BigInt* val) {return new BigInt(sqrt(*val));}
	DLLCALL int BigInt_Compare(const BigInt* lhs, const BigInt* rhs) {return lhs->compare(*rhs);}
	DLLCALL std::string* BigInt_ToString(const BigInt* lhs, const char** pStr)
	{
		std::string* result=new std::string(lhs->ToString());
		*pStr=result->c_str();
		return result;
	}
	DLLCALL void BigInt_FreeString(std::string* str) {delete str;}
}
