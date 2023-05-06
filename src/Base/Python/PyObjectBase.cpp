/************************************************************************
 *@description:
 *@author: zhaohaisheng
 *@date: 2023-05-03
 *@version: V1.0.0
 *************************************************************************/

#include "PyObjectBase.h"

using namespace Base;

namespace Base
{
    int streq(const char *A, const char *B)
    {
        return strcmp(A, B) == 0;
    }

    void Assert(int expr, char *msg)
    {
        if (!expr)
        {
            fprintf(stderr, "%s\n", msg);
            exit(-1);
        };
    }

    PyObject *getTypeAsObject(PyTypeObject *type)
    {
        PyObject *obj;
        std::memcpy(&obj, &type, sizeof(type));
        return obj;
    }

    bool asBoolean(PyObject *obj)
    {
        return PyObject_IsTrue(obj) != 0;
    }
}

PyTypeObject PyObjectBase::Type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0) "PyObjectBase", /*tp_name*/
    sizeof(PyObjectBase),                                  /*tp_basicsize*/
    0,                                                     /*tp_itemsize*/
    /* --- methods ---------------------------------------------- */
    PyDestructor, /*tp_dealloc*/
#if PY_VERSION_HEX >= 0x03080000
    0, /*tp_vectorcall_offset*/
#else
    nullptr, /*tp_print*/
#endif
    nullptr,    /*tp_getattr*/
    nullptr,    /*tp_setattr*/
    nullptr,    /*tp_compare*/
    __repr,     /*tp_repr*/
    nullptr,    /*tp_as_number*/
    nullptr,    /*tp_as_sequence*/
    nullptr,    /*tp_as_mapping*/
    nullptr,    /*tp_hash*/
    nullptr,    /*tp_call */
    nullptr,    /*tp_str  */
    __getattro, /*tp_getattro*/
    __setattro, /*tp_setattro*/
    /* --- Functions to access object as input/output buffer ---------*/
    nullptr, /* tp_as_buffer */
    /* --- Flags to define presence of optional/expanded features */
    Py_TPFLAGS_BASETYPE | Py_TPFLAGS_DEFAULT, /*tp_flags */
    "The most base class for Python binding", /*tp_doc */
    nullptr,                                  /*tp_traverse */
    nullptr,                                  /*tp_clear */
    nullptr,                                  /*tp_richcompare */
    0,                                        /*tp_weaklistoffset */
    nullptr,                                  /*tp_iter */
    nullptr,                                  /*tp_iternext */
    nullptr,                                  /*tp_methods */
    nullptr,                                  /*tp_members */
    nullptr,                                  /*tp_getset */
    nullptr,                                  /*tp_base */
    nullptr,                                  /*tp_dict */
    nullptr,                                  /*tp_descr_get */
    nullptr,                                  /*tp_descr_set */
    0,                                        /*tp_dictoffset */
    nullptr,                                  /*tp_init */
    nullptr,                                  /*tp_alloc */
    nullptr,                                  /*tp_new */
    nullptr,                                  /*tp_free   Low-level free-memory routine */
    nullptr,                                  /*tp_is_gc  For PyObject_IS_GC */
    nullptr,                                  /*tp_bases */
    nullptr,                                  /*tp_mro    method resolution order */
    nullptr,                                  /*tp_cache */
    nullptr,                                  /*tp_subclasses */
    nullptr,                                  /*tp_weaklist */
    nullptr,                                  /*tp_del */
    0,                                        /*tp_version_tag */
    nullptr                                   /*tp_finalize */
#if PY_VERSION_HEX >= 0x03090000
    ,
    0 /*tp_vectorcall */
#elif PY_VERSION_HEX >= 0x03080000
    ,
    0 /*tp_vectorcall */
    /* bpo-37250: kept for backwards compatibility in CPython 3.8 only */
    ,
    0 /*tp_print */
#endif
};

/*------------------------------
 * PyObjectBase Methods 	-- Every class, even the abstract one should have a Methods
------------------------------*/
PyMethodDef PyObjectBase::Methods[] = {
    {nullptr, nullptr, 0, nullptr} /* Sentinel */
};

PyObjectBase::~PyObjectBase()
{
}

bool PyObjectBase::isValid()
{
    return StatusBits.test(Valid);
}

/*------------------------------
 * PyObjectBase repr    representations
------------------------------*/
PyObject *PyObjectBase::_repr()
{
    std::stringstream a;
    a << "<base object at " << _pcTwinPointer << ">";
#ifdef FCDebug
    Console().Log("PyObjectBase::_repr() not overwritten representation!");
#endif
    return Py_BuildValue("s", a.str().c_str());
}

/*------------------------------
 * PyObjectBase attributes	-- attributes
------------------------------*/
PyObject *PyObjectBase::_getattr(const char *attr)
{
    if (streq(attr, "__class__"))
    {
        // Note: We must return the type object here,
        // so that our own types feel as really Python objects
        Py_INCREF(Py_TYPE(this));
        return reinterpret_cast<PyObject *>(Py_TYPE(this));
    }
    else if (streq(attr, "__members__"))
    {
        // Use __dict__ instead as __members__ is deprecated
        return nullptr;
    }
    else if (streq(attr, "__dict__"))
    {
        // Return the default dict
        PyTypeObject *tp = Py_TYPE(this);
        Py_XINCREF(tp->tp_dict);
        return tp->tp_dict;
    }
    else if (streq(attr, "softspace"))
    {
        // Internal Python stuff
        return nullptr;
    }
    else
    {
        // As fallback solution use Python's default method to get generic attributes
        PyObject *w, *res;
        w = PyUnicode_InternFromString(attr);
        if (w)
        {
            res = PyObject_GenericGetAttr(this, w);
            Py_XDECREF(w);
            return res;
        }
        else
        {
            // Throw an exception for unknown attributes
            PyTypeObject *tp = Py_TYPE(this);
            PyErr_Format(PyExc_AttributeError, "%.50s instance has no attribute '%.400s'", tp->tp_name, attr);
            return nullptr;
        }
    }
}

