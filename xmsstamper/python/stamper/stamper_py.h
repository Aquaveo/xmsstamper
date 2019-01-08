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
namespace xms {
  class XmStampRaster;
  class XmWingWall;
  class XmSlopedAbutment;
  class XmGuidebank;
  class XmStamperEndCap;
  class XmStampCrossSection;
  class XmStamperCenterlineProfile;
  class XmStamperIo;
}

void initStamper(py::module &);

//----- Function declarations --------------------------------------------------
void initXmStamperClass(py::module &);
void initXmStamperModule(py::module &);
void initXmStamperIo(py::module &);


std::string PyReprStringFromXmStampRaster(const xms::XmStampRaster& a_refinePoint);
std::string PyReprStringFromXmWingWall(const xms::XmWingWall& a_wingWall);
std::string PyReprStringFromXmSlopedAbutment(const xms::XmSlopedAbutment& a_slopedAbutment);
std::string PyReprStringFromXmGuidebank(const xms::XmGuidebank& a_guideBank);
std::string PyReprStringFromXmStamperEndCap(const xms::XmStamperEndCap& a_endCap);
std::string PyReprStringFromXmStampCrossSection(const xms::XmStampCrossSection& a_stampCrossSection);
std::string PyReprStringFromXmStamperCenterlineProfile(const xms::XmStamperCenterlineProfile& a_stampCenterlineProfile);
std::string PyReprStringFromXmStamperIo(const xms::XmStamperIo& a_stamperIo);


std::string StringFromVecDbl(const std::vector<double>& a_);
