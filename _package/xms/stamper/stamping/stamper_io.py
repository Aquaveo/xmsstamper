from .._xmsstamper.stamper import XmStampRaster
from .._xmsstamper.stamper import XmWingWall
from .._xmsstamper.stamper import XmSlopedAbutment
from .._xmsstamper.stamper import XmGuidebank
from .._xmsstamper.stamper import XmStamperEndCap
from .._xmsstamper.stamper import XmStampCrossSection
from .._xmsstamper.stamper import XmStamperIo

from xms.grid.triangulate.tin import Tin
from xms.grid._xmsgrid.triangulate import TrTin


class StampRaster(object):
    def __init__(self, num_pixels_x=None, num_pixels_y=None, pixel_size_x=None, pixel_size_y=None,
                 min_point=None, vals=None, no_data=float('nan'), **kwargs):
        if 'instance' in kwargs:
            self._instance = kwargs['instance']
            return

        self._instance = XmStampRaster()

        if num_pixels_x is not None:
            self.num_pixels_x = num_pixels_x

        if num_pixels_y is not None:
            self.num_pixels_y = num_pixels_y

        if pixel_size_x is not None:
            self.pixel_size_x = pixel_size_x

        if pixel_size_y is not None:
            self.pixel_size_y = pixel_size_y

        if min_point is not None:
            self.min_point = min_point

        if vals is not None:
            self.vals = vals

        self.no_data = no_data

    def _format_from_string(self, raster_format):
        raster_formats = {
            'ascii': XmStampRaster.raster_format_enum.RS_ARCINFO_ASCII,
        }
        requested_format = raster_formats.get(raster_format, None)
        if not requested_format:
            raise RuntimeError("raster format must be one of {}, not {}".format(", ".join(raster_formats), raster_format))
        return requested_format

    @property
    def num_pixels_x(self):
        return self._instance.numPixelsX

    @num_pixels_x.setter
    def num_pixels_x(self, value):
        self._instance.numPixelsX = value

    @property
    def num_pixels_y(self):
        return self._instance.numPixelsY

    @num_pixels_y.setter
    def num_pixels_y(self, value):
        self._instance.numPixelsY = value

    @property
    def pixel_size_x(self):
        return self._instance.pixelSizeX

    @pixel_size_x.setter
    def pixel_size_x(self, value):
        self._instance.pixelSizeX = value

    @property
    def pixel_size_y(self):
        return self._instance.pixelSizeY

    @pixel_size_y.setter
    def pixel_size_y(self, value):
        self._instance.pixelSizeY = value

    @property
    def min_point(self):
        return self._instance.min

    @min_point.setter
    def min_point(self, value):
        self._instance.min = value

    @property
    def vals(self):
        return self._instance.vals

    @vals.setter
    def vals(self, value):
        self._instance.vals = value

    @property
    def no_data(self):
        return self._instance.noData

    @no_data.setter
    def no_data(self, value):
        self._instance.noData = value

    def get_cell_index_from_col_row(self, col, row):
        return self._instance.GetCellIndexFromColRow(col, row)

    def get_col_row_from_cell_index(self, index):
        return self._instance.GetColRowFromCellIndex(index)

    def get_location_from_cell_index(self, index):
        return self._instance.GetLocationFromCellIndex(index)

    def write_grid_file(self, file_name, raster_format="ascii"):
        return self._instance.WriteGridFile(file_name, self._format_from_string(raster_format))

    def write_to_file(self, file_name, card_name):
        return self._instance.WriteToFile(file_name, card_name)

    def read_from_file(self, file_name):
        return self._instance.ReadFromFile(file_name)


class WingWall(object):
    def __init__(self, wing_wall_angle=None, **kwargs):
        if 'instance' in kwargs:
            self._instance = kwargs['instance']
            return

        self._instance = XmWingWall(wing_wall_angle)

    @property
    def wing_wall_angle(self):
        return self._instance.wingWallAngle

    @wing_wall_angle.setter
    def wing_wall_angle(self, value):
        self._instance.wingWallAngle = value

    def write_to_file(self, file_name, card_name):
        return self._instance.WriteToFile(file_name, card_name)

    def read_from_file(self, file_name):
        return self._instance.ReadFromFile(file_name)


