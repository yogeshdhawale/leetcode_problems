
/*
65. Valid Number
Hard
Topics
Companies

Given a string s, return whether s is a valid number.

For example, all the following are valid numbers: "2", "0089", "-0.1", "+3.14", "4.", "-.9", "2e10", "-90E3", "3e+7", "+6e-1", "53.5e93", "-123.456e789", while the following are not valid numbers: "abc", "1a", "1e", "e3", "99e2.5", "--6", "-+3", "95a54e53".

Formally, a valid number is defined using one of the following definitions:

    An integer number followed by an optional exponent.
    A decimal number followed by an optional exponent.

An integer number is defined with an optional sign '-' or '+' followed by digits.

A decimal number is defined with an optional sign '-' or '+' followed by one of the following definitions:

    Digits followed by a dot '.'.
    Digits followed by a dot '.' followed by digits.
    A dot '.' followed by digits.

An exponent is defined with an exponent notation 'e' or 'E' followed by an integer number.

The digits are defined as one or more digits.



Example 1:

Input: s = "0"

Output: true

Example 2:

Input: s = "e"

Output: false

Example 3:

Input: s = "."

Output: false



Constraints:

    1 <= s.length <= 20
    s consists of only English letters (both uppercase and lowercase), digits (0-9), plus '+', minus '-', or dot '.'.


*/

#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <map>
#include <queue>
#include <set>

#include <numeric>
#include <chrono>
#include <string_view>
#include <ranges>

#include "log.h"

using namespace std;

/****************************************************************************/

#include <ranges>

#ifndef __DEBUG_LOG
#define LOG(x)
#define VLOG(x)
#endif

#define INVALID -1
#define RESULT_MOD 1000000007
#define MY_MAX_VAL INT32_MAX

typedef vector<int> V1Int;
typedef vector<V1Int> V2Int;
typedef pair<int, int> IDX2D;

typedef bool OUTPUT_TYPE;

static int inputID = 1;
struct Input
{
    int id;
    string nums;

    Input() {
        id = inputID++;
    }
    Input(string nums) {
        this->nums = nums;
        id = inputID++;
    }
    /*Input(int max) {
        nums.resize(max);
        int i = 0;
        for_each(nums.begin(), nums.end(), [&](int& item) { ++i; item += i;});
        id = inputID++;
    }*/
};



struct Data {

    void clear() {
    }
    Data() {
        clear();
    }
};

ostream& operator<<(ostream& out, const Data& d);


enum State {
    SIGN = 1,
    SIMPLE_NUMBER = 2,
    DOT = 3,
    FRACTION = 4,
    EXPONENT_CHAR = 5,
    EXPONENT_SIGN = 6,
    END = 7
};

class Checker {

public:
    bool isValidDigit(const char c) {
        return c >= '0' && c <= '9';
    }

    bool isValidSign(const char c) {
        return c == '+' || c == '-';
    }

    bool isValidDot(const char c) {
        return c == '.';
    }

    bool isValidExponent(const char c) {
        return c == 'e' || c == 'E';
    }
};
class Solution {
public:

    bool isNumber(string s) {

        if (s.empty()) return false;

        size_t i = 0;
        State st = SIGN;

        Checker ck;

        if (ck.isValidSign(s[i])) i++;
        if (ck.isValidExponent(s[i])) return false;

        for (;i < s.size();i++) {

            LOG(s[i] << " " << st);
            auto c = s[i];

            switch (st) {

            case SIGN:
                if (ck.isValidSign(c)) return false;
                if (ck.isValidDigit(c)) {
                    st = SIMPLE_NUMBER;
                }
                else if (ck.isValidDot(c)) {
                    st = DOT;
                }
                else return false;
                break;

            case SIMPLE_NUMBER:
                if (ck.isValidSign(c)) return false;

                if (ck.isValidDigit(c)) {
                    st = SIMPLE_NUMBER;
                }
                else if (ck.isValidDot(c)) {
                    st = FRACTION;
                }
                else if (ck.isValidExponent(c)) {
                    st = EXPONENT_CHAR;
                }
                else return false;
                break;

            case DOT:
                if (!ck.isValidDigit(c)) return false;
                st = FRACTION;
                break;

            case FRACTION:
                if (ck.isValidDot(c) || ck.isValidSign(c)) return false;

                if (ck.isValidExponent(c)) {
                    st = EXPONENT_CHAR;
                }
                else if (!ck.isValidDigit(c)) {
                    return false;
                }
                break;

            case EXPONENT_CHAR:
                if (!ck.isValidSign(c)) {
                    i--;
                }
                st = EXPONENT_SIGN;
                break;

            case EXPONENT_SIGN:
                if (!ck.isValidDigit(c)) return false;
                st = END;
                break;
            case END:
                if (!ck.isValidDigit(c)) return false;
                break;
            }
        }

        if (st == SIGN || st == DOT || st == EXPONENT_CHAR || st == EXPONENT_SIGN)
            return false;
        return true;

    }

