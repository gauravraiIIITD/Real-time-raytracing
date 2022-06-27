#ifndef MAURICE_H
#define MAURICE_H

#if defined(_MSC_VER) //  Microsoft
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)
#else
    #define EXPORT 
    #define IMPORT
#endif
extern "C" {
__attribute__((constructor)) void enter();
__attribute__((destructor)) void Shape_exit();
__attribute__((visibility("default"))) const char* getDescription();
__attribute__((visibility("default"))) void initShape(char* imagepath);
__attribute__((visibility("default"))) void simplifyShapeCage(int i);
__attribute__((visibility("default"))) void dilateShapeCage(int i);
}
#endif
