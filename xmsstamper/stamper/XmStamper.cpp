//------------------------------------------------------------------------------
/// \file
/// \ingroup stamping
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My own header
#include <xmsstamper/stamper/XmStamper.h>

// 3. Standard library headers
#include <algorithm>
#include <fstream>
#include <iostream>

// 4. External library headers

// 5. Shared code headers
#include <xmscore/math/math.h>
#include <xmscore/misc/XmError.h> // XM_ENSURE_TRUE
#include <xmscore/misc/xmstype.h> // XM_NODATA
#include <xmsgrid/geometry/geoms.h>
#include <xmsinterp/interpolate/InterpLinear.h>
#include <xmscore/misc/Observer.h>
#include <xmsstamper/stamper/detail/XmBathymetryIntersector.h>
#include <xmsstamper/stamper/detail/XmBreaklines.h>
#include <xmsstamper/stamper/detail/XmStampEndCap.h>
#include <xmsstamper/stamper/detail/XmStamper3dPts.h>
#include <xmsstamper/stamper/detail/XmStampInterpCrossSection.h>
#include <xmsstamper/stamper/detail/XmUtil.h>
#include <xmsstamper/stamper/XmStamperIo.h>
#include <xmsgrid/triangulate/detail/TrOuterTriangleDeleter.h>
#include <xmsgrid/triangulate/TrTin.h>
#include <xmsgrid/triangulate/TrBreaklineAdder.h>
#include <xmsgrid/triangulate/TrTriangulatorPoints.h>
#include <xmscore/misc/XmConst.h>

// 6. Non-shared code headers

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
class XmStamperImpl : public XmStamper
{
public:
  XmStamperImpl();
  ~XmStamperImpl();

  virtual void DoStamp(XmStamperIo& a_io) override;

  //------------------------------------------------------------------------------
  /// \brief returns the point locations created by the stamp operation.
  /// \return points.
  //------------------------------------------------------------------------------
  virtual const VecPt3d& GetPoints() override { return *m_outPts; }
  //------------------------------------------------------------------------------
  /// \brief returns breaklines created by the stamp operation.
  /// \return segments.
  //------------------------------------------------------------------------------
  virtual const VecInt2d& GetSegments() override { return m_breaklines; }
  //------------------------------------------------------------------------------
  /// returns the type of breakline.
  /// \return breakline types.
  //------------------------------------------------------------------------------
  virtual const VecInt& GetBreaklineTypes() override { return m_blTypes; }

  //------------------------------------------------------------------------------
  /// sets the observer class to get feedback on the meshing process
  /// \param[in] a_ Observer class to provide feedback.
  //------------------------------------------------------------------------------
  virtual void SetObserver(BSHP<Observer> a_) override { m_observer = a_; }

  BSHP<Observer> m_observer; ///< progress observer
  XmStamperIo m_io;          ///< inputs to the stamp operation
  /// vector of stampers to break up center line where intersections occur
  std::vector<XmStamperIo> m_vIo;
  /// cross section interpolator. Interpolates cross sections to points along
  /// the polyline in the XmStamperIo class.
  BSHP<XmStampInterpCrossSection> m_interp;
  /// class to intersect the feature stamp with bathemetry
  BSHP<XmBathymetryIntersector> m_intersect;
  /// class to create breaklines
  BSHP<XmBreaklines> m_breaklineCreator;
  BSHP<VecPt3d> m_outPts; ///< the output points
  XmStamper3dPts m_3dpts; ///< 3d locations of the stamp operation
  VecInt2d m_breaklines;  ///< breaklines
  bool m_error;           ///< flag to indicate that an error has occurred processing the stamp
  BSHP<TrTin> m_tin;      ///< tin created by the stamp operation
  Pt3d m_stampBoundsMin;  ///< min x,y,z of stamp
  Pt3d m_stampBoundsMax;  ///< max x,y,z of stamp
  BSHP<VecPt3d> m_curPts; ///< the output points
  cs3dPtIdx m_ptIdx;      ///< indexes of point created from stamp
  VecInt m_blTypes;       ///< type of breakline

