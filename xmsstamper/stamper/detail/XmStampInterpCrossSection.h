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
#include <xmscore/misc/boost_defines.h>
#include <xmscore/misc/base_macros.h> // for XM_DISALLOW_COPY_AND_ASSIGN

// 5. Shared code headers

//----- Forward declarations ---------------------------------------------------

//----- Namespace declaration --------------------------------------------------

namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------
class XmStampCrossSection;
class XmStamperIo;
class Observer;

//----- Function prototypes ----------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \class XmStampInterpCrossSection
/// \brief Interpolate cross sections for stamping
/// \see XmStampInterpCrossSectionImpl
class XmStampInterpCrossSection
{
public:
  static BSHP<XmStampInterpCrossSection> New();

  XmStampInterpCrossSection();
  virtual ~XmStampInterpCrossSection();
  /// \cond
  virtual void InterpMissingCrossSections(XmStamperIo& a_) = 0;
  virtual bool ValidCrossSectionsExist(XmStamperIo& a_) = 0;
  virtual void InterpCs(XmStampCrossSection& a_prev,
                        XmStampCrossSection& a_next,
                        double a_percent,
                        XmStampCrossSection& a_cs) = 0;

private:
  XM_DISALLOW_COPY_AND_ASSIGN(XmStampInterpCrossSection);
  /// \endcond
}; // XmStampInterpCrossSection

} // namespace xms
