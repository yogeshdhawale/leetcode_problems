/*
2353. Design a Food Rating System
Medium
Topics
Companies
Hint

Design a food rating system that can do the following:

    Modify the rating of a food item listed in the system.
    Return the highest-rated food item for a type of cuisine in the system.

Implement the FoodRatings class:

    FoodRatings(String[] foods, String[] cuisines, int[] ratings) Initializes the system. The food items are described by foods, cuisines and ratings, all of which have a length of n.
        foods[i] is the name of the ith food,
        cuisines[i] is the type of cuisine of the ith food, and
        ratings[i] is the initial rating of the ith food.
    void changeRating(String food, int newRating) Changes the rating of the food item with the name food.
    String highestRated(String cuisine) Returns the name of the food item that has the highest rating for the given type of cuisine. If there is a tie, return the item with the lexicographically smaller name.

Note that a string x is lexicographically smaller than string y if x comes before y in dictionary order, that is, either x is a prefix of y, or if i is the first position such that x[i] != y[i], then x[i] comes before y[i] in alphabetic order.



Example 1:

Input
["FoodRatings", "highestRated", "highestRated", "changeRating", "highestRated", "changeRating", "highestRated"]
[[["kimchi", "miso", "sushi", "moussaka", "ramen", "bulgogi"], ["korean", "japanese", "japanese", "greek", "japanese", "korean"], [9, 12, 8, 15, 14, 7]], ["korean"], ["japanese"], ["sushi", 16], ["japanese"], ["ramen", 16], ["japanese"]]
Output
[null, "kimchi", "ramen", null, "sushi", null, "ramen"]

Explanation
FoodRatings foodRatings = new FoodRatings(["kimchi", "miso", "sushi", "moussaka", "ramen", "bulgogi"], ["korean", "japanese", "japanese", "greek", "japanese", "korean"], [9, 12, 8, 15, 14, 7]);
foodRatings.highestRated("korean"); // return "kimchi"
                                    // "kimchi" is the highest rated korean food with a rating of 9.
foodRatings.highestRated("japanese"); // return "ramen"
                                      // "ramen" is the highest rated japanese food with a rating of 14.
foodRatings.changeRating("sushi", 16); // "sushi" now has a rating of 16.
foodRatings.highestRated("japanese"); // return "sushi"
                                      // "sushi" is the highest rated japanese food with a rating of 16.
foodRatings.changeRating("ramen", 16); // "ramen" now has a rating of 16.
foodRatings.highestRated("japanese"); // return "ramen"
                                      // Both "sushi" and "ramen" have a rating of 16.
                                      // However, "ramen" is lexicographically smaller than "sushi".



Constraints:

    1 <= n <= 2 * 104
    n == foods.length == cuisines.length == ratings.length
    1 <= foods[i].length, cuisines[i].length <= 10
    foods[i], cuisines[i] consist of lowercase English letters.
    1 <= ratings[i] <= 108
    All the strings in foods are distinct.
    food will be the name of a food item in the system across all calls to changeRating.
    cuisine will be a type of cuisine of at least one food item in the system across all calls to highestRated.
    At most 2 * 104 calls in total will be made to changeRating and highestRated.



*/

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>

using namespace std;

#define __DEBUG_LOG 0

class RatingInfo
{
public:
    string &food;
    int rating;
    string &cuisine;
    RatingInfo(string &food, int rating, string &cuisine) : food(food), rating(rating), cuisine(cuisine) {}
};

bool ratingCompare(RatingInfo *a, RatingInfo *b)
{
    if (a->rating > b->rating)
        return false;
    if (a->rating < b->rating)
        return true;
    if (a->food > b->food)
        return true;
    return false;
}

class FoodRatings
{
private:
    map<string, RatingInfo *> ratingMap;
    map<string, string> foodCuisineMap;
    map<string, vector<RatingInfo *>> cuisinesMap;
    map<string, bool> sortingNeeded;

public:
    FoodRatings(vector<string> &foods, vector<string> &cuisines, vector<int> &ratings)
    {

        for (size_t i = 0; i < foods.size(); i++)
        {
            RatingInfo *obj = new RatingInfo(foods[i], ratings[i], cuisines[i]);
            ratingMap[foods[i]] = obj;
            foodCuisineMap[foods[i]] = cuisines[i];
            cuisinesMap[cuisines[i]].push_back(obj);
        }
        for (auto it = cuisinesMap.begin(); it != cuisinesMap.end(); ++it)
        {
            vector<RatingInfo *> &vec = it->second;
            make_heap(vec.begin(), vec.end(), ratingCompare);
            sortingNeeded[it->first] = false;
        }
    }

    void changeRating(string food, int newRating)
    {
        ratingMap[food]->rating = newRating;
        sortingNeeded[foodCuisineMap[food]] = true;
    }

    string highestRated(string cuisine)
    {
        vector<RatingInfo *> &vec = cuisinesMap[cuisine];
        if (vec.size() == 0)
            return nullptr;

        if (sortingNeeded[cuisine])
        {
            make_heap(vec.begin(), vec.end(), ratingCompare);
            sortingNeeded[cuisine] = false;
        }

        return vec[0]->food;
    }
};


void test1(int testID)
{
    cout << "test" << testID << endl;

    vector<string> rating = {"FoodRatings", "highestRated", "highestRated", "changeRating", "highestRated", "changeRating", "highestRated"};
    vector<string> foods = {"kimchi", "miso", "sushi", "moussaka", "ramen", "bulgogi"};
    vector<string> cuisines = {"korean", "japanese", "japanese", "greek", "japanese", "korean"};
    vector<int> ratings = {9, 12, 8, 15, 14, 7};

    FoodRatings foodRatings(foods, cuisines, ratings);

    assert(foodRatings.highestRated("korean") == "kimchi");
    assert(foodRatings.highestRated("japanese") == "ramen");
    foodRatings.changeRating("sushi", 16);
    assert(foodRatings.highestRated("japanese") == "sushi");
    foodRatings.changeRating("ramen", 16);
    assert(foodRatings.highestRated("japanese") == "ramen");
}

int main()
{

    test1(1);
}