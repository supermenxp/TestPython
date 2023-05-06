#ifndef BASE_INTERPRETER_H_
#define BASE_INTERPRETER_H_
/************************************************************************
 *@description:
 *@author: zhaohaisheng
 *@date: 2023-05-05
 *@version: V1.0.0
 *************************************************************************/
#include <list>
#include <CXX/Extensions.hxx>

#include "GlobalDefine.h"

namespace Base
{
    /// @brief 管理python解释器，并提供一些处理Python成员的辅助函数
    class BaseExport InterpreterSingleton
    {
    public:
        InterpreterSingleton();
        ~InterpreterSingleton() = default;

    public:
        /// @brief 执行python语句
        /// @param psCmd
        /// @return 执行结果
        std::string runString(const char *psCmd);

        /// @brief 加载python模块
        /// @param psModName
        /// @return
        bool loadModule(const char *psModName);

    public:
        static InterpreterSingleton &Instance();

    protected:
        static InterpreterSingleton *_pcSingleton;

    private:
        std::string _cDebugFileName;
        PyThreadState *_global;
        std::list<Py::ExtensionModuleBase *> _modules;
    };

    InterpreterSingleton &Interpreter();
}

#endif