class SlopedAbutment(object):
    def __init__(self, max_x=0, slope=None, **kwargs):
        if 'instance' in kwargs:
            self._instance = kwargs['instance']
            return

        if not slope:
            slope = []
        self._instance = XmSlopedAbutment(max_x, slope)

    @property
    def max_x(self):
        return self._instance.maxX

    @max_x.setter
    def max_x(self, value):
        self._instance.maxX = value

    @property
    def slope(self):
        return self._instance.slope

    @slope.setter
    def slope(self, value):
        self._instance.slope = value

    def write_to_file(self, file_name, card_name):
        return self._instance.WriteToFile(file_name, card_name)

    def read_from_file(self, file_name):
        return self._instance.ReadFromFile(file_name)


class Guidebank(object):
    def __init__(self, side=None, radius1=None, radius2=None,
                 width=None, n_points=None, **kwargs):
        if 'instance' in kwargs:
            self._instance = kwargs['instance']
            return

        self._instance = XmGuidebank()

        if side:
            self.side = side
        if radius1:
            self.radius1 = radius1
        if radius2:
            self.radius2 = radius2
        if width:
            self.width = width
        if n_points:
            self.n_points = n_points

    @property
    def side(self):
        return self._instance.side

    @side.setter
    def side(self, value):
        self._instance.side = value

    @property
    def radius1(self):
        return self._instance.radius1

    @radius1.setter
    def radius1(self, value):
        self._instance.radius1 = value

    @property
    def radius2(self):
        return self._instance.radius2

    @radius2.setter
    def radius2(self, value):
        self._instance.radius2 = value

    @property
    def width(self):
        return self._instance.width

    @width.setter
    def width(self, value):
        self._instance.width = value

    @property
    def n_point(self):
        return self._instance.nPts

    @n_point.setter
    def n_point(self, value):
        self._instance.nPts = value

    def write_to_file(self, file_name, card_name):
        return self._instance.WriteToFile(file_name, card_name)

    def read_from_file(self, file_name):
        return self._instance.ReadFromFile(file_name)


class EndCap(object):
    def __init__(self, endcap=None, angle=None, **kwargs):
        if 'instance' in kwargs:
            self._instance = kwargs['instance']
            return

        if endcap is None:
            raise ValueError("Must specify an endcap. Either guidebank, sloped_abutment, or wingwall")

        self._instance = XmStamperEndCap()

        self.endcap = endcap

        if angle is not None:
            self.angle = angle

    def _get_endcap_type(self):
        end_cap_types = {
            0: 'guidebank',
            1: 'sloped_abutment',
            2: 'wingwall',
        }
        return end_cap_types[self._instance.type]

    @property
    def type(self):
        return self._get_endcap_type()

    @property
    def angle(self):
        return self._instance.angle

    @angle.setter
    def angle(self, value):
        self._instance.angle = value

    @property
    def endcap(self):
        if self.type == 'guidebank':
            return self._instance.guidebank
        elif self.type == 'sloped_abutment':
            return self._instance.slopedAbutment
        elif self.type == 'wingwall':
            return self._instance.wingWall
        else:
            raise ValueError("This type of endcap is not supported: {}".format(self.type))

    @endcap.setter
    def endcap(self, value):
        if isinstance(value, Guidebank):
            self._instance.type = 0
            self._instance.guidebank = value._instance
        if isinstance(value, SlopedAbutment):
            self._instance.type = 1
            self._instance.slopedAbutment = value._instance
        if isinstance(value, WingWall):
            self._instance.type = 2
            self._instance.wingWall = value._instance

    def write_to_file(self, file_name, card_name):
        return self._instance.WriteToFile(file_name, card_name)

    def read_from_file(self, file_name):
        return self._instance.ReadFromFile(file_name)


