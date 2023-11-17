/*

6. Zigzag Conversion
Medium

3991

9028

Add to List

Share
The string "PAYPALISHIRING" is written in a zigzag pattern on a given number of rows like this: (you may want to display this pattern in a fixed font for better legibility)

P   A   H   N
A P L S I I G
Y   I   R
And then read line by line: "PAHNAPLSIIGYIR"

Write the code that will take a string and make this conversion given a number of rows:

string convert(string s, int numRows);


Example 1:

Input: s = "PAYPALISHIRING", numRows = 3
Output: "PAHNAPLSIIGYIR"
Example 2:

Input: s = "PAYPALISHIRING", numRows = 4
Output: "PINALSIGYAHRPI"
Explanation:
P     I    N
A   L S  I G
Y A   H R
P     I
Example 3:

Input: s = "A", numRows = 1
Output: "A"


Constraints:

1 <= s.length <= 1000
s consists of English letters (lower-case and upper-case), ',' and '.'.
1 <= numRows <= 1000
*/

#include <iostream>
#include <vector>
#include <string>
#include<sstream>

using namespace std;

class Solution
{

    vector<stringstream> data;

public:
    string convert(string s, int numRows)
    {
        data.resize(numRows);
        int tsize = s.length();

        int i=0;
        while (i < tsize)
        {
            int j = 0;
            for (; j < numRows && i < tsize; j++)
            {
                data[j] << s[i];
                i++;
            }
            j -= 2;
            for (; j > 0 && i < tsize; j--)
            {
                data[j] << s[i];
                i++;
            }
        }

        string result;
        for (int i = 0; i < numRows; i++)
            result += data[i].str();
        return result;
    }
};

int main()
{
    // if (false)
    {
        Solution s;

        cout << "Test 1:" << endl;
        string result = s.convert("PAYPALISHIRING", 4);
        cout << "Result is (expected=PINALSIGYAHRPI): " << result << endl;
    }
}