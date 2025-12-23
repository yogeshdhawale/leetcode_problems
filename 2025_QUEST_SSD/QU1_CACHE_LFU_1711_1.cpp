
/*
Q2. LFU Cache
Hard
Topics
premium lock iconCompanies

Design and implement a data structure for a Least Frequently Used (LFU) cache.

Implement the LFUCache class:

    LFUCache(int capacity) Initializes the object with the capacity of the data structure.
    int get(int key) Gets the value of the key if the key exists in the cache. Otherwise, returns -1.
    void put(int key, int value) Update the value of the key if present, or inserts the key if not already present. When the cache reaches its capacity, it should invalidate and remove the least frequently used key before inserting a new item. For this problem, when there is a tie (i.e., two or more keys with the same frequency), the least recently used key would be invalidated.

To determine the least frequently used key, a use counter is maintained for each key in the cache. The key with the smallest use counter is the least frequently used key.

When a key is first inserted into the cache, its use counter is set to 1 (due to the put operation). The use counter for a key in the cache is incremented either a get or put operation is called on it.

The functions get and put must each run in O(1) average time complexity.



Example 1:

Input
["LFUCache", "put", "put", "get", "put", "get", "get", "put", "get", "get", "get"]
[[2], [1, 1], [2, 2], [1], [3, 3], [2], [3], [4, 4], [1], [3], [4]]
Output
[null, null, null, 1, null, -1, 3, null, -1, 3, 4]

Explanation
// cnt(x) = the use counter for key x
// cache=[] will show the last used order for tiebreakers (leftmost element is  most recent)
LFUCache lfu = new LFUCache(2);
lfu.put(1, 1);   // cache=[1,_], cnt(1)=1
lfu.put(2, 2);   // cache=[2,1], cnt(2)=1, cnt(1)=1
lfu.get(1);      // return 1
                 // cache=[1,2], cnt(2)=1, cnt(1)=2
lfu.put(3, 3);   // 2 is the LFU key because cnt(2)=1 is the smallest, invalidate 2.
                 // cache=[3,1], cnt(3)=1, cnt(1)=2
lfu.get(2);      // return -1 (not found)
lfu.get(3);      // return 3
                 // cache=[3,1], cnt(3)=2, cnt(1)=2
lfu.put(4, 4);   // Both 1 and 3 have the same cnt, but 1 is LRU, invalidate 1.
                 // cache=[4,3], cnt(4)=1, cnt(3)=2
lfu.get(1);      // return -1 (not found)
lfu.get(3);      // return 3
                 // cache=[3,4], cnt(4)=1, cnt(3)=3
lfu.get(4);      // return 4
                 // cache=[4,3], cnt(4)=2, cnt(3)=3



Constraints:

    1 <= capacity <= 104
    0 <= key <= 105
    0 <= value <= 109
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
    int usedCnt;

    node* prev;
    node* next;

    node() : key(-1), value(-1), usedCnt(0),
        prev(nullptr), next(nullptr) {}
    node(int k, int v) : node() {
        key = k;
        value = v;
        usedCnt = 1;
    }
};


struct LRUList {
    node* head;
    node* tail;

    LRUList() : head(nullptr), tail(nullptr) {}

    void addNode(node* n) {
        if (head == nullptr) {
            head = n;
            tail = n;
            return;
        }
        n->next = head;
        head->prev = n;
        head = n;
    }
    void removeNode(node* n) {
        if (n == head && n == tail) {
            head = nullptr;
            tail = nullptr;
            return;
        }

        if (n == head) {
            head = n->next;
            head->prev = nullptr;
            return;
        }

        if (n == tail) {
            tail = n->prev;
            tail->next = nullptr;
            return;
        }

        n->prev->next = n->next;
        n->next->prev = n->prev;
    }
};

class LFUCache {
public:

    int capacity;
    node forcapacityOne;

    unordered_map <int, node*> cache;
    map<int, LRUList> usedCntMap; // map of usedCnt to nodelist (LRU for usedCnt)

    LFUCache(int capacity) {
        this->capacity = capacity;
    }

    int getForCapacityOne(int key) {
        if (forcapacityOne.key != key)  return -1;
        return forcapacityOne.value;
    }

    void putForCapacityOne(int key, int value) {
        forcapacityOne.key = key;
        forcapacityOne.value = value;
    }

    int get(int key) {

        if (capacity == 1) return getForCapacityOne(key);
        if (cache.find(key) == cache.end()) return -1;

        node* currNode = cache[key];

        usedCntMap[cache[key]->usedCnt].removeNode(currNode);
        currNode->usedCnt += 1;
        usedCntMap[cache[key]->usedCnt].addNode(currNode);

        return currNode->value;
    }

    void put(int key, int value) {

        if (capacity == 1) {
            putForCapacityOne(key, value);
            return;
        }

        if (cache.find(key) != cache.end()) {
            node* currNode = cache[key];
            usedCntMap[cache[key]->usedCnt].removeNode(currNode);
            currNode->usedCnt += 1;
            usedCntMap[cache[key]->usedCnt].addNode(currNode);
            currNode->value = value;
            return;
        }

        if (cache.size() == capacity) {
            //evict last element from lru i.e. lruTail
            evictLFU();
        }

        node* newNode = new node(key, value);
        cache[key] = newNode;
        usedCntMap[1].addNode(newNode);
    }
    void evictLFU()
    {
        auto it = usedCntMap.begin();
        while (it != usedCntMap.end())
        {
            if (it->second.head == nullptr) {
                it = usedCntMap.erase(it);
                continue;
            }
            break;
        }

        LRUList& lruList = it->second;
        node* lruTail = lruList.tail;
        LOG("evicting " << lruTail->key << "=" << lruTail->value << " usedCnt=" << lruTail->usedCnt);
        lruList.removeNode(lruTail);
        cache.erase(lruTail->key);
        delete lruTail;
    }
};

/**
class LFUCache {
public:
    LFUCache(int capacity) {

    }

    int get(int key) {

    }

    void put(int key, int value) {

    }
};

/**
 * Your LFUCache object will be instantiated and called as such:
 * LFUCache* obj = new LFUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */

class Solution {
public:
    int proxyFunction(TestCaseStruct& nums) {

        LFUCache* obj = nullptr;
        for (size_t i = 0; i < nums.mData1.size(); i++) {
            string cmd = nums.mData1[i];
            if (cmd == "LFUCache") {
                if (obj != nullptr) delete obj;
                obj = new LFUCache(nums.mData2[i][0]);
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
            { "LFUCache", "put", "put", "get", "put", "get", "get", "put", "get", "get", "get" },
            { { 2 }, { 1, 1 }, { 2, 2 }, { 1 }, { 3, 3 }, { 2 }, { 3 }, { 4, 4 }, { 1 }, { 3 }, { 4 } });
        nums.back().setOutput(0);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { "LFUCache", "put", "put", "get", "put", "get", "put", "get", "get", "get" },
            { { 1 }, { 1, 1 }, { 2, 2 }, { 1 }, { 3, 3 }, { 2 }, { 4, 4 }, { 1 }, { 3 }, { 4 } }
        );
        nums.back().setOutput(0);
    }

    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { "LFUCache", "put", "put", "put", "put", "get", "get" },
            { { 2 }, { 2,1 }, { 1,1 }, { 2,3 }, { 4,1 }, { 1 }, { 2 } }
        );
        nums.back().setOutput(0);
    }
    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { "LFUCache", "get", "get" },
            { { 1 }, { 1 }, { 2 } }
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


