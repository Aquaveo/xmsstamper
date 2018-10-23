//------------------------------------------------------------------------------
/// \file
/// \brief
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <boost/shared_ptr.hpp> // boost::shared_ptr
#include <xmscore/python/misc/PyUtils.h> // PyIterFromVecPt3d, etc.
#include <xmscore/python/misc/PublicObserver.h>
#include <xmsstamper/stamper/XmStamper.h>
#include <xmsstamper/stamper/XmStamperIo.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------
PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);

void initXmStamper(py::module &m)
{
  // Class
  const char* xm_stamper_doc = R"pydoc(
      Performs a feature stamp operation.
  )pydoc";
  py::class_<xms::XmStamper, boost::shared_ptr<xms::XmStamper>> stamper(m, 
    "XmStamper", xm_stamper_doc);
  stamper.def(py::init(&xms::XmStamper::New));
  // ---------------------------------------------------------------------------
  // function: do_stamp
  // definition: virtual void DoStamp(XmStamperIo& a_) = 0;
  // ---------------------------------------------------------------------------
  const char* do_stamp_doc = R"pydoc(
      Performs the feature stamping operation.

      Args:
          stamper_io (XmStamperIo): The stamping input/output class. When 
            sucessful, the out_tin and out_breaklines members of stamper_io are 
            filled by this method.
  )pydoc";
  stamper.def("do_stamp",
  [](xms::XmStamper &self, xms::XmStamperIo &stamper_io)
  {
    self.DoStamp(stamper_io);
  }, do_stamp_doc, py::arg("stamper_io"));
  // ---------------------------------------------------------------------------
  // function: fill_stamper_io_from_centerline_profile
  // definition: virtual void FillStamperIoFromCenterlineProfile(XmStamperIo& 
  //             a_io, XmStamperCenterlineProfile& a_profile) = 0;
  // ---------------------------------------------------------------------------
  const char* fill_stamper_io_from_centerline_profile_doc = R"pydoc(
      Converts XmStamperCenterlineProfile class to XmStamperIo class inputs.

      Args:
          stamper_io (XmStamperIo): The stamping input/output class.
          profile (XmStamperCenterlineProfile): The stamping centerline profile
              class.
  )pydoc";

  stamper.def("fill_stamper_io_from_centerline_profile",
  [](xms::XmStamper &self, xms::XmStamperIo &stamper_io, 
    xms::XmStamperCenterlineProfile& profile)
  {
    self.FillStamperIoFromCenterlineProfile(stamper_io, profile);
  }, fill_stamper_io_from_centerline_profile_doc, py::arg("stamper_io"),
  py::arg("profile"));
  // ---------------------------------------------------------------------------
  // function: get_points
  // definition: virtual const VecPt3d& GetPoints() = 0;
  // ---------------------------------------------------------------------------
  const char* get_points_doc = R"pydoc(
      Returns the point locations created by the stamp operation.

      Returns:
          iterable (VecPt3d): Point locations.
  )pydoc";

  stamper.def("get_points",
  [](xms::XmStamper &self) -> py::iterable
  {
    return xms::PyIterFromVecPt3d(self.GetPoints());
  }, get_points_doc);
  // ---------------------------------------------------------------------------
  // function: get_segments
  // definition:  virtual const VecInt2d& GetSegments() = 0;
  // ---------------------------------------------------------------------------
  const char* get_segments_doc = R"pydoc(
      Returns breaklines created by the stamp operation.

      Returns:
          iterable (VecInt2d): Breakline segments.
  )pydoc";

  stamper.def("get_segments",
  [](xms::XmStamper &self) -> py::iterable
  {
    return xms::PyIterFromVecInt2d(self.GetSegments());
  },
  get_segments_doc);
  // ---------------------------------------------------------------------------
  // function: get_breakline_types
  // definition:  virtual const VecInt& GetBreaklineTypes() = 0;
  // ---------------------------------------------------------------------------
  const char* get_breakline_types_doc = R"pydoc(
      Returns the type of breaklines.

      Returns:
          iterable (VecInt): Breakline types.
  )pydoc";
  stamper.def("get_breakline_types",
  [](xms::XmStamper &self) -> py::iterable
  {
    return xms::PyIterFromVecInt(self.GetBreaklineTypes());
  },get_breakline_types_doc);
  // ---------------------------------------------------------------------------
  // function: set_observer
  // definition:  virtual const VecInt& GetBreaklineTypes() = 0;
  // ---------------------------------------------------------------------------
  const char* set_observer_doc = R"pydoc(
      Sets the observer class to get feedback on the meshing process.

      Args:
          observer (Observer): Observer class to provide feedback.
  )pydoc";

  stamper.def("set_observer",
  [](xms::XmStamper &self, boost::shared_ptr<xms::PublicObserver> observer)
  {
    self.SetObserver(observer);
  },set_observer_doc);
}
