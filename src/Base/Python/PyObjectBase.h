#ifndef BASE_PYOBJECTBASE_H_
#define BASE_PYOBJECTBASE_H_
/************************************************************************
 *@description:
 *@author: zhaohaisheng
 *@date: 2023-05-03
 *@version: V1.0.0
 *************************************************************************/

#if defined(_POSIX_C_SOURCE)
#undef _POSIX_C_SOURCE
#endif
#if defined(_XOPEN_SOURCE)
#undef _XOPEN_SOURCE
#endif

#include <Python.h>

#include "CXX/Objects.hxx"

#include <bitset>
#include <cstring>

#include "GlobalDefine.h"

/** python函数定义
 * \code
 * static PyObject* X (PyObject *self,PyObject *args,PyObject *kwd);
 * \endcode
 * @param SFUNC 函数名
 * @see PYFUNCIMP_S
 */
#define PYFUNCDEF_S(SFUNC) static PyObject *SFUNC(PyObject *self, PyObject *args, PyObject *kwd)
/** Python 函数的实现
 * \code
 * PyObject* CLASS::SFUNC (PyObject *self,PyObject *args,PyObject *kwd)
 * \endcode
 * @param CLASS 类名
 * @param SFUNC 函数名
 * @see PYFUNCDEF_S
 */
#define PYFUNCIMP_S(CLASS, SFUNC) PyObject *CLASS::SFUNC(PyObject *self, PyObject *args, PyObject *kwd)

/** Python模块初始化函数宏.
 */
#define PyMOD_INIT_FUNC(name) PyMODINIT_FUNC PyInit_##name(void)

/** Python模块返回数据.
 */
#define PyMOD_Return(name) return name

namespace Base
{
    int streq(const char *A, const char *B);
    void Assert(int expr, char *msg);
    PyObject *getTypeAsObject(PyTypeObject *type);
    bool asBoolean(PyObject *obj);
}

/// some basic python macros
#define Py_NEWARGS 1
/// 返回None
#define Py_Return return Py_INCREF(Py_None), Py_None
/// returns an error
#define Py_Error(E, M) _Py_Error(return (nullptr), E, M)
#define _Py_Error(R, E, M)     \
    {                          \
        PyErr_SetString(E, M); \
        R;                     \
    }
/// returns an error
#define Py_ErrorObj(E, O) _Py_ErrorObj(return (nullptr), E, O)
#define _Py_ErrorObj(R, E, O)  \
    {                          \
        PyErr_SetObject(E, O); \
        R;                     \
    }
/// checks on a condition and returns an error on failure
#define Py_Try(F)        \
    {                    \
        if (!(F))        \
            return NULL; \
    }
/// assert which returns with an error on failure
#define Py_Assert(A, E, M)         \
    {                              \
        if (!(A))                  \
        {                          \
            PyErr_SetString(E, M); \
            return nullptr;        \
        }                          \
    }

/// This must be the first line of each PyC++ class
#define Py_Header                 \
public:                           \
    static PyTypeObject Type;     \
    static PyMethodDef Methods[]; \
    virtual PyTypeObject *GetType(void) { return &Type; }

namespace Base
{
    class BaseExport PyObjectBase : public PyObject
    {
        /** Py_Header struct from python.h.
         *  Every PyObjectBase object is also a python object. So you can use
         *  every Python C-Library function also on a PyObjectBase object
         */
        // Py_Header
    public:
        static PyTypeObject Type;
        static PyMethodDef Methods[];
        virtual PyTypeObject *GetType(void) { return &Type; }

        /// @brief
        enum Status
        {
            Valid = 0,
            Immutable = 1,
            Notify = 2,
            NoTrack = 3
        };

    protected:
        virtual ~PyObjectBase();

    protected:
        std::bitset<32> StatusBits;
        /// pointer to the handled class
        void *_pcTwinPointer;

    public:
        static void PyDestructor(PyObject *P); // python wrapper
        static PyObject *__repr(PyObject *PyObj);
        /// static wrapper for pythons _getattro()
        static PyObject *__getattro(PyObject *PyObj, PyObject *attro);
        /// static wrapper for pythons _setattro(). // This should be the entry in Type.
        static int __setattro(PyObject *PyObj, PyObject *attro, PyObject *value);

    public:
        bool isValid();
        virtual PyObject *_repr();
        /** GetAttribute implementation
         *  This method implements the retrieval of object attributes.
         *  If you want to implement attributes in your class, reimplement
         *  this method.
         *  You have to call the method of the base class.
         *  Note: if you reimplement _gettattr() in a inheriting class you
         *  need to call the method of the base class! Otherwise even the
         *  methods of the object will disappear!
         */
        virtual PyObject *_getattr(const char *attr);
        /** SetAttribute implementation
         *  This method implements the setting of object attributes.
         *  If you want to implement attributes in your class, reimplement
         *  this method.
         *  You have to call the method of the base class.
         */
        virtual int _setattr(const char *attro, PyObject *value); // _setattr method
    };
}

#endif