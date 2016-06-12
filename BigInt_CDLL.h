#pragma once

#define DLLCALL _declspec(dllimport)

struct BigInt;
struct StringData;

#ifdef __cplusplus
extern "C"
{
#endif
	DLLCALL BigInt* BigInt_CreateFromString(const char* str);
	DLLCALL BigInt* BigInt_CreateFromInt(long long ll);
	DLLCALL void BigInt_Delete(BigInt* bi);
	DLLCALL BigInt* BigInt_Add(const BigInt* lhs, const BigInt* rhs);
	DLLCALL BigInt* BigInt_Multiply(const BigInt* lhs, const BigInt* rhs);
	DLLCALL BigInt* BigInt_Power(const BigInt* lhs, const BigInt* rhs);
	DLLCALL BigInt* BigInt_Subtract(const BigInt* lhs, const BigInt* rhs);
	DLLCALL BigInt* BigInt_Divide(const BigInt* lhs, const BigInt* rhs, BigInt* mod);
	DLLCALL BigInt* BigInt_ChangeSign(const BigInt* val);
	DLLCALL BigInt* BigInt_Sqrt(const BigInt* val);
	DLLCALL int BigInt_Compare(const BigInt* lhs, const BigInt* rhs);
	DLLCALL StringData* BigInt_ToString(const BigInt* lhs, const char** pStr);
	DLLCALL void BigInt_FreeString(StringData* str);
#ifdef __cplusplus
}
#endif

