cmake_minimum_required(VERSION 3.9)

project(Maurice VERSION 1.0.0 DESCRIPTION "libMaurice library for shape-aware image deformation")


find_package(pybind11 REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(glm REQUIRED)
find_package(Python COMPONENTS Interpreter Development)
find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)


pybind11_add_module(${PROJECT_NAME} SHARED 
    src/maurice.cpp
	src/shape.cpp
	extern/poly2tri/common/shapes.cc
	extern/poly2tri/sweep/advancing_front.cc
	extern/poly2tri/sweep/cdt.cc
	extern/poly2tri/sweep/sweep_context.cc
	extern/poly2tri/sweep/sweep.cc
	)


set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER include/maurice.h)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(${PROJECT_NAME} PROPERTIES C_VISIBILITY_PRESET hidden)
set_target_properties(${PROJECT_NAME} PROPERTIES VISIBILITY_INLINES_HIDDEN 1)

target_include_directories(${PROJECT_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_include_directories(${PROJECT_NAME} PRIVATE extern/poly2tri)
target_include_directories(${PROJECT_NAME} PRIVATE extern/poly2tri/common)
target_include_directories(${PROJECT_NAME} PRIVATE extern/poly2tri/sweep)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	target_include_directories(${PROJECT_NAME} PRIVATE /opt/homebrew/include)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS} Eigen3::Eigen glm::glm)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
