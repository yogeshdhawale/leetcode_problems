/*

403. Frog Jump
Hard
Topics
Companies

A frog is crossing a river. The river is divided into some number of units, and at each unit, there may or may not exist a stone. The frog can jump on a stone, but it must not jump into the water.

Given a list of stones positions (in units) in sorted ascending order, determine if the frog can cross the river by landing on the last stone. Initially, the frog is on the first stone and assumes the first jump must be 1 unit.

If the frog's last jump was k units, its next jump must be either k - 1, k, or k + 1 units. The frog can only jump in the forward direction.



Example 1:

Input: stones = [0,1,3,5,6,8,12,17]
Output: true
Explanation: The frog can jump to the last stone by jumping 1 unit to the 2nd stone, then 2 units to the 3rd stone, then 2 units to the 4th stone, then 3 units to the 6th stone, 4 units to the 7th stone, and 5 units to the 8th stone.

Example 2:

Input: stones = [0,1,2,3,4,8,9,11]
Output: false
Explanation: There is no way to jump to the last stone as the gap between the 5th and 6th stone is too large.



Constraints:

    2 <= stones.length <= 2000
    0 <= stones[i] <= 231 - 1
    stones[0] == 0
    stones is sorted in a strictly increasing order.


*/

#include <bits/stdc++.h>

using namespace std;

#define __DEBUG_LOG 0

template <typename T>
void printVector(vector<T> nums)
{
    for (size_t i = 0; i < nums.size(); i++)
    {
        cout << nums[i] << " ";
    }
    cout << endl;
}

void printMap(int i, const map<int, int> &nums)
{
    cout << "for i[" << i << "] map size: " << nums.size() << "\t";
    for (auto it = nums.begin(); it != nums.end(); it++)
    {
        cout << it->first << " " << it->second << ", ";
    }
    cout << endl;
}
class Info
{

public:
    size_t startid;
    size_t endid;
    int currentStep;

    Info() : startid(-1), endid(-1), currentStep(-1) {}
    Info(size_t startid, size_t endid, int currentStep) : startid(startid), endid(endid), currentStep(currentStep) {}
    Info(const Info &info) : startid(info.startid), endid(info.endid), currentStep(info.currentStep) {}
    Info &operator=(const Info &info)
    {
        startid = info.startid;
        endid = info.endid;
        currentStep = info.currentStep;
        return *this;
    }
    void print(string message)
    {
        cout << message << " startid: " << startid << " endid: " << endid << " currentStep: " << currentStep << endl;
    }
};

class Solution
{

    map<int, map<int, int>> dp;
    map<int, map<int, bool>> invalidJumps;
    size_t size;

public:
    Solution()
    {
        dp.clear();
        invalidJumps.clear();
        size = 0;
    }

    bool canCross(vector<int> &stones)
    {
        size = stones.size();
        if (size < 2)
        {
            return false;
        }
        if (stones[1] - stones[0] != 1)
            return false;

        if (stones.size() == 2)
            return true;

        if (__DEBUG_LOG)
        {
            cout << "size: " << size << endl;
        }
        // if (size > 100)
        //     size = 100;

        bool canReachEnd = false;
        vector<int> minJumpSize(size, INT_MAX);
        vector<int> maxJumpSize(size, INT_MIN);

        dp[0][1] = 1;
        minJumpSize[0] = 1;
        maxJumpSize[0] = 1;
        minJumpSize[1] = 1;
        maxJumpSize[1] = 1;

        for (size_t i = 1; i < size - 1; i++)
        {
            dp[i] = map<int, int>();
            if (minJumpSize[i] == INT_MAX || maxJumpSize[i] == INT_MIN)
                continue;

            int prevMinJump = minJumpSize[i] - 1;
            int prevMaxJump = maxJumpSize[i] + 1;
            if (__DEBUG_LOG == 1)
            {
                cout << "minJumpSize: " << prevMinJump << " maxJumpSize: " << prevMaxJump << "\t";
                printMap(i - 1, dp[i - 1]);
            }
            for (size_t j = i + 1; j < size; j++)
            {
                int jumpsize = stones[j] - stones[i];
                if (jumpsize < prevMinJump || jumpsize > prevMaxJump)
                    continue;

                if (__DEBUG_LOG == 1)
                {
                    cout << "[" << i << "][" << j << "] => from stone " << stones[i] << " to stone " << stones[j] << " jumpsize: " << jumpsize << endl;
                }

                dp[i][j] = jumpsize;
                minJumpSize[j] = min(minJumpSize[j], jumpsize);
                maxJumpSize[j] = max(maxJumpSize[j], jumpsize);

                if (maxJumpSize[j] > (i + 1))
                {
                    return false;
                }
                if (j == size - 1)
                {
                    canReachEnd = true;
                }
            }
        }

        if (__DEBUG_LOG)
        {
            cout << "minJumpSize: " << minJumpSize[size - 1] << " maxJumpSize: " << maxJumpSize[size - 1] << "\t";
            printMap(size - 1, dp[size - 1]);
        }

        if (!canReachEnd)
        {
            return false;
        }

        Info newInfo(0, 1, 1);
        newInfo = processNextStep(stones, newInfo);
        if (newInfo.currentStep == -1 || newInfo.endid != size - 1)
        {
            return false;
        }
        return true;
    }

