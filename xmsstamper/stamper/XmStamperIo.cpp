//------------------------------------------------------------------------------
/// \file
/// \ingroup stamping
/// \brief Implements data classes in XmStamperIo.h
//
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My own header
#include <xmsstamper/stamper/XmStamperIo.h>

// 3. Standard library headers
#include <array>
#include <fstream> // std::ofstream
#include <sstream> // std::stringstream

// 4. External library headers
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

// 5. Shared code headers
#include <xmscore/misc/StringUtil.h> // stEqualNoCase
#include <xmscore/misc/XmError.h> // XM_ENSURE_TRUE
#include <xmscore/misc/xmstype.h> // XM_NODATA
#include <xmsgrid/triangulate/TrTin.h>

// 6. Non-shared code headers

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------

//----- Internal functions -----------------------------------------------------
namespace
{
void iWriteVecIntToFile(std::ofstream &a_file, const std::string &a_cardName, const VecInt &a_vals);
bool iReadVecIntFromFile(std::ifstream &a_file, VecInt &a_vals);
}

//----- Class / Function definitions -------------------------------------------
namespace
{
//------------------------------------------------------------------------------
/// \brief Holds the value of precision for writing floating point numbers
/// to output
//------------------------------------------------------------------------------
int& iPrecision()
{
  static int m_precision(10);
  return m_precision;
} // iPrecision
//------------------------------------------------------------------------------
/// \brief Uses the defined precision to convert a double to a string using the
/// STRstd function.
/// \param[in] a_: the double that is converted to a string
/// \return the string representation of the double
//------------------------------------------------------------------------------
std::string iDblToStr(double a_)
{
  return STRstd(a_, -1, iPrecision(), STR_FULLWIDTH);
} // iDblToStr
//------------------------------------------------------------------------------
/// \brief Writes a VecDbl to an ASCII file
//------------------------------------------------------------------------------
void iWriteVecDblToFile(std::ofstream &a_file, const std::string &a_cardName, const VecDbl &a_vals)
{
  XM_ENSURE_TRUE(a_file.is_open());
  a_file << a_cardName + " " << a_vals.size() << " ";
  for (const auto &val : a_vals)
  {
    a_file << iDblToStr(val) << " ";
  }
  a_file << "\n";
} // iWriteVecDblToFile
//------------------------------------------------------------------------------
/// \brief Reads a VecDbl from an ASCII file
//------------------------------------------------------------------------------
bool iReadVecDblFromFile(std::ifstream &a_file, VecDbl &a_vals)
{
  XM_ENSURE_TRUE(a_file.is_open(), false);
  int num(0);
  XM_ENSURE_TRUE(a_file >> num, false);
  a_vals.assign(num, 0);
  for (auto &val : a_vals)
  {
    XM_ENSURE_TRUE(a_file >> val, false);
  }
  return true;
} // iReadVecDblFromFile
//------------------------------------------------------------------------------
/// \brief Writes a VecPt3d to an ASCII file
//------------------------------------------------------------------------------
void iWriteVecPt3dToFile(std::ofstream &a_file, const std::string &a_cardName, const VecPt3d &a_pts)
{
  XM_ENSURE_TRUE(a_file.is_open());
  a_file << a_cardName + " " << a_pts.size() << "\n";
  for (const auto &pt : a_pts)
  {
    a_file << iDblToStr(pt.x) << " " << iDblToStr(pt.y) << " " << iDblToStr(pt.z) << "\n";
  }
} // iWriteVecPt3dToFile
//------------------------------------------------------------------------------
/// \brief Reads a VecPt3d from an ASCII file
//------------------------------------------------------------------------------
bool iReadVecPt3dFromFile(std::ifstream &a_file, VecPt3d &a_pts)
{
  XM_ENSURE_TRUE(a_file.is_open(), false);
  int numPts(0);
  XM_ENSURE_TRUE(a_file >> numPts, false);
  a_pts.assign(numPts, xms::Pt3d());
  for (auto &pt : a_pts)
  {
    XM_ENSURE_TRUE(a_file >> pt.x >> pt.y, false);
    if (a_file.peek() != '\n')
    {
      XM_ENSURE_TRUE(a_file >> pt.z, false);
    }
  }
  return true;
} // iReadVecPt3dFromFile
//------------------------------------------------------------------------------
/// \brief Writes a VecInt2d to an ASCII file
//------------------------------------------------------------------------------
void iWriteVecInt2dToFile(std::ofstream &a_file, const std::string &a_cardName, const VecInt2d &a_vals)
{
  XM_ENSURE_TRUE(a_file.is_open());
  a_file << a_cardName + " " << a_vals.size() << "\n";
  for (const auto &valArray : a_vals)
  {
    iWriteVecIntToFile(a_file, "", valArray);
  }
} // iWriteVecInt2dToFile
//------------------------------------------------------------------------------
/// \brief Reads a VecInt from an ASCII file
//------------------------------------------------------------------------------
bool iReadVecInt2dFromFile(std::ifstream &a_file, VecInt2d &a_vals)
{
  XM_ENSURE_TRUE(a_file.is_open(), false);
  int num(0);
  XM_ENSURE_TRUE(a_file >> num, false);
  a_vals.assign(num, VecInt());
  for (auto &valArray : a_vals)
  {
    XM_ENSURE_TRUE(iReadVecIntFromFile(a_file, valArray), false);
  }
  return true;
} // iReadVecInt2dFromFile
//------------------------------------------------------------------------------
/// \brief Writes a VecInt to an ASCII file
//------------------------------------------------------------------------------
void iWriteVecIntToFile(std::ofstream &a_file, const std::string &a_cardName, const VecInt &a_vals)
{
  XM_ENSURE_TRUE(a_file.is_open());
  const std::string card(a_cardName.empty() ? "" : a_cardName + " ");
  a_file << card << a_vals.size() << " ";
  for (const auto &val : a_vals)
  {
    a_file << val << " ";
  }
  a_file << "\n";
} // iWriteVecIntToFile
//------------------------------------------------------------------------------
/// \brief Reads a VecInt from an ASCII file
//------------------------------------------------------------------------------
bool iReadVecIntFromFile(std::ifstream &a_file, VecInt &a_vals)
{
  XM_ENSURE_TRUE(a_file.is_open(), false);
  int num(0);
  XM_ENSURE_TRUE(a_file >> num, false);
  a_vals.assign(num, 0);
  for (auto &val : a_vals)
  {
    XM_ENSURE_TRUE(a_file >> val, false);
  }
  return true;
} // iReadVecIntFromFile
//------------------------------------------------------------------------------
/// \brief Writes a Tin to an ASCII file
//------------------------------------------------------------------------------
void iWriteTinToFile(std::ofstream &a_file, const std::string &a_cardName, const BSHP<const TrTin> &a_tin)
{
  XM_ENSURE_TRUE(a_file.is_open());
  a_file << a_cardName + "\n";
  const VecPt3d& points = a_tin->Points();
  iWriteVecPt3dToFile(a_file, "POINTS", points);

  VecInt vTri = a_tin->Triangles();
  std::vector<std::array<int, 3>> sortableTris;
  // sort the triangles so that the point with the lowest index is first and then sort all
  // triangles
  for (size_t i = 0; i < vTri.size(); i += 3)
  {
    std::array<int, 3> tri = { -1, -1, -1 };
    tri[0] = vTri[i + 0];
    tri[1] = vTri[i + 1];
    tri[2] = vTri[i + 2];
    auto minIter = tri.begin();
    if (tri[1] < tri[0] && tri[1] < tri[2])
      minIter += 1;
    else if (tri[2] < tri[0] && tri[2] < tri[1])
      minIter += 2;
    std::rotate(tri.begin(), minIter, tri.begin() + 3);
    sortableTris.push_back(tri);
  }
  std::sort(sortableTris.begin(), sortableTris.end());
  auto it = sortableTris.begin();
  for (size_t i = 0; i < vTri.size(); i += 3, ++it)
  {
    vTri[i + 0] = (*it)[0];
    vTri[i + 1] = (*it)[1];
    vTri[i + 2] = (*it)[2];
  }
  a_file << "TRIANGLES " << vTri.size() << "\n";
  for (size_t i = 0; i < vTri.size(); i += 3)
  {
    a_file << std::setw(10) << vTri[i + 0] << " "
           << std::setw(10) << vTri[i + 1] << " "
           << std::setw(10) << vTri[i + 2] << "\n";
  }
  //const VecInt2d& trisAdjToPts = a_tin->TrisAdjToPts();
  a_file << "TRIS_ADJ_TO_PTS 0\n";
  //iWriteVecInt2dToFile(a_file, "TRIS_ADJ_TO_PTS", trisAdjToPts);
} // iWriteTinToFile
//------------------------------------------------------------------------------
/// \brief Reads a Tin from an ASCII file
//------------------------------------------------------------------------------
bool iReadTinFromFile(std::ifstream &a_file, const BSHP<TrTin> &a_tin)
{
  XM_ENSURE_TRUE(a_file.is_open(), false);
  std::string card;
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "POINTS"), false);
  XM_ENSURE_TRUE(iReadVecPt3dFromFile(a_file, a_tin->Points()), false);
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "TRIANGLES"), false);
  XM_ENSURE_TRUE(iReadVecIntFromFile(a_file, a_tin->Triangles()), false);
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "TRIS_ADJ_TO_PTS"), false);
  XM_ENSURE_TRUE(iReadVecInt2dFromFile(a_file, a_tin->TrisAdjToPts()), false);
  // always just build this. The default behavior is to not write this array to the file 
  a_tin->BuildTrisAdjToPts();
  return true;
} // iReadTinFromFile
}
//------------------------------------------------------------------------------
/// \brief Constructor that sets all the raster values
/// \param[in] a_numPixelsX: number of pixels in x direction
/// \param[in] a_numPixelsY: number of pixels in y direction
/// \param[in] a_pixelSizeX: size of the pixels in the x direction
/// \param[in] a_pixelSizeY: size of the pixels in the y direction
/// \param[in] a_min: The bottom left corner (x,y) of the raster
/// \param[in] a_vals: The raster values
/// \param[in] a_noData: The "no data" (or inactive) value to identify raster
/// pixels where no value exists.
//------------------------------------------------------------------------------
XmStampRaster::XmStampRaster(const int a_numPixelsX, const int a_numPixelsY, const double a_pixelSizeX,
  const double a_pixelSizeY, const Pt3d &a_min, const std::vector<double> &a_vals, const int a_noData)
  : m_numPixelsX(a_numPixelsX)
  , m_numPixelsY(a_numPixelsY)
  , m_pixelSizeX(a_pixelSizeX)
  , m_pixelSizeY(a_pixelSizeY)
  , m_min(a_min)
  , m_vals(a_vals)
  , m_noData(a_noData)
{
} // XmStampRaster::XmStampRaster
//------------------------------------------------------------------------------
/// \brief Default Constructor
//------------------------------------------------------------------------------
XmStampRaster::XmStampRaster()
  : m_numPixelsX(0)
  , m_numPixelsY(0)
  , m_pixelSizeX(0.0)
  , m_pixelSizeY(0.0)
  , m_min()
  , m_vals()
  , m_noData(XM_NODATA)
{
} // XmStampRaster::XmStampRaster
//------------------------------------------------------------------------------
/// \brief Gets the zero-based cell index from the given column and row.
/// \param[in] a_col: The zero-based column index for the raster.
/// \param[in] a_row: The zero-based row index for the raster.
/// \return The zero-based cell index from the given a_col, a_row.
//------------------------------------------------------------------------------
int XmStampRaster::GetCellIndexFromColRow(const int a_col, const int a_row) const
{
  if (a_col >= 0 && a_col < m_numPixelsX && a_row >= 0 && a_row < m_numPixelsY)
  {
    return a_row * m_numPixelsX + a_col;
  }
  return -1;
} // XmStampRaster::GetCellIndexFromColRow
//------------------------------------------------------------------------------
/// \brief Gets the zero-based column and row from the cell index.
/// \param[in] a_index: The zero-based raster cell index.
/// \param[out] a_col: The zero-based column index for the raster.
/// \param[out] a_row: The zero-based row index for the raster.
//------------------------------------------------------------------------------
void XmStampRaster::GetColRowFromCellIndex(const int a_index, int &a_col, int &a_row) const
{
  a_row = a_col = -1;
  if (a_index > 0 && a_index < m_numPixelsX * m_numPixelsY)
  {
    a_col = a_index % m_numPixelsX;
    a_row = a_index / m_numPixelsX;
  }
} // XmStampRaster::GetColRowFromCellIndex
  //------------------------------------------------------------------------------