  void WriteInputsForDebug();
  bool InputErrorsFound();
  void CreateBathymetryIntersector();
  void GetStampBounds();
  void IntersectCenterLineWithBathemetry();
  void InterpolateMissingCrossSections();
  void DecomposeCenterLine();
  void ConvertCrossSectionsTo3d();
  void ConvertEndCapsTo3d();
  void IntersectWithTin();
  bool CreateOutputs();
  void AddCrossSectionPointsToArray(stXs3dPts& a_csPts, VecPt3d& a_pts, csPtIdx& a_ptIdx);
  bool CreateBreakLines(cs3dPtIdx& a_ptIdx);
  void AppendTinAndBreakLines(bool a_errors);
  void Convert3dPtsToVec();
};
namespace
{
//------------------------------------------------------------------------------
/// \brief Stamps a TrTin to an XmStampRaster object.  If a raster cell is
///        out of bounds of a_tin, nothing is interpolated to a_raster.
///        If the raster cell has XM_NODATA values, the a_tin values are still
///        interpolated to the raster at any location where a_tin overlaps
///        a_raster.
/// \param[in] a_tin: The TIN to interpolate from.
/// \param[in, out] a_raster: The raster to interpolate to.
/// \param[in] a_stampingType: The type of stamping to perform. 0=cut, 1=fill,
///        2=both
//------------------------------------------------------------------------------
bool iInterpTinToRaster(const boost::shared_ptr<const TrTin> &a_tin, XmStampRaster &a_raster,
                        int a_stampingType = 2)
{
  XM_ENSURE_TRUE(a_tin != nullptr, false);
  BSHP<InterpLinear> interp = InterpLinear::New();
  BSHP<VecPt3d> pts(new VecPt3d());
  *pts = a_tin->Points();
  BSHP<VecInt> tris(new VecInt());
  *tris = a_tin->Triangles();
  interp->SetPtsTris(pts, tris);
  interp->SetExtrapVal(XM_NODATA);
  int rasterSize = static_cast<int>(a_raster.m_vals.size());
  for (int i = 0; i < rasterSize; ++i)
  {
    float val = interp->InterpToPt(Pt3d(a_raster.GetLocationFromCellIndex(i)));
    if (!EQ_TOL(val, XM_NODATA, XM_ZERO_TOL))
    {
      // Take the stamp value if the raster has none, regardless of stamping type.
      if (EQ_TOL(a_raster.m_vals[i], a_raster.m_noData, XM_ZERO_TOL))
      {
        a_raster.m_vals[i] = val;
      }
      else if (a_stampingType == 0) // Cut stamp, take the minimum
      {
        a_raster.m_vals[i] = std::min(double(val), a_raster.m_vals[i]);
      }
      else if (a_stampingType == 1) // Fill stamp, take the maximum
      {
        a_raster.m_vals[i] = std::max(double(val), a_raster.m_vals[i]);
      }
      else // Both, always stamp the feature object
      {
        a_raster.m_vals[i] = val;
      }
    }
  }
  return true;
} // iInterpTinToRaster
}
////////////////////////////////////////////////////////////////////////////////
/// \class XmStamperImpl
/// \brief Performs a feature stamp operation
/// The result of the operation is a TIN and a list of breakline segments.
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
XmStamperImpl::XmStamperImpl()
: m_observer()
, m_io()
, m_interp(XmStampInterpCrossSection::New())
, m_outPts(new VecPt3d())
, m_error(false)
{
} // XmStamperImpl::XmStamperImpl
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
XmStamperImpl::~XmStamperImpl()
{
} // XmStamperImpl::~XmStamperImpl
//------------------------------------------------------------------------------
/// \brief Performs the feature stamping operation
/// \param a_io The stamping input/output class. When sucessful, the m_outTin and
/// m_outBreakLines members of a_io are filled by this method.
//------------------------------------------------------------------------------
void XmStamperImpl::DoStamp(XmStamperIo& a_io)
{
  m_io = a_io;
  m_io.m_outTin.reset();
  m_io.m_outBreakLines.clear();

  WriteInputsForDebug();

  if (InputErrorsFound())
    return;

  CreateBathymetryIntersector();

  IntersectCenterLineWithBathemetry();
  InterpolateMissingCrossSections();
  DecomposeCenterLine();

  for (auto& io : m_vIo)
  {
    m_io = io;
    m_3dpts = XmStamper3dPts();

    ConvertCrossSectionsTo3d();
    ConvertEndCapsTo3d();
    IntersectWithTin();
    bool rval = CreateOutputs();
    AppendTinAndBreakLines(!rval);
  }

  if (!m_error && m_breaklineCreator && m_outPts)
  {
    m_error = m_breaklineCreator->BreaklinesIntersect(m_breaklines, *m_outPts);
    if (m_error)
    {
      XM_LOG(xmlog::warning, "Intersection found in stamp outputs. Stamping operation aborted.");
    }
  }

  if (!m_error)
  {
    a_io.m_outBreakLines = m_breaklines;
    a_io.m_outTin = m_tin;
    if (!a_io.m_raster.m_vals.empty())
    {
      iInterpTinToRaster(a_io.m_outTin, a_io.m_raster, a_io.m_stampingType);
    }
  }
  
} // XmStamperImpl::DoStamp
//------------------------------------------------------------------------------
/// \brief Writes the XmStamperIo class to a file for debugging
//------------------------------------------------------------------------------
void XmStamperImpl::WriteInputsForDebug()
{
  std::fstream os;
  os.open("c:\\temp\\xmsstamper_DoStamp_SaveInputs.dbg", std::fstream::in);
  if (os.good())
  {
    std::ofstream os("c:\\temp\\xmsng_StamperIo.txt");
    m_io.WriteToFile(os, "STAMPER_IO_VERSION_1");
  }
} // WriteInputsForDebug
//------------------------------------------------------------------------------
/// \brief Checks for input errors.
/// \return true or false.
//------------------------------------------------------------------------------
bool XmStamperImpl::InputErrorsFound()
{
  XM_ENSURE_TRUE(m_io.m_centerLine.size() > 1, true);
  XM_ENSURE_TRUE(m_io.m_centerLine.size() == m_io.m_cs.size(), true);
  // make sure that at least one cross section has data
  if (!m_interp->ValidCrossSectionsExist(m_io))
  {
    XM_LOG(xmlog::warning, "No valid cross sections defined. Aborting stamp operation.");
    return true;
  }

  return false;
} // XmStamperImpl::InputErrorsFound
//------------------------------------------------------------------------------
/// \brief Creates the intersector for the bathymetry
//------------------------------------------------------------------------------
void XmStamperImpl::CreateBathymetryIntersector()
{
  m_intersect.reset();
  if (m_io.m_bathymetry)
  {
    XmStamperIo tmp(m_io);

    InterpolateMissingCrossSections();
    ConvertCrossSectionsTo3d();
    ConvertEndCapsTo3d();
    Convert3dPtsToVec();
    CreateOutputs();

    // get the boundary of the stamp
    GetStampBounds();

    m_intersect = XmBathymetryIntersector::New(m_io.m_bathymetry, m_io.m_outTin);
    m_intersect->IntersectCenterLine(m_io);

    m_io = tmp;
  }
} // XmStamperImpl::IntersectCenterLineWithBathemetry
//------------------------------------------------------------------------------
/// \brief Gets the bounds of the stamp
//------------------------------------------------------------------------------
void XmStamperImpl::GetStampBounds()
{
  m_stampBoundsMax = XM_DBL_LOWEST;
  m_stampBoundsMin = XM_DBL_HIGHEST;

  auto myLamda = [](stXs3dPts& pts, Pt3d& a_min, Pt3d& a_max) {
    for (auto& p : pts.m_centerLine)
      gmAddToExtents(p, a_min, a_max);
    for (auto& v : pts.m_left)
    {
      for (auto& p : v)
        gmAddToExtents(p, a_min, a_max);
    }
    for (auto& v : pts.m_right)
    {
      for (auto& p : v)
        gmAddToExtents(p, a_min, a_max);
    }
  };

  myLamda(m_3dpts.m_xsPts, m_stampBoundsMin, m_stampBoundsMax);
  myLamda(m_3dpts.m_first_endcap, m_stampBoundsMin, m_stampBoundsMax);
  myLamda(m_3dpts.m_last_endcap, m_stampBoundsMin, m_stampBoundsMax);
} // XmStamperImpl::IntersectCenterLineWithBathemetry
//------------------------------------------------------------------------------
/// \brief Intersects the center line with the bathemetry
//------------------------------------------------------------------------------
void XmStamperImpl::IntersectCenterLineWithBathemetry()
{
  if (m_intersect)
  {
    m_intersect->IntersectCenterLine(m_io);
  }
} // XmStamperImpl::IntersectCenterLineWithBathemetry
//------------------------------------------------------------------------------
/// \brief Interpolates cross sections for any points that do not have a
/// cross section specified.
//------------------------------------------------------------------------------
void XmStamperImpl::InterpolateMissingCrossSections()
{
  m_interp->InterpMissingCrossSections(m_io);
} // XmStamperImpl::InterpolateMissingCrossSections
//------------------------------------------------------------------------------
/// \brief Decomposes the centerline (more help needed)
//------------------------------------------------------------------------------
void XmStamperImpl::DecomposeCenterLine()
{
  m_vIo.push_back(m_io);
  if (!m_intersect)
    return;
  m_intersect->DecomposeCenterLine(m_io, m_vIo);
} // XmStamperImpl::DecomposeCenterLine
//------------------------------------------------------------------------------
/// \brief Converts the cross section data to 3d point locations
//------------------------------------------------------------------------------
void XmStamperImpl::ConvertCrossSectionsTo3d()
{
  for (size_t i = 0; i < m_io.m_cs.size(); ++i)
  {
    XmStampCrossSection& cs(m_io.m_cs[i]);
    Pt3d p(m_io.m_centerLine[i]);
    double leftAngle, rightAngle;
    XmUtil::GetAnglesFromCenterLine(i, m_io.m_centerLine, leftAngle, rightAngle);
    XmUtil::ConvertXsPointsTo3d(p, cs.m_left, cs.m_leftMax, leftAngle, m_3dpts.m_xsPts.m_left);
    XmUtil::ConvertXsPointsTo3d(p, cs.m_right, cs.m_rightMax, rightAngle, m_3dpts.m_xsPts.m_right);
  }
} // XmStamperImpl::ConvertCrossSectionsTo3d
//------------------------------------------------------------------------------
/// \brief Converts the cross section data to 3d point locations
//------------------------------------------------------------------------------
void XmStamperImpl::ConvertEndCapsTo3d()
{
  XM_ENSURE_TRUE_NO_ASSERT(!m_3dpts.m_xsPts.m_left.empty() && !m_3dpts.m_xsPts.m_right.empty());

  BSHP<XmStampEndCap> cap = XmStampEndCap::New();
  cap->RotateEndCapCrossSections(m_io, m_3dpts.m_xsPts.m_left, m_3dpts.m_xsPts.m_right);
  // convert the end cap to 3D points
  cap->ConvertTo3dPts(m_io, m_3dpts);
} // XmStamperImpl::ConvertEndCapsTo3d
//------------------------------------------------------------------------------
/// \brief Intersects the feature stampe with the elevation TIN
//------------------------------------------------------------------------------
void XmStamperImpl::IntersectWithTin()
{
  if (!m_io.m_bathymetry)
    return;

  // intersect the left and right side xsects
  m_3dpts.m_xsPts.m_centerLine = m_io.m_centerLine;
  m_intersect->IntersectXsects(m_3dpts);
  m_3dpts.m_xsPts.m_centerLine.clear();

  // intersect endcap points
  m_intersect->IntersectEndCaps(m_io, m_3dpts);
} // XmStamperImpl::IntersectWithTin
//------------------------------------------------------------------------------
/// \brief Creates the output TIN and breaklines
/// \return true on success.
//------------------------------------------------------------------------------
bool XmStamperImpl::CreateOutputs()
{
  Convert3dPtsToVec();

  // define the breaklines
  if (!CreateBreakLines(m_ptIdx))
    return false;

  // Triangulate
  m_io.m_outTin = TrTin::New();
  m_io.m_outTin->SetPoints(m_curPts);
  TrTriangulatorPoints client(m_io.m_outTin->Points(), m_io.m_outTin->Triangles(),
                              &m_io.m_outTin->TrisAdjToPts());
  client.SetObserver(m_observer);
  if (!client.Triangulate())
    return false;
  if (m_io.m_outTin && m_io.m_outTin->NumTriangles() < 1)
  {
    m_io.m_outTin.reset();
    return false;
  }

  // force in the breaklines
  BSHP<TrBreaklineAdder> bl = TrBreaklineAdder::New();
  bl->SetTin(m_io.m_outTin);
  bl->AddBreaklines(m_io.m_outBreakLines);

  // delete triangles outside the outer boundary
  BSHP<TrOuterTriangleDeleter> deleter = TrOuterTriangleDeleter::New();
  VecInt2d poly(1, m_breaklineCreator->GetOuterPolygon());
  deleter->Delete(poly, m_io.m_outTin);

  return true;
} // XmStamperImpl::CreateOutputs
//------------------------------------------------------------------------------
/// \brief puts all of the generated 3d points into one vector
//------------------------------------------------------------------------------
void XmStamperImpl::Convert3dPtsToVec()
{
  m_ptIdx = cs3dPtIdx();
  // put all of the points into 1 array for the output TIN
  m_curPts = BSHP<VecPt3d>(new VecPt3d());
  for (const auto& p : m_io.m_centerLine)
  {
    m_ptIdx.m_centerLine.push_back((int)m_curPts->size());
    m_curPts->push_back(p);
  }
  AddCrossSectionPointsToArray(m_3dpts.m_xsPts, *m_curPts, m_ptIdx.m_xsPts);
  AddCrossSectionPointsToArray(m_3dpts.m_first_endcap, *m_curPts, m_ptIdx.m_first_end_cap);
  AddCrossSectionPointsToArray(m_3dpts.m_last_endcap, *m_curPts, m_ptIdx.m_last_end_cap);
} // XmStamperImpl::Convert3dPtsToVec

