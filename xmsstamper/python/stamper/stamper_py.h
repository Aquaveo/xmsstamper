#pragma once
//------------------------------------------------------------------------------
/// \file
/// \brief initializer functions for members of ugrid python module.
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <pybind11/pybind11.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

void initStamper(py::module &);

//----- Function declarations --------------------------------------------------
void initXmStamperModule(py::module &);
void initXmStamperIo(py::module &);
