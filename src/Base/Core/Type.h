#ifndef BASE_TYPE_H_
#define BASE_TYPE_H_
/************************************************************************
 *@description:
 *@author: zhaohaisheng
 *@date: 2023-05-05
 *@version: V1.0.0
 *************************************************************************/
#include "GlobalDefine.h"
#include <set>
#include <map>
#include <vector>
#include <string>
// #include "Exception.h"

namespace Base
{
    struct TypeData;

    class BaseExport Type
    {
    public:
        Type(const Type &type);
        Type();
        virtual ~Type() = default;

        typedef void *(*instantiationMethod)();

    private:
        /// @brief 类注册时的索引值？
        unsigned int index;

        /// @brief 名字为键，index为值得字典对象
        static std::map<std::string, unsigned int> typemap;

        /// @brief 类注册时的索引值为vector数组的索引值
        static std::vector<TypeData *> typedata;

        /// @brief python模块集合
        static std::set<std::string> loadModuleSet;

    public:
        static Type badType();
        /// @brief 根据名字创建实例对象
        /// @param TypeName
        /// @param bLoadModule
        /// @return
        static void *createInstanceByName(const char *TypeName, bool bLoadModule = false);

        /// @brief 导入python模块
        /// @param TypeName
        static void importModule(const char *TypeName);

        /// @brief 根据类型的名称获得Type对象
        /// @param name
        /// @return 如果字典中不存在，则返回默认的Type对象
        static Type fromName(const char *name);

        /// @brief 根据类的ID得到类型对象
        /// @param key
        /// @return 如果key的值超过数组大小，则返回默认的Type对象
        static Type fromKey(unsigned int key);

        /// @brief
        /// @param parent
        /// @param name
        /// @param method
        /// @return
        static const Type createType(const Type parent, const char *name, instantiationMethod method = nullptr);

        /// @brief 获得所有子类的typeId
        /// @param type 父类的类型对象
        /// @param List
        /// @return
        static int getAllDerivedFrom(const Type type, std::vector<Type> &List);

        /// @brief 根据名称找到类型，如果该类型是继承自父类型，则返回该类型，否则返回默认的类型
        /// @param name
        /// @param parent
        /// @param bLoadModule
        /// @return
        static Type getTypeIfDerivedFrom(const char *name, const Type parent, bool bLoadModule = false);

        /// @brief 返回已注册类型的数量
        /// @return
        static int getNumTypes();

        static void init();
        static void destruct();

    protected:
        /// @brief 获取模块名
        /// @param ClassName
        /// @return
        static std::string getModuleName(const char *ClassName);

    public:
        /// @brief 创建类的实例
        /// @return
        void *createInstance();

        /// @brief 获取类型index值
        /// @return
        unsigned int getKey() const;

        bool isDerivedFrom(const Type type) const;

        /// @brief 获取父类的Typeid
        /// @return
        const Type getParent() const;

        const char *getName() const;

        bool isBad() const;

    public:
        bool operator==(const Type type) const;
        bool operator!=(const Type type) const;

        void operator=(const Type type);

        bool operator<(const Type type) const;
        bool operator<=(const Type type) const;
        bool operator>=(const Type type) const;
        bool operator>(const Type type) const;
    };
}

#endif