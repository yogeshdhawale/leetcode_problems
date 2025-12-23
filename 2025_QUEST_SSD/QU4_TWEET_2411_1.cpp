
/*

Q2. Design Twitter
Medium

Design a simplified version of Twitter where users can post tweets, follow/unfollow another user, and is able to see the 10 most recent tweets in the user's news feed.

Implement the Twitter class:

    Twitter() Initializes your twitter object.
    void postTweet(int userId, int tweetId) Composes a new tweet with ID tweetId by the user userId. Each call to this function will be made with a unique tweetId.
    List<Integer> getNewsFeed(int userId) Retrieves the 10 most recent tweet IDs in the user's news feed. Each item in the news feed must be posted by users who the user followed or by the user themself. Tweets must be ordered from most recent to least recent.
    void follow(int followerId, int followeeId) The user with ID followerId started following the user with ID followeeId.
    void unfollow(int followerId, int followeeId) The user with ID followerId started unfollowing the user with ID followeeId.



Example 1:

Input
["Twitter", "postTweet", "getNewsFeed", "follow", "postTweet", "getNewsFeed", "unfollow", "getNewsFeed"]
[[], [1, 5], [1], [1, 2], [2, 6], [1], [1, 2], [1]]
Output
[null, null, [5], null, null, [6, 5], null, [5]]

Explanation
Twitter twitter = new Twitter();
twitter.postTweet(1, 5); // User 1 posts a new tweet (id = 5).
twitter.getNewsFeed(1);  // User 1's news feed should return a list with 1 tweet id -> [5]. return [5]
twitter.follow(1, 2);    // User 1 follows user 2.
twitter.postTweet(2, 6); // User 2 posts a new tweet (id = 6).
twitter.getNewsFeed(1);  // User 1's news feed should return a list with 2 tweet ids -> [6, 5]. Tweet id 6 should precede tweet id 5 because it is posted after tweet id 5.
twitter.unfollow(1, 2);  // User 1 unfollows user 2.
twitter.getNewsFeed(1);  // User 1's news feed should return a list with 1 tweet id -> [5], since user 1 is no longer following user 2.



Constraints:

    1 <= userId, followerId, followeeId <= 500
    0 <= tweetId <= 104
    All the tweets have unique IDs.
    At most 3 * 104 calls will be made to postTweet, getNewsFeed, follow, and unfollow.
    A user cannot follow himself.

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
typedef vector<long long > V1LL;

typedef vector<string> V1Str;
typedef vector<V1Str> V2Str;

typedef int IDX, VALUEI, IDXR, IDXC;
typedef pair<int, int> IDX2D;
/*---------------------------------------------------------------------------*/
typedef V1Int INPUT_TYPE;
typedef int OUTPUT_TYPE;

static int inputID = 1;
struct TestCaseStruct
{
    int mID;
    V1Str mData1;
    vector<V1LL> mData2;
    OUTPUT_TYPE mExpected1;

    TestCaseStruct() {
        mID = inputID++;
    }
    void setInput(V1Str input1, vector < V1LL> input2) {
        this->mData1 = input1;
        this->mData2 = input2;
    }
    void setOutput(OUTPUT_TYPE output1) {
        this->mExpected1 = output1;
    }
};


struct user {
    int userId;
    list <int> mytweets;

    set<int> followers; // send updates to
    set<int> following; // get updates from

    list <int> newsfeed;
    bool refreshNeeded;

    user(int id = -1) {
        userId = id;
        followers.clear();
        mytweets.clear();
        newsfeed.clear();
        refreshNeeded = false;
    }

    void setRefreshNewsFeed() {
        newsfeed.clear();
        refreshNeeded = true;
    }

    void postTweet(int tweetId, vector<user>& userInfo) {

        if (mytweets.size() == 10) {
            mytweets.pop_back();
        }
        mytweets.push_front(tweetId);

        notify(tweetId);

        for (auto followerId : followers) {
            userInfo[followerId].notify(tweetId);
        }
    }

    void addFollower(int followerId) {
        followers.insert(followerId);
    }

    void removeFollower(int followerId) {
        followers.erase(followerId);
    }
    void addFollowing(int followeeId) {
        following.insert(followeeId);
        setRefreshNewsFeed();
    }
    void removeFollowing(int followeeId) {
        following.erase(followeeId);
        setRefreshNewsFeed();
    }

    void notify(int tweetId) {

        if (refreshNeeded) return;

        if (newsfeed.size() == 10) {
            newsfeed.pop_back();
        }
        newsfeed.push_front(tweetId);
    }

