#ifndef __TESTER_HPP__
#define __TESTER_HPP__

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

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

    void h3(const std::string& s){
        std::cout << "    [" << s << "]:" << std::endl;
        indent = "      ";
    }

    void h4(const std::string& s){
        std::cout << "      [" << s << "]:" << std::endl;
        indent = "        ";
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

    void h3w(){
        h3(ss.str());
        ss.clear();
        ss.str(std::string());
    }

    void h4w(){
        h4(ss.str());
        ss.clear();
        ss.str(std::string());
    }
}

#define ASSERT(test, m) \
    if (!(test)){\
        ss << "Test: [" << m << "], FAILED!"; \
        Testing::err(); \
        return false; \
    } \
    else {\
        ss << "Test: [" << m << "], PASSED!"; \
        Testing::write(); \
    }

#define ASSERT_EQ(p1, p2, m) \
    ASSERT((p1) == (p2), m << ", got:" << p1 << ", expected:" << p2)

#define TESTER_COMMA() ,
#endif
