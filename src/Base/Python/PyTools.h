#ifndef BASE_PYTOOLS_H_
#define BASE_PYTOOLS_H_
/************************************************************************
 *@description:
 *@author: zhaohaisheng
 *@date: 2023-05-05
 *@version: V1.0.0
 *************************************************************************/

#include <Python.h>

/// @brief reload模块
extern int PP_RELOAD;
/// @brief 启动调试
extern int PP_DEBUG;

/// @brief 1.初始化python解释器
/// 2.如果没有指定模块名，返回"__main__"；如果指定了模块名，则返回模块名
/// @param modname
/// @return
extern const char *PP_Init(const char *modname);

/// @brief 加载python模块
/// @param modname 模块名，可以是"package.module"
/// @return python模块对象
extern PyObject *PP_Load_Module(const char *modname);

#endif