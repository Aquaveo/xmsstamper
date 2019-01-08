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
  initXmStamperClass(m);
  initXmStamperModule(m);
  initXmStamperIo(m);
}
//------------------------------------------------------------------------------
/// \brief Create string from XmStampRaster
/// \param[in] a_stampRaster: XmStampRaster object
/// \return a string
//------------------------------------------------------------------------------
std::string PyReprStringFromXmStampRaster(const xms::XmStampRaster& a_stampRaster)
{
  std::stringstream ss;
  ss << "num_pixels_x: " << a_stampRaster.m_numPixelsX << "\n";
  ss << "num_pixels_y: " << a_stampRaster.m_numPixelsY << "\n";
  ss << "pixel_size_x: " << a_stampRaster.m_pixelSizeX << "\n";
  ss << "pixel_size_y: " << a_stampRaster.m_pixelSizeY << "\n";
  ss << "min: (" << a_stampRaster.m_min.x << ", " << a_stampRaster.m_min.y << ")\n";
  ss << "vals: " << StringFromVecDbl(a_stampRaster.m_vals);
  ss << "no_data: " << a_stampRaster.m_noData;
  return ss.str();
} // PyReprStringFromXmStampRaster
//------------------------------------------------------------------------------
/// \brief Create string from XmWingWall
/// \param[in] a_wingWall: XmWingWall object
/// \return a string
//------------------------------------------------------------------------------
std::string PyReprStringFromXmWingWall(const xms::XmWingWall& a_wingWall)
{
  std::stringstream ss;
  ss << "wing_wall_angle: " << a_wingWall.m_wingWallAngle;
  return ss.str();
} // PyReprStringFromXmWingWall
//------------------------------------------------------------------------------
/// \brief Create string from XmSlopedAbutment
/// \param[in] a_slopedAbutment: XmSlopedAbutment object
/// \return a string
//------------------------------------------------------------------------------
std::string PyReprStringFromXmSlopedAbutment(const xms::XmSlopedAbutment& a_slopedAbutment)
{
  std::stringstream ss;
  ss << "max_x: " << xms::STRstd(a_slopedAbutment.m_maxX) << "\n";
  ss << "slope: \n" << xms::StringFromVecPt3d(a_slopedAbutment.m_slope);
  return ss.str();
} // PyReprStringFromXmSlopedAbutment
//------------------------------------------------------------------------------
/// \brief Create string from XmGuidebank
/// \param[in] a_guideBank: XmGuidebank object
/// \return a string
//------------------------------------------------------------------------------
std::string PyReprStringFromXmGuidebank(const xms::XmGuidebank& a_guideBank)
{
  std::stringstream ss;
  ss << "side: " << xms::STRstd(a_guideBank.m_side) << "\n";
  ss << "radius1: " << xms::STRstd(a_guideBank.m_radius1) << "\n";
  ss << "radius2: " << xms::STRstd(a_guideBank.m_radius2) << "\n";
  ss << "width: " << xms::STRstd(a_guideBank.m_width) << "\n";
  ss << "n_pts: " << a_guideBank.m_nPts;
  return ss.str();
} // PyReprStringFromXmGuidebank
//------------------------------------------------------------------------------
/// \brief Create string from XmStamperEndCap
/// \param[in] a_endCap: XmStamperEndCap object
/// \return a string
//------------------------------------------------------------------------------
std::string PyReprStringFromXmStamperEndCap(const xms::XmStamperEndCap& a_endCap)
{
  std::stringstream ss;
  ss << "type: " << a_endCap.m_type << "\n";
  ss << "angle: " << xms::STRstd(a_endCap.m_angle) << "\n";
  ss << "guidebank: \n" << PyReprStringFromXmGuidebank(a_endCap.m_guidebank) << "\n";
  ss << "sloped_abutment: \n" << PyReprStringFromXmSlopedAbutment(a_endCap.m_slopedAbutment) << "\n";
  ss << "wing_wall: \n" << PyReprStringFromXmWingWall(a_endCap.m_wingWall);
  return ss.str();
} // PyReprStringFromXmStamperEndCap
//------------------------------------------------------------------------------
/// \brief Create string from XmStampCrossSection
/// \param[in] a_stampCrossSection: XmStampCrossSection object
/// \return a string
//------------------------------------------------------------------------------
std::string PyReprStringFromXmStampCrossSection(const xms::XmStampCrossSection& a_stampCrossSection)
{
  std::stringstream ss;
  ss << "left: \n" << xms::StringFromVecPt3d(a_stampCrossSection.m_left);
  ss << "left_max: " << a_stampCrossSection.m_leftMax << "\n";
  ss << "index_left_shoulder: " << a_stampCrossSection.m_idxLeftShoulder << "\n";
  ss << "right: \n" << xms::StringFromVecPt3d(a_stampCrossSection.m_right);
  ss << "right_max: " << a_stampCrossSection.m_rightMax << "\n";
  ss << "index_right_shoulder: " << a_stampCrossSection.m_idxRightShoulder;
  return ss.str();
} // PyReprStringFromXmStampCrossSection
//------------------------------------------------------------------------------
/// \brief Create string from XmStamperCenterlineProfile
/// \param[in] a_stampCenterlineProfile: XmStamperCenterlineProfile object
/// \return a string
//------------------------------------------------------------------------------
std::string PyReprStringFromXmStamperCenterlineProfile(const xms::XmStamperCenterlineProfile& a_stampCenterlineProfile)
{
  std::stringstream ss;
  ss << "distance: " << StringFromVecDbl(a_stampCenterlineProfile.m_distance) << "\n";
  ss << "elevation: " << StringFromVecDbl(a_stampCenterlineProfile.m_elevation) << "\n";
  for (int i = 0; i < a_stampCenterlineProfile.m_cs.size(); ++i)
  {
    ss << "Cross Section #" << i + 1 << ":\n";
    ss << PyReprStringFromXmStampCrossSection(a_stampCenterlineProfile.m_cs[i]) << "\n";
  }
  return ss.str();
} // PyReprStringFromXmStamperCenterlineProfile
//------------------------------------------------------------------------------
/// \brief Create string from XmStamperIo
/// \param[in] a_stamperIo: XmStamperIo object
/// \return a string
//------------------------------------------------------------------------------
std::string PyReprStringFromXmStamperIo(const xms::XmStamperIo& a_stamperIo)
{
  std::stringstream ss;
  ss << "center_line: \n" << xms::StringFromVecPt3d(a_stamperIo.m_centerLine);
  ss << "stamping_type: " << a_stamperIo.m_stampingType << "\n";
  for (int i = 0; i < a_stamperIo.m_cs.size(); ++i)
  {
    ss << "Cross Section #" << i + 1 << ":\n";
    ss << PyReprStringFromXmStampCrossSection(a_stamperIo.m_cs[i]) << "\n";
  }
  ss << "first_end_cap: \n" << PyReprStringFromXmStamperEndCap(a_stamperIo.m_firstEndCap);
  ss << "last_end_cap: \n" << PyReprStringFromXmStamperEndCap(a_stamperIo.m_lastEndCap);
  ss << "bathymetry: " << "TIN GOES HERE";
  return ss.str();
} // PyReprStringFromXmStamperIo
//------------------------------------------------------------------------------
/// \brief Create a __repr__ string from a VecDbl
/// \param[in] a_vals: VecDbl
/// \return a string
//------------------------------------------------------------------------------
std::string StringFromVecDbl(const std::vector<double>& a_vals)
{
  std::stringstream ss;
  ss << "(length " << a_vals.size() << "): ";
  std::vector<double> v;
  bool printDotDotDot(false);
  if (a_vals.size() < 11)
  {
    v = a_vals;
  }
  else
  {
    printDotDotDot = true;
    size_t idx = a_vals.size();
    v.insert(v.end(), a_vals.begin(), a_vals.begin()+5);
    v.insert(v.end(), a_vals.begin()+(idx-5), a_vals.end());
  }

  ss << "[";
  for (size_t i = 0; i < v.size(); ++i)
  {
    if (printDotDotDot && i == 5)
      ss << "... ";
    ss << xms::STRstd(v[i]);
    if (i + 1 != v.size())
      ss << ", ";
  }
  ss << "]\n";
  return ss.str();
} // StringFromDynBitset