    OUTPUT_TYPE ProxyFunction(Input& nums) {
        return isNumber(nums.nums);
    }
};


/****************************************************************************/

ostream& operator<<(ostream& out, const Input& nums) {
    out << "id:" << nums.id;
    out << " nums:" << nums.nums;
    return out;
}

/* ostream& operator<<(ostream& out, const Data* obj) {
    return out << *obj;
} */
ostream& operator<<(ostream& out, const Data& d) {
    return out;
}

/****************************************************************************/

void logtime(string msg, std::chrono::_V2::system_clock::time_point starttime, std::chrono::_V2::system_clock::time_point endtime) {
    cout << TEXT_LOG_PREFIX << msg
        << std::chrono::duration_cast<std::chrono::microseconds>(endtime - starttime).count() << "us "
        << std::chrono::duration_cast<std::chrono::milliseconds>(endtime - starttime).count() << "ms "
        << std::chrono::duration_cast<std::chrono::seconds>(endtime - starttime).count() << "s "
        << endl;
}

void test1(int testID)
{
    vector<string> nums{
        "2", "0089", "-0.1", "+3.14", "4.", "-.9", "2e10", "-90E3", "3e+7", "+6e-1", "53.5e93", "-123.456e789",
        "abc", "1a", "1e", "e3", "99e2.5", "--6", "-+3", "95a54e53",
         ".", "-.", ".e", "1e", "te1"
    };
    vector<OUTPUT_TYPE> expected{
         true, true, true, true, true, true, true, true, true, true, true, true,
        false, false, false, false, false, false, false, false,
        false,false,false, false, false
    };

    /*     nums.push_back(Input("0"));
        expected.push_back(true);

        nums.push_back(Input("-1e2"));
        expected.push_back(true);

        nums.push_back(Input("-1e-2"));
        expected.push_back(true); */

    cout << endl << TEXT_LOG_PREFIX << "test : " << testID << " - (" << nums.size() << ") " << endl;

#if defined(__DEBUG_LOG) && __DEBUG_LOG == __DEBUG_LOG_VB
    for (size_t i = __D_CASE_ID; i <= __D_CASE_ID; i++)
#else
    for (size_t i = 0; i < nums.size(); i++)
#endif
    {
        cout << TEXT_LOG_PREFIX << "test for: loop: [" << i << "] => " << nums[i] << endl;
        Solution s;

        auto starttime = std::chrono::high_resolution_clock::now();
        Input input = Input(nums[i]);
        auto result = s.ProxyFunction(input);
        auto endtime = std::chrono::high_resolution_clock::now();

        cout << TEXT_LOG_PREFIX << "test loop: " << i << " ==> " << result << " vs e:" << expected[i] << endl;
        logtime("*****Time: ", starttime, endtime);
        cout << endl;
        assert(result == expected[i]);
    }
}

#if defined(__DEBUG_LOG) && __DEBUG_LOG >= __DEBUG_LOG_BIG

#include "data.h"

void test2(int testID)
{

    cout << TEXT_LOG_PREFIX << "test" << testID << endl;
    vector<Input> nums;
    vector<OUTPUT_TYPE> expected;

    nums.push_back(Input(d1));
    expected.push_back(o1);

    cout << TEXT_LOG_PREFIX << "test for: " << nums << endl;

    Solution s;
    auto starttime = std::chrono::high_resolution_clock::now();
    auto result = s.ProxyFunction(nums[0]);
    auto endtime = std::chrono::high_resolution_clock::now();
    cout << TEXT_LOG_PREFIX << "test: " << testID << " ==> " << result << " vs e:" << expected[0] << endl;
    logtime("*****Time: ", starttime, endtime);
    cout << endl;

    assert(result == expected[0]);
}
#endif

int main()
{
    cout << "--- " << endl;
#if !defined(__DEBUG_LOG) || __DEBUG_LOG <= __DEBUG_LOG_VB
    test1(1);
#endif
#if defined(__DEBUG_LOG) && __DEBUG_LOG >= __DEBUG_LOG_BIG
    test2(2);
#endif

    return 0;
}