    Info processNextStep(vector<int> &stones, Info &prevJump)
    {
        size_t i = prevJump.endid;
        int prevJumpSize = prevJump.currentStep;

        if (i == size - 1)
        {
            return prevJump;
        }
        if (prevJumpSize == -1 || dp[i].size() == 0 ||
            invalidJumps[i].find(prevJumpSize) != invalidJumps[i].end())
        {
            if (__DEBUG_LOG)
            {
                cout << "invalidJumps:(cache) [" << i << "] stone = " << stones[i] << " jumpsize = " << prevJumpSize << endl;
            }
            return Info(i, size, -1);
        }

        for (auto it = dp[i].begin(); it != dp[i].end(); it++)
        {
            int j = it->first;
            int jumpsize = it->second;

            if (jumpsize < (prevJump.currentStep - 1) || jumpsize > (prevJump.currentStep + 1))
            {
                continue;
            }

            Info newInfo(i, j, jumpsize);
            newInfo = processNextStep(stones, newInfo);
            if (newInfo.currentStep != -1 && newInfo.endid == size - 1)
            {
                if (__DEBUG_LOG)
                    cout << "newInfo: success - [" << i << " " << j << "] -> stones " << stones[i] << " " << stones[j] << " jumpsize: " << jumpsize << endl;

                return newInfo;
            }
        }

        if (__DEBUG_LOG)
        {
            cout << "Failed for : stone " << stones[i] << " current step: " << prevJump.currentStep << " ";
            printMap(i, dp[i]);
        }

        invalidJumps[i][prevJump.currentStep] = true;
        return Info(i, size, -1);
    }
};

void test1(int testID)
{
    Solution s;
    cout << "test" << testID << endl;

    auto nums = vector<vector<int>>({
        {0, 1, 3, 5, 6, 8, 12, 17},
        {0, 1, 2, 3, 4, 8, 9, 11},
        {0, 1, 3, 6, 10, 13, 15, 18},
        {0, 1, 3, 6, 7},
        {0, 1, 3, 6, 10, 13, 15, 16, 19, 21, 25},
    });
    auto expected = vector<bool>({true, false, true, false, false});

#if __DEBUG_LOG == 3
    for (size_t i = nums.size() - 1; i < nums.size(); i++)
#else
    for (size_t i = 0; i < nums.size(); i++)
#endif
    {
        cout << "###loop: " << i << " test for: size: " << nums[i].size() << endl;
        printVector(nums[i]);
        auto result = s.canCross(nums[i]);

        cout << "result " << result << " vs " << expected[i] << endl;
        assert(result == expected[i]);
    }
}

