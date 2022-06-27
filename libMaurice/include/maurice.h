#include<vector>
#define GLM_COMPILER 0
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
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

__attribute__((visibility("default"))) void init_deform(int index, float *deformedPoint);
__attribute__((visibility("default"))) void init_addLineHandle(std::vector<float>);
__attribute__((visibility("default"))) void init_inverseKinematicsFABRIK(int handleID, int endEffector, glm::vec2 target);

}
#endif
