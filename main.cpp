#include "BigInt.h"

#include <iostream>
using namespace std;

int main()
{
	BigInt A, B;
	size_t C;
	cout << "Enter A, B and C:" << endl;
	cin >> A >> B >> C;
	cout << "A = " << A << ", B = " << B << ", C = " << C << endl;
	cout << "A + 1 = " << A+1 << endl;
	cout << "A + B = " << A+B << endl;
	cout << "A - B = " << A-B << endl;
	cout << "B - A = " << B-A << endl;
	cout << "A * B = " << A*B << endl;
	cout << "A / B = " << A/B << endl;
	cout << "A mod B = " << A%B << endl;
	cout << "B / A = " << B/A << endl;
	cout << "B mod A = " << B%A << endl;
	cout << "log10(A^C) = " << pow(A, C).ToString().size() << endl;
	cout << "sqrt(A) = " << sqrt(A) << endl;
	auto fact = factorial(C).ToString();
	cout << "log10(C!) = " << fact.size() << endl;
	cout << "C! = " << fact << endl;

	return 0;
}