int PyObjectBase::_setattr(const char *attr, PyObject *value)
{
    if (streq(attr, "softspace"))
        return -1; // filter out softspace
    PyObject *w;
    // As fallback solution use Python's default method to get generic attributes
    w = PyUnicode_InternFromString(attr); // new reference
    if (w)
    {
        // call methods from tp_getset if defined
        int res = PyObject_GenericSetAttr(this, w, value);
        Py_DECREF(w);
        return res;
    }
    else
    {
        // Throw an exception for unknown attributes
        PyTypeObject *tp = Py_TYPE(this);
        PyErr_Format(PyExc_AttributeError, "%.50s instance has no attribute '%.400s'", tp->tp_name, attr);
        return -1;
    }
}

void PyObjectBase::PyDestructor(PyObject *P)
{
    delete ((PyObjectBase *)P);
}

PyObject *PyObjectBase::__repr(PyObject *PyObj)
{
    if (!((PyObjectBase *)PyObj)->isValid())
    {
        PyErr_Format(PyExc_ReferenceError, "Cannot print representation of deleted object");
        return nullptr;
    }
    return ((PyObjectBase *)PyObj)->_repr();
}

PyObject *PyObjectBase::__getattro(PyObject *obj, PyObject *attro)
{
    const char *attr;
    attr = PyUnicode_AsUTF8(attro);

    // For the __class__ attribute get it directly as with
    // ExtensionContainerPy::getCustomAttributes we may get
    // the wrong type object (#0003311)
    if (streq(attr, "__class__"))
    {
        PyObject *res = PyObject_GenericGetAttr(obj, attro);
        if (res)
            return res;
    }

    // This should be the entry in Type
    PyObjectBase *pyObj = static_cast<PyObjectBase *>(obj);
    if (!pyObj->isValid())
    {
        PyErr_Format(PyExc_ReferenceError, "Cannot access attribute '%s' of deleted object", attr);
        return nullptr;
    }

#ifdef ATTR_TRACKING
    // If an attribute references this as parent then reset it (bug #0002902)
    PyObject *cur = pyObj->getTrackedAttribute(attr);
    if (cur)
    {
        if (PyObject_TypeCheck(cur, &(PyObjectBase::Type)))
        {
            PyObjectBase *base = static_cast<PyObjectBase *>(cur);
            base->resetAttribute();
            pyObj->untrackAttribute(attr);
        }
    }
#endif

    PyObject *value = pyObj->_getattr(attr);
#ifdef ATTR_TRACKING
    if (value && PyObject_TypeCheck(value, &(PyObjectBase::Type)))
    {
        if (!static_cast<PyObjectBase *>(value)->isConst() &&
            !static_cast<PyObjectBase *>(value)->isNotTracking())
        {
            static_cast<PyObjectBase *>(value)->setAttributeOf(attr, pyObj);
            pyObj->trackAttribute(attr, value);
        }
    }
    else
#endif
        if (value && PyCFunction_Check(value))
    {
        // ExtensionContainerPy::initialization() transfers the methods of an
        // extension object by creating PyCFunction objects.
        // At this point no 'self' object is passed but is handled and determined
        // in ExtensionContainerPy::getCustomAttributes().
        // So, if we come through this section then it's an indication that
        // something is wrong with the Python types. For example, a C++ class
        // that adds an extension uses the same Python type as a wrapper than
        // another C++ class without this extension.
        PyCFunctionObject *cfunc = reinterpret_cast<PyCFunctionObject *>(value);
        if (!cfunc->m_self)
        {
            Py_DECREF(cfunc);
            value = nullptr;
            PyErr_Format(PyExc_AttributeError, "<no object bound to built-in method %s>", attr);
        }
    }

    return value;
}

int PyObjectBase::__setattro(PyObject *obj, PyObject *attro, PyObject *value)
{
    const char *attr;
    attr = PyUnicode_AsUTF8(attro);

    // Hint: In general we don't allow to delete attributes (i.e. value=0). However, if we want to allow
    // we must check then in _setattr() of all subclasses whether value is 0.
    if (!value)
    {
        PyErr_Format(PyExc_AttributeError, "Cannot delete attribute: '%s'", attr);
        return -1;
    }
    else if (!static_cast<PyObjectBase *>(obj)->isValid())
    {
        PyErr_Format(PyExc_ReferenceError, "Cannot access attribute '%s' of deleted object", attr);
        return -1;
    }

#ifdef ATTR_TRACKING
    // If an attribute references this as parent then reset it
    // before setting the new attribute
    PyObject *cur = static_cast<PyObjectBase *>(obj)->getTrackedAttribute(attr);
    if (cur)
    {
        if (PyObject_TypeCheck(cur, &(PyObjectBase::Type)))
        {
            PyObjectBase *base = static_cast<PyObjectBase *>(cur);
            base->resetAttribute();
            static_cast<PyObjectBase *>(obj)->untrackAttribute(attr);
        }
    }
#endif

    int ret = static_cast<PyObjectBase *>(obj)->_setattr(attr, value);
#ifdef ATTR_TRACKING
    if (ret == 0)
    {
        static_cast<PyObjectBase *>(obj)->startNotify();
    }
#endif
    return ret;
}
