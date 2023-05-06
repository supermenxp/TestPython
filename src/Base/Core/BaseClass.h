#ifndef BASE_BASECLASS_H_
#define BASE_BASECLASS_H_
/************************************************************************
 *@description:
 *@author: zhaohaisheng
 *@date: 2023-05-05
 *@version: V1.0.0
 *************************************************************************/

#include "Type.h"

// Python stuff
using PyObject = struct _object;

/// define for subclassing Base::BaseClass
/// 声明类型系统
#define TYPESYSTEM_HEADER()                   \
public:                                       \
    static Base::Type getClassTypeId(void);   \
    virtual Base::Type getTypeId(void) const; \
    static void init(void);                   \
    static void *create(void);                \
                                              \
private:                                      \
    static Base::Type classTypeId

/// Like TYPESYSTEM_HEADER, but declare getTypeId as 'override'
/// 声明类型系统
#define TYPESYSTEM_HEADER_WITH_OVERRIDE()              \
public:                                                \
    static Base::Type getClassTypeId(void);            \
    virtual Base::Type getTypeId(void) const override; \
    static void init(void);                            \
    static void *create(void);                         \
                                                       \
private:                                               \
    static Base::Type classTypeId

/// define to implement a  subclass of Base::BaseClass
/// 实现类型系统
#define TYPESYSTEM_SOURCE_P(_class_)                                           \
    Base::Type _class_::getClassTypeId(void) { return _class_::classTypeId; }  \
    Base::Type _class_::getTypeId(void) const { return _class_::classTypeId; } \
    Base::Type _class_::classTypeId = Base::Type::badType();                   \
    void *_class_::create(void)                                                \
    {                                                                          \
        return new _class_();                                                  \
    }

/// define to implement a  subclass of Base::BaseClass
/// 模板类型的类型系统实现
#define TYPESYSTEM_SOURCE_TEMPLATE_P(_class_)                                  \
    template <>                                                                \
    Base::Type _class_::getClassTypeId(void) { return _class_::classTypeId; }  \
    template <>                                                                \
    Base::Type _class_::getTypeId(void) const { return _class_::classTypeId; } \
    template <>                                                                \
    void *_class_::create(void)                                                \
    {                                                                          \
        return new _class_();                                                  \
    }

/// define to implement a  subclass of Base::BaseClass
/// 实现类型系统，返回是nullptr
#define TYPESYSTEM_SOURCE_ABSTRACT_P(_class_)                                  \
    Base::Type _class_::getClassTypeId(void) { return _class_::classTypeId; }  \
    Base::Type _class_::getTypeId(void) const { return _class_::classTypeId; } \
    Base::Type _class_::classTypeId = Base::Type::badType();                   \
    void *_class_::create(void) { return 0; }

/// define to implement a subclass of Base::BaseClass
/// 实现类型系统，增加了init函数的实现
#define TYPESYSTEM_SOURCE(_class_, _parentclass_)                                         \
    TYPESYSTEM_SOURCE_P(_class_)                                                          \
    void _class_::init(void)                                                              \
    {                                                                                     \
        initSubclass(_class_::classTypeId, #_class_, #_parentclass_, &(_class_::create)); \
    }

/// define to implement a subclass of Base::BaseClass
/// 实现模板类型的类型系统，增加了init函数的实现
#define TYPESYSTEM_SOURCE_TEMPLATE_T(_class_, _parentclass_)                              \
    TYPESYSTEM_SOURCE_TEMPLATE_P(_class_)                                                 \
    template <>                                                                           \
    void _class_::init(void)                                                              \
    {                                                                                     \
        initSubclass(_class_::classTypeId, #_class_, #_parentclass_, &(_class_::create)); \
    }

/// define to implement a subclass of Base::BaseClass
/// 实现类型系统，增加了init函数的实现
#define TYPESYSTEM_SOURCE_ABSTRACT(_class_, _parentclass_)                                \
    TYPESYSTEM_SOURCE_ABSTRACT_P(_class_)                                                 \
    void _class_::init(void)                                                              \
    {                                                                                     \
        initSubclass(_class_::classTypeId, #_class_, #_parentclass_, &(_class_::create)); \
    }

namespace Base
{
    /// @brief
    class BaseExport BaseClass
    {
    public:
        BaseClass() = default;
        BaseClass(const BaseClass &other) = default;
        BaseClass &operator=(const BaseClass &) = default;
        /// Destruction
        virtual ~BaseClass() = default;

    private:
        static Type classTypeId;

    public:
        static void *create(); // { return nullptr; }
        static Type getClassTypeId();
        static void init();

    public:
        /// @brief 获取类型的id
        /// @return
        virtual Type getTypeId() const;
        /// @brief 判断实例是否继承自另一个类？
        /// @param type
        /// @return
        bool isDerivedFrom(const Type type) const; //{ return getTypeId().isDerivedFrom(type); }

        virtual PyObject *getPyObject();

        virtual void setPyObject(PyObject *);

    protected:
        /// @brief 注册子类？
        /// @param toInit
        /// @param ClassName
        /// @param ParentName
        /// @param method
        static void initSubclass(Base::Type &toInit, const char *ClassName, const char *ParentName, Type::instantiationMethod method = nullptr);
    };

    /**
     * Template that works just like dynamic_cast, but expects the argument to
     * inherit from Base::BaseClass.
     *
     */
    template <typename T>
    T *freecad_dynamic_cast(Base::BaseClass *t)
    {
        if (t && t->isDerivedFrom(T::getClassTypeId()))
            return static_cast<T *>(t);
        else
            return nullptr;
    }

    /**
     * Template that works just like dynamic_cast, but expects the argument to
     * inherit from a const Base::BaseClass.
     *
     */
    template <typename T>
    const T *freecad_dynamic_cast(const Base::BaseClass *t)
    {
        if (t && t->isDerivedFrom(T::getClassTypeId()))
            return static_cast<const T *>(t);
        else
            return nullptr;
    }
}

#endif