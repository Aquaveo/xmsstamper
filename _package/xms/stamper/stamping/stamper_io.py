from .._xmsstamper.stamper import XmStampRaster
from .._xmsstamper.stamper import XmWingWall
from .._xmsstamper.stamper import XmSlopedAbutment
from .._xmsstamper.stamper import XmGuidebank


class StampRaster(object):
    def __init__(self, num_pixels_x=None, num_pixels_y=None, pixel_size_x=None, pixel_size_y=None,
                 min_point=None, vals=None, no_data=float('nan'), **kwargs):
        if 'instance' in kwargs:
            self._instance = kwargs['instance']
            return

        self._instance = XmStampRaster(num_pixels_x, num_pixels_y, pixel_size_x,
                                       pixel_size_y, min_point, vals, no_data)

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
        return self._instance.numPixelsY

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

    def write_to_grid_file(self, file_name, raster_format="ascii"):
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

        self._instance = XmSlopedAbutment()

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
    def __init__(self, **kwargs):
        if 'instance' in kwargs:
            self._instance = kwargs['instance']
            return

        self._instance = XmStamperEndCap()

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

    def write_to_file(self, file_name, card_name):
        return self._instance.WriteToFile(file_name, card_name)

    def read_from_file(self, file_name):
        return self._instance.ReadFromFile(file_name)


class CrossSection(object):
    pass


class StamperIo(object):
    pass
