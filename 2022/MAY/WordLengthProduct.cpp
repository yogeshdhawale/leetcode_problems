/*

318. Maximum Product of Word Lengths
Medium

Given a string array words, return the maximum value of length(word[i]) * length(word[j]) where the two words do not share common letters. If no such two words exist, return 0.



Example 1:

Input: words = ["abcw","baz","foo","bar","xtfn","abcdef"]
Output: 16
Explanation: The two words can be "abcw", "xtfn".

Example 2:

Input: words = ["a","ab","abc","d","cd","bcd","abcd"]
Output: 4
Explanation: The two words can be "ab", "cd".

Example 3:

Input: words = ["a","aa","aaa","aaaa"]
Output: 0
Explanation: No such pair of words.



Constraints:

    2 <= words.length <= 1000
    1 <= words[i].length <= 1000
    words[i] consists only of lowercase English letters.


*/

#include <iostream>
#include <vector>

using namespace std;

class WordLengthProduct
{
private:
    vector<string> &data;
    vector<int> flags;
    vector<int> lengths;

public:
    WordLengthProduct(vector<string> &words) : data(words)
    {
        lengths.resize(data.size());
        flags.resize(data.size());
    }

    void process()
    {
        for (int i = 0; i < data.size(); i++)
        {
            lengths[i] = data[i].length();
            for (auto s : data[i])
            {
                flags[i] |= 1 << (s - 'a');
            }
        }
    }
    int getMaxProduct()
    {
        int result = 0;
        for (int i = 0; i < data.size(); i++)
        {
            int max_size = 0;
            for (int j = i + 1; j < data.size(); j++)
            {
                if (flags[i] & flags[j])
                    continue;

                if (max_size < lengths[j])
                    max_size = lengths[j];
            }
            int pr = lengths[i] * max_size;
            if (pr > result)
                result = pr;
        }
        return result;
    }
};

class Solution
{

public:
    int maxProduct(vector<string> &words)
    {
        WordLengthProduct w(words);
        w.process();
        return w.getMaxProduct();
    }
};

int main()
{
    // if (false)
    {
        Solution s;
        vector<string> words = {"abcw", "baz", "foo", "bar", "xtfn", "abcdef"};
        int result = s.maxProduct(words);
        cout << "Result is (exp:16) actual: " << result << endl;
    }
}