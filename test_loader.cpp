
#include <vector>
#include <fstream>
#include <iostream>
#include<string>
#include<sstream>

using namespace std;

inline std::string trim(std::string& str)
{
    str.erase(str.find_last_not_of(' ') + 1);
    str.erase(0, str.find_first_not_of(' '));
    return str;
}


int load_data_int(string fileName) {

    fstream file(fileName, ios::in);

    if (!file.is_open()) {
        cout << "Can't open file: " << fileName << endl;
        return 0;
    }
    int data;
    file >> data;
    file.close();
    return data;
}

template <typename T>
T getVal(string token) {
    if (typeid(T) == typeid(int)) {
        return std::stoi(token);
    }
    else if (typeid(T) == typeid(bool)) {
        bool val = false;
        if (token == "true") val = true;
        if (token == "false") val = false;
        return val;
    }
    return T();
}

template <typename T>
vector<T> load_data_v1(string fileName) {

    vector<T> data;

    fstream file(fileName, ios::in);

    if (!file.is_open()) {
        cout << "Can't open file: " << fileName << endl;
        return data;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        while (getline(ss, token, ',')) {
            token = trim(token);
            if (token == "") continue;
            if (token[0] == '[') token.erase(token.begin());
            if (token[token.size() - 1] == ']') {
                token.pop_back();
            }
            if (token.length() == 0) continue;
            data.push_back(getVal<T>(token));
        }
    }
    file.close();
    return data;
}
vector<vector<int>> load_data_v2(string fileName) {

    vector<vector<int>> data;

    fstream file(fileName, ios::in);

    if (!file.is_open()) {
        cout << "Can't open file: " << fileName << endl;
        return data;
    }
    bool bArrayEnded = false;
    string line;
    while (getline(file, line)) {
        vector<int> cur;
        stringstream ss(line);
        string token;
        while (getline(ss, token, ',')) {
            token = trim(token);
            if (token == "") continue;
            while (token[0] == '[') token.erase(token.begin());
            while (token[token.size() - 1] == ']') {
                token.pop_back();
                bArrayEnded = true;
            }
            if (token.length() == 0) {
                if (bArrayEnded) {
                    data.push_back(cur);
                    cur.clear();
                    bArrayEnded = false;
                }
                continue;
            }
            cur.push_back(std::stoi(token));
            if (bArrayEnded) {
                data.push_back(cur);
                cur.clear();
                bArrayEnded = false;
            }
        }
    }
    file.close();
    return data;
}

int test() {
    {
        cout << "*** Test v1 int ***" << endl;
        vector<int> data = load_data_v1<int>("test_data/d_i1.txt");
        for (size_t i = 0; i < data.size(); i++) {
            cout << data[i] << ",";
        }
        cout << endl;
    }
    {
        cout << "*** Test v1 bool ***" << endl;
        vector<bool> data = load_data_v1<bool>("test_data/d_b1.txt");
        for (size_t i = 0; i < data.size(); i++) {
            cout << data[i] << ",";
        }
        cout << endl;
    }
    {
        cout << "*** Test v2 int ***" << endl;
        vector<vector<int>> data = load_data_v2("test_data/d_i2.txt");
        for (size_t i = 0; i < data.size(); i++) {
            cout << "[";
            for (size_t j = 0; j < data[i].size(); j++) {
                cout << data[i][j] << ",";
            }
            cout << "]";
            cout << endl;
        }
    }
    return 0;
}

/* int main() {
    test();
    return 0;
} */
