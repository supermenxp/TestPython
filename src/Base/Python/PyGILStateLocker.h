#ifndef BASE_PYGILSTATELOCKER_H_
#define BASE_PYGILSTATELOCKER_H_
/************************************************************************
 *@description:
 *@author: zhaohaisheng
 *@date: 2023-05-05
 *@version: V1.0.0
 *************************************************************************/

#include <Python.h>

namespace Base
{

    /// @brief 当应用程序启动后，会立即释放全局解释器锁
    /// 此时即使是主线程也不包括GIL。因此，每个线程都必须实例化PyGILStateLocker
    /// 此类的对象应避免在堆中创建
    class BaseExport PyGILStateLocker
    {
    public:
        PyGILStateLocker()
        {
            gstate = PyGILState_Ensure();
        }
        ~PyGILStateLocker()
        {
            PyGILState_Release(gstate);
        }

    private:
        PyGILState_STATE gstate;
    };
}

#endif