#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <unordered_map>
using namespace std;
std::unordered_map<string, int> hash_map;
bool make_test(const string& perft, const string& test, ofstream& log) {
    string s2 = " ";
    bool has_errors = false;
    for (unsigned int i=0;i<test.size();++i) {
        if (test[i] == ' ') continue;
        if (test[i] != '\n') {s2+=test[i]; continue;}
        if (s2 == " ") continue;
        const auto pos = s2.find(':');
        auto name = s2.substr(0, pos);
        const auto number = s2.substr(pos+1);
        std::transform(name.begin(), name.end(), name.begin(),
    [](unsigned char c){ return std::tolower(c); });
        //std::cout << name << ' ' << number << '\n';
        hash_map[name] = stoi(number);
        s2=" ";
    }s2= " ";
    for (unsigned int i=0;i<perft.size();++i) {
        if (perft[i] == ' ') continue;
        if (perft[i] != '\n') {s2+=perft[i]; continue;}
        if (s2 == " ") continue;
        const auto pos = s2.find(':');
        auto name = s2.substr(0, pos);
        const auto number = s2.substr(pos+1);
        std::transform(name.begin(), name.end(), name.begin(),
    [](unsigned char c){ return std::tolower(c); });
        s2=" ";
        const auto it = hash_map.find(name);
        if (it == hash_map.end()) {
            has_errors = true;
            std::cout << "Move is missing: " << name << ":" << number << '\n';
            log  << "Move is missing: " << name << ":" << number << '\n';continue;
        }
        const auto num = stoi(number);
        if (it->second != num) {
            has_errors = true;
            log << "Move is evaluated incorrectly => " << name << " (correct)" << num << " : " << it->second << "(incorrect)\n";
            std::cout << "Move is evaluated incorrectly => " << name << " (correct)" << num << " : " << it->second << "(incorrect)\n";
        }
        hash_map.erase(it);
        //std::cout << name << ' ' << number << '\n';
        //hash_map[name] = stoi(number);
    }
    for (auto& x : hash_map) {
        std::cout << "Wrong move: " << x.first << "  (" << x.second << ")\n";
        log << "Wrong move: " << x.first << "  (" << x.second << ")\n";
        has_errors = true;
    }
    return !has_errors;
}
inline std::string slurp(std::ifstream& in) {
    std::ostringstream sstr;
    sstr << in.rdbuf();
    return sstr.str() + "\n";
}
int main() {
    ifstream fin1("perft.txt");
    ifstream fin2("test.txt");
    if (fin1.fail() || fin2.fail() || fin1.eof() || fin2.eof()) {std::cout << "files not found\n";getchar();return 0;}
    ofstream log("result.txt");
    if (make_test(slurp(fin1), slurp(fin2), log)) {log << "passed\n";std::cout << "passed\n";}
    else {std::cout << "failed\n"; log << "failed\n";}
    log.flush();
    log.close();
    getchar();
    return 0;
}