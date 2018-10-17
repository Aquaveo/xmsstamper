//------------------------------------------------------------------------------
/// \file
/// \brief root module for xmsgrid Python bindings.
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <pybind11/pybind11.h>
#include <xmsstamper/python/stamper/stamper_py.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------
std::string version() {
    return "1.0.0";
}


//------ Primary Module --------------------------------------------------------
PYBIND11_MODULE(xmsstamper_py, m) {
    m.doc() = "Python bindings for the xmsstamper library"; 
    m.def("version", &version,
          "Get current version of xmsstamper Python bindings.");

    const char* stamper_doc = R"pydoc(
        The stamper module of the xmsstamper python library contains classes 
        and functions for performing feature stamping. The basic idea is to 
        associate cross sections with a center line to produce a new feature 
        (a "fill" embankment, a "cut" channel). The output from the feature 
        stamp is a TIN and a set of break lines defining distinct 
        characteristics of the new feature, such as the center line, shoulder, 
        toe, and end cap.
    )pydoc";
    // Stamper module
    py::module modStamper = m.def_submodule("stamper",stamper_doc);
    initStamper(modStamper);
}

