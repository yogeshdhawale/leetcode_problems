
/*
Q1. LRU Cache
Medium
Topics
premium lock iconCompanies

Design a data structure that follows the constraints of a Least Recently Used (LRU) cache.

Implement the LRUCache class:

    LRUCache(int capacity) Initialize the LRU cache with positive size capacity.
    int get(int key) Return the value of the key if the key exists, otherwise return -1.
    void put(int key, int value) Update the value of the key if the key exists. Otherwise, add the key-value pair to the cache. If the number of keys exceeds the capacity from this operation, evict the least recently used key.

The functions get and put must each run in O(1) average time complexity.



Example 1:

Input
["LRUCache", "put", "put", "get", "put", "get", "put", "get", "get", "get"]
[[2], [1, 1], [2, 2], [1], [3, 3], [2], [4, 4], [1], [3], [4]]
Output
[null, null, null, 1, null, -1, null, -1, 3, 4]

Explanation
LRUCache lRUCache = new LRUCache(2);
lRUCache.put(1, 1); // cache is {1=1}
lRUCache.put(2, 2); // cache is {1=1, 2=2}
lRUCache.get(1);    // return 1
lRUCache.put(3, 3); // LRU key was 2, evicts key 2, cache is {1=1, 3=3}
lRUCache.get(2);    // returns -1 (not found)
lRUCache.put(4, 4); // LRU key was 1, evicts key 1, cache is {4=4, 3=3}
lRUCache.get(1);    // return -1 (not found)
lRUCache.get(3);    // return 3
lRUCache.get(4);    // return 4



Constraints:

    1 <= capacity <= 3000
    0 <= key <= 104
    0 <= value <= 105
    At most 2 * 105 calls will be made to get and put.


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
//#define __DEBUG_BIG_TEST
enum TestOptions {
    TEST_ALL = 10,
    TEST_SINGLE = 11,
};

struct TestConfig {
    TestOptions options;
    int testCaseID;
};
#include "log.h"
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
    V2Int mData2;
    OUTPUT_TYPE mExpected1;

    TestCaseStruct() {
        mID = inputID++;
    }
    void setInput(vector<string> input1, V2Int input2) {
        this->mData1 = input1;
        this->mData2 = input2;
    }
    void setOutput(OUTPUT_TYPE output1) {
        this->mExpected1 = output1;
    }
};

/*---------------------------------------------------------------------------*/


