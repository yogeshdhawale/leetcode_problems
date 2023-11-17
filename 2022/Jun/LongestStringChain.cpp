/*
1048. Longest String Chain
Medium

You are given an array of words where each word consists of lowercase English letters.

wordA is a predecessor of wordB if and only if we can insert exactly one letter anywhere in wordA without changing the order of the other characters to make it equal to wordB.

    For example, "abc" is a predecessor of "abac", while "cba" is not a predecessor of "bcad".

A word chain is a sequence of words [word1, word2, ..., wordk] with k >= 1, where word1 is a predecessor of word2, word2 is a predecessor of word3, and so on. A single word is trivially a word chain with k == 1.

Return the length of the longest possible word chain with words chosen from the given list of words.



Example 1:

Input: words = ["a","b","ba","bca","bda","bdca"]
Output: 4
Explanation: One of the longest word chains is ["a","ba","bda","bdca"].

Example 2:

Input: words = ["xbc","pcxbcf","xb","cxbc","pcxbc"]
Output: 5
Explanation: All the words can be put in a word chain ["xb", "xbc", "cxbc", "pcxbc", "pcxbcf"].

Example 3:

Input: words = ["abcd","dbqca"]
Output: 1
Explanation: The trivial word chain ["abcd"] is one of the longest word chains.
["abcd","dbqca"] is not a valid word chain because the ordering of the letters is changed.



Constraints:

    1 <= words.length <= 1000
    1 <= words[i].length <= 16
    words[i] only consists of lowercase English letters.



 */

#include <iostream>
#include <vector>

using namespace std;

class Node
{
    char element;
    bool isWord[26];
    Node *next[26];

public:
    int level;

    Node(char c, int l)
    {
        element = c;
        level = l;
        for (int i = 0; i < 26; i++)
        {
            isWord[i] = false;
            next[i] = nullptr;
        }
    }

    Node *SetNextChar(char x, int l)
    {
        int index = x - 'a';
        if (next[index] == nullptr)
            next[index] = new Node(x, l);
        return next[index];
    }
    void SetWord(char x)
    {
        isWord[x - 'a'] = true;
    }
    bool CheckNextChar(char x)
    {
        int index = x - 'a';
        if (next[index] == nullptr)
            return false;
        return true;
    }
    Node * getNextNodePtr(char c)
    {
        return next[c-'a'];
    }
};

class StringData
{
private:
    // root is placeholder that stores 1st letter for each word, its Node.element is not used
    Node *root;
    vector<vector<Node *>> levelData;

public:
    StringData(vector<string> &words)
    {
        levelData.resize(16);
        root = new Node('.', 0);
        levelData[0].push_back(root);
        processData(words);
    }
    ~StringData()
    {
        for (int i = 0; i < levelData.size(); i++)
            for (int j = 0; i < levelData[i].size(); j++)
                delete levelData[i][j];
    }

    void processData(vector<string> &words)
    {
        for (auto word : words)
        {
            Node *prevNode = root;
            Node *curNode = prevNode;
            int i=0;
            for ( ; i < word.length(); i++)
            {
                prevNode = curNode;
                bool isCharNodePresent = curNode->CheckNextChar(word[i]);
                curNode = curNode->SetNextChar(word[i], i);
                if (!isCharNodePresent)
                    levelData[i].push_back(prevNode);
            }
            prevNode->SetWord(i-1);
        }
    }

    int findString(string & s)
    {
        Node * node = findStringNode(s);
        return node->level;
    }
    Node *findStringNode(string & s)
    {
        Node *curNode = root->getNextNodePtr(s[0]);
        if (curNode == nullptr)
            return nullptr;

        Node * nextNode;
        for (int i = 0; i < s.length(); i++)
        {
            nextNode = curNode->getNextNodePtr(s[i]);
            if(nextNode == nullptr)
                return curNode;
            curNode = nextNode;
        }
        return curNode;
    }
    int getResult()
    {
        return 0;
    }
};

class Solution
{
public:
    int longestStrChain(vector<string> &words)
    {
        StringData data(words);
        return data.getResult();
    }
};

int main()
{
    if (true)
    {
        Solution s;

        vector<string> data =
            {"a", "b", "ba", "bca", "bda", "bdca"};
        cout << "Test 1" << endl;
        int result = s.longestStrChain(data);
        cout << "Result is (expected=4): " << result << endl;
    }
}