#include <iostream>
#include <vector>

using namespace std;

class Solution {
    
    class GridMap {
        private:
            int row;
            int col;
            vector<vector<char>>& data;

            int count;

        public:
        GridMap(int r, int c, vector<vector<char>>& d) : data (d) {
            row = r;
            col = c;
            count = 0;
        }
        
        void findIsland() {
            int i=0;
            int j=0;
            bool found = false;
            for (; i < row; i++) {
                int j = 0;
                for (; j < col; j++) {
                    
                    //if (data[i][j] == 'x')
                    //    continue; // already processed
                    //if(data[i][j] == '0')
                    //    continue; // its water
                    if(data[i][j] == '1') {
                        found = true;
                        break;
                    }
                }
                if (found == true)
                    break;
            }
            
            if (i >= row || j >= col) {
                cout << "finished searching " << i << "," << j << endl;
                return;
            }
            
            // we are at start of potential island
            cout << "Data: " << i << "," << j << endl;

            MarkLand(i, j);
            
            count++;
            cout <<"Found island:" << count << endl;
            findIsland();
        }

        void MarkLand(int i, int j) {
            if (i >= row || j>= col)
                return;

            if (i < 0 || j < 0)
                return;

            if (data[i][j] != '1')
                return;

            cout << "Marking " << i << ", " << j << endl;
            data[i][j] = 'x';

            MarkLand(i-1, j);
            MarkLand(i+1, j);
            MarkLand(i, j-1);
            MarkLand(i, j+1);
        }
        int getIslandCount() {
            return count;
        }
        
    };
        
public:
    int numIslands(vector<vector<char>>& grid) {
        int row = grid.size();
        int col = grid[0].size();

        GridMap info(row, col, grid);
        info.findIsland();
        return info.getIslandCount();
    }
};


int main () {

    {
        cout << "*** Starting test 1" << endl;

        Solution s1;
        vector<vector<char>> grid1 = 
        {
            {'1','1'},
            {'0','0'},
            {'1','1'}
        };
        int result = s1.numIslands(grid1);
        cout << "Grid1 output (expected 2): " << result << endl;
    }
    {
        cout << "*** Starting test 2" << endl;

        Solution s1;
        vector<vector<char>> grid1 = 
        {
            {'1','1'},
            {'0','0'}
        };
        int result = s1.numIslands(grid1);
        cout << "Grid1 output (expected 1): " << result << endl;
    }
}