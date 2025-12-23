
/*
Q2. Stream of Characters
Attempted
Hard
Topics
premium lock iconCompanies
Hint

Design an algorithm that accepts a stream of characters and checks if a suffix of these characters is a string of a given array of strings words.

For example, if words = ["abc", "xyz"] and the stream added the four characters (one by one) 'a', 'x', 'y', and 'z', your algorithm should detect that the suffix "xyz" of the characters "axyz" matches "xyz" from words.

Implement the StreamChecker class:

    StreamChecker(String[] words) Initializes the object with the strings array words.
    boolean query(char letter) Accepts a new character from the stream and returns true if any non-empty suffix from the stream forms a word that is in words.



Example 1:

Input
["StreamChecker", "query", "query", "query", "query", "query", "query", "query", "query", "query", "query", "query", "query"]
[[["cd", "f", "kl"]], ["a"], ["b"], ["c"], ["d"], ["e"], ["f"], ["g"], ["h"], ["i"], ["j"], ["k"], ["l"]]
Output
[null, false, false, false, true, false, true, false, false, false, false, false, true]

Explanation
StreamChecker streamChecker = new StreamChecker(["cd", "f", "kl"]);
streamChecker.query("a"); // return False
streamChecker.query("b"); // return False
streamChecker.query("c"); // return False
streamChecker.query("d"); // return True, because 'cd' is in the wordlist
streamChecker.query("e"); // return False
streamChecker.query("f"); // return True, because 'f' is in the wordlist
streamChecker.query("g"); // return False
streamChecker.query("h"); // return False
streamChecker.query("i"); // return False
streamChecker.query("j"); // return False
streamChecker.query("k"); // return False
streamChecker.query("l"); // return True, because 'kl' is in the wordlist



Constraints:

    1 <= words.length <= 2000
    1 <= words[i].length <= 200
    words[i] consists of lowercase English letters.
    letter is a lowercase English letter.
    At most 4 * 104 calls will be made to query.

*/

#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>
#include <set>
#include<list>

#include <numeric>
#include <chrono>
#include <string_view>
#include <ranges>

using namespace std;

//#define __DEBUG_LOG __DEBUG_LOG_ON
#define __DEBUG_BIG_TEST

enum TestOptions {
    TEST_ALL = 10,
    TEST_SINGLE = 11,
};

struct TestConfig {
    TestOptions options;
    int testCaseID;
};
#include "log.h"
#include <unordered_set>
TestConfig gTestConfig(TestOptions::TEST_ALL, 0);
//TestConfig gTestConfig(TestOptions::TEST_SINGLE, 1);
int gCurTestId = -1;

/****************************************************************************/
/****************************************************************************/

#ifndef __DEBUG_LOG
#define LOG(x)
#define LOGV(x)
#endif

#define INVALID -1
#define MY_MAX_VAL INT32_MAX
#define RESULT_MOD 1000000007
#define GET_MOD(x) (x % RESULT_MOD)

typedef vector<int> V1Int;
typedef vector<V1Int> V2Int;

typedef int IDX, VALUEI, IDXR, IDXC;
typedef pair<int, int> IDX2D;
/*---------------------------------------------------------------------------*/
typedef V1Int INPUT_TYPE;
typedef int OUTPUT_TYPE;

static int inputID = 1;
struct TestCaseStruct
{
    int mID;
    vector<string> mData1;
    vector<vector<string>> mData2;
    OUTPUT_TYPE mExpected1;

    TestCaseStruct() {
        mID = inputID++;
    }
    void setInput(vector<string> input1, vector<vector<string>> input2) {
        this->mData1 = input1;
        this->mData2 = input2;
    }
    void setOutput(OUTPUT_TYPE output1) {
        this->mExpected1 = output1;
    }
};

/*---------------------------------------------------------------------------*/


#define SUFFIX_MAX_LEN 200


struct ReverseTrieNode {
    unordered_map<char, ReverseTrieNode*> children;
    bool isStart;

    ReverseTrieNode() : isStart(false) {}

    ReverseTrieNode* get(char c) {
        if (children.find(c) == children.end()) {
            children[c] = new ReverseTrieNode();
        }
        return children[c];
    }

    void setStart() {
        isStart = true;
    }
};

class StreamChecker {
public:

    ReverseTrieNode* endNode;
    list<char> buffer;
    StreamChecker(vector<string>& words) {

        endNode = new ReverseTrieNode();
        buffer.clear();

        LOG("StreamChecker init with words:" << words.size());

        for (auto word : words) {
            ReverseTrieNode* node = endNode;
            for (int i = word.length() - 1; i >= 0; i--) {
                node = node->get(word[i]);
                if (node->isStart) break;
            }
            node->setStart();
        }
    }

    bool query(char letter) {

        bool ret = false;

        buffer.push_front(letter);

        ReverseTrieNode* node = endNode;
        for (char c : buffer) {
            if (node->children.find(c) == node->children.end()) {
                break;
            }
            node = node->children[c];
            if (node->isStart) {
                ret = true;
                break;
            }
        }

        if (buffer.size() > SUFFIX_MAX_LEN) {
            buffer.pop_back();
        }
        return ret;
    }
};

