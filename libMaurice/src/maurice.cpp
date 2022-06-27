#include "maurice.h"
#include "shape.h"
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <Eigen/Dense>

using namespace cv;
namespace py = pybind11;


Shape *shape;
const char description[] = "libMaurice: A library for shape-aware image deformation";

void enter()
{
  shape = new Shape();
}

void Shape_exit()
{
	delete shape;
}

const char* getDescription()
{
  return description;
}


// Init and operations
void initShape(char* imagepath)
{
  shape->loadImage(imagepath); 
}

void simplifyShapeCage(int i)
{
  shape->simplifyCage(i);
	shape->postCageModify();
}

void dilateShapeCage(int i)
{
  shape->dilateCage(i);
	shape->postCageModify();
}

//Getters
const std::vector<double>& getHandleCoordinatesOriginal() 
{
	return shape->m_p;
}

const std::vector<double>& getHandleCoordinatesModified()
{
	return shape->m_q;
}


// utils
const std::vector<double> getHandleMap() 
{
	std::vector<double> m_handleMap_new;
	auto s0 = shape->m_handleMap;
	for(int i=0; i<s0.size();i++)
	{
		auto val = s0[i];
		m_handleMap_new.push_back(val.x);
		m_handleMap_new.push_back(val.y);
	}
	return m_handleMap_new;
}

const std::vector<double> getShape() 
{
	std::vector<double> m_shape_new;
	auto s1 = shape->m_shape;
	for(int i=0; i<s1.size();i++)
	{
		auto val = s1[i];
		m_shape_new.push_back(val.x);
		m_shape_new.push_back(val.y);
	}
	return m_shape_new;
}

const std::vector<double> getCage() 
{
	std::vector<double> m_cage_new;
	auto s2 = shape->m_cage;
	for(int i=0; i<s2.size();i++)
	{
		auto val = s2[i];
		m_cage_new.push_back(val.x);
		m_cage_new.push_back(val.y);
	}
	return m_cage_new;
}

const std::vector<double> getTriangulationVertices() 
{
	std::vector<double> m_points_new;
	auto s3 = shape->m_points;
	for(int i=0; i<s3.size();i++)
	{
		auto val = s3[i];
		m_points_new.push_back(val->x);
		m_points_new.push_back(val->y);
	}
	return m_points_new;
}


const std::vector<p2t::Triangle*>& getTriangulationFaces() 
{
	return shape->m_cdt->GetTriangles();
}

const cv::Mat& getTexture() 
{
  return shape->m_image;
}


// Add line/polyline handle
void init_addLineHandle(std::vector<float> handle)
{
	shape->addLineHandle(handle);
}

// deformation function
void init_deform(int index, float* deformedPoint)
{
	shape->deform(index, deformedPoint);
}

// inverse kinematics
void init_inverseKinematicsFABRIK(int handleID, int endEffector, glm::vec2 target)
{
	shape->inverseKinematicsFABRIK(handleID, endEffector, target);
}




const std::vector<double> getShapeFromImage(char* imagepath) 
{
	Shape *shape = new Shape();
	shape->loadImage(imagepath);
	std::vector<double> m_shape_new;
	auto s2 = shape->m_shape;
	for(int i=0; i<s2.size();i++)
	{
		auto val = s2[i];
		m_shape_new.push_back(val.x);
		m_shape_new.push_back(val.y);
	}
	return m_shape_new;
}



const std::vector<double> getCageFromImage(char* imagepath) 
{
	Shape *shape = new Shape();
	shape->loadImage(imagepath);
	std::vector<double> m_cage_new;
	auto s2 = shape->m_cage;
	for(int i=0; i<s2.size();i++)
	{
		auto val = s2[i];
		m_cage_new.push_back(val.x);
		m_cage_new.push_back(val.y);
	}
	return m_cage_new;
}



const std::vector<double> getTriangulationVerticesFromImage(char* imagepath) 
{
	Shape *shape = new Shape();
	shape->loadImage(imagepath);
	std::vector<double> m_TriangleVertices_new;
	auto s2 = shape->m_points;
	for(int i=0; i<s2.size();i++)
	{
		auto val = s2[i];
		m_TriangleVertices_new.push_back(val->x);
		m_TriangleVertices_new.push_back(val->y);
	}
	return m_TriangleVertices_new;
}



const std::vector<double> getHandleMapFromImage(char* imagepath) 
{
	Shape *shape = new Shape();
	shape->loadImage(imagepath);
	std::vector<double> m_HandleMap_new;
	auto s2 = shape->m_handleMap;
	for(int i=0; i<s2.size();i++)
	{
		auto val = s2[i];
		m_HandleMap_new.push_back(val.x);
		m_HandleMap_new.push_back(val.y);
	}
	return m_HandleMap_new;
}




PYBIND11_MODULE(Maurice, m) {
m.def("getDescription", &getDescription);
m.def("initShape", &initShape);
m.def("init_addLineHandle", &init_addLineHandle);
m.def("init_deform", &init_deform);
m.def("init_inverseKinematicsFABRIK", &init_inverseKinematicsFABRIK);
m.def("getCageFromImage", &getCageFromImage);
m.def("getShapeFromImage", &getShapeFromImage);
m.def("getTriangulationVerticesFromImage", &getTriangulationVerticesFromImage);
m.def("getHandleCoordinatesOriginal", &getHandleCoordinatesOriginal);
m.def("getHandleCoordinatesModified", &getHandleCoordinatesModified);
m.def("getHandleMapFromImage", &getHandleMapFromImage);

py::class_<Shape>(m, "Shape")
    .def(py::init<>())
    .def("loadImage", &Shape::loadImage)
    .def("simplifyCage", &Shape::simplifyCage)
    .def("dilateCage", &Shape::dilateCage)
    .def("addLineHandle", &Shape::addLineHandle)
    .def("deform", &Shape::deform)
    .def("inverseKinematicsFABRIK", &Shape::inverseKinematicsFABRIK);
}




