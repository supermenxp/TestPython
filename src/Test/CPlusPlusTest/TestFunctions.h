#ifndef _TESTFUNCTIONS_H_
#define _TESTFUNCTIONS_H_
/************************************************************************
 *@description:
 *@author: zhaohaisheng
 *@date: 2023-05-05
 *@version: V1.0.0
 *************************************************************************/

/// @brief 测试string类的构造函数
extern void test_string_construct();
/// @brief 测试看看能不能调用namespace中未明确输出的类
/// 测试结果表明，没有明确输出的函数是不能在dll模块之外调用的
extern void test_namespace_functions();

/// @brief 测试std::bitset
extern void test_bitset();

#endif