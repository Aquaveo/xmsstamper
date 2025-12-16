//------------------------------------------------------------------------------
/// \file
/// \ingroup tutorial
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My own header

// 3. Standard library headers

// 4. External library headers

// 5. Shared code headers

// 6. Non-shared code headers

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------

namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------

//----- Internal functions -----------------------------------------------------

//----- Class / Function definitions -------------------------------------------
} // namespace xms

#if CXX_TEST
////////////////////////////////////////////////////////////////////////////////
// UNIT TESTS
////////////////////////////////////////////////////////////////////////////////
#include <xmsstamper/stamper/TutStamping.t.h>

#include <fstream> // std::ofstream

#include <xmscore/misc/xmstype.h> // XM_NODATA
#include <xmscore/testing/TestTools.h> // TS_ASSERT_EQUALS

#include <xmsgrid/geometry/geoms.h>
#include <xmsstamper/stamper/XmStamper.h>
#include <xmsstamper/stamper/XmStamperIo.h>
#include <xmsgrid/triangulate/TrTin.h>

////////////////////////////////////////////////////////////////////////////////
/// \class TutStampingUnitTests
/// \brief Tests stamping code
////////////////////////////////////////////////////////////////////////////////
//! [snip_testStampFillEmbankment]
//------------------------------------------------------------------------------
/// \brief    Tests stamping a simple fill embankment
//------------------------------------------------------------------------------
void TutStampingUnitTests::test_StampFillEmbankment()
{
  // declare a XmStamperIo class to create the inputs
  xms::XmStamperIo io;

  // set as a "fill" stamp
  io.m_stampingType = 1;

  // define the center line
  io.m_centerLine = {{0, 0, 15}, {0, 10, 15}};

  // define a cross section at each point on the center line. This will
  // be a symmetric cross section. The top width will be 10 and the side
  // slope will be 1. The cross section will have a total width of 40.
  xms::XmStampCrossSection cs;
  cs.m_left = {{0, 15}, {5, 15}, {6, 14}};
  cs.m_leftMax = 20;
  cs.m_idxLeftShoulder = 1;
  cs.m_right = cs.m_left;
  cs.m_rightMax = cs.m_leftMax;
  cs.m_idxRightShoulder = cs.m_idxLeftShoulder;
  io.m_cs.push_back(cs);
  io.m_cs.push_back(cs);

  // create a Raster to stamp to
  const int numPixelsX = 41, numPixelsY = 11;
  const double pixelSize = 1.0;
  const xms::Pt3d minPt(-20.0, 0.0);
  const std::vector<double> rasterVals(numPixelsX * numPixelsY, 5);
  xms::XmStampRaster raster(numPixelsX, numPixelsY, pixelSize, pixelSize, minPt, rasterVals, XM_NODATA);
  // set the raster member of the raster class.  The values are interpolated from io.m_outTin in
  // the XmStamper::DoStamp function.
  io.m_raster = raster;

  // create a XmStamper class. This class performs the stamp operation.
  BSHP<xms::XmStamper> st = xms::XmStamper::New();
  st->DoStamp(io);

  std::string rasterFileName(XMS_TEST_PATH + std::string("stamping/rasterTestFiles/testFillEmbankment_out.asc"));
  std::string baseFile(XMS_TEST_PATH + std::string("stamping/rasterTestFiles/testFillEmbankment_base.asc"));
  io.m_raster.WriteGridFile(rasterFileName, xms::XmStampRaster::XmRasterFormatEnum::RS_ARCINFO_ASCII);
  TS_ASSERT_TXT_FILES_EQUAL(baseFile, rasterFileName);

  // verify the outputs
  TS_ASSERT(io.m_outTin); // the output TIN should exist
  xms::VecPt3d basePts = {{0, 0, 15},   {0, 10, 15}, {-5, 0, 15}, {-20, 0, 0}, {-5, 10, 15},
                          {-20, 10, 0}, {5, 0, 15},  {20, 0, 0},  {5, 10, 15}, {20, 10, 0}};
  TS_ASSERT_DELTA_VECPT3D(basePts, io.m_outTin->Points(), 1e-9);

  TS_ASSERT_EQUALS(7, io.m_outBreakLines.size());
  xms::VecInt baseLines = {0, 1}; // centerline
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[0]);
  baseLines = {3, 2, 0, 6, 7}; // first cross section
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[1]);
  baseLines = {5, 4, 1, 8, 9}; // second cross section
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[2]);
  baseLines = {3, 5}; // left toe
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[3]);
  baseLines = {7, 9}; // right toe
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[4]);
  baseLines = {2, 4}; // left shoulder
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[5]);
  baseLines = {6, 8}; // right shoulder
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[6]);
  // Write output TIN for viewing in XMS
  std::string tinFileName(XMS_TEST_PATH + std::string("stamping/rasterTestFiles/testFillEmbankmentTin_out.tin"));
  std::ofstream ofs(tinFileName, std::ofstream::trunc);
  io.m_outTin->ExportTinFile(ofs);
} // TutStampingUnitTests::test_StampFillEmbankment
//! [snip_testStampFillEmbankment]
//! [snip_testStampCutEmbankment]
//------------------------------------------------------------------------------
/// \brief    Tests stamping a simple cut embankment
//------------------------------------------------------------------------------
void TutStampingUnitTests::test_StampCutEmbankment()
{
  // declare a XmStamperIo class to create the inputs
  xms::XmStamperIo io;

  // set as a "cut" stamp
  io.m_stampingType = 0;

  // define the center line
  io.m_centerLine = {{0, 0, 0}, {0, 10, 0}};

  // define a cross section at each point on the center line. This will
  // be a symmetric cross section. The top width will be 10 and the side
  // slope will be 1. The cross section will have a total width of 40.
  xms::XmStampCrossSection cs;
  cs.m_left = {{0, 0}, {5, 0}, {6, 1}};
  cs.m_leftMax = 20;
  cs.m_idxLeftShoulder = 1;
  cs.m_right = cs.m_left;
  cs.m_rightMax = cs.m_leftMax;
  cs.m_idxRightShoulder = cs.m_idxLeftShoulder;
  io.m_cs.push_back(cs);
  io.m_cs.push_back(cs);

  // create a Raster to stamp to
  const int numPixelsX = 41, numPixelsY = 11;
  const double pixelSize = 1.0;
  const xms::Pt3d minPt(-20.0, 0.0);
  const std::vector<double> rasterVals(numPixelsX * numPixelsY, 5);
  xms::XmStampRaster raster(numPixelsX, numPixelsY, pixelSize, pixelSize, minPt, rasterVals, XM_NODATA);
  // set the raster member of the raster class.  The values are interpolated from io.m_outTin in
  // the XmStamper::DoStamp function.
  io.m_raster = raster;

  // create a XmStamper class. This class performs the stamp operation.
  BSHP<xms::XmStamper> st = xms::XmStamper::New();
  st->DoStamp(io);

  std::string rasterFileName(XMS_TEST_PATH + std::string("stamping/rasterTestFiles/testCutEmbankment_out.asc"));
  std::string baseFile(XMS_TEST_PATH + std::string("stamping/rasterTestFiles/testCutEmbankment_base.asc"));
  io.m_raster.WriteGridFile(rasterFileName, xms::XmStampRaster::XmRasterFormatEnum::RS_ARCINFO_ASCII);
  TS_ASSERT_TXT_FILES_EQUAL(baseFile, rasterFileName);

  // verify the outputs
  TS_ASSERT(io.m_outTin); // the output TIN should exist
  xms::VecPt3d basePts = {{0, 0, 0},     {0, 10, 0}, {-5, 0, 0},  {-20, 0, 15}, {-5, 10, 0},
                          {-20, 10, 15}, {5, 0, 0},  {20, 0, 15}, {5, 10, 0},   {20, 10, 15}};
  TS_ASSERT_DELTA_VECPT3D(basePts, io.m_outTin->Points(), 1e-9);

  TS_ASSERT_EQUALS(7, io.m_outBreakLines.size());
  xms::VecInt baseLines = {0, 1}; // centerline
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[0]);
  baseLines = {3, 2, 0, 6, 7}; // first cross section
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[1]);
  baseLines = {5, 4, 1, 8, 9}; // second cross section
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[2]);
  baseLines = {3, 5}; // left toe
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[3]);
  baseLines = {7, 9}; // right toe
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[4]);
  baseLines = {2, 4}; // left shoulder
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[5]);
  baseLines = {6, 8}; // right shoulder
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[6]);
  // Write output TIN for viewing in XMS
  std::string tinFileName(XMS_TEST_PATH + std::string("stamping/rasterTestFiles/testCutEmbankmentTin_out.tin"));
  std::ofstream ofs(tinFileName, std::ofstream::trunc);
  io.m_outTin->ExportTinFile(ofs);
} // TutStampingUnitTests::test_StampCutEmbankment
//! [snip_testStampCutEmbankment]
//! [snip_testStampWingWall]
//------------------------------------------------------------------------------
/// \brief    Tests stamping a simple fill embankment with a wing wall end cap
//------------------------------------------------------------------------------
void TutStampingUnitTests::test_StampWingWall()
{
  // declare a XmStamperIo class to create the inputs
  xms::XmStamperIo io;

  // set as a "fill" stamp
  io.m_stampingType = 1;

  // define the center line
  io.m_centerLine = {{0, 0, 15}, {20, 20, 15}};

  // define a cross section at each point on the center line. This will
  // be a symmetric cross section. The top width will be 10 and the side
  // slope will be 1. The cross section will have a total width of 40.
  xms::XmStampCrossSection cs;
  cs.m_left = {{0, 15}, {5, 15}, {6, 14}};
  cs.m_leftMax = 20;
  cs.m_idxLeftShoulder = 1;
  cs.m_right = cs.m_left;
  cs.m_rightMax = cs.m_leftMax;
  cs.m_idxRightShoulder = cs.m_idxLeftShoulder;
  io.m_cs.push_back(cs);
  io.m_cs.push_back(cs);

  // wing wall is the default end cap

  // set the angles on the wing wall
  io.m_firstEndCap.m_angle = 15;
  io.m_firstEndCap.m_wingWall.m_wingWallAngle = 15;

  // set the angles on the wing wall
  io.m_lastEndCap.m_angle = -15;
  io.m_lastEndCap.m_wingWall.m_wingWallAngle = 10;

  // create a Raster to stamp to
  const int numPixelsX = 46, numPixelsY = 45;
  const double pixelSize = 1.0;
  const xms::Pt3d minPt(-16.0, -8.0);
  const std::vector<double> rasterVals(numPixelsX * numPixelsY, XM_NODATA);
  xms::XmStampRaster raster(numPixelsX, numPixelsY, pixelSize, pixelSize, minPt, rasterVals, XM_NODATA);
  // set the raster member of the raster class.  The values are interpolated from io.m_outTin in
  // the XmStamper::DoStamp function.
  io.m_raster = raster;

  // create a XmStamper class. This class performs the stamp operation.
  BSHP<xms::XmStamper> st = xms::XmStamper::New();
  st->DoStamp(io);

  std::string rasterFileName(XMS_TEST_PATH + std::string("stamping/rasterTestFiles/testWingWall_out.asc"));
  std::string baseFile(XMS_TEST_PATH + std::string("stamping/rasterTestFiles/testWingWall_base.asc"));
  io.m_raster.WriteGridFile(rasterFileName, xms::XmStampRaster::XmRasterFormatEnum::RS_ARCINFO_ASCII);
  TS_ASSERT_TXT_FILES_EQUAL(baseFile, rasterFileName);

  // verify the outputs
  TS_ASSERT(io.m_outTin); // the output TIN should exist
  xms::VecPt3d basePts = {{0, 0, 15},       {20, 20, 15}, {-4.5, 2.6, 15}, {-15.1, 13.2, 0},
                          {17.4, 24.5, 15}, {7.7, 36, 0}, {4.5, -2.6, 15}, {21.2, -7.1, 0},
                          {22.6, 15.5, 15}, {28.2, 0, 0}};
  TS_ASSERT_DELTA_VECPT3D(basePts, io.m_outTin->Points(), 1e-1);

  TS_ASSERT_EQUALS(7, io.m_outBreakLines.size());
  xms::VecInt baseLines = {0, 1}; // centerline
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[0]);
  baseLines = {3, 2, 0, 6, 7}; // first cross section
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[1]);
  baseLines = {5, 4, 1, 8, 9}; // second cross section
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[2]);
  baseLines = {3, 5}; // left toe
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[3]);
  baseLines = {7, 9}; // right toe
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[4]);
  baseLines = {2, 4}; // left shoulder
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[5]);
  baseLines = {6, 8}; // right shoulder
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[6]);
  // Write output TIN for viewing in XMS
  std::string tinFileName(XMS_TEST_PATH + std::string("stamping/rasterTestFiles/testWingWallTin_out.tin"));
  std::ofstream ofs(tinFileName, std::ofstream::trunc);
  io.m_outTin->ExportTinFile(ofs);
} // TutStampingUnitTests::test_StampWingWall
//! [snip_testStampWingWall]
//! [snip_testStampSlopedAbutment]
//------------------------------------------------------------------------------
/// \brief    Tests stamping a simple fill embankment with sloped abutment
/// end cap
//------------------------------------------------------------------------------
void TutStampingUnitTests::test_StampSlopedAbutment()
{
  // declare a XmStamperIo class to create the inputs
  xms::XmStamperIo io;

  // set as a "fill" stamp
  io.m_stampingType = 1;

  // define the center line
  io.m_centerLine = {{0, 0, 15}, {20, 20, 15}};

  // define a cross section at each point on the center line. This will
  // be a symmetric cross section. The top width will be 10 and the side
  // slope will be 1. The cross section will have a total width of 40.
  xms::XmStampCrossSection cs;
  cs.m_left = {{0, 15}, {5, 15}, {6, 14}};
  cs.m_leftMax = 20;
  cs.m_idxLeftShoulder = 1;
  cs.m_right = cs.m_left;
  cs.m_rightMax = cs.m_leftMax;
  cs.m_idxRightShoulder = cs.m_idxLeftShoulder;
  io.m_cs.push_back(cs);
  io.m_cs.push_back(cs);

  // change to a sloped abutment end cap
  io.m_firstEndCap.m_type = 1;

  // set the slope and max length for the end cap
  io.m_firstEndCap.m_slopedAbutment.m_slope = {{0, 15}, {1, 14}};
  io.m_firstEndCap.m_slopedAbutment.m_maxX = 10;

  // make the last end cap the same as the first
  io.m_lastEndCap = io.m_firstEndCap;
  // change the angle on the last end cap
  io.m_lastEndCap.m_angle = 20;

  // create a Raster to stamp to
  const int numPixelsX = 56, numPixelsY = 53;
  const double pixelSize = 1.0;
  const xms::Pt3d minPt(-17.0, -17.0);
  const std::vector<double> rasterVals(numPixelsX * numPixelsY, XM_NODATA);
  xms::XmStampRaster raster(numPixelsX, numPixelsY, pixelSize, pixelSize, minPt, rasterVals, XM_NODATA);
  // set the raster member of the raster class.  The values are interpolated from io.m_outTin in
  // the XmStamper::DoStamp function.
  io.m_raster = raster;

  // create a XmStamper class. This class performs the stamp operation.
  BSHP<xms::XmStamper> st = xms::XmStamper::New();
  st->DoStamp(io);

  std::string rasterFileName(XMS_TEST_PATH + std::string("stamping/rasterTestFiles/testSlopedAbutment_out.asc"));
  std::string baseFile(XMS_TEST_PATH + std::string("stamping/rasterTestFiles/testSlopedAbutment_base.asc"));
  io.m_raster.WriteGridFile(rasterFileName, xms::XmStampRaster::XmRasterFormatEnum::RS_ARCINFO_ASCII);
  TS_ASSERT_TXT_FILES_EQUAL(baseFile, rasterFileName);

  // verify the outputs
  TS_ASSERT(io.m_outTin); // the output TIN should exist
  xms::VecPt3d basePts = {
    {0, 0, 15},         {20, 20, 15},       {-3.5, 3.5, 15},   {-14.1, 14.1, 0},
    {15.2, 22.2, 15},   {4.6, 32.9, 0},     {3.5, -3.5, 15},   {14.1, -14.1, 0},
    {24.8, 17.8, 15},   {35.4, 7.1, 0},     {-10.6, -3.5, 5},  {-12.9, -1.9, 4.2},
    {-14.8, 0.5, 3.3},  {-16, 3.5, 2.5},    {-16.4, 7, 1.7},   {-15.8, 10.6, 0.8},
    {-3.5, -10.6, 5.0}, {-1.9, -12.9, 4.2}, {0.5, -14.8, 3.3}, {3.5, -16.0, 2.5},
    {7.0, -16.4, 1.7},  {10.6, -15.8, 0.8}, {19.4, 31.3, 5},   {16.6, 33.4, 3.8},
    {13, 34.6, 2.5},    {8.8, 34.4, 1.3},   {29, 26.8, 5},     {31.8, 25.9, 4.3},
    {34.3, 24.1, 3.6},  {36.4, 21.5, 2.9},  {37.7, 18.2, 2.1}, {38, 14.6, 1.4},
    {37.3, 10.8, 0.7}};
  TS_ASSERT_DELTA_VECPT3D(basePts, io.m_outTin->Points(), 1e-1);
  // Write output TIN for viewing in XMS
  std::string tinFileName(XMS_TEST_PATH + std::string("stamping/rasterTestFiles/testSlopedAbutmentTin_out.tin"));
  std::ofstream ofs(tinFileName, std::ofstream::trunc);
  io.m_outTin->ExportTinFile(ofs);
} // TutStampingUnitTests::test_StampSlopedAbutment
//! [snip_testStampSlopedAbutment]
//! [snip_testStampGuidebank]
//------------------------------------------------------------------------------
/// \brief    Tests stamping a simple fill embankment with guidebank
/// end cap
//------------------------------------------------------------------------------
void TutStampingUnitTests::test_StampGuidebank()
{
  // declare a XmStamperIo class to create the inputs
  xms::XmStamperIo io;

  // set as a "fill" stamp
  io.m_stampingType = 1;

  // define the center line
  io.m_centerLine = {{0, 0, 15}, {50, 50, 15}};

  // define a cross section at each point on the center line. This will
  // be a symmetric cross section. The top width will be 10 and the side
  // slope will be 1. The cross section will have a total width of 40.
  xms::XmStampCrossSection cs;
  cs.m_left = {{0, 15}, {5, 15}, {6, 14}};
  cs.m_leftMax = 10;
  cs.m_idxLeftShoulder = 1;
  cs.m_right = cs.m_left;
  cs.m_rightMax = cs.m_leftMax;
  cs.m_idxRightShoulder = cs.m_idxLeftShoulder;
  io.m_cs.push_back(cs);
  io.m_cs.push_back(cs);

  // change to a guidebank end cap
  io.m_firstEndCap.m_type = 0;

  io.m_firstEndCap.m_guidebank.m_nPts = 10;
  io.m_firstEndCap.m_guidebank.m_radius1 = 30;
  io.m_firstEndCap.m_guidebank.m_radius2 = 15;
  io.m_firstEndCap.m_guidebank.m_side = 0; // left side
  io.m_firstEndCap.m_guidebank.m_width = 6;

  // make the last end cap the same as the first
  io.m_lastEndCap = io.m_firstEndCap;
  // change the angle on the last end cap
  io.m_lastEndCap.m_angle = 10;

  // create a Raster to stamp to
  const int numPixelsX = 90, numPixelsY = 81;
  const double pixelSize = 1.0;
  const xms::Pt3d minPt(-21.0, -12.0);
  const std::vector<double> rasterVals(numPixelsX * numPixelsY, XM_NODATA);
  xms::XmStampRaster raster(numPixelsX, numPixelsY, pixelSize, pixelSize, minPt, rasterVals, XM_NODATA);
  // set the raster member of the raster class.  The values are interpolated from io.m_outTin in
  // the XmStamper::DoStamp function.
  io.m_raster = raster;

  // create a XmStamper class. This class performs the stamp operation.
  BSHP<xms::XmStamper> st = xms::XmStamper::New();
  st->DoStamp(io);

  std::string rasterFileName(XMS_TEST_PATH + std::string("stamping/rasterTestFiles/testGuidebank_out.asc"));
  std::string baseFile(XMS_TEST_PATH + std::string("stamping/rasterTestFiles/testGuidebank_base.asc"));
  io.m_raster.WriteGridFile(rasterFileName, xms::XmStampRaster::XmRasterFormatEnum::RS_ARCINFO_ASCII);
  TS_ASSERT_TXT_FILES_EQUAL(baseFile, rasterFileName);

  // verify the outputs
  TS_ASSERT(io.m_outTin); // the output TIN should exist
  TS_ASSERT_EQUALS(144, io.m_outTin->Points().size());
  // Write output TIN for viewing in XMS
  std::string tinFileName(XMS_TEST_PATH + std::string("stamping/rasterTestFiles/testGuidebankTin_out.tin"));
  std::ofstream ofs(tinFileName, std::ofstream::trunc);
  io.m_outTin->ExportTinFile(ofs);
} // TutStampingUnitTests::test_StampGuidebank
//! [snip_testStampGuidebank]
//! [snip_testStampIntersectBathymetry]
//------------------------------------------------------------------------------
/// \brief    Tests stamping a simple fill embankment bathymetry cut off
//------------------------------------------------------------------------------
void TutStampingUnitTests::test_StampIntersectBathymetry()
{
  // declare a XmStamperIo class to create the inputs
  xms::XmStamperIo io;

  // set as a "fill" stamp
  io.m_stampingType = 1;

  // define the center line
  io.m_centerLine = {{0, 0, 15}, {10, 10, 15}};

  // define a cross section at each point on the center line. This will
  // be a symmetric cross section. The top width will be 10 and the side
  // slope will be 1. The cross section will have a total width of 40.
  xms::XmStampCrossSection cs;
  cs.m_left = {{0, 15}, {5, 15}, {6, 14}};
  cs.m_leftMax = 20;
  cs.m_idxLeftShoulder = 1;
  cs.m_right = cs.m_left;
  cs.m_rightMax = cs.m_leftMax;
  cs.m_idxRightShoulder = cs.m_idxLeftShoulder;
  io.m_cs.push_back(cs);
  io.m_cs.push_back(cs);

  // create a TIN to represent Bathymetry
  BSHP<xms::TrTin> tin = xms::TrTin::New();
  BSHP<xms::VecPt3d> tPts(new xms::VecPt3d());
  *tPts = {{-1, 25, 6}, {-15, 11, 6}, {5, -11, 10}, {20, 4, 10}};
  BSHP<xms::VecInt> tTris(new xms::VecInt());
  *tTris = {0, 1, 2, 0, 2, 3};
  tin->SetPoints(tPts);
  tin->SetTriangles(tTris);
  // set the bathymetry member of the io class
  io.m_bathymetry = tin;

  // create a Raster to stamp to
  const int numPixelsX = 29, numPixelsY = 34;
  const double pixelSize = 1.0;
  const xms::Pt3d minPt(-10.0, -8.0);
  const std::vector<double> rasterVals(numPixelsX * numPixelsY, XM_NODATA);
  xms::XmStampRaster raster(numPixelsX, numPixelsY, pixelSize, pixelSize, minPt, rasterVals, XM_NODATA);
  // set the raster member of the raster class.  The values are interpolated from io.m_outTin in
  // the XmStamper::DoStamp function.
  io.m_raster = raster;

  // create a XmStamper class. This class performs the stamp operation.
  BSHP<xms::XmStamper> st = xms::XmStamper::New();
  st->DoStamp(io);

  std::string rasterFileName(XMS_TEST_PATH + std::string("stamping/rasterTestFiles/testIntersectBathymetry_out.asc"));
  std::string baseFile(XMS_TEST_PATH + std::string("stamping/rasterTestFiles/testIntersectBathymetry_base.asc"));
  io.m_raster.WriteGridFile(rasterFileName, xms::XmStampRaster::XmRasterFormatEnum::RS_ARCINFO_ASCII);
  TS_ASSERT_TXT_FILES_EQUAL(baseFile, rasterFileName);

  // verify the outputs
  TS_ASSERT(io.m_outTin); // the output TIN should exist
  xms::VecPt3d basePts = {{0, 0, 15},          {10, 10, 15},        {-3.54, 3.54, 15},
                          {-9.42, 9.42, 6.68}, {6.46, 13.54, 15},   {0.583102, 19.4169, 6.6825},
                          {3.54, -3.54, 15},   {7.18, -7.18, 9.84}, {13.54, 6.46, 15},
                          {17.18, 2.82, 9.84}};
  TS_ASSERT_DELTA_VECPT3D(basePts, io.m_outTin->Points(), 1e-2);
  // Write output TIN for viewing in XMS
  std::string tinFileName(XMS_TEST_PATH + std::string("stamping/rasterTestFiles/testIntersectBathymetryTin_out.tin"));
  std::ofstream ofs(tinFileName, std::ofstream::trunc);
  io.m_outTin->ExportTinFile(ofs);
} // TutStampingUnitTests::test_StampIntersectBathymetry
  //! [snip_testStampIntersectBathymetry]
