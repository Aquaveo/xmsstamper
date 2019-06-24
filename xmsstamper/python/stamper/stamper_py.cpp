//------------------------------------------------------------------------------
/// \file
/// \brief
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <sstream>

#include <pybind11/pybind11.h>

#include <xmscore/misc/StringUtil.h>
#include <xmscore/python/misc/PyUtils.h>

#include <xmsstamper/python/stamper/stamper_py.h>
#include <xmsstamper/stamper/XmStamperIo.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------

void initStamper(py::module &m) {
  initXmStamperModule(m);
  initXmStamperIo(m);
}
