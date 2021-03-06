#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup stamping
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 3. Standard library headers

// 4. External library headers
#include <xmscore/stl/vector.h>

// 5. Shared code headers

//----- Namespace declaration --------------------------------------------------
namespace xms
{
//----- Forward declarations ---------------------------------------------------
class XmStampCrossSection;

//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------

//----- Function prototypes ----------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \class XmUtil
/// \brief Utility functions
class XmUtil
{
public:
  XmUtil();
  virtual ~XmUtil();

  static void ConvertXsPointsTo3d(const Pt3d& a_cl,
                                  const VecPt3d& a_pts,
                                  double a_maxX,
                                  double a_angle,
                                  VecPt3d2d& a_3dpts);
  static void EnsureVectorAtMaxX(VecPt3d& a_pts, double a_maxX);
  static void GetAnglesFromCenterLine(size_t a_idx,
                                      const VecPt3d& a_cl,
                                      double& a_leftAngle,
                                      double& a_rightAngle);
  static void ScaleCrossSectionXvals(XmStampCrossSection& a_xs, double a_factor);

  /// \cond

protected:
private:
  /// \endcond
}; // StSlopedAbutmentUtil

} // namespace xms
