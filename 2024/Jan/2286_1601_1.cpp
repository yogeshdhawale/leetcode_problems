/*
2286. Booking Concert Tickets in Groups
Hard
Topics
Companies
Hint

A concert hall has n rows numbered from 0 to n - 1, each with m seats, numbered from 0 to m - 1. You need to design a ticketing system that can allocate seats in the following cases:

    If a group of k spectators can sit together in a row.
    If every member of a group of k spectators can get a seat. They may or may not sit together.

Note that the spectators are very picky. Hence:

    They will book seats only if each member of their group can get a seat with row number less than or equal to maxRow. maxRow can vary from group to group.
    In case there are multiple rows to choose from, the row with the smallest number is chosen. If there are multiple seats to choose in the same row, the seat with the smallest number is chosen.

Implement the BookMyShow class:

    BookMyShow(int n, int m) Initializes the object with n as number of rows and m as number of seats per row.
    int[] gather(int k, int maxRow) Returns an array of length 2 denoting the row and seat number (respectively) of the first seat being allocated to the k members of the group, who must sit together. In other words, it returns the smallest possible r and c such that all [c, c + k - 1] seats are valid and empty in row r, and r <= maxRow. Returns [] in case it is not possible to allocate seats to the group.
    boolean scatter(int k, int maxRow) Returns true if all k members of the group can be allocated seats in rows 0 to maxRow, who may or may not sit together. If the seats can be allocated, it allocates k seats to the group with the smallest row numbers, and the smallest possible seat numbers in each row. Otherwise, returns false.



Example 1:

Input
["BookMyShow", "gather", "gather", "scatter", "scatter"]
[[2, 5], [4, 0], [2, 0], [5, 1], [5, 1]]
Output
[null, [0, 0], [], true, false]

Explanation
BookMyShow bms = new BookMyShow(2, 5); // There are 2 rows with 5 seats each
bms.gather(4, 0); // return [0, 0]
                  // The group books seats [0, 3] of row 0.
bms.gather(2, 0); // return []
                  // There is only 1 seat left in row 0,
                  // so it is not possible to book 2 consecutive seats.
bms.scatter(5, 1); // return True
                   // The group books seat 4 of row 0 and seats [0, 3] of row 1.
bms.scatter(5, 1); // return False
                   // There is only one seat left in the hall.



Constraints:

    1 <= n <= 5 * 104
    1 <= m, k <= 109
    0 <= maxRow <= n - 1
    At most 5 * 104 calls in total will be made to gather and scatter.


*/

/**
 * Your BookMyShow object will be instantiated and called as such:
 * BookMyShow* obj = new BookMyShow(n, m);
 * vector<int> param_1 = obj->gather(k,maxRow);
 * bool param_2 = obj->scatter(k,maxRow);
 */
#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
#include <map>
#include <chrono>

using namespace std;

#define __DEBUG_LOG 1

template <typename T>
void printVector(vector<T> &nums)
{
    for (size_t i = 0; i < nums.size(); i++)
    {
        cout << to_string(nums[i]) << ", ";
    }
    cout << endl;
}

template <typename T>
void printMatrix(vector<vector<T>> &nums)
{
    for (size_t i = 0; i < nums.size(); i++)
    {
        printVector(nums[i]);
    }
}
template <typename T>
void printMap(map<T, T> &nums)
{
    for (auto it = nums.begin(); it != nums.end(); it++)
    {
        cout << to_string(it->first) << "=" << to_string(it->second) << ",";
    }
}

class BookMyShow
{
    int n, m;
    map<int, int> partialRowIds;
    map<int, map<int, bool>> availableSeats;
    long partialSum;
    int completedRowIdxMax;
    int fullRowIdxMin;

public:
    BookMyShow(int n, int m)
    {
        this->n = n;
        this->m = m;
        completedRowIdxMax = -1;
        fullRowIdxMin = 0;
        partialSum = 0;
    }

