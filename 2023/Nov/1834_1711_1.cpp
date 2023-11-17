/*
1834. Single-Threaded CPU
Medium
Topics
Companies
Hint

You are given n​​​​​​ tasks labeled from 0 to n - 1 represented by a 2D integer array tasks, where tasks[i] = [enqueueTimei, processingTimei] means that the i​​​​​​th​​​​ task will be available to process at enqueueTimei and will take processingTimei to finish processing.

You have a single-threaded CPU that can process at most one task at a time and will act in the following way:

    If the CPU is idle and there are no available tasks to process, the CPU remains idle.
    If the CPU is idle and there are available tasks, the CPU will choose the one with the shortest processing time. If multiple tasks have the same shortest processing time, it will choose the task with the smallest index.
    Once a task is started, the CPU will process the entire task without stopping.
    The CPU can finish a task then start a new one instantly.

Return the order in which the CPU will process the tasks.



Example 1:

Input: tasks = [[1,2],[2,4],[3,2],[4,1]]
Output: [0,2,3,1]
Explanation: The events go as follows:
- At time = 1, task 0 is available to process. Available tasks = {0}.
- Also at time = 1, the idle CPU starts processing task 0. Available tasks = {}.
- At time = 2, task 1 is available to process. Available tasks = {1}.
- At time = 3, task 2 is available to process. Available tasks = {1, 2}.
- Also at time = 3, the CPU finishes task 0 and starts processing task 2 as it is the shortest. Available tasks = {1}.
- At time = 4, task 3 is available to process. Available tasks = {1, 3}.
- At time = 5, the CPU finishes task 2 and starts processing task 3 as it is the shortest. Available tasks = {1}.
- At time = 6, the CPU finishes task 3 and starts processing task 1. Available tasks = {}.
- At time = 10, the CPU finishes task 1 and becomes idle.

Example 2:

Input: tasks = [[7,10],[7,12],[7,5],[7,4],[7,2]]
Output: [4,3,2,0,1]
Explanation: The events go as follows:
- At time = 7, all the tasks become available. Available tasks = {0,1,2,3,4}.
- Also at time = 7, the idle CPU starts processing task 4. Available tasks = {0,1,2,3}.
- At time = 9, the CPU finishes task 4 and starts processing task 3. Available tasks = {0,1,2}.
- At time = 13, the CPU finishes task 3 and starts processing task 2. Available tasks = {0,1}.
- At time = 18, the CPU finishes task 2 and starts processing task 0. Available tasks = {1}.
- At time = 28, the CPU finishes task 0 and starts processing task 1. Available tasks = {}.
- At time = 40, the CPU finishes task 1 and becomes idle.



Constraints:

    tasks.length == n
    1 <= n <= 105
    1 <= enqueueTimei, processingTimei <= 109

*/

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace std;

#define __DEBUG_LOG 0

struct Task
{
    int id;
    long enq_time;
    long processing_time;
};

int getPriorityTaskId(Task *t1, Task *t2)
{
    if (t1->processing_time < t2->processing_time)
    {
        return t1->id;
    }
    else if (t1->processing_time > t2->processing_time)
    {
        return t2->id;
    }
    return t1->id < t2->id ? t1->id : t2->id;
}
/*
 * MinHeap - heapify according to the processing time, given that tasks are eligible for enqueue
 */
class MinHeap
{
private:
    vector<Task *> heap;
    int count;

    int getParentId(int i)
    {
        return (i - 1) / 2;
    }

    void heapify(int i)
    {
        while (i > 0)
        {
            int p = getParentId(i);

            int taskId = getPriorityTaskId(heap[p], heap[i]);

            if (taskId != heap[p]->id)
            {
                swap(heap[i], heap[p]);
                i = p;
            }
            else
            {
                break;
            }
        }
    }
    void heapifydown(int i)
    {
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        int smallest = i;

        if (l < count && r < count)
        {
            int taskId = getPriorityTaskId(heap[l], heap[r]);
            if (taskId == heap[l]->id)
                smallest = l;
            else if (taskId == heap[r]->id)
                smallest = r;
        }
        else if (l < count)
        {
            int taskId = getPriorityTaskId(heap[l], heap[i]);
            if (taskId == heap[l]->id)
                smallest = l;
        }
        else
            return;

        int taskId = getPriorityTaskId(heap[smallest], heap[i]);
        if (taskId != heap[i]->id)
        {
            swap(heap[i], heap[smallest]);
            heapifydown(smallest);
        }
    }

public:
    MinHeap() : count(0) {}

