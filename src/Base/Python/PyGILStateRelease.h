#ifndef BASE_PYGILSTATERELEASE_H_
#define BASE_PYGILSTATERELEASE_H_
/************************************************************************
 *@description:
 *@author: zhaohaisheng
 *@date: 2023-05-05
 *@version: V1.0.0
 *************************************************************************/

#include <Python.h>
#include "GlobalDefine.h"

namespace Base
{

    /// @brief 如果线程持有全局解释器锁（GIL），但在C中运行一个不需要持有GIL的长操作，则可以暂时释放它。
    /// 或者，如果线程必须在可能执行Python代码的主线程中运行代码，则必须释放GIL以避免死锁。
    /// 在任何一种情况下，当实例化PyGILStateRelease的对象时，线程都必须获取了GIL
    class BaseExport PyGILStateRelease
    {
    public:
        PyGILStateRelease()
        {
            // 释放GIL
            state = PyEval_SaveThread();
        }
        ~PyGILStateRelease()
        {
            // 重新获取GIL
            PyEval_RestoreThread(state);
        }

    private:
        PyThreadState *state;
    };
}

#endif