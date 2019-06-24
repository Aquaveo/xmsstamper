//------------------------------------------------------------------------------
/// \file
/// \brief
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/shared_ptr.hpp> // boost::shared_ptr
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <xmscore/misc/xmstype.h> // XM_NODATA
#include <xmscore/python/misc/PyUtils.h> // PyIterFromVecPt3d, etc.

#include <xmsgrid/triangulate/TrTin.h>

#include <xmsstamper/python/stamper/stamper_py.h>
#include <xmsstamper/stamper/XmStamperIo.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Internal functions -----------------------------------------------------

//----- Python Interface -------------------------------------------------------
PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);

void initXmStamperIo(py::module &m)
{
// -----------------------------------------------------------------------------
// XMSTAMPRASTER
// -----------------------------------------------------------------------------
  py::class_<xms::XmStampRaster, boost::shared_ptr<xms::XmStampRaster>> stamp_raster(m, "XmStampRaster");

  stamp_raster.def(py::init<>([](py::object num_pixels_x, py::object num_pixels_y,
           py::object pixel_size_x, py::object pixel_size_y, py::object min,
           py::object vals, py::object no_data) {
    boost::shared_ptr<xms::XmStampRaster> xm_stamp_raster(new xms::XmStampRaster());
    if (!num_pixels_x.is_none())
    {
      xm_stamp_raster->m_numPixelsX = py::cast<int>(num_pixels_x);
    }
    if (!num_pixels_y.is_none())
    {
      xm_stamp_raster->m_numPixelsY = py::cast<int>(num_pixels_y);
    }
    if (!pixel_size_x.is_none())
    {
      xm_stamp_raster->m_pixelSizeX = py::cast<double>(pixel_size_x);
    }
    if (!pixel_size_y.is_none())
    {
      xm_stamp_raster->m_pixelSizeY = py::cast<double>(pixel_size_y);
    }
    if (!min.is_none())
    {
      xm_stamp_raster->m_min = xms::Pt3dFromPyIter(min);
    }
    if (!vals.is_none())
    {
      xm_stamp_raster->m_vals = *xms::VecDblFromPyIter(vals);
    }
    if (!no_data.is_none())
    {
      xm_stamp_raster->m_noData = py::cast<int>(no_data);
    }
    return xm_stamp_raster;
  }),
  py::arg("num_pixels_x") = py::none(), py::arg("num_pixels_y") = py::none(),
  py::arg("pixel_size_x") = py::none(), py::arg("pixel_size_y") = py::none(),
  py::arg("min") = py::none(), py::arg("vals") = py::none(),
  py::arg("no_data") = py::none());

  // ---------------------------------------------------------------------------
  // property: num_pixels_x
  // ---------------------------------------------------------------------------
  stamp_raster.def_readwrite("numPixelsX", &xms::XmStampRaster::m_numPixelsX);
  // ---------------------------------------------------------------------------
  // property: num_pixels_y
  // ---------------------------------------------------------------------------
  stamp_raster.def_readwrite("numPixelsY", &xms::XmStampRaster::m_numPixelsY);
  // ---------------------------------------------------------------------------
  // property: pixel_size_x
  // ---------------------------------------------------------------------------
  stamp_raster.def_readwrite("pixelSizeX", &xms::XmStampRaster::m_pixelSizeX);
  // ---------------------------------------------------------------------------
  // property: pixel_size_y
  // ---------------------------------------------------------------------------
  stamp_raster.def_readwrite("pixelSizeY", &xms::XmStampRaster::m_pixelSizeY);
  // ---------------------------------------------------------------------------
  // property: no_data
  // ---------------------------------------------------------------------------
  stamp_raster.def_readwrite("noData", &xms::XmStampRaster::m_noData);
  // ---------------------------------------------------------------------------
  // property: min
  // ---------------------------------------------------------------------------
  stamp_raster.def_property("min",
  [](xms::XmStampRaster &self) -> py::tuple 
  {
    return xms::PyIterFromPt3d(self.m_min);
  },
  [](xms::XmStampRaster &self, py::tuple _min)
  {
    self.m_min = xms::Pt3dFromPyIter(_min);
  });
  // ---------------------------------------------------------------------------
  // property: vals
  // ---------------------------------------------------------------------------
  stamp_raster.def_property("vals",
  [](xms::XmStampRaster &self) -> py::iterable
  {
    return xms::PyIterFromVecDbl(self.m_vals);
  },
  [](xms::XmStampRaster &self, py::iterable _vals)
  {
    self.m_vals = *xms::VecDblFromPyIter(_vals);
  });
  // ---------------------------------------------------------------------------
  // function: GetCellIndexFromColRow
  // ---------------------------------------------------------------------------
  stamp_raster.def("GetCellIndexFromColRow", &xms::XmStampRaster::GetCellIndexFromColRow,
    py::arg("col"), py::arg("row"));
  // ---------------------------------------------------------------------------
  // function: GetColRowFromCellIndex
  // ---------------------------------------------------------------------------
  stamp_raster.def("GetColRowFromCellIndex",
  [](xms::XmStampRaster &self, int index)
  {
    int c, r;
    self.GetColRowFromCellIndex(index, c, r);
    return py::make_tuple(c, r);
  }, py::arg("int"));
  // ---------------------------------------------------------------------------
  // function: GetLocationFromCellIndex
  // ---------------------------------------------------------------------------
  stamp_raster.def("GetLocationFromCellIndex",
  [](xms::XmStampRaster &self, int index)
  {
    return xms::PyIterFromPt3d(self.GetLocationFromCellIndex(index));
  }, py::arg("index"));
  // ---------------------------------------------------------------------------
  // function: WriteGridFile
  // ---------------------------------------------------------------------------
  stamp_raster.def("WriteGridFile", &xms::XmStampRaster::WriteGridFile, 
    py::arg("file_name"), py::arg("format"));
  // ---------------------------------------------------------------------------
  // function: ReadFromFile
  // ---------------------------------------------------------------------------
  stamp_raster.def("ReadFromFile",
  [](xms::XmStampRaster &self, std::string file_name)
  {
    std::ifstream is;
    is.open(file_name, std::ios::in);
    self.ReadFromFile(is);
    is.close();
  }, py::arg("file_name"));
  // ---------------------------------------------------------------------------
  // function: WriteToFile
  // ---------------------------------------------------------------------------
  stamp_raster.def("WriteToFile",
  [](xms::XmStampRaster &self, std::string file_name, std::string card_name)
  {
    // TODO: This needs some error checking
    std::ofstream os;
    os.open(file_name, std::ios::out);
    self.WriteToFile(os, card_name);
    os.close();
  }, py::arg("file_name"), py::arg("card_name"));


// -----------------------------------------------------------------------------
// XMSTAMPRASTERENUM
// -----------------------------------------------------------------------------
  py::enum_<xms::XmStampRaster::XmRasterFormatEnum>(stamp_raster, 
         "raster_format_enum", "weight_enum for InterpIdw class")
    .value("RS_ARCINFO_ASCII", 
                 xms::XmStampRaster::XmRasterFormatEnum::RS_ARCINFO_ASCII)
    .export_values();


// -----------------------------------------------------------------------------
// XMWINGWALL
// -----------------------------------------------------------------------------
  py::class_<xms::XmWingWall, boost::shared_ptr<xms::XmWingWall>> 
    stamper_wing_wall(m, "XmWingWall");

  stamper_wing_wall.def(py::init<>([](py::object wing_wall_angle) {
    boost::shared_ptr<xms::XmWingWall> rval(new xms::XmWingWall());
    if (!wing_wall_angle.is_none())
    {
      rval->m_wingWallAngle = py::cast<double>(wing_wall_angle);
    }
    return rval;
  }), py::arg("wing_wall_angle") = py::none());
  // ---------------------------------------------------------------------------
  // property: wingWallAngle
  // ---------------------------------------------------------------------------
  stamper_wing_wall.def_readwrite("wingWallAngle", 
             &xms::XmWingWall::m_wingWallAngle);
  // ---------------------------------------------------------------------------
  // function: ReadFromFile
  // ---------------------------------------------------------------------------
  stamper_wing_wall.def("ReadFromFile",
    [](xms::XmWingWall &self, std::string file_name)
  {
    std::ifstream is;
    is.open(file_name, std::ios::in);
    self.ReadFromFile(is);
    is.close();
  }, py::arg("file_name"));
  // ---------------------------------------------------------------------------
  // function: WriteToFile
  // ---------------------------------------------------------------------------
  stamper_wing_wall.def("WriteToFile",
    [](xms::XmWingWall &self, std::string file_name, std::string card_name)
  {
    // TODO: This needs some error checking
    std::ofstream os;
    os.open(file_name, std::ios::out);
    self.WriteToFile(os, card_name);
    os.close();
  }, py::arg("file_name"), py::arg("card_name"));

// -----------------------------------------------------------------------------
// XMSLOPEDABUTMENT
// -----------------------------------------------------------------------------
  py::class_<xms::XmSlopedAbutment, boost::shared_ptr<xms::XmSlopedAbutment>>
    stamper_sloped_abutment(m, "XmSlopedAbutment");

  stamper_sloped_abutment.def(py::init<>([](py::object max_x, py::object slope) {
    boost::shared_ptr<xms::XmSlopedAbutment> xm_sloped_abutment(new xms::XmSlopedAbutment());
    if (!max_x.is_none())
    {
      xm_sloped_abutment->m_maxX = py::cast<double>(max_x);
    }
    if (!slope.is_none())
    {
      xm_sloped_abutment->m_slope = *xms::VecPt3dFromPyIter(slope);
    }
    return xm_sloped_abutment;
  }),
  py::arg("max_x") = py::none(), py::arg("slope") = py::none());
  // ---------------------------------------------------------------------------
  // property: maxX
  // ---------------------------------------------------------------------------
  stamper_sloped_abutment.def_readwrite("maxX", 
    &xms::XmSlopedAbutment::m_maxX);
  // ---------------------------------------------------------------------------
  // property: slope
  // ---------------------------------------------------------------------------
  stamper_sloped_abutment.def_property("slope",
    [](xms::XmSlopedAbutment &self) -> py::iterable
  {
    return xms::PyIterFromVecPt3d(self.m_slope);
  },
    [](xms::XmSlopedAbutment &self, py::iterable slope)
  {
    self.m_slope = *xms::VecPt3dFromPyIter(slope);
  });
  // ---------------------------------------------------------------------------
  // function: ReadFromFile
  // ---------------------------------------------------------------------------
  stamper_sloped_abutment.def("ReadFromFile",
    [](xms::XmSlopedAbutment &self, std::string file_name)
  {
    std::ifstream is;
    is.open(file_name, std::ios::in);
    self.ReadFromFile(is);
    is.close();
  }, py::arg("file_name"));
  // ---------------------------------------------------------------------------
  // function: WriteToFile
  // ---------------------------------------------------------------------------
  stamper_sloped_abutment.def("WriteToFile",
    [](xms::XmSlopedAbutment &self, std::string file_name, std::string card_name)
  {
    // TODO: This needs some error checking
    std::ofstream os;
    os.open(file_name, std::ios::out);
    self.WriteToFile(os, card_name);
    os.close();
  }, py::arg("file_name"), py::arg("card_name"));

// -----------------------------------------------------------------------------
// XMGUIDEBANK
// -----------------------------------------------------------------------------
  py::class_<xms::XmGuidebank, boost::shared_ptr<xms::XmGuidebank>>
    stamper_guide_bank(m, "XmGuidebank");

  stamper_guide_bank.def(py::init<>([](py::object side, py::object radius1,
           py::object radius2, py::object width, py::object n_pts) {
    boost::shared_ptr<xms::XmGuidebank> xm_guidebank(new xms::XmGuidebank());
    if (!side.is_none())
    {
      xm_guidebank->m_side = py::cast<int>(side);
    }
    if (!radius1.is_none())
    {
      xm_guidebank->m_radius1 = py::cast<double>(radius1);
    }
    if (!radius2.is_none())
    {
      xm_guidebank->m_radius2 = py::cast<double>(radius2);
    }
    if (!width.is_none())
    {
      xm_guidebank->m_width = py::cast<double>(width);
    }
    if (!n_pts.is_none())
    {
      xm_guidebank->m_nPts = py::cast<int>(n_pts);
    }
    return xm_guidebank;
  }),
  py::arg("side") = py::none(), py::arg("radius1") = py::none(),
  py::arg("radius2") = py::none(), py::arg("width") = py::none(),
  py::arg("n_pts") = py::none());
  // ---------------------------------------------------------------------------
  // property: side
  // ---------------------------------------------------------------------------
  stamper_guide_bank.def_readwrite("side",
    &xms::XmGuidebank::m_side);
  // ---------------------------------------------------------------------------
  // property: radius1
  // ---------------------------------------------------------------------------
  stamper_guide_bank.def_readwrite("radius1",
    &xms::XmGuidebank::m_radius1);
  // ---------------------------------------------------------------------------
  // property: m_radius2
  // ---------------------------------------------------------------------------
  stamper_guide_bank.def_readwrite("radius2",
    &xms::XmGuidebank::m_radius2);
  // ---------------------------------------------------------------------------
  // property: m_width
  // ---------------------------------------------------------------------------
  stamper_guide_bank.def_readwrite("width",
    &xms::XmGuidebank::m_width);
  // ---------------------------------------------------------------------------
  // property: n_pts
  // ---------------------------------------------------------------------------
  stamper_guide_bank.def_readwrite("nPts",
    &xms::XmGuidebank::m_nPts);
  // ---------------------------------------------------------------------------
  // function: ReadFromFile
  // ---------------------------------------------------------------------------
  stamper_guide_bank.def("ReadFromFile",
    [](xms::XmGuidebank &self, std::string file_name)
  {
    std::ifstream is;
    is.open(file_name, std::ios::in);
    self.ReadFromFile(is);
    is.close();
  }, py::arg("file_name"));
  // ---------------------------------------------------------------------------
  // function: WriteToFile
  // ---------------------------------------------------------------------------
  stamper_guide_bank.def("WriteToFile",
    [](xms::XmGuidebank &self, std::string file_name, std::string card_name)
  {
    // TODO: This needs some error checking
    std::ofstream os;
    os.open(file_name, std::ios::out);
    self.WriteToFile(os, card_name);
    os.close();
  }, py::arg("file_name"), py::arg("card_name"));

// -----------------------------------------------------------------------------
// XMSTAMPERENDCAP
// -----------------------------------------------------------------------------
  py::class_<xms::XmStamperEndCap, boost::shared_ptr<xms::XmStamperEndCap>>
    stamper_end_cap(m, "XmStamperEndCap");

  stamper_end_cap.def(py::init<>(
  [](int endcap_type, double angle, py::object guidebank,
     py::object sloped_abutment, py::object wing_wall) {
    boost::shared_ptr<xms::XmStamperEndCap> rval(new xms::XmStamperEndCap);
    rval->m_angle = angle;
    if (!guidebank.is_none())
    {
      rval->m_guidebank = py::cast<xms::XmGuidebank>(guidebank);
      rval->m_type = 0;
    }
    else if (!sloped_abutment.is_none())
    {
      rval->m_slopedAbutment = py::cast<xms::XmSlopedAbutment>(sloped_abutment);
      rval->m_type = 1;
    }
    else if (!wing_wall.is_none())
    {
      rval->m_wingWall = py::cast<xms::XmWingWall>(wing_wall);
      rval->m_type = 2;
    }
    else
    {
      if (endcap_type < 0 || endcap_type > 2)
        rval->m_type = 2;
      else
        rval->m_type = endcap_type;
    }
    return rval;
  }
  ), py::arg("endcap_type") = -1, py::arg("angle") = 0,
     py::arg("guidebank") = py::none(), py::arg("sloped_abutment") = py::none(),
     py::arg("wing_wall") = py::none());
  // ---------------------------------------------------------------------------
  // property: type
  // ---------------------------------------------------------------------------
  stamper_end_cap.def_readwrite("type",
    &xms::XmStamperEndCap::m_type);
  // ---------------------------------------------------------------------------
  // property: angle
  // ---------------------------------------------------------------------------
  stamper_end_cap.def_readwrite("angle",
    &xms::XmStamperEndCap::m_angle);
  // ---------------------------------------------------------------------------
  // property: guidebank
  // ---------------------------------------------------------------------------
  stamper_end_cap.def_readwrite("guidebank",
    &xms::XmStamperEndCap::m_guidebank);
  // ---------------------------------------------------------------------------
  // property: slopedAbutment
  // ---------------------------------------------------------------------------
  stamper_end_cap.def_readwrite("slopedAbutment",
    &xms::XmStamperEndCap::m_slopedAbutment);
  // ---------------------------------------------------------------------------
  // property: wingWall
  // ---------------------------------------------------------------------------
  stamper_end_cap.def_readwrite("wingWall",
    &xms::XmStamperEndCap::m_wingWall);
  // ---------------------------------------------------------------------------
  // function: ReadFromFile
  // ---------------------------------------------------------------------------
  stamper_end_cap.def("ReadFromFile",
    [](xms::XmStamperEndCap &self, std::string file_name)
  {
    std::ifstream is;
    is.open(file_name, std::ios::in);
    self.ReadFromFile(is);
    is.close();
  }, py::arg("file_name"));
  // ---------------------------------------------------------------------------
  // function: WriteToFile
  // ---------------------------------------------------------------------------
  stamper_end_cap.def("WriteToFile",
    [](xms::XmStamperEndCap &self, std::string file_name, std::string card_name)
  {
    // TODO: This needs some error checking
    std::ofstream os;
    os.open(file_name, std::ios::out);
    self.WriteToFile(os, card_name);
    os.close();
  }, py::arg("file_name"), py::arg("card_name"));

// -----------------------------------------------------------------------------
// XMSTAMPCROSSSECTION
// -----------------------------------------------------------------------------
  py::class_<xms::XmStampCrossSection, 
    boost::shared_ptr<xms::XmStampCrossSection>> 
    stamper_cross_section(m, "XmStampCrossSection");
  stamper_cross_section.def(py::init<>(
  [](py::iterable left, py::iterable right, float left_max, float right_max,
     int index_left_shoulder, int index_right_shoulder) {
      boost::shared_ptr<xms::XmStampCrossSection> rval(new xms::XmStampCrossSection);
      rval->m_left = *xms::VecPt3dFromPyIter(left);
      rval->m_right = *xms::VecPt3dFromPyIter(right);
      rval->m_leftMax = left_max;
      rval->m_rightMax = right_max;
      rval->m_idxLeftShoulder = index_left_shoulder;
      rval->m_idxRightShoulder = index_right_shoulder;
      return rval;
  }
  ), py::arg("left"), py::arg("right"), py::arg("left_max") = 0.0,
     py::arg("right_max") = 0.0, py::arg("index_left_shoulder") = 0,
     py::arg("index_right_shoulder") = 0);
  // ---------------------------------------------------------------------------
  // property: left
  // ---------------------------------------------------------------------------
  stamper_cross_section.def_property("left",
  [](xms::XmStampCrossSection &self) -> py::iterable
  {
      return xms::PyIterFromVecPt3d(self.m_left);
  },
  [](xms::XmStampCrossSection &self, py::iterable left)
  {
      self.m_left = *xms::VecPt3dFromPyIter(left);
  });
  // ---------------------------------------------------------------------------
  // property: leftMax
  // ---------------------------------------------------------------------------
  stamper_cross_section.def_readwrite("leftMax", &xms::XmStampCrossSection::m_leftMax);
  // ---------------------------------------------------------------------------
  // property: idxLeftShoulder
  // ---------------------------------------------------------------------------
  stamper_cross_section.def_readwrite("idxLeftShoulder", &xms::XmStampCrossSection::m_idxLeftShoulder);
  // ---------------------------------------------------------------------------
  // property: right
  // ---------------------------------------------------------------------------
  stamper_cross_section.def_property("right",
  [](xms::XmStampCrossSection &self) -> py::iterable
  {
      return xms::PyIterFromVecPt3d(self.m_right);
  },
  [](xms::XmStampCrossSection &self, py::iterable right)
  {
      self.m_right = *xms::VecPt3dFromPyIter(right);
  });
  // ---------------------------------------------------------------------------
  // property: rightMax
  // ---------------------------------------------------------------------------
  stamper_cross_section.def_readwrite("rightMax", &xms::XmStampCrossSection::m_rightMax);
  // ---------------------------------------------------------------------------
  // property: idxRightShoulder
  // ---------------------------------------------------------------------------
  stamper_cross_section.def_readwrite("idxRightShoulder", &xms::XmStampCrossSection::m_idxRightShoulder);
  // ---------------------------------------------------------------------------
  // function: ReadFromFile
  // ---------------------------------------------------------------------------
  stamper_cross_section.def("ReadFromFile",
    [](xms::XmStampCrossSection &self, std::string file_name)
  {
    std::ifstream is;
    is.open(file_name, std::ios::in);
    self.ReadFromFile(is);
    is.close();
  }, py::arg("file_name"));
  // ---------------------------------------------------------------------------
  // function: WriteToFile
  // ---------------------------------------------------------------------------
  stamper_cross_section.def("WriteToFile",
    [](xms::XmStampCrossSection &self, std::string file_name, std::string card_name)
  {
    // TODO: This needs some error checking
    std::ofstream os;
    os.open(file_name, std::ios::out);
    self.WriteToFile(os, card_name);
    os.close();
  }, py::arg("file_name"), py::arg("card_name"));

// -----------------------------------------------------------------------------
// XMSTAMPERIO
// -----------------------------------------------------------------------------
  py::class_<xms::XmStamperIo, boost::shared_ptr<xms::XmStamperIo>> 
    stamper_io(m, "XmStamperIo");
  stamper_io.def(py::init<>(
  [](py::iterable centerline, py::object stamping_type, py::iterable cs,
     py::object first_end_cap, py::object last_end_cap,
     py::object bathymetry, py::object raster) {
    boost::shared_ptr<xms::XmStamperIo> rval(new xms::XmStamperIo);
    rval->m_centerLine = *xms::VecPt3dFromPyIter(centerline);
    rval->m_stampingType = py::cast<int>(stamping_type);
    std::vector<xms::XmStampCrossSection> vec_cs;
    vec_cs.resize(py::len(cs));
    int i = 0;
    for (auto item : cs)
    {
        if (!item.is_none())
        {
            vec_cs.at(i) = item.cast<xms::XmStampCrossSection>();
        }
        i++;
    }
    rval->m_cs = vec_cs;
    if (!first_end_cap.is_none())
    {
      rval->m_firstEndCap= py::cast<xms::XmStamperEndCap>(first_end_cap);
    }
    if (!last_end_cap.is_none())
    {
      rval->m_lastEndCap= py::cast<xms::XmStamperEndCap>(last_end_cap);
    }
    if (!bathymetry.is_none())
    {

      rval->m_bathymetry = py::cast<boost::shared_ptr<xms::TrTin>>(bathymetry);
    }
    if (!raster.is_none())
    {

      rval->m_raster = py::cast<xms::XmStampRaster>(raster);
    }
    return rval;
  }
  ), py::arg("centerline"), py::arg("stamping_type") = py::none(), py::arg("cs"),
     py::arg("first_end_cap") = py::none(), py::arg("last_end_cap") = py::none(),
     py::arg("bathymetry") = py::none(), py::arg("raster") = py::none());
  // ---------------------------------------------------------------------------
  // property: centerLine
  // ---------------------------------------------------------------------------
  stamper_io.def_property("centerLine",
  [](xms::XmStamperIo &self) -> py::iterable
  {
  return xms::PyIterFromVecPt3d(self.m_centerLine);
  },
  [](xms::XmStamperIo &self, py::iterable centerline)
  {
    self.m_centerLine = *xms::VecPt3dFromPyIter(centerline);
  });
  // ---------------------------------------------------------------------------
  // property: stampingType
  // ---------------------------------------------------------------------------
  stamper_io.def_readwrite("stampingType",
    &xms::XmStamperIo::m_stampingType);
  // ---------------------------------------------------------------------------
  // property: cs
  // ---------------------------------------------------------------------------
  stamper_io.def_property("cs",
    [](xms::XmStamperIo &self) -> py::iterable
  {
    auto tuple_ret = py::tuple(self.m_cs.size());
    for (size_t i = 0; i < tuple_ret.size(); i++) {
      tuple_ret[i] = self.m_cs.at(i);
    }
    return tuple_ret;
  },
  [](xms::XmStamperIo &self, py::iterable& py_cs)
  {
    std::vector<xms::XmStampCrossSection> vec_cs;
    vec_cs.resize(py::len(py_cs));
    int i = 0;
    for (auto item : py_cs)
    {
        if (!item.is_none())
        {
            vec_cs.at(i) = item.cast<xms::XmStampCrossSection>();
        }
        i++;
    }
    self.m_cs = vec_cs;
  });
  // ---------------------------------------------------------------------------
  // property: firstEndCap
  // ---------------------------------------------------------------------------
  stamper_io.def_readwrite("firstEndCap", &xms::XmStamperIo::m_firstEndCap);
  // ---------------------------------------------------------------------------
  // property: last_end_cap
  // ---------------------------------------------------------------------------
  stamper_io.def_readwrite("lastEndCap", &xms::XmStamperIo::m_lastEndCap);
  // ---------------------------------------------------------------------------
  // property: raster
  // ---------------------------------------------------------------------------
  stamper_io.def_readwrite("raster", &xms::XmStamperIo::m_raster);
  // ---------------------------------------------------------------------------
  // property: bathymetry
  // ---------------------------------------------------------------------------
  stamper_io.def_property("bathymetry",
  [](xms::XmStamperIo &self) -> boost::shared_ptr<xms::TrTin>
  {
    return self.m_bathymetry;
  }, 
  [](xms::XmStamperIo &self, boost::shared_ptr<xms::TrTin> &bathymetry)
  {
    self.m_bathymetry = bathymetry;
  });
  // ---------------------------------------------------------------------------
  // function: outTin
  // ---------------------------------------------------------------------------
  stamper_io.def("outTin",
  [](xms::XmStamperIo &self) -> boost::shared_ptr<xms::TrTin>
  {
    return self.m_outTin;
  });
  // ---------------------------------------------------------------------------
  // property: outBreaklines
  // ---------------------------------------------------------------------------
  stamper_io.def_property_readonly("outBreaklines",
  [](xms::XmStamperIo &self) -> py::iterable
  {
    return xms::PyIterFromVecInt2d(self.m_outBreakLines);
  });
  // ---------------------------------------------------------------------------
  // function: ReadFromFile
  // ---------------------------------------------------------------------------
  stamper_io.def("ReadFromFile",
  [](xms::XmStamperIo &self, std::string file_name)
  {
    std::ifstream is;
    is.open(file_name, std::ios::in);
    self.ReadFromFile(is);
    is.close();
  }, py::arg("file_name"));
  // ---------------------------------------------------------------------------
  // function: WriteToFile
  // ---------------------------------------------------------------------------
  stamper_io.def("WriteToFile",
  [](xms::XmStamperIo &self, std::string file_name, std::string card_name)
  {
    std::ofstream os;
    os.open(file_name, std::ios::out);
    self.WriteToFile(os, card_name);
    os.close();
  }, py::arg("file_name"), py::arg("card_name"));
}