/// \brief Gets the location of the cell center from the zero-based cell index.
/// \param[in] a_index: The zero-based raster cell index.
/// \return The location of the cell with the given index.
//------------------------------------------------------------------------------
Pt3d XmStampRaster::GetLocationFromCellIndex(const int a_index) const
{
  int col, row;
  GetColRowFromCellIndex(a_index, col, row);
  if (col >= 0 && row >= 0)
  {
    Pt3d loc;
    loc.x = m_min.x + col * m_pixelSizeX;
    loc.y = m_min.y + (m_numPixelsY - 1 - row) * m_pixelSizeY;
    return loc;
  }
  return m_min;
} // XmStampRaster::GetLocationFromCellIndex
//------------------------------------------------------------------------------
/// \brief Writes the raster in the given format to the given filename.
/// \param[in] a_fileName: The output raster filename.
/// \param[in] a_format: The output raster format.
//------------------------------------------------------------------------------
void XmStampRaster::WriteGridFile(const std::string &a_fileName,
  const XmRasterFormatEnum a_format)
{
  std::ofstream outGrid(a_fileName, std::ofstream::trunc);
  XM_ENSURE_TRUE(outGrid.is_open());
  switch (a_format)
  {
    case RS_ARCINFO_ASCII:
      outGrid << "ncols " << m_numPixelsX << std::endl;
      outGrid << "nrows " << m_numPixelsY << std::endl;
      outGrid << "xllcorner " << std::fixed << std::setprecision(13) << m_min.x - m_pixelSizeX / 2.0 << std::endl;
      outGrid << "yllcorner " << m_min.y - m_pixelSizeY / 2.0 << std::endl;
      outGrid << "cellsize " << m_pixelSizeX << std::endl;
      outGrid << "NODATA_value " << m_noData << std::endl;
      outGrid << std::setprecision(2);
      int count = 0;
      for (const double v : m_vals)
      {
        outGrid << v << " ";
        ++count;
        if (count % m_numPixelsX == 0)
          outGrid << std::endl;
      }
      break;
  }
} // XmStampRaster::WriteGridFile
//------------------------------------------------------------------------------
/// \brief Writes the XmStampRaster class information to a file.
/// \param[in] a_file: The output file.
/// \param[in] a_cardName: The card name to be written to the output file.
//------------------------------------------------------------------------------
void XmStampRaster::WriteToFile(std::ofstream &a_file, const std::string &a_cardName) const
{
  XM_ENSURE_TRUE(a_file.is_open());
  a_file << a_cardName + "\n"; // Not sure we should be writing this since we don't read it.
  a_file << "NUM_PIXELS_X " << m_numPixelsX << "\n";
  a_file << "NUM_PIXELS_Y " << m_numPixelsY << "\n";
  a_file << "PIXEL_SIZE_X " << std::fixed << std::setprecision(13) << m_pixelSizeX << "\n";
  a_file << "PIXEL_SIZE_Y " << m_pixelSizeY << "\n";
  a_file << "MIN_X " << m_min.x << "\n";
  a_file << "MIN_Y " << m_min.y << "\n";
  iWriteVecDblToFile(a_file, "VALS", m_vals);
  a_file << "NODATA " << m_noData;
} // XmStampRaster::WriteToFile
//------------------------------------------------------------------------------
/// \brief Reads the XmStampRaster class information to a file.
/// \param[in] a_file: The input file.
/// \return true if file read is successful. false if errors encountered.
//------------------------------------------------------------------------------
bool XmStampRaster::ReadFromFile(std::ifstream &a_file)
{
  XM_ENSURE_TRUE(a_file.is_open(), false);
  std::string card;
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "NUM_PIXELS_X"), false);
  XM_ENSURE_TRUE(a_file >> m_numPixelsX, false);
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "NUM_PIXELS_Y"), false);
  XM_ENSURE_TRUE(a_file >> m_numPixelsY, false);
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "PIXEL_SIZE_X"), false);
  XM_ENSURE_TRUE(a_file >> m_pixelSizeX, false);
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "PIXEL_SIZE_Y"), false);
  XM_ENSURE_TRUE(a_file >> m_pixelSizeY, false);
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "MIN_X"), false);
  XM_ENSURE_TRUE(a_file >> m_min.x, false);
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "MIN_Y"), false);
  XM_ENSURE_TRUE(a_file >> m_min.y, false);
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "VALS"), false);
  XM_ENSURE_TRUE(iReadVecDblFromFile(a_file, m_vals), false);
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "NODATA"), false);
  XM_ENSURE_TRUE(a_file >> m_noData, false);
  return true;
} // XmStampRaster::ReadFromFile
//------------------------------------------------------------------------------
/// \brief Writes the XmWingWall class information to a file.
/// \param[in] a_file: The output file.
/// \param[in] a_cardName: The card name to be written to the output file.
//------------------------------------------------------------------------------
void XmWingWall::WriteToFile(std::ofstream &a_file, const std::string &a_cardName) const
{
  XM_ENSURE_TRUE(a_file.is_open());
  a_file << a_cardName + "\n";
  a_file << "WING_WALL_ANGLE " << std::fixed << std::setprecision(13) << m_wingWallAngle << "\n";
} // XmWingWall::WriteToFile
//------------------------------------------------------------------------------
/// \brief Reads the XmWingWall class information to a file.
/// \param[in] a_file: The input file.
/// \return true if file read is successful. false if errors encountered.
//------------------------------------------------------------------------------
bool XmWingWall::ReadFromFile(std::ifstream &a_file)
{
  XM_ENSURE_TRUE(a_file.is_open(), false);
  std::string card;
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "WING_WALL_ANGLE"), false);
  XM_ENSURE_TRUE(a_file >> m_wingWallAngle, false);
  return true;
} // XmWingWall::ReadFromFile
//------------------------------------------------------------------------------
/// \brief Writes the XmSlopedAbutment class information to a file.
/// \param[in] a_file: The output file.
/// \param[in] a_cardName: The card name to be written to the output file.
//------------------------------------------------------------------------------
void XmSlopedAbutment::WriteToFile(std::ofstream &a_file, const std::string &a_cardName) const
{
  XM_ENSURE_TRUE(a_file.is_open());
  a_file << a_cardName + "\n";
  a_file << "MAX_X " << std::fixed << std::setprecision(13) << m_maxX << "\n";
  iWriteVecPt3dToFile(a_file, "SLOPE", m_slope);
} // XmSlopedAbutment::WriteToFile
//------------------------------------------------------------------------------
/// \brief Reads the XmSlopedAbutment class information to a file.
/// \param[in] a_file: The input file.
/// \return true if file read is successful. false if errors encountered.
//------------------------------------------------------------------------------
bool XmSlopedAbutment::ReadFromFile(std::ifstream &a_file)
{
  XM_ENSURE_TRUE(a_file.is_open(), false);
  std::string card;
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "MAX_X"), false);
  XM_ENSURE_TRUE(a_file >> m_maxX, false);
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "SLOPE"), false);
  XM_ENSURE_TRUE(iReadVecPt3dFromFile(a_file, m_slope), false);
  return true;
} // XmSlopedAbutment::ReadFromFile
//------------------------------------------------------------------------------
/// \brief Writes the XmGuidebank class information to a file.
/// \param[in] a_file: The output file.
/// \param[in] a_cardName: The card name to be written to the output file.
//------------------------------------------------------------------------------
void XmGuidebank::WriteToFile(std::ofstream &a_file, const std::string &a_cardName) const
{
  XM_ENSURE_TRUE(a_file.is_open());
  a_file << a_cardName + "\n";
  a_file << "SIDE " << m_side << "\n";
  a_file << "RADIUS_1 " << std::fixed << std::setprecision(13) << m_radius1 << "\n";
  a_file << "RADIUS_2 " << m_radius2 << "\n";
  a_file << "WIDTH " << m_width << "\n";
  a_file << "N_PTS " << m_nPts << "\n";
} // XmGuidebank::WriteToFile
//------------------------------------------------------------------------------
/// \brief Reads the XmGuidebank class information to a file.
/// \param[in] a_file: The input file.
/// \return true if file read is successful. false if errors encountered.
//------------------------------------------------------------------------------
bool XmGuidebank::ReadFromFile(std::ifstream &a_file)
{
  XM_ENSURE_TRUE(a_file.is_open(), false);
  std::string card;
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "SIDE"), false);
  XM_ENSURE_TRUE(a_file >> m_side, false);
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "RADIUS_1"), false);
  XM_ENSURE_TRUE(a_file >> m_radius1, false);
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "RADIUS_2"), false);
  XM_ENSURE_TRUE(a_file >> m_radius2, false);
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "WIDTH"), false);
  XM_ENSURE_TRUE(a_file >> m_width, false);
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "N_PTS"), false);
  XM_ENSURE_TRUE(a_file >> m_nPts, false);
  return true;
} // XmGuidebank::ReadFromFile
//------------------------------------------------------------------------------
/// \brief Writes the XmStamperEndCap class information to a file.
/// \param[in] a_file: The output file.
/// \param[in] a_cardName: The card name to be written to the output file.
//------------------------------------------------------------------------------
void XmStamperEndCap::WriteToFile(std::ofstream &a_file, const std::string &a_cardName) const
{
  XM_ENSURE_TRUE(a_file.is_open());
  a_file << a_cardName + "\n";
  a_file << "TYPE " << m_type << "\n";
  a_file << "ANGLE " << std::fixed << std::setprecision(13) << m_angle << "\n";
  m_guidebank.WriteToFile(a_file, "GUIDEBANK");
  m_slopedAbutment.WriteToFile(a_file, "SLOPED_ABUTMENT");
  m_wingWall.WriteToFile(a_file, "WING_WALL");
} // XmStamperEndCap::WriteToFile
//------------------------------------------------------------------------------
/// \brief Reads the XmStamperEndCap class information to a file.
/// \param[in] a_file: The input file.
/// \return true if file read is successful. false if errors encountered.
//------------------------------------------------------------------------------
bool XmStamperEndCap::ReadFromFile(std::ifstream &a_file)
{
  XM_ENSURE_TRUE(a_file.is_open(), false);
  std::string card;
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "TYPE"), false);
  XM_ENSURE_TRUE(a_file >> m_type, false);
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "ANGLE"), false);
  XM_ENSURE_TRUE(a_file >> m_angle, false);
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "GUIDEBANK"), false);
  XM_ENSURE_TRUE(m_guidebank.ReadFromFile(a_file), false);
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "SLOPED_ABUTMENT"), false);
  XM_ENSURE_TRUE(m_slopedAbutment.ReadFromFile(a_file), false);
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "WING_WALL"), false);
  XM_ENSURE_TRUE(m_wingWall.ReadFromFile(a_file), false);
  return true;
} // XmStamperEndCap::ReadFromFile
//------------------------------------------------------------------------------
/// \brief Writes the XmStampCrossSection class information to a file.
/// \param[in] a_file: The output file.
/// \param[in] a_cardName: The card name to be written to the output file.
//------------------------------------------------------------------------------
void XmStampCrossSection::WriteToFile(std::ofstream &a_file, const std::string &a_cardName) const
{
  XM_ENSURE_TRUE(a_file.is_open());
  a_file << a_cardName + "\n";
  iWriteVecPt3dToFile(a_file, "LEFT", m_left);
  a_file << "LEFT_MAX " << std::fixed << std::setprecision(13) << m_leftMax << "\n";
  a_file << "IDX_LEFT_SHOULDER " << m_idxLeftShoulder << "\n";
  iWriteVecPt3dToFile(a_file, "RIGHT", m_right);
  a_file << "RIGHT_MAX " << std::fixed << std::setprecision(13) << m_rightMax << "\n";
  a_file << "IDX_RIGHT_SHOULDER " << m_idxRightShoulder << "\n";
} // XmStampCrossSection::WriteToFile
//------------------------------------------------------------------------------
/// \brief Reads the XmStampCrossSection class information to a file.
/// \param[in] a_file: The input file.
/// \return true if file read is successful. false if errors encountered.
//------------------------------------------------------------------------------
bool XmStampCrossSection::ReadFromFile(std::ifstream &a_file)
{
  XM_ENSURE_TRUE(a_file.is_open(), false);
  std::string card;
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "CS"), false);
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "LEFT"), false);
  XM_ENSURE_TRUE(iReadVecPt3dFromFile(a_file, m_left), false);
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "LEFT_MAX"), false);
  XM_ENSURE_TRUE(a_file >> m_leftMax, false);
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "IDX_LEFT_SHOULDER"), false);
  XM_ENSURE_TRUE(a_file >> m_idxLeftShoulder, false);
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "RIGHT"), false);
  XM_ENSURE_TRUE(iReadVecPt3dFromFile(a_file, m_right), false);
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "RIGHT_MAX"), false);
  XM_ENSURE_TRUE(a_file >> m_rightMax, false);
  XM_ENSURE_TRUE(a_file >> card, false);
  XM_ENSURE_TRUE(stEqualNoCase(card, "IDX_RIGHT_SHOULDER"), false);
  XM_ENSURE_TRUE(a_file >> m_idxRightShoulder, false);
  return true;
} // XmStampCrossSection::ReadFromFile
//------------------------------------------------------------------------------
/// \brief Writes the XmStamperIo class information to a file.
/// \param[in] a_file: The output file.
/// \param[in] a_cardName: The card name to be written to the output file.
//------------------------------------------------------------------------------
void XmStamperIo::WriteToFile(std::ofstream &a_file, const std::string &a_cardName) const
{
  XM_ENSURE_TRUE(a_file.is_open());
  a_file << a_cardName + "\n";
  iWriteVecPt3dToFile(a_file, "CENTER_LINE", m_centerLine);
  a_file << "STAMPING_TYPE " << m_stampingType << "\n";
  a_file << "CROSS_SECTIONS " << m_cs.size() << "\n";
  for (const auto &cs : m_cs)
  {
    cs.WriteToFile(a_file, "CS");
  }
  m_firstEndCap.WriteToFile(a_file, "FIRST_END_CAP");
  m_lastEndCap.WriteToFile(a_file, "LAST_END_CAP");
  if (m_bathymetry)
  {
    iWriteTinToFile(a_file, "BATHYMETRY", m_bathymetry);
  }
  if (m_outTin)
  {
    iWriteTinToFile(a_file, "OUT_TIN", m_outTin);
  }
  iWriteVecInt2dToFile(a_file, "OUT_BREAK_LINES", m_outBreakLines);
  m_raster.WriteToFile(a_file, "RASTER");
} // XmStamperIo::WriteToFile
//------------------------------------------------------------------------------
/// \brief Reads the XmStamperIo class information from a file.
/// \param[in] a_file: The input file.
/// \return true if file read is successful. false if errors encountered.
//------------------------------------------------------------------------------
bool XmStamperIo::ReadFromFile(std::ifstream &a_file)
{
  XM_ENSURE_TRUE(a_file.is_open(), false);
  std::string card;
  while (a_file >> card)
  {
    if (stEqualNoCase(card, "CENTER_LINE"))
    {
      XM_ENSURE_TRUE(iReadVecPt3dFromFile(a_file, m_centerLine), false);
    }
    else if (stEqualNoCase(card, "STAMPING_TYPE"))
    {
      XM_ENSURE_TRUE(a_file >> m_stampingType, false);
    }
    else if (stEqualNoCase(card, "CROSS_SECTIONS"))
    {
      int numCs(0);
      XM_ENSURE_TRUE(a_file >> numCs, false);
      m_cs.assign(numCs, XmStampCrossSection());
      for (auto &cs : m_cs)
      {
        XM_ENSURE_TRUE(cs.ReadFromFile(a_file), false);
      }
    }
    else if (stEqualNoCase(card, "FIRST_END_CAP"))
    {
      XM_ENSURE_TRUE(m_firstEndCap.ReadFromFile(a_file), false);
    }
    else if (stEqualNoCase(card, "LAST_END_CAP"))
    {
      XM_ENSURE_TRUE(m_lastEndCap.ReadFromFile(a_file), false);
    }
    else if (stEqualNoCase(card, "BATHYMETRY"))
    {
      m_bathymetry = TrTin::New();
      XM_ENSURE_TRUE(iReadTinFromFile(a_file, m_bathymetry), false);
    }
    else if (stEqualNoCase(card, "OUT_TIN"))
    {
      m_outTin = TrTin::New();
      XM_ENSURE_TRUE(iReadTinFromFile(a_file, m_outTin), false);
    }
    else if (stEqualNoCase(card, "OUT_BREAK_LINES"))
    {
      XM_ENSURE_TRUE(iReadVecInt2dFromFile(a_file, m_outBreakLines), false);
    }
    else if (stEqualNoCase(card, "RASTER"))
    {
      XM_ENSURE_TRUE(m_raster.ReadFromFile(a_file), false);
    }
    else
    {
      XM_ASSERT(0);
    }
  }
  return true;
} // XmStamperIo::ReadFromFile
//------------------------------------------------------------------------------
/// \brief Sets the precision for stamper output
/// \param[in] a_precision: The number of digits of precision for stamper output
//------------------------------------------------------------------------------
void XmStamperIo::SetPrecisionForOutput(int a_precision)
{
  iPrecision() = a_precision;
} // XmStamperIo::SetPrecisionForOutput

} // namespace xms