    void updateAvailableSeats(int rowid, int oldseats, int newseats)
    {
        if (oldseats == newseats)
        {
            return;
        }
        auto itr1 = availableSeats.find(oldseats);
        if (itr1 != availableSeats.end())
        {
            auto itr2 = itr1->second.find(rowid);
            if (itr2 != itr1->second.end())
            {
                itr1->second.erase(rowid);
            }
            if (itr1->second.empty())
            {
                availableSeats.erase(itr1);
            }
        }
        if (newseats != 0)
            availableSeats[newseats][rowid] = true;
    }
    vector<int> gather(int k, int maxRow)
    {
        if (k > m)
        {
            return {};
        }

        if (k == m)
        {
            if (maxRow < fullRowIdxMin)
            {
                return {};
            }
            partialRowIds[fullRowIdxMin] = 0;
            fullRowIdxMin++;
            return {fullRowIdxMin - 1, 0};
        }

        auto itr1 = availableSeats.lower_bound(k);

        int rowid = n + 1;
        while (itr1 != availableSeats.end())
        {
            auto itr2 = itr1->second.lower_bound(0);
            if (itr2 != itr1->second.end())
                rowid = min(rowid, itr2->first);
            itr1++;
        }

        if (rowid <= maxRow)
        {
            int seats = partialRowIds[rowid];
            int start = m - seats;

            partialRowIds[rowid] -= k;
            partialSum -= k;
            updateAvailableSeats(rowid, seats, seats - k);
            return {rowid, start};
        }

        rowid = fullRowIdxMin;

        if (fullRowIdxMin > maxRow) // no seats available
        {
            return {};
        }

        partialRowIds[rowid] = m - k;
        partialSum += m - k;
        updateAvailableSeats(rowid, m, m - k);
        fullRowIdxMin++;
        return {rowid, 0};
    }
    bool scatter(int k, int maxRow)
    {
        int requiredRows = (k / m);
        if (k % m != 0)
        {
            requiredRows++;
        }
        if (completedRowIdxMax >= maxRow || requiredRows > (maxRow - completedRowIdxMax))
        {
            return false;
        }

        if (k > partialSum)
        {
            if (maxRow < fullRowIdxMin)
                return false;
            if (((k - partialSum) / m) > (1 + maxRow - fullRowIdxMin))
                return false;

            k -= partialSum;
            completedRowIdxMax += partialRowIds.size();
            partialRowIds.clear();
            availableSeats.clear();
            partialSum = 0;
        }

        for (auto i = completedRowIdxMax + 1; i <= maxRow && partialRowIds.size(); i++)
        {
            if (partialRowIds.find(i) == partialRowIds.end())
                continue;
            if (k > partialRowIds[i])
            {
                k -= partialRowIds[i];
                partialSum -= partialRowIds[i];
                updateAvailableSeats(i, partialRowIds[i], 0);
                partialRowIds.erase(i);
                completedRowIdxMax = i;
                continue;
            }
            else if (k == partialRowIds[i])
            {
                partialSum -= partialRowIds[i];
                updateAvailableSeats(i, partialRowIds[i], 0);
                partialRowIds.erase(i);
                completedRowIdxMax = i;
                return true;
            }
            else
            {
                partialSum -= k;
                updateAvailableSeats(i, partialRowIds[i], partialRowIds[i] - k);
                partialRowIds[i] -= k;
                return true;
            }
        }

        completedRowIdxMax = fullRowIdxMin - 1;
        int rowdiff = k / m;
        fullRowIdxMin += rowdiff;
        completedRowIdxMax += rowdiff;
        k -= (rowdiff * m);
        if (k == 0)
            return true;

        updateAvailableSeats(fullRowIdxMin, m, m - k);
        partialRowIds[fullRowIdxMin] = m - k;
        partialSum += m - k;
        fullRowIdxMin++;
        return true;
    }

    void print()
    {
        cout << "partialRowIds:";
        printMap(partialRowIds);
        cout << " completedRowIdxMax:" << completedRowIdxMax << endl;
    }
};

void test1(int testID)
{
    cout << "test" << testID << endl;
    auto input = vector<string>({"BookMyShow", "gather", "gather", "scatter", "scatter"});
    auto operation = vector<vector<int>>({{2, 5}, {4, 0}, {2, 0}, {5, 1}, {5, 1}});
    auto expected = vector<vector<int>>({{}, {0, 0}, {}, {1}, {0}});

    BookMyShow *bms = new BookMyShow(operation[0][0], operation[0][1]);
    for (size_t i = 1; i < operation.size(); i++)
    {
        bms->print();
        cout << "task:" << i << " input:" << input[i] << " operation:" << operation[i][0] << "," << operation[i][1] << endl;
        if (input[i] == "gather")
        {
            auto result = bms->gather(operation[i][0], operation[i][1]);
            if (expected[i].size() == 0)
            {
                cout << "result:" << result.size() << endl;
                assert(result.size() == 0);
            }
            else
            {
                cout << "result:" << result[0] << "," << result[1] << endl;
                assert(result[0] == expected[i][0]);
                assert(result[1] == expected[i][1]);
            }
        }
        else
        {
            auto result = bms->scatter(operation[i][0], operation[i][1]);
            assert((int)result == expected[i][0]);
        }
    }
}

void test2(int testID)
{
    cout << "test" << testID << endl;
    auto input = vector<string>({"BookMyShow", "scatter", "gather"});
    auto operation = vector<vector<int>>({{1, 2}, {2, 0}});
    auto expected = vector<vector<int>>({{}, {1}, {}});

    BookMyShow *bms = new BookMyShow(operation[0][0], operation[0][1]);
    for (size_t i = 1; i < operation.size(); i++)
    {
        bms->print();
        cout << "operation:" << i << " input:" << input[i] << " operation:" << operation[i][0] << "," << operation[i][1] << endl;
        if (input[i] == "gather")
        {
            auto result = bms->gather(operation[i][0], operation[i][1]);
            if (expected[i].size() == 0)
            {
                cout << "result:" << result.size() << endl;

                assert(result.size() == 0);
            }
            else
            {
                cout << "result:" << result[0] << "," << result[1] << endl;
                assert(result[0] == expected[i][0]);
                assert(result[1] == expected[i][1]);
            }
        }
        else
        {
            auto result = bms->scatter(operation[i][0], operation[i][1]);
            cout << "result:" << result << "," << endl;
            assert((int)result == expected[i][0]);
        }
    }
}

