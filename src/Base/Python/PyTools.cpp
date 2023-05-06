/************************************************************************
 *@description:
 *@author: zhaohaisheng
 *@date: 2023-05-05
 *@version: V1.0.0
 *************************************************************************/

#include "PyTools.h"

int PP_RELOAD = 0;
int PP_DEBUG = 0;

const char *PP_Init(const char *modname)
{
    Py_Initialize(); // 初始化python解释器

    if (modname != NULL)
        return modname;
    return "__main__";
}

PyObject *PP_Load_Module(const char *modname)
{
    PyObject *module, *sysmods;
    modname = PP_Init(modname); // 默认是__main__

    if (strcmp(modname, "__main__") == 0)   /* main: no file */
        return PyImport_AddModule(modname); /* not increfd */

    sysmods = PyImport_GetModuleDict();              /* get sys.modules dict */
    module = PyDict_GetItemString(sysmods, modname); /* mod in sys.modules? */

    return nullptr;
}