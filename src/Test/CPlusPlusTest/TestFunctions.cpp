/************************************************************************
 *@description:
 *@author: zhaohaisheng
 *@date: 2023-05-05
 *@version: V1.0.0
 *************************************************************************/
#include "TestFunctions.h"

#include <string>
#include <iostream>
#include <bitset>
#include "PyObjectBase.h"

using namespace std;

void test_string_construct()
{
    char *str = "Base::Type";

    string tmp(str);

    string::size_type pos = tmp.find_first_of("::");

    if (pos != std::string::npos)
    {
        string temp(tmp, 0, pos);
    }
}

void test_namespace_functions()
{
    // Base::streq("test", "Test");
}

void test_bitset()
{
    bitset<16> foo;                                 // 无参构造，长度为16，默认每一位为０
    bitset<16> bar(0xfa2);                          // 长度为16，二进制保存，前面用０补充
    std::bitset<16> baz(std::string("0101111001")); // 长度为16，前面用０补充

    cout << "sizeof(bitset<16>) = " << sizeof(foo) << endl;
    cout << "foo = " << foo << endl;

    cout << "sizeof(bitset<16>) = " << sizeof(bar) << endl;
    cout << "bar = " << bar << endl;

    cout << "sizeof(bitset<16>) = " << sizeof(baz) << endl;
    cout << "baz = " << baz << endl;
}