/**
 * Your StreamChecker object will be instantiated and called as such:
 * StreamChecker* obj = new StreamChecker(words);
 * bool param_1 = obj->query(letter);
 */

class Solution {
public:
    int proxyFunction(TestCaseStruct& tc) {

        StreamChecker* obj = nullptr;
        for (size_t i = 0; i < tc.mData1.size(); i++) {
            string cmd = tc.mData1[i];
            if (cmd == "StreamChecker") {
                if (obj != nullptr) delete obj;
                obj = new StreamChecker(tc.mData2[i]);
                continue;
            }
            else if (cmd == "query") {
                bool ret = obj->query(tc.mData2[i][0][0]);
                if (ret)
                    LOG("query(" << tc.mData2[i][0][0] << ") = " << ret);
            }
        }
        return 0;
    }
};

/****************************************************************************/
/****************************************************************************/

ostream& operator<<(ostream& out, const TestCaseStruct& ipObj) {
    out << "{ id:" << ipObj.mID;
    out << " nums:" << ipObj.mData1;
    out << " data2:" << ipObj.mData2;
    out << " }";
    return out;
}

void logtime(string msg, std::chrono::_V2::system_clock::time_point starttime, std::chrono::_V2::system_clock::time_point endtime) {
    cout << __LOGPREFIX << msg
        << std::chrono::duration_cast<std::chrono::microseconds>(endtime - starttime).count() << "us "
        << std::chrono::duration_cast<std::chrono::milliseconds>(endtime - starttime).count() << "ms "
        << std::chrono::duration_cast<std::chrono::seconds>(endtime - starttime).count() << "s "
        << endl;
}

V1Int generateInput(int seed, int size, int max) {
    V1Int nums(size);
    srand(seed);
    for_each(nums.begin(), nums.end(), [&](int& item) {item = rand() % max;});
    return nums;
}


void test1(int testID)
{
    vector<TestCaseStruct> nums;
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { "StreamChecker", "query", "query", "query", "query", "query", "query", "query", "query", "query", "query", "query", "query" },
            { { { "cd", "f", "kl" } }, { "a" }, { "b" }, { "c" }, { "d" }, { "e" }, { "f" }, { "g" }, { "h" }, { "i" }, { "j" }, { "k" }, { "l" } }
        );
        nums.back().setOutput(0);
    }


    cout << endl << __LOGPREFIX << "test:" << testID << " sub-tests count (" << nums.size() << ") " << endl;

    size_t i = 0, size = nums.size();

    if (gTestConfig.options == TestOptions::TEST_SINGLE) {
        i = gTestConfig.testCaseID;
        size = i + 1;
    }

    for (; i < size; i++)
    {
        string strTestId = "test:" + to_string(testID) + "." + to_string(nums[i].mID) + " => ";
        cout << __LOGPREFIX << strTestId << nums[i] << endl;

        gCurTestId = nums[i].mID;
        std::chrono::_V2::system_clock::time_point endtime, starttime = std::chrono::high_resolution_clock::now();

        Solution s;
        auto result = s.proxyFunction(nums[i]);
        endtime = std::chrono::high_resolution_clock::now();

        cout << __LOGPREFIX << strTestId << "returns:" << result << " vs e:" << nums[i].mExpected1 << endl;
        logtime("### Time: ", starttime, endtime);
        cout << endl;
        assert(result == nums[i].mExpected1);
    }
}

#ifdef __DEBUG_BIG_TEST

template <typename T> vector<T> load_data_v1(string fileName);
V2Int load_data_v2(string fileName);
int load_data_int(string fileName);

vector<string> load_data_v1_string(string fileName);
vector<vector<string>> load_data_v2_string(string fileName);

string TEST_DIR = "./test_data/";
void test2(int testID)
{

    cout << __LOGPREFIX << "big test" << testID << endl;

    vector<TestCaseStruct> nums;
    vector<string> testcases_str = { "test1", "test2" };

    for (auto& tc : testcases_str)
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            load_data_v1_string(TEST_DIR + tc + "-i1.txt"),
            load_data_v2_string(TEST_DIR + tc + "-i2.txt")
        );
        nums.back().setOutput(0);
    }

    cout << __LOGPREFIX << "big test for: " << nums.size() << endl;

    for (size_t i = 0; i < nums.size(); i++)
    {
        cout << __LOGPREFIX << "big test: " << testID << "." << nums[i].mID << " ==> " << nums[i] << endl;
        Solution s;
        std::chrono::_V2::system_clock::time_point endtime, starttime = std::chrono::high_resolution_clock::now();
        auto result = s.proxyFunction(nums[i]);
        endtime = std::chrono::high_resolution_clock::now();
        cout << __LOGPREFIX << "big test: " << testID << " ==> " << result << " vs e:" << nums[0].mExpected1 << endl;
        logtime("### Time: ", starttime, endtime);
        cout << endl;
        //assert(result == nums[i].mExpected1);
    }

}
#endif

int main()
{
    cout << "^^^Start^^^" << endl;
    test1(1);
#ifdef __DEBUG_BIG_TEST
    test2(2);
#endif
    cout << "^^^END^^^" << endl;
    return 0;
}


