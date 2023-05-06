/************************************************************************
 *@description:
 *@author: zhaohaisheng
 *@date: 2023-05-05
 *@version: V1.0.0
 *************************************************************************/
#include "BaseClass.h"
#include "PyObjectBase.h"
#include <cassert>

using namespace Base;

Type BaseClass::classTypeId = Base::Type::badType();

void BaseClass::initSubclass(Base::Type &toInit, const char *ClassName, const char *ParentName,
                             Type::instantiationMethod method)
{
    // don't init twice!
    assert(toInit == Base::Type::badType());
    // get the parent class
    Base::Type parentType(Base::Type::fromName(ParentName));
    // forgot init parent!
    assert(parentType != Base::Type::badType());

    // create the new type
    toInit = Base::Type::createType(parentType, ClassName, method);
}

void *BaseClass::create()
{
    return nullptr;
}

Type BaseClass::getClassTypeId()
{
    return BaseClass::classTypeId;
}

void BaseClass::init()
{
    assert(BaseClass::classTypeId == Type::badType() && "don't init() twice!");
    /* Make sure superclass gets initialized before subclass. */
    /*assert(strcmp(#_parentclass_), "inherited"));*/
    /*Type parentType(Type::fromName(#_parentclass_));*/
    /*assert(parentType != Type::badType() && "you forgot init() on parentclass!");*/

    /* Set up entry in the type system. */
    BaseClass::classTypeId =
        Type::createType(Type::badType(),
                         "Base::BaseClass",
                         BaseClass::create);
}

Type BaseClass::getTypeId() const
{
    return BaseClass::classTypeId;
}

bool BaseClass::isDerivedFrom(const Type type) const
{
    return getTypeId().isDerivedFrom(type);
}

PyObject *BaseClass::getPyObject()
{
    assert(0);
    Py_Return;
}

void BaseClass::setPyObject(PyObject *)
{
    assert(0);
}