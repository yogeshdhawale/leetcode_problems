/*
1704. Determine if String Halves Are Alike
Easy
Topics
Companies
Hint

You are given a string s of even length. Split this string into two halves of equal lengths, and let a be the first half and b be the second half.

Two strings are alike if they have the same number of vowels ('a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U'). Notice that s contains uppercase and lowercase letters.

Return true if a and b are alike. Otherwise, return false.



Example 1:

Input: s = "book"
Output: true
Explanation: a = "bo" and b = "ok". a has 1 vowel and b has 1 vowel. Therefore, they are alike.

Example 2:

Input: s = "textbook"
Output: false
Explanation: a = "text" and b = "book". a has 1 vowel whereas b has 2. Therefore, they are not alike.
Notice that the vowel o is counted twice.


*/
#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>

using namespace std;

#define __DEBUG_LOG 0

template <typename T>
void printVector(vector<T> &nums)
{
    for (size_t i = 0; i < nums.size(); i++)
    {
        cout << nums[i] << ", ";
    }
    cout << endl;
}

void LOG(string msg)
{
    if (__DEBUG_LOG)
        cout << msg << endl;
}

class Solution
{
public:
    bool halvesAreAlike(string s)
    {
        int count = 0;
        int len = s.length();
        for (int i = 0; i < len / 2; i++)
        {
            if (s[i] == 'a' || s[i] == 'e' || s[i] == 'i' || s[i] == 'o' || s[i] == 'u' || s[i] == 'A' || s[i] == 'E' || s[i] == 'I' || s[i] == 'O' || s[i] == 'U')
            {
                count++;
            }
        }
        for (int i = len / 2; i < len; i++)
        {
            if (s[i] == 'a' || s[i] == 'e' || s[i] == 'i' || s[i] == 'o' || s[i] == 'u' || s[i] == 'A' || s[i] == 'E' || s[i] == 'I' || s[i] == 'O' || s[i] == 'U')
            {
                count--;
            }
        }

        return count == 0 ? true : false;
    }
};

void test1(int testID)
{
    Solution s;
    cout << "test" << testID << endl;
    auto input = vector<string>({"book", "textbook"});

    auto expected = vector<bool>({true, false});

    for (size_t i = 0; i < input.size(); i++)
    {
        cout << "test for: " << input[i] << endl;
        auto result = s.halvesAreAlike(input[i]);
        cout << result << " " << expected[i] << endl;
        assert(result == expected[i]);
    }
}

int main()
{

    cout << "--- " << endl;
    test1(1);
    // test2(2);
    // test3(3);

    return 0;
}