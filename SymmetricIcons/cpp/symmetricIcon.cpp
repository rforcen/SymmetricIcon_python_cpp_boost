

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

namespace p = boost::python;
namespace np = boost::python::numpy;
typedef unsigned char byte;

#include "symmetricIcon.h"

// python interface

BOOST_PYTHON_MODULE(SymmetricIcon)
{
    p::class_<SymmetricIcon<float>>("SymmetricIcon", p::init<int, int, int>())
        .def("gen",&SymmetricIcon<float>::gen )
        .def_readonly("n_presets",  &SymmetricIcon<float>::nPreset)
        .def("getimage_np", &SymmetricIcon<float>::getimage_np)
        .def("generate",  &SymmetricIcon<float>::generate)
        .def("generate_image",  &SymmetricIcon<float>::generate_image)
        .def("set_parameters", &SymmetricIcon<float>::set_parameters)
    ;
}