class CrossSection(object):
    def __init__(self, right=None, left=None, right_max=None, left_max=None, index_left_shoulder=None,
                 index_right_shoulder=None, **kwargs):
        if 'instance' in kwargs:
            self._instance = kwargs['instance']
            return

        if right is None:
            raise ValueError("right is a required argument")
        if left is None:
            raise ValueError("left is a required argument")

        self._instance = XmStampCrossSection(left, right)

        if right_max is not None:
            self.right_max = right_max
        if left_max is not None:
            self.left_max = left_max
        if index_left_shoulder is not None:
            self.index_left_shoulder = index_left_shoulder
        if index_right_shoulder is not None:
            self.index_right_shoulder = index_right_shoulder

    @property
    def left(self):
        return self.left

    @left.setter
    def left(self, value):
        self.left = value

    @property
    def right(self):
        return self._instance.left

    @right.setter
    def right(self, value):
        self._instance.left = value

    @property
    def left_max(self):
        return self._instance.leftMax

    @left_max.setter
    def left_max(self, value):
        self._instance.leftMax = value

    @property
    def right_max(self):
        return self._instance.rightMax

    @right_max.setter
    def right_max(self, value):
        self._instance.rightMax = value

    @property
    def index_left_shoulder(self):
        return self._instance.idxLeftShoulder

    @index_left_shoulder.setter
    def index_left_shoulder(self, value):
        self._instance.idxLeftShoulder = value

    @property
    def index_right_shoulder(self):
        return self._instance.idxRightShoulder

    @index_right_shoulder.setter
    def index_right_shoulder(self, value):
        self._instance.idxRightShoulder = value

    def write_to_file(self, file_name, card_name):
        return self._instance.WriteToFile(file_name, card_name)

    def read_from_file(self, file_name):
        return self._instance.ReadFromFile(file_name)


class StamperIo(object):
    def __init__(self, center_line=None, stamping_type=None, cs=None, first_end_cap=None, last_end_cap=None,
                 bathymetry=None, raster=None, **kwargs):
        if 'instance' in kwargs:
            self._instance = kwargs['instance']
            return


        if center_line is None:
            raise ValueError("center_line is a required argument")

        if cs is None:
            raise ValueError("cs is a required argument")

        cs_instances = [x._instance for x in cs]

        self._instance = XmStamperIo(centerline=center_line, cs=cs_instances)

        if stamping_type is not None:
            self.stamping_type = stamping_type

        if first_end_cap is not None:
            self.first_end_cap = first_end_cap

        if last_end_cap is not None:
            self.last_end_cap = last_end_cap

        if bathymetry is not None:
            self.bathymetry = bathymetry

        if raster is not None:
            self.raster = raster

    @property
    def center_line(self):
        return self._instance.centerLine

    @center_line.setter
    def center_line(self, value):
        self._instance.centerLine = value

    @property
    def stamping_type(self):
        stamp_type = {
            0: "cut",
            1: "fill",
            2: "both",
        }
        stamping_type = self._instance.stampingType
        if stamping_type not in stamp_type:
            raise RuntimeError("Unknown stamping type... ({})".format(stamping_type))
        return stamp_type[stamping_type]

    @stamping_type.setter
    def stamping_type(self, value):
        stamp_type = {
            "cut": 0,
            "fill": 1,
            "both": 2,
        }
        if value not in stamp_type:
            raise RuntimeError("Unknown stamping type... ({})".format(value))
        self._instance.stampingType = stamp_type[value]

    @property
    def cs(self):
        _cs = [CrossSection(instance=x) for x in self._instance.cs]
        return _cs

    @cs.setter
    def cs(self, value):
        _cs = [x._instance for x in value]
        self._instance.cs = _cs

    @property
    def first_end_cap(self):
        return EndCap(instance=self._instance.firstEndCap)

    @first_end_cap.setter
    def first_end_cap(self, value):
        if not isinstance(value, EndCap):
            raise ValueError("first_end_cap must be an EndCap")
        self._instance.firstEndCap = value._instance

    @property
    def last_end_cap(self):
        return EndCap(instance=self._instance.lastEndCap)

    @last_end_cap.setter
    def last_end_cap(self, value):
        if not isinstance(value, EndCap):
            raise ValueError("last_end_cap must be an EndCap")
        self._instance.lastEndCap = value._instance

    @property
    def bathymetry(self):
        return Tin(instance=self._instance.bathymetry)

    @bathymetry.setter
    def bathymetry(self, value):
        if not isinstance(value, Tin):
            raise ValueError("bathymetry must be a Tin")
        self._instance.bathymetry = value._instance

    @property
    def out_tin(self):
        return Tin(instance=self._instance.outTin)

    @property
    def out_breaklines(self):
        return self._instance.outBreaklines

    @property
    def raster(self):
        return StampRaster(instance=self._instance.raster)

    @raster.setter
    def raster(self, value):
        if not isinstance(value, StampRaster):
            raise ValueError("raster must be of type StampRaster")
        self._instance.raster = value._instance

    def write_to_file(self, file_name, card_name):
        return self._instance.WriteToFile(file_name, card_name)

    def read_from_file(self, file_name):
        return self._instance.ReadFromFile(file_name)

    def set_precision_for_output(self, precision):
        self._instance.SetPrecisionForOutput(precision)
