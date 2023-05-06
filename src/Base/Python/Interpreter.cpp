/************************************************************************
 *@description:
 *@author: zhaohaisheng
 *@date: 2023-05-05
 *@version: V1.0.0
 *************************************************************************/
#include "Interpreter.h"
#include "PyGILStateLocker.h"
#include "PyGILStateRelease.h"
#include "PyTools.h"

using namespace Base;

InterpreterSingleton::InterpreterSingleton()
{
    _global = nullptr;
}

std::string InterpreterSingleton::runString(const char *sCmd)
{
    //     PyObject *module, *dict, *presult; /* "exec code in d, d" */

    //     PyGILStateLocker locker;
    //     module = PP_Load_Module("__main__"); /* get module, init python */
    //     if (!module)
    //         throw PyException();         /* not incref'd */
    //     dict = PyModule_GetDict(module); /* get dict namespace */
    //     if (!dict)
    //         throw PyException(); /* not incref'd */

    //     presult = PyRun_String(sCmd, Py_file_input, dict, dict); /* eval direct */
    //     if (!presult)
    //     {
    //         if (PyErr_ExceptionMatches(PyExc_SystemExit))
    //             throw SystemExitException();
    //         else
    //         {
    //             PyException::ThrowException();
    //             return std::string(); // just to quieten code analyzers
    //             // throw PyException();
    //         }
    //     }

    //     PyObject *repr = PyObject_Repr(presult);
    //     Py_DECREF(presult);
    //     if (repr)
    //     {
    //         std::string ret(PyUnicode_AsUTF8(repr));
    //         Py_DECREF(repr);
    //         return ret;
    //     }
    //     else
    //     {
    //         PyErr_Clear();
    //         return std::string();
    //     }

    return std::string(); // 后面要删除的
}

bool InterpreterSingleton::loadModule(const char *psModName)
{
    // buffer acrobatics
    // PyBuf ModName(psModName);
    PyObject *module;

    PyGILStateLocker locker;
    module = PP_Load_Module(psModName);

    // 如果导入失败则发出异常

    // if (!module)
    // {
    //     if (PyErr_ExceptionMatches(PyExc_SystemExit))
    //         throw SystemExitException();
    //     else
    //         throw PyException();
    // }

    return true;
}

InterpreterSingleton *InterpreterSingleton::_pcSingleton = nullptr;

InterpreterSingleton &InterpreterSingleton::Instance()
{
    if (_pcSingleton == nullptr)
        _pcSingleton = new InterpreterSingleton();
    return *_pcSingleton;
}

InterpreterSingleton &Base::Interpreter()
{
    return InterpreterSingleton::Instance();
}