    vector<int> getNewsFeed(vector<user>& userInfo, unordered_map<int, int>& t2ts) {

        if (refreshNeeded) {
            rebuildNewsFeed(userInfo, t2ts);
        }

        return vector<int>(newsfeed.begin(), newsfeed.end());
    }

    void rebuildNewsFeed(vector<user>& userInfo, unordered_map<int, int>& t2ts) {

        LOG("\tRebuild news feed for user:" << userId << endl);
        newsfeed.clear();
        map<int, int> tmpNewsFeed;

        for (auto tid : mytweets) {
            auto ts = t2ts[tid];
            tmpNewsFeed[ts] = tid;
        }
        while (tmpNewsFeed.size() > 10) {
            tmpNewsFeed.erase(tmpNewsFeed.begin());
        }

        for (auto followeeId : following) {
            for (auto tid : userInfo[followeeId].mytweets) {
                auto ts = t2ts[tid];
                tmpNewsFeed[ts] = tid;
            }

            while (tmpNewsFeed.size() > 10) {
                tmpNewsFeed.erase(tmpNewsFeed.begin());
            }
        }

        for (auto it = tmpNewsFeed.rbegin(); it != tmpNewsFeed.rend(); ++it) {
            newsfeed.push_back(it->second);
        }
        refreshNeeded = false;
    }

};


class Twitter {

    vector<user> userInfo;
    int tweetTimeStamp = 0;
    unordered_map<int, int> t2ts; // tweetId to timestamp

public:
    Twitter() {
        userInfo.resize(501);
        for (int i = 0;i <= 500;i++) {
            userInfo[i] = user(i);
        }
    }

    void postTweet(int userId, int tweetId) {
        t2ts[tweetId] = tweetTimeStamp++;
        userInfo[userId].postTweet(tweetId, userInfo);
    }

    void follow(int followerId, int followeeId) {
        if (followerId == followeeId) return;
        userInfo[followeeId].addFollower(followerId);
        userInfo[followerId].addFollowing(followeeId);
    }

    void unfollow(int followerId, int followeeId) {
        if (followerId == followeeId) return;
        userInfo[followeeId].removeFollower(followerId);
        userInfo[followerId].removeFollowing(followeeId);
    }
    vector<int> getNewsFeed(int userId) {
        return userInfo[userId].getNewsFeed(userInfo, t2ts);
    }
};

/**
 * Your Twitter object will be instantiated and called as such:
 * Twitter* obj = new Twitter();
 * obj->postTweet(userId,tweetId);
 * vector<int> param_2 = obj->getNewsFeed(userId);
 * obj->follow(followerId,followeeId);
 * obj->unfollow(followerId,followeeId);
 */



class Solution {
public:
    int proxyFunction(TestCaseStruct& tc) {

        Twitter* obj = nullptr;
        for (size_t i = 0; i < tc.mData1.size(); i++) {
            string cmd = tc.mData1[i];

            LOG("command:" << cmd << " with data:" << tc.mData2[i]);
            if (cmd == "Twitter") {
                if (obj) delete obj;
                obj = new Twitter();
            }
            else if (cmd == "postTweet") {
                obj->postTweet(tc.mData2[i][0], tc.mData2[i][1]);
            }
            else if (cmd == "getNewsFeed") {
                auto ret = obj->getNewsFeed(tc.mData2[i][0]);
                LOG(" returns:" << ret << endl);
            }
            else if (cmd == "follow") {
                obj->follow(tc.mData2[i][0], tc.mData2[i][1]);
            }
            else if (cmd == "unfollow") {
                obj->unfollow(tc.mData2[i][0], tc.mData2[i][1]);
            }
            else {
                cout << "Unknown command:" << cmd << endl;
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
            { "Twitter", "postTweet", "getNewsFeed", "follow", "postTweet", "getNewsFeed", "unfollow", "getNewsFeed" },
            { {}, { 1, 5 }, { 1 }, { 1, 2 }, { 2, 6 }, { 1 }, { 1, 2 }, { 1 } }
        );
        nums.back().setOutput(0);
    }

    {
        nums.push_back(TestCaseStruct());
        nums.back().setInput(
            { "Twitter", "postTweet", "postTweet", "postTweet", "postTweet", "postTweet", "postTweet", "postTweet", "postTweet", "postTweet", "postTweet", "getNewsFeed", "follow", "getNewsFeed" },
            { {}, { 2,5 }, { 1,3 }, { 1,101 }, { 2,13 }, { 2,10 }, { 1,2 }, { 2,94 }, { 2,505 }, { 1,333 }, { 1,22 }, { 2 }, { 2,1 }, { 2 } }
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