//------------------------------------------------------------------------------
/// \brief    Tests stamping with real data from double pipe creek
//------------------------------------------------------------------------------
void TutStampingUnitTests::test_RealDataStamping()
{
    // declare a XmStamperIo class to create the inputs
    xms::XmStamperIo io;

    // set as a "fill" stamp
    io.m_stampingType = 1;

    // define the center line
    io.m_centerLine = { 
        {1001673.2,               14390623.5,               336.0              },
        {1001629.780754384200000, 14390618.277580569000000, 336.000000000000000},
        {1001589.675662567800000, 14390613.453783935000000, 336.000000000000000},
        {1001552.631758032300000, 14390608.998183563000000, 336.000000000000000},
        {1001518.415382976300000, 14390604.882675350000000, 336.000000000000000},
        {1001486.810714498500000, 14390601.081300350000000, 336.000000000000000},
        {1001457.618403274400000, 14390597.570081044000000, 336.000000000000000},
        {1001430.654316142900000, 14390594.326870101000000, 336.000000000000000},
        {1001405.748374668900000, 14390591.331210665000000, 336.000000000000000},
        {1001382.743482358400000, 14390588.564207349000000, 336.000000000000000},
        {1001361.494533758600000, 14390586.008407025000000, 336.000000000000000},
        {1001341.867499191000000, 14390583.647688746000000, 336.000000000000000},
        {1001323.738579348000000, 14390581.467162071000000, 336.000000000000000},
        {1001306.993424416500000, 14390579.453073129000000, 336.000000000000000},
        {1001291.526412807400000, 14390577.592717867000000, 336.000000000000000},
        {1001277.239984936200000, 14390575.874361930000000, 336.000000000000000},
        {1001264.044027858300000, 14390574.287166627000000, 336.000000000000000},
        {1001251.855306872700000, 14390572.821120583000000, 336.000000000000000},
        {1001240.596940512100000, 14390571.466976576000000, 336.000000000000000},
        {1001230.197915605000000, 14390570.216193220000000, 336.000000000000000},
        {1001220.592639355300000, 14390569.060881084000000, 336.000000000000000},
        {1001211.720525608300000, 14390567.993752934000000, 336.000000000000000},
        {1001203.525612698400000, 14390567.008077763000000, 336.000000000000000},
        {1001195.956210466300000, 14390566.097638329000000, 336.000000000000000},
        {1001188.964574218400000, 14390565.256691961000000, 336.000000000000000},
        {1001182.506603571900000, 14390564.479934305000000, 336.000000000000000},
        {1001176.541564289400000, 14390563.762465896000000, 336.000000000000000},
        {1001171.031831342000000, 14390563.099761236000000, 336.000000000000000},
        {1001165.942651587700000, 14390562.487640254000000, 336.000000000000000},
        {1001161.241924561600000, 14390561.922241943000000, 336.000000000000000},
        {1001156.9,               14390561.4,               336.0              }
    };

    // define a cross section at each point on the center line. This will
    // be a symmetric cross section. The top width will be 10 and the side
    // slope will be 1. The cross section will have a total width of 40.
    xms::XmStampCrossSection cs;
    cs.m_left = { { 0, 332 },{ 12.5, 332},{ 13.5, 331 } };
    cs.m_leftMax = 35;
    cs.m_idxLeftShoulder = 1;
    cs.m_right = cs.m_left;
    cs.m_rightMax = cs.m_leftMax;
    cs.m_idxRightShoulder = cs.m_idxLeftShoulder;
    io.m_cs.assign(io.m_centerLine.size(), cs);

    // Endcaps and Sloped Abutments
    io.m_firstEndCap.m_type = 2;  // wing wall
    io.m_firstEndCap.m_wingWall.m_wingWallAngle = 0;

    io.m_lastEndCap.m_type = 1;  // sloped abutment
    io.m_lastEndCap.m_slopedAbutment.m_slope = { {0.0, 332.0}, {1.0, 331.0} };
    io.m_lastEndCap.m_slopedAbutment.m_maxX = 25;
    io.m_lastEndCap.m_angle = 0;

    // create a Raster to stamp to
    xms::XmStampRaster raster;

    std::string rasterFileName(XMS_TEST_PATH + std::string("stamping/rasterTestFiles/dpipe_base.asc"));
    std::ifstream ifs(rasterFileName.c_str(), std::ifstream::in);
    raster.ReadFromFile(ifs);
    
    // set the raster member of the raster class.  The values are interpolated from io.m_outTin in
    // the XmStamper::DoStamp function.
    io.m_raster = raster;

    // create a XmStamper class. This class performs the stamp operation.
    BSHP<xms::XmStamper> st = xms::XmStamper::New();
    st->DoStamp(io);

    std::string baseFile(XMS_TEST_PATH + std::string("stamping/rasterTestFiles/garbageOutput.asc"));
    io.m_raster.WriteGridFile(baseFile, xms::XmStampRaster::XmRasterFormatEnum::RS_ARCINFO_ASCII);
} // TutStampingUnitTests::test_RealDataStamping
#endif
