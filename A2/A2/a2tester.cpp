#define TEST_SIMPLE 0       // Set this to 0 to turn off testing of simple table
#define TEST_LPHASH 1       // Set this to 0 to turn off testing of LPHash table
#define TEST_CHAINHASH 0    // Set this to 0 to turn off testing of ChainHash table

#include "table.h"

// Note: you may not need to change this!
                    // If you defined your classes in a file other than table.h
                    // then include it here instead of table.h

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

namespace Testing {
    static const char* indent = "";
    static std::stringstream ss;
    static bool success = true;

    void msg(const std::string& s){
        std::cout << indent << s << std::endl;
    }

    void write(){
        msg(ss.str());
        ss.clear();
        ss.str(std::string());
    }

    void err(){
        std::string m = ss.str();
        ss.clear();
        ss.str(std::string());
        ss << "ERROR! " << m;
        write();
        success = false;
    }

    void h1(const std::string& s){
        std::cout << s << ':' << std::endl;
        indent = "  ";
    }

    void h2(const std::string& s){
        std::cout << "  [" << s << "]:" << std::endl;
        indent = "    ";
    }

    void h1w(){
        h1(ss.str());
        ss.clear();
        ss.str(std::string());
    }

    void h2w(){
        h2(ss.str());
        ss.clear();
        ss.str(std::string());
    }
}

#define MAX_EXPECTED 1000
#define BONUS_MAX_EXPECTED 100000
#define SIMPLE_BONUS_MAX_EXPECTED 10000
#define STR(m) #m

template <typename Key_t, typename KeyGen_t>
bool testBattery(int numElems, Table<Key_t, int>& table, KeyGen_t& keyGen){
    using namespace Testing;

    std::unordered_map<Key_t, int> map;
    std::vector<Key_t> keys;
    ss << "Entering test battery.";
    h2w();

    ss << "Inserting upto " << numElems << " elements.";
    h2w();
    for (auto i = 0; success && i < numElems; ++i) {
        int r = rand();
        keys.push_back(keyGen(i));
        map[keys.back()] = r;
        if (!table.insert(keys.back(), r)) {
            ss << "Could not insert key " << keys.back() << " into table";
            err();
        }

        // Now double insert should fail
        if (table.insert(keys.back(), r)) {
            ss << "Table reported true after inserting already inserted key, " << keys.back();
            err();
        }
    }

    if (!success) {
        ss << "Skipping find/remove tests because there were insert errors." << std::endl
            << "Fix the errors then try again.";
        err();
        return success;
    }
    
    ss << "Randomizing keys for find/remove tests...";
    h2w();
    Key_t temp;
    for (size_t i = 0; i < keys.size(); ++i) {
        int k = i + rand() % (keys.size() - i);
        temp = keys[i];
        keys[i] = keys[k];
        keys[k] = temp;
    }

    ss << "Find/Remove Tests";
    h2w();
    for (auto i = keys.begin(), end = keys.end(); success && i != end; ++i) {
        auto* val = table.find(*i);
        if (!val) {
            ss << "Table has no value for key " << *i;
            err();
        }
        else if (map[*i] != *val) {
            ss << "Expected value " << map[*i] << ", got " << *val << ", for key " << *i;
            err();
        }

        // Now remove value
        if (!table.remove(*i)) {
            ss << "Table failed to remove value " << *i;
            err();
        }

        map.erase(*i);
        if (!table.size() == map.size()) {
            ss << "After remove, table should now be size "
                << map.size()
                << ", table reported that it is size " << table.size();
            err();
        }

        // Now double remove should fail
        if (table.remove(*i)) {
            ss << "Table reported true after removing already removed key, " << *i;
            err();
        }
    }

    if (success) {
        ss << "Exiting test battery with no errors! Success!";
        write();
    }
    else {
        ss << "Exiting test battery with errors... fix them and try again";
        write();
    }

    return success;
}

template <typename Key_t, typename KeyGen_t>
bool testTable(const std::string& name, Table<Key_t, int>& table, KeyGen_t& keyGen, bool isRehashBonus = false, size_t bonusMaxExpected = BONUS_MAX_EXPECTED){
    using namespace Testing;

    success = true;
    ss << name << " Basic Tests (Only upto maxExpected elements where maxExpected is " << MAX_EXPECTED << ")";
    h1w();
    testBattery<Key_t, KeyGen_t>(MAX_EXPECTED - 1, table, keyGen);

    // Attempt to bonus
    if (success) {
        if (isRehashBonus) {
            ss << name << " Bonus Exceed Tests (will attempt to exceed maxExpected elements and insert " << bonusMaxExpected << " elements)";
        }
        else {
            ss << name << " Exceed Tests (will attempt to exceed maxExpected elements and insert " << bonusMaxExpected << " elements)";
        }
        h1w();
        testBattery<Key_t, KeyGen_t>(bonusMaxExpected, table, keyGen);
    }
    else {
        if (isRehashBonus) {
            ss << "There were errors, skipping bonus exceed tests for " << name << "...";
        }
        else {
            ss << "There were errors, skipping exceed tests for " << name << "...";
        }
        h1w();
    }

    if (success) {
        ss << "Finished " << name << " tests with no errors! Success!";
        h1w();
    }
    else {
        ss << "Finished " << name << " tests with errors... fix them and try again";
        h1w();
    }

    return success;
}

struct StrKeyGen {
    std::stringstream stream;

    std::string operator()(int k) {
        stream.clear();
        stream.str(std::string());
        stream << "[KEY_" << k << ']';
        return stream.str();
    }
};

int main(){
    StrKeyGen keyGen;
#if TEST_SIMPLE
    Simple<std::string, int> simple(MAX_EXPECTED);
    testTable(std::string("Simple Table"), simple, keyGen, false, SIMPLE_BONUS_MAX_EXPECTED);
#endif

#if TEST_LPHASH
#if TEST_SIMPLE
    std::cout << std::endl;
#endif
    LPHash<std::string, int> lphash(MAX_EXPECTED);
    testTable(std::string("LPHash Table"), lphash, keyGen, true);
#endif

#if TEST_CHAINHASH
#if TEST_SIMPLE || TEST_LPHASH
    std::cout << std::endl;
#endif
    ChainHash<std::string, int> chainhash(MAX_EXPECTED);
    testTable(std::string("ChainHash Table"), chainhash, keyGen);
#endif

    std::cout << "Done. Type anything to continue..." << std::endl;
    std::string s;
    std::cin >> s;
}