void test2(int testID)
{
    cout << "test" << testID << endl;

    auto nums = vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359, 360, 361, 362, 363, 364, 365, 366, 367, 368, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380, 381, 382, 383, 384, 385, 386, 387, 388, 389, 390, 391, 392, 393, 394, 395, 396, 397, 398, 399, 400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417, 418, 419, 420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434, 435, 436, 437, 438, 439, 440, 441, 442, 443, 444, 445, 446, 447, 448, 449, 450, 451, 452, 453, 454, 455, 456, 457, 458, 459, 460, 461, 462, 463, 464, 465, 466, 467, 468, 469, 470, 471, 472, 473, 474, 475, 476, 477, 478, 479, 480, 481, 482, 483, 484, 485, 486, 487, 488, 489, 490, 491, 492, 493, 494, 495, 496, 497, 498, 499, 500, 501, 502, 503, 504, 505, 506, 507, 508, 509, 510, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 524, 525, 526, 527, 528, 529, 530, 531, 532, 533, 534, 535, 536, 537, 538, 539, 540, 541, 542, 543, 544, 545, 546, 547, 548, 549, 550, 551, 552, 553, 554, 555, 556, 557, 558, 559, 560, 561, 562, 563, 564, 565, 566, 567, 568, 569, 570, 571, 572, 573, 574, 575, 576, 577, 578, 579, 580, 581, 582, 583, 584, 585, 586, 587, 588, 589, 590, 591, 592, 593, 594, 595, 596, 597, 598, 599, 600, 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611, 612, 613, 614, 615, 616, 617, 618, 619, 620, 621, 622, 623, 624, 625, 626, 627, 628, 629, 630, 631, 632, 633, 634, 635, 636, 637, 638, 639, 640, 641, 642, 643, 644, 645, 646, 647, 648, 649, 650, 651, 652, 653, 654, 655, 656, 657, 658, 659, 660, 661, 662, 663, 664, 665, 666, 667, 668, 669, 670, 671, 672, 673, 674, 675, 676, 677, 678, 679, 680, 681, 682, 683, 684, 685, 686, 687, 688, 689, 690, 691, 692, 693, 694, 695, 696, 697, 698, 699, 700, 701, 702, 703, 704, 705, 706, 707, 708, 709, 710, 711, 712, 713, 714, 715, 716, 717, 718, 719, 720, 721, 722, 723, 724, 725, 726, 727, 728, 729, 730, 731, 732, 733, 734, 735, 736, 737, 738, 739, 740, 741, 742, 743, 744, 745, 746, 747, 748, 749, 750, 751, 752, 753, 754, 755, 756, 757, 758, 759, 760, 761, 762, 763, 764, 765, 766, 767, 768, 769, 770, 771, 772, 773, 774, 775, 776, 777, 778, 779, 780, 781, 782, 783, 784, 785, 786, 787, 788, 789, 790, 791, 792, 793, 794, 795, 796, 797, 798, 799, 800, 801, 802, 803, 804, 805, 806, 807, 808, 809, 810, 811, 812, 813, 814, 815, 816, 817, 818, 819, 820, 821, 822, 823, 824, 825, 826, 827, 828, 829, 830, 831, 832, 833, 834, 835, 836, 837, 838, 839, 840, 841, 842, 843, 844, 845, 846, 847, 848, 849, 850, 851, 852, 853, 854, 855, 856, 857, 858, 859, 860, 861, 862, 863, 864, 865, 866, 867, 868, 869, 870, 871, 872, 873, 874, 875, 876, 877, 878, 879, 880, 881, 882, 883, 884, 885, 886, 887, 888, 889, 890, 891, 892, 893, 894, 895, 896, 897, 898, 899, 900, 901, 902, 903, 904, 905, 906, 907, 908, 909, 910, 911, 912, 913, 914, 915, 916, 917, 918, 919, 920, 921, 922, 923, 924, 925, 926, 927, 928, 929, 930, 931, 932, 933, 934, 935, 936, 937, 938, 939, 940, 941, 942, 943, 944, 945, 946, 947, 948, 949, 950, 951, 952, 953, 954, 955, 956, 957, 958, 959, 960, 961, 962, 963, 964, 965, 966, 967, 968, 969, 970, 971, 972, 973, 974, 975, 976, 977, 978, 979, 980, 981, 982, 983, 984, 985, 986, 987, 988, 989, 990, 991, 992, 993, 994, 995, 996, 997, 998, 1000035});
    auto expected = 0;

    Solution s;
    auto starttime = std::chrono::high_resolution_clock::now();
    auto result = s.canCross(nums);
    cout << "Time:" << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - starttime).count() << " ms" << endl;
    cout << result << " " << expected << endl;
    assert(result == expected);
}

int main()
{
    cout << "--- " << endl;

    if (__DEBUG_LOG != 2)
    {
        test1(1);
    }
    else
        test2(2);
    return 0;
}