void test3(int testID)
{
    cout << "test" << testID << endl;
    auto input = vector<string>({"BookMyShow", "scatter", "gather", "scatter", "gather", "gather", "gather", "gather", "scatter", "gather", "gather", "scatter", "gather", "scatter", "gather", "gather", "scatter", "scatter", "scatter", "scatter", "scatter", "scatter"});
    auto operation = vector<vector<int>>({{30, 583}, {699, 10}, {507, 24}, {151, 15}, {753, 19}, {384, 4}, {178, 1}, {962, 3}, {894, 2}, {742, 10}, {161, 2}, {259, 8}, {692, 19}, {530, 3}, {741, 22}, {304, 10}, {841, 27}, {828, 24}, {624, 17}, {579, 0}, {897, 17}, {445, 10}});
    auto expected = vector<vector<int>>({{}, {1}, {2, 0}, {1}, {}, {3, 0}, {1, 267}, {}, {0}, {}, {}, {1}, {}, {0}, {}, {4, 0}, {1}, {1}, {1}, {0}, {1}, {1}});

    BookMyShow *bms = new BookMyShow(operation[0][0], operation[0][1]);
    for (size_t i = 1; i < operation.size(); i++)
    {
        bms->print();
        cout << "task:" << i << " input:" << input[i] << " operation:" << operation[i][0] << "," << operation[i][1] << endl;
        if (input[i] == "gather")
        {
            auto result = bms->gather(operation[i][0], operation[i][1]);
            if (expected[i].size() == 0)
            {
                cout << "result-size:" << result.size() << "," << endl;

                assert(result.size() == 0);
            }
            else
            {
                cout << "result:" << result[0] << "," << result[1] << endl;

                assert(result[0] == expected[i][0]);
                assert(result[1] == expected[i][1]);
            }
        }
        else
        {
            auto result = bms->scatter(operation[i][0], operation[i][1]);
            cout << "result:" << result << endl;
            assert((int)result == expected[i][0]);
        }
    }
}

void test4(int testID)
{
    cout << "test" << testID << endl;
    auto input = vector<string>({"BookMyShow", "scatter", "gather", "scatter", "gather", "gather", "gather", "gather", "scatter", "gather", "gather", "scatter", "gather", "scatter", "gather", "gather", "scatter", "scatter", "scatter", "scatter", "scatter", "scatter"});
    auto operation = vector<vector<int>>({{30, 583}, {699, 10}, {507, 24}, {151, 15}, {753, 19}, {384, 4}, {178, 1}, {962, 3}, {894, 2}, {742, 10}, {161, 2}, {259, 8}, {692, 19}, {530, 3}, {741, 22}, {304, 10}, {841, 27}, {828, 24}, {624, 17}, {579, 0}, {897, 17}, {445, 10}});
    auto expected = vector<vector<int>>({{}, {1}, {2, 0}, {1}, {}, {3, 0}, {1, 267}, {}, {0}, {}, {}, {1}, {}, {0}, {}, {4, 0}, {1}, {1}, {1}, {0}, {1}, {1}});

    long long totalSum = 0;
    BookMyShow *bms = new BookMyShow(operation[0][0], operation[0][1]);
    auto starttime = std::chrono::high_resolution_clock::now();
    for (size_t i = 1; i < operation.size(); i++)
    {
        // bms->print();
        // cout << "task:" << i << " input:" << input[i] << " operation:" << operation[i][0] << "," << operation[i][1] << endl;

        if (input[i] == "gather")
        {
            auto result = bms->gather(operation[i][0], operation[i][1]);
            if (expected[i].size() == 0)
            {
                // cout << "result:" << result.size() << endl;
                assert(result.size() == 0);
            }
            else
            {
                // long diff = 0;
                // diff = (expected[i][0] * operation[0][1]) - totalSum;
                //  cout << "result:" << result[0] << "," << result[1] << endl;

                assert(result[0] == expected[i][0]);
                assert(result[1] == expected[i][1]);
                totalSum += operation[i][0];
                // cout << "\ttotal Sum so far: " << totalSum << " maxSum = " << ((long long)(expected[i][0] + 1) * operation[0][1]) << " diff:" << diff << "" << endl;
            }
        }
        else
        {
            auto result = bms->scatter(operation[i][0], operation[i][1]);
            // cout << "result:" << result << endl;
            assert((int)result == expected[i][0]);

            if (result == true)
            {
                totalSum += operation[i][0];
                // cout << "\ttotal Sum so far: " << totalSum << endl;
            }
        }
    }
    cout << "time:" << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - starttime).count() << endl;
}

int main()
{
    test1(1);
    test2(2);
    test3(3);
    // test4(4);
    return 0;
}
