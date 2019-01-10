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
}

