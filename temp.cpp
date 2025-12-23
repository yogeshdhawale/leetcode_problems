
#include <string>
#include<iostream>
#include<cmath>

using namespace std;


int mod;

struct modNumber {
    int value;
    int modCnt;
    modNumber(int v) {
        value = v % mod;
        modCnt = v / mod;
    }
    void mult(const modNumber& other) {
        modCnt = mod * modCnt * other.modCnt
            + value * other.modCnt
            + modCnt * other.value
            + (value * other.value) / mod;
        value = (value * other.value) % mod;
    }
    void power(int p) {
        modNumber org = *this;
        while (--p > 0) {
            mult(org);
        }
    }

    void square() {
        modCnt = mod * pow(modCnt, 2)
            + 2 * value * modCnt
            + pow(value, 2) / mod;
        value = (int)pow(value, 2) % mod;
    }
    void cube() {
        modCnt = mod * pow(modCnt, 3)
            + 3 * pow(modCnt, 2) * value
            + 3 * modCnt * pow(value, 2)
            + pow(value, 3) / mod;
        value = (int)pow(value, 3) % mod;
    }
};

ostream& operator<<(ostream& out, const modNumber& num) {
    out << num.value << ":" << num.modCnt;
    return out;
}
int main()
{
    {
        cout << endl << "Test modNumber multiply" << endl;
        mod = 5;
        modNumber a(3);
        modNumber b(4);
        cout << "a: " << a << endl;
        cout << "b: " << b << endl;
        a.mult(b);
        modNumber e(3 * 4);
        cout << "a * b = e:" << e << " vs " << a << endl;
    }
    {
        cout << endl << "Test modNumber square" << endl;
        mod = 5;
        int curNumber = 22;
        modNumber a(curNumber);
        cout << "a: " << a << endl;
        a.square();
        cout << curNumber << "^2=" << pow(curNumber, 2) << "\te:" << modNumber(pow(curNumber, 2)) << " vs " << a << endl;
        a.mult(modNumber(curNumber));
        cout << curNumber << "^3=" << pow(curNumber, 3) << "\te:" << modNumber(pow(curNumber, 3)) << " vs " << a << endl;

        modNumber b(curNumber);
        b.square();
        b.square();
        cout << curNumber << "^4=" << pow(curNumber, 4) << "\te:" << modNumber(pow(curNumber, 4)) << " vs " << b << endl;
    }
    {
        cout << endl << "Test modNumber cube" << endl;
        mod = 5;
        int curNumber = 22;
        modNumber a(curNumber);
        cout << "a: " << a << endl;
        a.cube();
        cout << curNumber << "^3=" << pow(curNumber, 3) << "\te:" << modNumber(pow(curNumber, 3)) << " vs " << a << endl;
    }
    {
        cout << endl << "Test modNumber power" << endl;
        mod = 5;
        int curNumber = 22;
        modNumber a(curNumber);
        cout << "a: " << a << endl;

        for (int i = 1;i < 5;i++) {
            auto b = modNumber(22);
            auto e = modNumber(pow(22, i));
            b.power(i);
            cout << curNumber << "^" << i << "=" << pow(curNumber, i) << "\te:" << e << " vs " << b << endl;
        }

    }


    cout << endl << "done" << endl;
    return 0;
}

