//------------------------------------------------------------------------------
/// \file
/// \brief
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
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

void initXmStamperClass(py::module &m)
{
  py::class_<xms::XmStamper, boost::shared_ptr<xms::XmStamper>> stamper(m, "Stamper");
  stamper.def(py::init(&xms::XmStamper::New));
  // ---------------------------------------------------------------------------
  // function: do_stamp
  // definition: virtual void DoStamp(XmStamperIo& a_) = 0;
  // ---------------------------------------------------------------------------
  const char* do_stamp_doc = R"pydoc(
      Performs the feature stamping operation

      Args:
          stamper_io (XmStamperIo): The stamping input/output class. When sucessful, the out_tin and
          out_breaklines members of stamper_io and filled by this method.
  )pydoc";
  stamper.def("do_stamp",
  [](xms::XmStamper &self, xms::XmStamperIo &stamper_io)
  {
    self.DoStamp(stamper_io);
  },
  do_stamp_doc,
  py::arg("stamper_io"));
  // ---------------------------------------------------------------------------
  // function: fill_stamper_io_from_centerline_profile
  // definition: virtual void FillStamperIoFromCenterlineProfile(XmStamperIo& a_io, XmStamperCenterlineProfile& a_profile) = 0;
  // ---------------------------------------------------------------------------
  const char* fill_stamper_io_from_centerline_profile_doc = R"pydoc(
      Converts XmStamperCenterlineProfile class to XmStamperIo class inputs

      Args:
          stamper_io (XmStamperIo): The stamping input/output class
          profile (XmStamperCenterlineProfile): The stamping centerline profile class
  )pydoc";
  stamper.def("fill_stamper_io_from_centerline_profile",
  [](xms::XmStamper &self, xms::XmStamperIo &stamper_io, xms::XmStamperCenterlineProfile& profile)
  {
    self.FillStamperIoFromCenterlineProfile(stamper_io, profile);
  },
  fill_stamper_io_from_centerline_profile_doc,
  py::arg("stamper_io"),
  py::arg("profile"));
  // ---------------------------------------------------------------------------
  // function: get_points
  // definition: virtual const VecPt3d& GetPoints() = 0;
  // ---------------------------------------------------------------------------
  const char* get_points_doc = R"pydoc(
      Returns the point locations created by the stamp operation

      Returns:
          py::iterable (VecPt3d): point locations
  )pydoc";
  stamper.def("get_points",
  [](xms::XmStamper &self) -> py::iterable
  {
    return xms::PyIterFromVecPt3d(self.GetPoints());
  },
  get_points_doc);
  // ---------------------------------------------------------------------------
  // function: get_segments
  // definition:  virtual const VecInt2d& GetSegments() = 0;
  // ---------------------------------------------------------------------------
  const char* get_segments_doc = R"pydoc(
      Returns breaklines created by the stamp operation

      Returns:
          py::iterable (VecInt2d): breakline segments
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
      Returns the type of breaklines

      Returns:
          py::iterable (VecInt): breakline types
  )pydoc";
  stamper.def("get_breakline_types",
  [](xms::XmStamper &self) -> py::iterable
  {
    return xms::PyIterFromVecInt(self.GetBreaklineTypes());
  },
  get_breakline_types_doc);
  // ---------------------------------------------------------------------------
  // function: set_observer
  // definition:  virtual const VecInt& GetBreaklineTypes() = 0;
  // ---------------------------------------------------------------------------
  const char* set_observer_doc = R"pydoc(
      Sets the observer class to get feedback on the meshing process

      Args:
          observer (Observer): Observer class to provide feedback.
  )pydoc";
  stamper.def("set_observer",
  [](xms::XmStamper &self, boost::shared_ptr<xms::PublicObserver> observer)
  {
    self.SetObserver(observer);
  },
  set_observer_doc);
}

void initXmStamperModule(py::module &m)
{
    py::module modStamper = m.def_submodule("stamper");

    // -------------------------------------------------------------------------------------------
    // function: stamp
    // -------------------------------------------------------------------------------------------
    const char* stamp_doc = R"pydoc(
        Performs the feature stamping operation

        Args:
            stamper_io (XmStamperIo): The stamping input/output class. When sucessful, the out_tin and out_breaklines members of stamper_io and filled by this method.
    )pydoc";
    modStamper.def("stamp",
        [](xms::XmStamperIo &stamper_io) {
            boost::shared_ptr<xms::XmStamper> stamper = xms::XmStamper::New();
            stamper->DoStamp(stamper_io);
        }, stamp_doc, py::arg("stamper_io")
    );

    // -------------------------------------------------------------------------------------------
    // function: fill_stamper_io_from_centerline_profile
    // -------------------------------------------------------------------------------------------
    const char* fill_stamper_io_from_centerline_profile_doc = R"pydoc(
        Converts XmStamperCenterlineProfile class to XmStamperIo class inputs

        Args:
            stamper_io (XmStamperIo): The stamping input/output class
            profile (XmStamperCenterlineProfile): The stamping centerline profile class
    )pydoc";
    modStamper.def("fill_stamper_io_from_centerline_profile",
        [](xms::XmStamperIo &stamper_io, xms::XmStamperCenterlineProfile& profile)
        {
          boost::shared_ptr<xms::XmStamper> stamper = xms::XmStamper::New();
          stamper->FillStamperIoFromCenterlineProfile(stamper_io, profile);
        }, fill_stamper_io_from_centerline_profile_doc, py::arg("stamper_io"), py::arg("profile")
    );
}

