
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
auto getVal(string token) -> std::common_type_t<T> {

    if (typeid(T) == typeid(int)) {
        return std::stoi(token);
    }
    if (typeid(T) == typeid(bool)) {
        bool val = false;
        if (token == "true") val = true;
        if (token == "false") val = false;
        return val;
    }
    cout << "Unsupported type" << endl;
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
            token = trim(token);
            if (token.length() == 0) continue;
            data.push_back(getVal<T>(token));
        }
    }
    file.close();
    return data;
}

template <typename T>
void print_1d_array(const vector<T>& data) {
    cout << "[";
    for (size_t i = 0; i < data.size(); i++) {
        cout << data[i] << ",";
    }
    cout << "]";
}

template <typename T>
void print_2d_array(const vector<vector<T>>& data) {
    cout << "[";
    for (size_t i = 0; i < data.size(); i++) {
        print_1d_array(data[i]);
        cout << ",";
    }
    cout << "]";
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
            while (token[0] == '[') {
                token.erase(token.begin());
                token = trim(token);
            }
            while (token[token.size() - 1] == ']') {
                token.pop_back();
                bArrayEnded = true;
            }

            if (token.length() != 0) cur.push_back(std::stoi(token));

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

vector<string> load_data_v1_string(string fileName) {

    vector<string> data;

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
            if (token[0] == '[') {
                token.erase(token.begin());
                token = trim(token);
            }
            if (token[token.size() - 1] == ']') {
                token.pop_back();
            }
            token = trim(token);
            if (token.length() == 0) continue;
            if (token[0] == '\"' && token[token.size() - 1] == '\"') {
                token = token.substr(1, token.size() - 2);
            }
            data.push_back(token);
        }
    }
    file.close();
    return data;
}

vector<vector<string>> load_data_v2_string(string fileName) {
    vector<vector<string>> data;

    fstream file(fileName, ios::in);

    if (!file.is_open()) {
        cout << "Can't open file: " << fileName << endl;
        return data;
    }
    string line;
    bool bArrayEnded = false;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> cur;
        while (getline(ss, token, ',')) {

            token = trim(token);
            if (token.length() == 0) continue;

            while (token[0] == '[') {
                token.erase(token.begin());
                token = trim(token);
            }

            if (token.length() == 0) continue;

            while (token[token.size() - 1] == ']') {
                token.pop_back();
                bArrayEnded = true;
            }

            if (token[0] == '\"' && token[token.size() - 1] == '\"') {
                token = token.substr(1, token.size() - 2);
            }

            if (token.length() != 0) cur.push_back(token);

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


void test_int(string test_dir) {
    {
        cout << "*** Test v1 int ***" << endl;
        auto data = load_data_v1<int>(test_dir + "/d_i1.txt");
        print_1d_array(data);
        cout << endl;
    }
    {
        cout << "*** Test v2 int ***" << endl;
        auto data = load_data_v2(test_dir + "/d_i2.txt");
        print_2d_array(data);
        cout << endl;
    }
}

void test_bool(string test_dir) {
    cout << "*** Test v1 bool ***" << endl;
    auto data = load_data_v1<bool>(test_dir + "/d_b1.txt");
    print_1d_array(data);
    cout << endl;
}


void test_string(string test_dir) {
    {
        cout << "*** Test v1 string ***" << endl;
        auto data = load_data_v1_string(test_dir + "/d_s1.txt");
        print_1d_array(data);
        cout << endl;
    }
    {
        cout << "*** Test v2 string ***" << endl;
        auto data = load_data_v2_string(test_dir + "/d_s2.txt");
        print_2d_array(data);
        cout << endl;
    }
}

/*
int main() {

    cout << "Test Started" << endl;

    auto test_dir = "./test_loader_data/";
    test_int(test_dir);
    test_bool(test_dir);
    test_string(test_dir);

    cout << " Test Ended" << endl << endl;
    return 0;
}
/**/