    int size()
    {
        return count;
    }

    void insert(Task *t)
    {
        heap.push_back(t);
        count++;
        heapify(count - 1);
    }

    Task *getTask()
    {
        Task *t = heap[0];
        heap[0] = heap[count - 1];
        heap.pop_back();
        count--;
        heapifydown(0);
        return t;
    }

    void print()
    {
        for (int i = 0; i < count; i++)
        {
            cout << heap[i]->id << ": " << heap[i]->enq_time << ", " << heap[i]->processing_time << endl;
        }
        cout << endl;
    }
};

class Solution
{
public:
    vector<int> getOrder(vector<vector<int>> &tasks);
};

vector<int> Solution::getOrder(vector<vector<int>> &tasks)
{
    MinHeap heap;
    vector<int> result;

    vector<Task *> task_enq_list(tasks.size());

    for (int i = 0; i < tasks.size(); i++)
    {
        Task *t = new Task{i, tasks[i][0], tasks[i][1]};
        task_enq_list[i] = t; // insert to task_enq_list
    }

    sort(task_enq_list.begin(), task_enq_list.end(), [](Task *t1, Task *t2) { return t1->enq_time < t2->enq_time; });

    long time = 1;
    int i = 0;
    while (heap.size() > 0 || (task_enq_list.size() - i) > 0)
    {
        if (__DEBUG_LOG)
            cout << "time = " << time << " task_enq_list size = " << task_enq_list.size() - i << " heap size = " << heap.size() << endl;
        if (heap.size() == 0)
        {
            time = max(time, task_enq_list[i]->enq_time);
        }

        while ((task_enq_list.size() - i) > 0 && task_enq_list[i]->enq_time <= time)
        {
            Task *t = task_enq_list[i];
            i++;
            heap.insert(t);

            if (__DEBUG_LOG)
                cout << "task added to queue: " << t->id << ": " << t->enq_time << ", " << t->processing_time << endl;
        }

        if (__DEBUG_LOG)
            heap.print();

        Task *t = heap.getTask();
        time = max(time, t->enq_time);
        time += t->processing_time;
        result.push_back(t->id);
        delete t;
        if (__DEBUG_LOG)
            cout << "completed task: " << t->id << ": " << t->enq_time << ", " << t->processing_time << endl;
    }
    return result;
}

void test1(Solution &s, int testID)
{
    cout << "test" << testID << endl;
    auto nums = vector<vector<int>>({{1, 2}, {2, 4}, {3, 2}, {4, 1}});
    auto expected = vector<int>({0, 2, 3, 1});
    vector<int> result = s.getOrder(nums);

    for (int i = 0; i < result.size(); i++)
    {
        cout << result[i] << " " << expected[i] << endl;
        assert(result[i] == expected[i]);
    }
}

void test2(Solution &s, int testID)
{
    cout << "test" << testID << endl;
    auto nums = vector<vector<int>>({{1, 2}});
    auto expected = vector<int>({0});
    vector<int> result = s.getOrder(nums);

    for (int i = 0; i < result.size(); i++)
    {
        cout << result[i] << " " << expected[i] << endl;
        assert(result[i] == expected[i]);
    }
}

void test3(Solution &s, int testID)
{
    cout << "test" << testID << endl;
    auto nums = vector<vector<int>>({{1, 10}, {1, 2}, {8, 9}, {4, 9}, {30, 1}});
    auto expected = vector<int>({1, 0, 2, 3, 4});
    vector<int> result = s.getOrder(nums);

    for (int i = 0; i < result.size(); i++)
    {
        cout << result[i] << " " << expected[i] << endl;
        assert(result[i] == expected[i]);
    }
}

int main()
{

    cout << "1877. Minimize Maximum Pair Sum in Array" << endl;
    Solution s;

    // test1(s, 1);
    // test2(s, 2);
    test3(s, 3);

    return 0;
}