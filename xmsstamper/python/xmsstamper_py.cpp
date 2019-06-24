//------------------------------------------------------------------------------
/// \file
/// \brief root module for xmsgrid Python bindings.
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <pybind11/pybind11.h>
#include <xmsstamper/python/stamper/stamper_py.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------

//------ Primary Module --------------------------------------------------------
PYBIND11_MODULE(_xmsstamper, m) {
    m.doc() = "Python bindings for the xmsstamper library"; // optional module docstring

    // Stamper module
    py::module modStamper = m.def_submodule("stamper");
    initStamper(modStamper);
}

