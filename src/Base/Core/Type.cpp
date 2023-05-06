/************************************************************************
 *@description:
 *@author: zhaohaisheng
 *@date: 2023-05-05
 *@version: V1.0.0
 *************************************************************************/
#include "Type.h"
#include "Interpreter.h"

using namespace Base;
using namespace std;

struct Base::TypeData
{
    TypeData(const char *theName,
             const Type type = Type::badType(),
             const Type theParent = Type::badType(),
             Type::instantiationMethod method = nullptr) : name(theName), parent(theParent), type(type), instMethod(method) {}

    std::string name;
    Type parent;
    Type type;
    Type::instantiationMethod instMethod;
};

map<string, unsigned int> Type::typemap;
vector<TypeData *> Type::typedata;
set<string> Type::loadModuleSet;

Type::Type()
    : index(0)
{
}

Type::Type(const Type &type)
    : index(type.index)
{
}

Type Type::badType()
{
    Type bad;
    bad.index = 0;
    return bad;
}

void *Type::createInstance()
{
    instantiationMethod method = typedata[index]->instMethod;

    if (method != nullptr)
    {
        return (*method)();
    }

    return nullptr;
}

const Type Type::getParent() const
{
    return typedata[index]->parent;
}

bool Type::isDerivedFrom(const Type type) const
{

    Type temp(*this);
    do
    {
        if (temp == type)
            return true;
        temp = temp.getParent();
    } while (temp != badType());

    return false;
}

unsigned int Type::getKey() const
{
    return index;
}

const char *Type::getName() const
{
    return typedata[index]->name.c_str();
}

bool Type::isBad() const
{
    return (this->index == 0);
}

void *Type::createInstanceByName(const char *TypeName, bool bLoadModule)
{
    // 根据参数看是否导入模块
    if (bLoadModule)
        importModule(TypeName);

    // now the type should be in the type map
    Type t = fromName(TypeName);
    if (t == badType())
        return nullptr;
    return t.createInstance();
}

void Type::importModule(const char *TypeName)
{
    // 得到模块的名字
    string Mod = getModuleName(TypeName);

    // 基础模块忽略掉
    if (Mod != "App" && Mod != "Gui" && Mod != "Base")
    {
        // 找到已经加载的模块
        set<string>::const_iterator pos = loadModuleSet.find(Mod);
        if (pos == loadModuleSet.end())
        {
            // python解释器加载模块
            Interpreter().loadModule(Mod.c_str());
#ifdef FC_LOGLOADMODULE
            Console().Log("Act: Module %s loaded through class %s \n", Mod.c_str(), TypeName);
#endif
            loadModuleSet.insert(Mod);
        }
    }
}

Type Type::fromName(const char *name)
{
    std::map<std::string, unsigned int>::const_iterator pos;

    pos = typemap.find(name);
    if (pos != typemap.end())
        return typedata[pos->second]->type;
    else
        return Type::badType();
}

Type Type::fromKey(unsigned int key)
{
    if (key < typedata.size())
        return typedata[key]->type;
    else
        return Type::badType();
}

void Type::init()
{
    assert(Type::typedata.size() == 0);

    Type::typedata.push_back(new TypeData("BadType"));
    Type::typemap["BadType"] = 0;
}

void Type::destruct()
{
    for (std::vector<TypeData *>::const_iterator it = typedata.begin(); it != typedata.end(); ++it)
        delete *it;
    typedata.clear();
    typemap.clear();
    loadModuleSet.clear();
}

string Type::getModuleName(const char *ClassName)
{
    string temp(ClassName);
    // 找到第一个::
    std::string::size_type pos = temp.find_first_of("::");

    if (pos != std::string::npos)
        return string(temp, 0, pos); // 返回第一个::之前字符串
    else
        return string();
}

const Type Type::createType(const Type parent, const char *name, instantiationMethod method)
{
    Type newType;
    // 新建类型序号是Type::typedata数组的大小
    newType.index = static_cast<unsigned int>(Type::typedata.size());
    TypeData *typeData = new TypeData(name, newType, parent, method);
    Type::typedata.push_back(typeData);

    // add to dictionary for fast lookup
    Type::typemap[name] = newType.getKey();

    return newType;
}

int Type::getAllDerivedFrom(const Type type, std::vector<Type> &List)
{
    int cnt = 0;

    for (std::vector<TypeData *>::const_iterator it = typedata.begin(); it != typedata.end(); ++it)
    {
        if ((*it)->type.isDerivedFrom(type))
        {
            List.push_back((*it)->type);
            cnt++;
        }
    }
    return cnt;
}

Type Type::getTypeIfDerivedFrom(const char *name, const Type parent, bool bLoadModule)
{
    if (bLoadModule)
        importModule(name);

    Type type = fromName(name);

    if (type.isDerivedFrom(parent))
        return type;
    else
        return Type::badType();
}

int Type::getNumTypes()
{
    return static_cast<int>(typedata.size());
}

bool Type::operator==(const Type type) const
{
    return (this->getKey() == type.getKey());
}

bool Type::operator!=(const Type type) const
{
    return (this->getKey() != type.getKey());
}

void Type::operator=(const Type type)
{
    this->index = type.getKey();
}

bool Type::operator<(const Type type) const
{
    return (this->getKey() < type.getKey());
}

bool Type::operator<=(const Type type) const
{
    return (this->getKey() <= type.getKey());
}

bool Type::operator>=(const Type type) const
{
    return (this->getKey() >= type.getKey());
}

bool Type::operator>(const Type type) const
{
    return (this->getKey() > type.getKey());
}