struct node {
    int key;
    int value;
    node* prev;
    node* next;
    node(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

class LRUCache {
public:

    int capacity;
    unordered_map <int, node*> cache;
    node* lruHead;
    node* lruTail;


    LRUCache(int capacity) {
        this->capacity = capacity;
        lruHead = nullptr;
        lruTail = nullptr;
    }

    int getForCapacityOne(int key) {
        if (lruHead == nullptr || lruHead->key != key) {
            return -1;
        }
        return lruHead->value;
    }

    void putForCapacityOne(int key, int value) {
        if (lruHead == nullptr) {
            lruHead = new node(-1, -1);
        }
        lruHead->key = key;
        lruHead->value = value;
    }

    int get(int key) {

        if (capacity == 1) return getForCapacityOne(key);
        if (cache.find(key) == cache.end()) return -1;

        // update lru as the element is accessed
        node* currNode = cache[key];

        DetachNode(currNode);
        AddNode(currNode);

        return currNode->value;
    }

    void put(int key, int value) {

        if (capacity == 1) {
            putForCapacityOne(key, value);
            return;
        }

        if (cache.find(key) != cache.end()) {
            cache[key]->value = value;
            node* currNode = cache[key];
            DetachNode(currNode);
            AddNode(currNode);
            return;
        }

        if (cache.size() == capacity) {
            //evict last element from lru i.e. lruTail
            LOG("put " << key << "=" << value << "  evicting " << lruTail->key << "=" << lruTail->value);
            evictLFU();
        }

        node* newNode = new node(key, value);
        cache[key] = newNode;
        AddNode(newNode);
    }
    void evictLFU()
    {
        node* lastNode = lruTail;
        node* oneBeforeLast = lruTail->prev;
        oneBeforeLast->next = nullptr;
        lruTail = oneBeforeLast;

        cache.erase(lastNode->key);
        delete lastNode;
    }
    void AddNode(node* newNode)
    {
        if (lruHead == nullptr) {
            lruHead = newNode;
            lruTail = newNode;
            return;
        }

        lruHead->prev = newNode;
        newNode->next = lruHead;
        lruHead = newNode;
    }

    void DetachNode(node* node) {

        if (node == lruHead && node == lruTail) {
            lruHead = nullptr;
            lruTail = nullptr;
            return;
        }

        if (node == lruHead) {
            lruHead = node->next;
            lruHead->prev = nullptr;
            return;
        }

        if (node == lruTail) {
            lruTail = node->prev;
            lruTail->next = nullptr;
            return;
        }

        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
};


/**class LRUCache {
public:
    LRUCache(int capacity) {

    }

    int get(int key) {

    }

    void put(int key, int value) {

    }
};


 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */

class Solution {
public:
    int proxyFunction(TestCaseStruct& nums) {

        LRUCache* obj = nullptr;
        for (size_t i = 0; i < nums.mData1.size(); i++) {
            string cmd = nums.mData1[i];
            if (cmd == "LRUCache") {
                if (obj != nullptr) delete obj;
                obj = new LRUCache(nums.mData2[i][0]);
                continue;
            }
            else if (cmd == "put") {
                obj->put(nums.mData2[i][0], nums.mData2[i][1]);
            }
            else if (cmd == "get") {
                int ret = obj->get(nums.mData2[i][0]);
                LOG("get " << nums.mData2[i][0] << "=" << ret);
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
            { "LRUCache", "put", "put", "get", "put", "get", "put", "get", "get", "get" },
            { { 2 }, { 1, 1 }, { 2, 2 }, { 1 }, { 3, 3 }, { 2 }, { 4, 4 }, { 1 }, { 3 }, { 4 } }
        );
        nums.back().setOutput(0);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { "LRUCache", "put", "put", "get", "put", "get", "put", "get", "get", "get" },
            { { 1 }, { 1, 1 }, { 2, 2 }, { 1 }, { 3, 3 }, { 2 }, { 4, 4 }, { 1 }, { 3 }, { 4 } }
        );
        nums.back().setOutput(0);
    }

    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { "LRUCache", "put", "put", "put", "put", "get", "get" },
            { { 2 }, { 2,1 }, { 1,1 }, { 2,3 }, { 4,1 }, { 1 }, { 2 } }
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

string TEST_DIR = "./test_data/";
void test2(int testID)
{

    cout << __LOGPREFIX << "test" << testID << endl;
    vector<TestCaseStruct> nums;

    auto t1i1 = TEST_DIR + "test1-i1.txt";
    auto t1i2 = TEST_DIR + "test1-i2.txt";
    auto t1e1 = TEST_DIR + "test1-e1.txt";

    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(load_data_v1<int>(t1i1), load_data_v2(t1i2));
        nums.back().setOutput(load_data_v1<int>(t1e1));
    }

    cout << __LOGPREFIX << "test for: " << nums << endl;

    Solution s;
    std::chrono::_V2::system_clock::time_point endtime, starttime = std::chrono::high_resolution_clock::now();
    auto result = s.proxyFunction(nums[0]);
    endtime = std::chrono::high_resolution_clock::now();
    cout << __LOGPREFIX << "test: " << testID << " ==> " << result << " vs e:" << nums[0].mExpected1 << endl;
    logtime("### Time: ", starttime, endtime);
    cout << endl;

    //assert(result == nums[0].mExpected1);
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


