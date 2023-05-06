#ifndef _GLOBALDEFINE_H_
#define _GLOBALDEFINE_H_
/************************************************************************
 *@description:
 *@author: zhaohaisheng
 *@date: 2023-05-05
 *@version: V1.0.0
 *************************************************************************/

#if defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(__CYGWIN__)
#define LIB_DECL_EXPORT __declspec(dllexport)
#define LIB_DECL_IMPORT __declspec(dllimport)
#else
#define LIB_DECL_EXPORT
#define LIB_DECL_IMPORT
#endif

// FreeCADBase
#ifdef LIB_Base_EXPORTS
#define BaseExport LIB_DECL_EXPORT
#else
#define BaseExport LIB_DECL_IMPORT
#endif

// FreeCADApp
#ifdef LIB_App_EXPORTS
#define AppExport LIB_DECL_EXPORT
#define DataExport LIB_DECL_EXPORT
#else
#define AppExport LIB_DECL_IMPORT
#define DataExport LIB_DECL_IMPORT
#endif

// FreeCADGui
#ifdef LIB_Gui_EXPORTS
#define GuiExport LIB_DECL_EXPORT
#else
#define GuiExport LIB_DECL_IMPORT
#endif

#endif