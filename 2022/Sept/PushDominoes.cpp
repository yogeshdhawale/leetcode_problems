
/*

838. Push Dominoes
Medium

There are n dominoes in a line, and we place each domino vertically upright. In the beginning, we simultaneously push some of the dominoes either to the left or to the right.

After each second, each domino that is falling to the left pushes the adjacent domino on the left. Similarly, the dominoes falling to the right push their adjacent dominoes standing on the right.

When a vertical domino has dominoes falling on it from both sides, it stays still due to the balance of the forces.

For the purposes of this question, we will consider that a falling domino expends no additional force to a falling or already fallen domino.

You are given a string dominoes representing the initial state where:

dominoes[i] = 'L', if the ith domino has been pushed to the left,
dominoes[i] = 'R', if the ith domino has been pushed to the right, and
dominoes[i] = '.', if the ith domino has not been pushed.
Return a string representing the final state.



Example 1:

Input: dominoes = "RR.L"
Output: "RR.L"
Explanation: The first domino expends no additional force on the second domino.
Example 2:


Input: dominoes = ".L.R...LR..L.."
Output: "LL.RR.LLRRLL.."


Constraints:

n == dominoes.length
1 <= n <= 105
dominoes[i] is either 'L', 'R', or '.'.

*/

#include <iostream>

using namespace std;

enum DTYPE
{
    LEFT = 'L',
    RIGHT = 'R',
    NONE = '.'
};

struct DominosPos
{
    int index;
    int type;
    int otherEnd;

    DominosPos()
    {
        reset();
    }
    void reset()
    {
        index = -1;
        type = NONE;
        otherEnd = -1;
    }
};

class Solution
{

private:
    string result;

public:
    void fallLeft(int end, int start)
    {
        // cout << "Labelling: L (" << start << ", " << end - 1 << ")" << endl;
        for (int j = start; j < end; j++)
            result[j] = LEFT;
    }
    void fallRight(int start, int end)
    {
        // cout << "Labelling: R (" << start << ", " << end - 1 << ")" << endl;
        for (int j = start; j < end; j++)
            result[j] = RIGHT;
    }

    string pushDominoes(string dominoes)
    {

        result = dominoes;
        int len = dominoes.length();

        DominosPos pos[2];

        bool found1 = false;

        int i = 0;
        int startpos = 0;

        // process rest
        startpos = i;
        for (; i < len; i++)
        {
            if (dominoes[i] == NONE)
            {
                continue;
            }
            else if (dominoes[i] == LEFT)
            {
                // cout << "L:" << i << endl;
                if (found1 == true)
                {
                    if (pos[0].type == LEFT)
                    {
                        cout << "ERR, unexpected" << endl;
                    }
                    if (pos[0].type == RIGHT)
                    {
                        pos[1].index = i;
                        pos[1].type = LEFT;
                        int diff = (i - pos[0].index - 1) / 2;

                        // cout << "In between, range: " << diff <<  endl;

                        pos[0].otherEnd = pos[0].index + diff;
                        pos[1].otherEnd = i - diff;

                        fallRight(pos[0].index, pos[0].otherEnd + 1);
                        fallLeft(pos[1].index, pos[1].otherEnd);
                    }
                    found1 = false;
                    pos[0].reset();
                    pos[1].reset();
                    startpos = i + 1;
                }
                else
                {
                    fallLeft(i, startpos);
                    startpos = i + 1;
                }
            }
            else if (dominoes[i] == RIGHT)
            {
                // cout << "R:" << i << endl;
                if (found1 == true)
                {
                    fallRight(pos[0].index + 1, i);
                    startpos = i + 1;
                }
                found1 = true;
                pos[0].index = i;
                pos[0].type = RIGHT;
            }
        }

        if (found1 == true)
        {
            fallRight(pos[0].index + 1, len);
            found1 = false;
        }

        return result;
    }
};

int main()
{
    //if (false)
    {
        cout << "Test 1:" << endl;
        Solution s;

        string result = s.pushDominoes("RR.L");
        cout << "Result is (expected=\tRR.L):\n\t\t\t" << result << endl;
    }
    //if (false)
    {
        cout << "Test 2:" << endl;
        Solution s;

        string result = s.pushDominoes(".L.R...LR..L..");
        cout << "Result is (expected=\tLL.RR.LLRRLL..):\n\t\t\t" << result << endl;
    }
    //if (false)
    {
        cout << "Test 2:" << endl;
        Solution s;

        string result = s.pushDominoes(".L.R.");
        cout << "Result is (expected=\tLL.RR):\n\t\t\t" << result << endl;
    }
    // if (false)
    {
        cout << "Test 2:" << endl;
        Solution s;

        string result = s.pushDominoes("R.R.L");
        cout << "Result is (expected=\tRRR.L):\n\t\t\t" << result << endl;
    }
}
