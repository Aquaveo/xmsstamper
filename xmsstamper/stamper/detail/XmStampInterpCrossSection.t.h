#pragma once
//------------------------------------------------------------------------------
/// \file
/// \brief
/// \ingroup stamping
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

#ifdef CXX_TEST

// 3. Standard Library Headers

// 4. External Library Headers
#include <cxxtest/TestSuite.h>

// 5. Shared Headers

// 6. Non-shared Headers

////////////////////////////////////////////////////////////////////////////////
/// \brief Tests the XmStampInterpCrossSection class
class XmStampInterpCrossSectionUnitTests : public CxxTest::TestSuite
{
public:
  void test0();
  void test1();
  void testCrossSectionTutorial();
}; // XmStampInterpCrossSectionUnitTests

#endif