//------------------------------------------------------------------------------
/// \brief puts cross section points into a single array to be used by a TIN
/// \param[in] a_csPts Left and right points for cross sections
/// \param[out] a_pts The array of points for the TIN
/// \param[out] a_ptIdx The indexes of the points in the a_pts array
//------------------------------------------------------------------------------
void XmStamperImpl::AddCrossSectionPointsToArray(stXs3dPts& a_csPts,
                                                 VecPt3d& a_pts,
                                                 csPtIdx& a_ptIdx)
{
  for (const auto& v : a_csPts.m_left)
  {
    a_ptIdx.m_left.push_back(VecInt());
    for (const auto& p : v)
    {
      a_ptIdx.m_left.back().push_back((int)a_pts.size());
      a_pts.push_back(p);
    }
  }
  for (const auto& v : a_csPts.m_right)
  {
    a_ptIdx.m_right.push_back(VecInt());
    for (const auto& p : v)
    {
      a_ptIdx.m_right.back().push_back((int)a_pts.size());
      a_pts.push_back(p);
    }
  }
  for (const auto p : a_csPts.m_centerLine)
  {
    a_ptIdx.m_centerLine.push_back((int)a_pts.size());
    a_pts.push_back(p);
  }
} // XmStamperImpl::AddCrossSectionPointsToArray
//------------------------------------------------------------------------------
/// \brief Creates breaklines that will be honored in the TIN
/// \param[in] a_ptIdx The indexes of the points in the TIN
/// \return true if breaklines successfully created.
//------------------------------------------------------------------------------
bool XmStamperImpl::CreateBreakLines(cs3dPtIdx& a_ptIdx)
{
  m_breaklineCreator.reset();
  m_breaklineCreator = XmBreaklines::New();

  bool rval = m_breaklineCreator->CreateBreaklines(m_io, a_ptIdx, m_blTypes);
  return rval;
} // XmStamperImpl::CreateBreakLines
//------------------------------------------------------------------------------
/// \brief appends to the output TIN and breaklines
/// \param[in] a_errors flag indicating if there were errors creating the stamp
//------------------------------------------------------------------------------
void XmStamperImpl::AppendTinAndBreakLines(bool a_errors)
{
  if (!m_tin && m_io.m_outTin)
  {
    m_tin = m_io.m_outTin;
    m_breaklines = m_io.m_outBreakLines;
    m_outPts = m_tin->PointsPtr();
  }
  else if (m_io.m_outTin)
  {
    // get the number of points in the current TIN
    int nPts = (int)m_outPts->size();
    // add the points from the current TIN
    m_outPts->reserve(nPts + m_io.m_outTin->Points().size());
    {
      auto beg = m_io.m_outTin->Points().begin();
      auto end = m_io.m_outTin->Points().end();
      m_outPts->insert(m_outPts->end(), beg, end);
    }
    // add the triangles after updating indices
    for (auto& t : m_io.m_outTin->Triangles())
      t += nPts;

    {
      auto e1 = m_tin->Triangles().end();
      auto beg = m_io.m_outTin->Triangles().begin();
      auto end = m_io.m_outTin->Triangles().end();
      m_tin->Triangles().insert(e1, beg, end);
    }

    // update indices on breaklines
    for (auto& b : m_io.m_outBreakLines)
    {
      for (auto& i : b)
        i += nPts;
    }
    {
      auto e1 = m_breaklines.end();
      auto beg = m_io.m_outBreakLines.begin();
      auto end = m_io.m_outBreakLines.end();
      m_breaklines.insert(e1, beg, end);
    }
  }
  if (a_errors)
    m_error = true;
} // XmStamperImpl::AppendTinAndBreakLines

//------------------------------------------------------------------------------
/// \brief Creates a XmStamper class
/// \return shared pointer to XmStamper.
//------------------------------------------------------------------------------
BSHP<XmStamper> XmStamper::New()
{
  BSHP<XmStamper> stamper(new XmStamperImpl);
  return stamper;
} // XmStamper::New
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
XmStamper::XmStamper()
{
} // XmStamper::XmStamper
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
XmStamper::~XmStamper()
{
} // XmStamper::~XmStamper

} // namespace xms