"""Classes for reading and writing raster files."""
from xms.grid.triangulate.tin import Tin

from .._xmsstamper.stamper import XmGuidebank
from .._xmsstamper.stamper import XmSlopedAbutment
from .._xmsstamper.stamper import XmStampCrossSection
from .._xmsstamper.stamper import XmStamperEndCap
from .._xmsstamper.stamper import XmStamperIo
from .._xmsstamper.stamper import XmStampRaster
from .._xmsstamper.stamper import XmWingWall


NAN_FLOAT = float('nan')


class StampRaster(object):
    """Class for reading and writing rasters."""
    def __init__(self, num_pixels_x=None, num_pixels_y=None, pixel_size_x=None, pixel_size_y=None,
                 min_point=None, vals=None, no_data=NAN_FLOAT, **kwargs):
        """Constructor.

        Args:
            num_pixels_x (int, required): *see documentation below*
            num_pixels_y (int, required): *see documentation below*
            pixel_size_x (int, required): *see documentation below*
            pixel_size_y (int, required): *see documentation below*
            min_point (iterable, required): *see documentation below*
            vals (iterable, required): *see documentation below*
            no_data (float): *see documentation below* default is nan
            **kwargs (dict): Generic keyword arguments
        """
        if 'instance' in kwargs:
            self._instance = kwargs['instance']
            return

        if num_pixels_x is None:
            raise ValueError("num_pixels_x is a required argument")

        if num_pixels_y is None:
            raise ValueError("num_pixels_y is a required argument")

        if pixel_size_x is None:
            raise ValueError("pixel_size_x is a required argument")

        if pixel_size_y is None:
            raise ValueError("pixel_size_y is a required argument")

        if min_point is None:
            raise ValueError("min_point is a required argument")

        if vals is None:
            raise ValueError("vals is a required argument")

        self._instance = XmStampRaster()

        self.num_pixels_x = num_pixels_x
        self.num_pixels_y = num_pixels_y
        self.pixel_size_x = pixel_size_x
        self.pixel_size_y = pixel_size_y
        self.min_point = min_point
        self.vals = vals

        self.no_data = no_data

    @staticmethod
    def _format_from_string(raster_format):
        """Get the raster format enum from a string.

        Args:
            raster_format: a string representing the raster format you wish to use

        Returns:
            The raster format enum matching the string provided
        """
        raster_formats = {
            'ascii': XmStampRaster.raster_format_enum.RS_ARCINFO_ASCII,
        }
        requested_format = raster_formats.get(raster_format, None)
        if not requested_format:
            raise RuntimeError("raster format must be one of {}, not {}".format(", ".join(raster_formats),
                                                                                raster_format))
        return requested_format

    def __eq__(self, other):
        """Equality operator.

        Args:
            other (StampRaster): StampRaster to compare

        Returns:
            bool: True if StampRasters are equal
        """
        other_instance = getattr(other, '_instance', None)
        if not other_instance or not isinstance(other_instance, XmStampRaster):
            return False
        return other_instance == self._instance

    def __ne__(self, other):
        """Equality operator.

        Args:
            other (StampRaster): StampRaster to compare

        Returns:
            bool: True if StampRasters are not equal
        """
        result = self.__eq__(other)
        return not result

    def __repr__(self):
        """Returns a string representation of the StampRaster."""
        return "<StampRaster - num_pixels_x: {}, num_pixels_y: {}, pixel_size_x: {} pixels_size_y: {} " \
               "no_data: {}, minimum value: {}, maximum value: {}>".format(
                   self.num_pixels_x, self.num_pixels_y, self.pixel_size_x, self.pixel_size_y, self.no_data,
                   min(self.vals), max(self.vals)
               )

    def __str__(self):
        """Returns a string representation of the StampRaster."""
        return "<StampRaster - num_pixels_x: {}, num_pixels_y: {}, pixel_size_x: {} pixels_size_y: {} " \
               "no_data: {}, minimum value: {}, maximum value: {}>".format(
                   self.num_pixels_x, self.num_pixels_y, self.pixel_size_x, self.pixel_size_y, self.no_data,
                   min(self.vals), max(self.vals)
               )

    @property
    def num_pixels_x(self):
        """Number of pixels in the x direction."""
        return self._instance.numPixelsX

    @num_pixels_x.setter
    def num_pixels_x(self, value):
        """Set number of pixels in the x direction."""
        self._instance.numPixelsX = value

    @property
    def num_pixels_y(self):
        """Number of pixels in the y direction."""
        return self._instance.numPixelsY

    @num_pixels_y.setter
    def num_pixels_y(self, value):
        """Set number of pixels in the y direction."""
        self._instance.numPixelsY = value

    @property
    def pixel_size_x(self):
        """Pixel size x."""
        return self._instance.pixelSizeX

    @pixel_size_x.setter
    def pixel_size_x(self, value):
        """Set pixel size x."""
        self._instance.pixelSizeX = value

    @property
    def pixel_size_y(self):
        """Pixel size y."""
        return self._instance.pixelSizeY

    @pixel_size_y.setter
    def pixel_size_y(self, value):
        """Set pixel size y."""
        self._instance.pixelSizeY = value

    @property
    def min_point(self):
        """Minimum (lower left) X, Y coordinate of the raster at the center of the raster cell."""
        return self._instance.min

    @min_point.setter
    def min_point(self, value):
        """Set minimum (lower left) X, Y coordinate of the raster at the center of the raster cell."""
        self._instance.min = value

    @property
    def vals(self):
        """Raster values defined from the top left corner to the bottom right corner.

        Use the no_data value to specify a cell value with no data.
        """
        return self._instance.vals

    @vals.setter
    def vals(self, value):
        """Set raster values defined from the top left corner to the bottom right corner.

        Use the no_data value to specify a cell value with no data.
        """
        self._instance.vals = value

    @property
    def no_data(self):
        """The value that represents cells with no data."""
        return self._instance.noData

    @no_data.setter
    def no_data(self, value):
        """Set the value that represents cells with no data."""
        self._instance.noData = value

    def get_cell_index_from_col_row(self, col, row):
        """Gets the zero-based cell index from the given column and row.

        Args:
            col (int): The zero-based column index for the raster.
            row (int): The zero-based row index for the raster.

        Returns:
            The zero-based cell index from the given a_col, a_row.
        """
        return self._instance.GetCellIndexFromColRow(col, row)

    def get_col_row_from_cell_index(self, index):
        """Gets the zero-based column and row from the cell index.

        Args:
            index (int): The zero-based raster cell index.

        Returns:
            Tuple of (col, row) of the cell by index
        """
        return self._instance.GetColRowFromCellIndex(index)

    def get_location_from_cell_index(self, index):
        """Gets the location of the cell center from the zero-based cell index.

        Args:
            index (int): The zero-based raster cell index.

        Returns:
            The location of the cell with the given index.
        """
        return self._instance.GetLocationFromCellIndex(index)

    def write_grid_file(self, file_name, raster_format="ascii"):
        """Writes the raster in the given format to the given filename.

        Args:
            file_name (str): The output raster filename.
            raster_format (str): The output raster format
        """
        return self._instance.WriteGridFile(file_name, self._format_from_string(raster_format))

    def write_to_file(self, file_name, card_name):
        """Writes the StampRaster class information to a file.

        Args:
            file_name (str): The input file.
            card_name (str): The card name to be written to the output file.
        """
        return self._instance.WriteToFile(file_name, card_name)

    def read_from_file(self, file_name):
        """Reads the StampRaster class information from a file.

        Args:
            file_name (str): The input file.
        """
        return self._instance.ReadFromFile(file_name)


class WingWall(object):
    """Class for reading and writing a wing wall."""
    def __init__(self, wing_wall_angle=0, **kwargs):
        """Constructor.

        Args:
            wing_wall_angle (float): *see documentation below* Default is 0
            **kwargs (dict): Generic keyword arguments
        """
        if 'instance' in kwargs:
            self._instance = kwargs['instance']
            return

        self._instance = XmWingWall(wing_wall_angle)

    def __eq__(self, other):
        """Equality operator.

        Args:
            other (WingWall): WingWall to compare

        Returns:
            bool: True if WingWalls are equal
        """
        other_instance = getattr(other, '_instance', None)
        if not other_instance or not isinstance(other_instance, XmWingWall):
            return False
        return other_instance == self._instance

    def __ne__(self, other):
        """Equality operator.

        Args:
            other (WingWall): WingWall to compare

        Returns:
            bool: True if WingWalls are not equal
        """
        result = self.__eq__(other)
        return not result

    def __repr__(self):
        """Returns a string representation of the WingWall."""
        return "<WingWall - wing_wall_angle: {}>".format(self.wing_wall_angle)

    def __str__(self):
        """Returns a string representation of the WingWall."""
        return "<WingWall - wing_wall_angle: {}>".format(self.wing_wall_angle)

    @property
    def wing_wall_angle(self):
        """The angle of the wing_wall."""
        return self._instance.wingWallAngle

    @wing_wall_angle.setter
    def wing_wall_angle(self, value):
        """Set the angle of the wing_wall."""
        self._instance.wingWallAngle = value

    def write_to_file(self, file_name, card_name):
        """Writes the WingWall class information to a file.

        Args:
            file_name (str): The input file.
            card_name (str): The card name to be written to the output file.
        """
        return self._instance.WriteToFile(file_name, card_name)

    def read_from_file(self, file_name):
        """Reads the WingWall class information from a file.

        Args:
            file_name (str): The input file.
        """
        return self._instance.ReadFromFile(file_name)


class SlopedAbutment(object):
    """Class for reading and writing a sloped abutment."""
    def __init__(self, max_x=0, slope=None, **kwargs):
        """Constructor.

        Args:
            max_x (float): *see documentation below*
            slope (iterable): *see documentation below*
            **kwargs (dict): Generic keyword arguments
        """
        if 'instance' in kwargs:
            self._instance = kwargs['instance']
            return

        if not slope:
            slope = []

        self._instance = XmSlopedAbutment(max_x, slope)

    def __eq__(self, other):
        """Equality operator.

        Args:
            other (SlopedAbutment): SlopedAbutment to compare

        Returns:
            bool: True if SlopedAbutments are equal
        """
        other_instance = getattr(other, '_instance', None)
        if not other_instance or not isinstance(other_instance, XmSlopedAbutment):
            return False
        return other_instance == self._instance

    def __ne__(self, other):
        """Equality operator.

        Args:
            other (SlopedAbutment): SlopedAbutment to compare

        Returns:
            bool: True if SlopedAbutments are not equal
        """
        result = self.__eq__(other)
        return not result

    def __repr__(self):
        """Returns a string representation of the SlopedAbutment."""
        return "<SlopedAbutment - max_x: {}, slope: {}>".format(self.max_x, self.slope)

    def __str__(self):
        """Returns a string representation of the SlopedAbutment."""
        return "<SlopedAbutment - max_x: {}, slope: {}>".format(self.max_x, self.slope)

    @property
    def max_x(self):
        """Max distance from the center line."""
        return self._instance.maxX

    @max_x.setter
    def max_x(self, value):
        """Set max distance from the center line."""
        self._instance.maxX = value

    @property
    def slope(self):
        """x, y pairs defining slope from the center line."""
        return self._instance.slope

    @slope.setter
    def slope(self, value):
        """Set x, y pairs defining slope from the center line."""
        self._instance.slope = value

    def write_to_file(self, file_name, card_name):
        """Writes the SlopedAbutment class information to a file.

        Args:
            file_name (str): The input file.
            card_name (str): The card name to be written to the output file.
        """
        return self._instance.WriteToFile(file_name, card_name)

    def read_from_file(self, file_name):
        """Reads the SlopedAbutment class information from a file.

        Args:
            file_name (str): The input file.
        """
        return self._instance.ReadFromFile(file_name)


class Guidebank(object):
    """Class for reading and writing a guidebank."""
    def __init__(self, side='left', radius1=0, radius2=0,
                 width=0, n_points=10, **kwargs):
        """Constructor.

        Args:
            side (str): *see documentation below*
            radius1 (float): *see documentation below*
            radius2 (float): *see documentation below*
            width (float): *see documentation below*
            n_points (int): *see documentation below*
            **kwargs (dict): Generic keyword arguments
        """
        if 'instance' in kwargs:
            self._instance = kwargs['instance']
            return

        self._instance = XmGuidebank()

        if side is not None:
            self.side = side
        if radius1 is not None:
            self.radius1 = radius1
        if radius2 is not None:
            self.radius2 = radius2
        if width is not None:
            self.width = width
        if n_points is not None:
            self.n_points = n_points

    def __eq__(self, other):
        """Equality operator.

        Args:
            other (Guidebank): Guidebank to compare

        Returns:
            bool: True if Guidebanks are equal
        """
        other_instance = getattr(other, '_instance', None)
        if not other_instance or not isinstance(other_instance, XmGuidebank):
            return False
        return other_instance == self._instance

    def __ne__(self, other):
        """Equality operator.

        Args:
            other (Guidebank): Guidebank to compare

        Returns:
            bool: True if Guidebanks are not equal
        """
        result = self.__eq__(other)
        return not result

    def __repr__(self):
        """Returns a string representation of the Guidebank."""
        return "<Guidebank - side: {}, radius1: {}, radius2: {}, width: {}, n_points: {}>".format(
            self.side, self.radius1, self.radius2, self.width, self.n_points
        )

    def __str__(self):
        """Returns a string representation of the Guidebank."""
        return "<Guidebank - side: {}, radius1: {}, radius2: {}, width: {}, n_points: {}>".format(
            self.side, self.radius1, self.radius2, self.width, self.n_points
        )

    @property
    def side(self):
        """Position of guidebank relative to center line, left, or right."""
        side_converter = {
            0: 'left',
            1: 'right',
        }
        return side_converter[self._instance.side]

    @side.setter
    def side(self, value):
        """Set position of guidebank relative to center line, left, or right."""
        side_converter = {
            'left': 0,
            'right': 1,
        }
        if value not in side_converter.keys():
            raise ValueError('side must be one of {}. Not {}.'.format(", ".join(side_converter.keys()), value))
        self._instance.side = side_converter[value]

    @property
    def radius1(self):
        """First radius (R1) for guidebank creation."""
        return self._instance.radius1

    @radius1.setter
    def radius1(self, value):
        """Set first radius (R1) for guidebank creation."""
        self._instance.radius1 = value

    @property
    def radius2(self):
        """Second radius (R2) for guidebank creation."""
        return self._instance.radius2

    @radius2.setter
    def radius2(self, value):
        """Set second radius (R2) for guidebank creation."""
        self._instance.radius2 = value

    @property
    def width(self):
        """Width of guidebank about the center line."""
        return self._instance.width

    @width.setter
    def width(self, value):
        """Set width of guidebank about the center line."""
        self._instance.width = value

    @property
    def n_points(self):
        """Number of points of guidebank about the center line."""
        return self._instance.nPts

    @n_points.setter
    def n_points(self, value):
        """Set number of points of guidebank about the center line."""
        self._instance.nPts = value

    def write_to_file(self, file_name, card_name):
        """Writes the Guidebank class information to a file.

        Args:
            file_name (str): The input file.
            card_name (str): The card name to be written to the output file.
        """
        return self._instance.WriteToFile(file_name, card_name)

    def read_from_file(self, file_name):
        """Reads the Guidebank class information from a file.

        Args:
            file_name (str): The input file.
        """
        return self._instance.ReadFromFile(file_name)


class EndCap(object):
    """Class for reading and writing an end cap."""
    def __init__(self, endcap=None, angle=None, **kwargs):
        """Constructor.

        Args:
            endcap (object): *see documentation below*
            angle (float): *see documentation below*
            **kwargs (dict): Generic keyword arguments
        """
        if 'instance' in kwargs:
            self._instance = kwargs['instance']
            return

        if endcap is None:
            raise ValueError("Must specify an endcap. Either guidebank, sloped_abutment, or wingwall")

        self._instance = XmStamperEndCap()

        self.endcap = endcap

        if angle is not None:
            self.angle = angle

    def __eq__(self, other):
        """Equality operator.

        Args:
            other (EndCap): EndCap to compare

        Returns:
            bool: True if EndCaps are equal
        """
        other_instance = getattr(other, '_instance', None)
        if not other_instance or not isinstance(other_instance, XmStamperEndCap):
            return False
        return other_instance == self._instance

    def __ne__(self, other):
        """Equality operator.

        Args:
            other (EndCap): EndCap to compare

        Returns:
            bool: True if EndCaps are not equal
        """
        result = self.__eq__(other)
        return not result

    def _get_endcap_type(self):
        """Returns the end cap type as a str."""
        end_cap_types = {
            0: 'guidebank',
            1: 'sloped_abutment',
            2: 'wingwall',
        }
        return end_cap_types[self._instance.type]

    @property
    def type(self):
        """Type of end cap."""
        return self._get_endcap_type()

    @property
    def angle(self):
        """Angle of the endcap degrees from -45 to 45."""
        return self._instance.angle

    @angle.setter
    def angle(self, value):
        """Set angle of the endcap degrees from -45 to 45."""
        if value < -45 or value > 45:
            raise ValueError("angle must be less than -45 or greater than 45, not {}".format(value))
        self._instance.angle = value

    @property
    def endcap(self):
        """The Guidebank, SlopedAbutment, or Wingwall represented in this EndCap."""
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
        """Set the Guidebank, SlopedAbutment, or Wingwall represented in this EndCap."""
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
        """Writes the EndCap class information to a file.

        Args:
            file_name (str): The input file.
            card_name (str): The card name to be written to the output file.
        """
        return self._instance.WriteToFile(file_name, card_name)

    def read_from_file(self, file_name):
        """Reads the EndCap class information from a file.

        Args:
            file_name (str): The input file.
        """
        return self._instance.ReadFromFile(file_name)


class CrossSection(object):
    """Class for reading and writing a cross section."""
    def __init__(self, right=None, left=None, right_max=None, left_max=None, index_right_shoulder=None,
                 index_left_shoulder=None, **kwargs):
        """Constructor.

        Args:
            right (int, required): *see documentation below*
            left (int, required): *see documentation below*
            right_max (iterable): *see documentation below*
            left_max (iterable): *see documentation below*
            index_right_shoulder (int): *see documentation below*
            index_left_shoulder (int): *see documentation below*
            **kwargs (dict): Generic keyword arguments
        """
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
        """Points defining the right side of the  cross section."""
        return self.left

    @left.setter
    def left(self, value):
        """Set points defining the right side of the  cross section."""
        self.left = value

    @property
    def right(self):
        """Points defining the right side of the cross section."""
        return self._instance.left

    @right.setter
    def right(self, value):
        """Set points defining the right side of the cross section."""
        self._instance.left = value

    @property
    def left_max(self):
        """Max x value for left side."""
        return self._instance.leftMax

    @left_max.setter
    def left_max(self, value):
        """Set nax x value for left side."""
        self._instance.leftMax = value

    @property
    def right_max(self):
        """Max x value for right side."""
        return self._instance.rightMax

    @right_max.setter
    def right_max(self, value):
        """Set max x value for right side."""
        self._instance.rightMax = value

    @property
    def index_left_shoulder(self):
        """Index to the shoulder point in the left point list."""
        return self._instance.idxLeftShoulder

    @index_left_shoulder.setter
    def index_left_shoulder(self, value):
        """Set index to the shoulder point in the left point list."""
        self._instance.idxLeftShoulder = value

    @property
    def index_right_shoulder(self):
        """Index to the shoulder point in the right point list."""
        return self._instance.idxRightShoulder

    @index_right_shoulder.setter
    def index_right_shoulder(self, value):
        """Set index to the shoulder point in the right point list."""
        self._instance.idxRightShoulder = value

    def write_to_file(self, file_name, card_name):
        """Writes the CrossSection class information to a file.

        Args:
            file_name (str): The input file.
            card_name (str): The card name to be written to the output file.
        """
        return self._instance.WriteToFile(file_name, card_name)

    def read_from_file(self, file_name):
        """Reads the CrossSection class information from a file.

        Args:
            file_name (str): The input file.
        """
        return self._instance.ReadFromFile(file_name)


class StamperIo(object):
    """Class for reading and writing stamper objects."""
    def __init__(self, center_line=None, stamping_type=None, cs=None, first_end_cap=None, last_end_cap=None,
                 bathymetry=None, raster=None, **kwargs):
        """Constructor.

        Args:
            center_line (iterable): *see documentation below*
            stamping_type (str): *see documentation below*
            cs (:obj:`CrossSection <xms.stamper.stamping.CrossSection>`): *see documentation below*
            first_end_cap (:obj:`EndCap <xms.stamper.stamping.EndCap>`): *see documentation below*
            last_end_cap (:obj:`EndCap <xms.stamper.stamping.EndCap>`): *see documentation below*
            bathymetry (:obj:`Tin <xms.grid.triangulate.Tin>`): *see documentation below*
            raster (:obj:`StampRaster <xms.stamper.stamping.StampRaster>`): *see documentation below*
            **kwargs (dict): Generic keyword arguments
        """
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
        """The center line for the stamping options."""
        return self._instance.centerLine

    @center_line.setter
    def center_line(self, value):
        """Set the center line for the stamping options."""
        self._instance.centerLine = value

    @property
    def stamping_type(self):
        """The stamping type, cut, fill or both."""
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
        """Set the stamping type, cut, fill or both."""
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
        """The CrossSection for the stamping options."""
        _cs = [CrossSection(instance=x) for x in self._instance.cs]
        return _cs

    @cs.setter
    def cs(self, value):
        """Set the CrossSection for the stamping options."""
        _cs = [x._instance for x in value]
        self._instance.cs = _cs

    @property
    def first_end_cap(self):
        """End cap at beginning of polyline."""
        return EndCap(instance=self._instance.firstEndCap)

    @first_end_cap.setter
    def first_end_cap(self, value):
        """Set end cap at beginning of polyline."""
        if not isinstance(value, EndCap):
            raise ValueError("first_end_cap must be an EndCap")
        self._instance.firstEndCap = value._instance

    @property
    def last_end_cap(self):
        """End cap at end of polyline."""
        return EndCap(instance=self._instance.lastEndCap)

    @last_end_cap.setter
    def last_end_cap(self, value):
        """Set end cap at end of polyline."""
        if not isinstance(value, EndCap):
            raise ValueError("last_end_cap must be an EndCap")
        self._instance.lastEndCap = value._instance

    @property
    def bathymetry(self):
        """Underlying bathymetry."""
        return Tin(instance=self._instance.bathymetry)

    @bathymetry.setter
    def bathymetry(self, value):
        """Set underlying bathymetry."""
        if not isinstance(value, Tin):
            raise ValueError("bathymetry must be a Tin")
        self._instance.bathymetry = value._instance

    @property
    def out_tin(self):
        """The output from the stamping procedure."""
        return Tin(instance=self._instance.outTin)

    @property
    def out_breaklines(self):
        """The output breaklines from the stamping procedure."""
        return self._instance.outBreaklines

    @property
    def raster(self):
        """The raster to stamp the feature into."""
        return StampRaster(instance=self._instance.raster)

    @raster.setter
    def raster(self, value):
        """Set the raster to stamp the feature into."""
        if not isinstance(value, StampRaster):
            raise ValueError("raster must be of type StampRaster")
        self._instance.raster = value._instance

    def write_to_file(self, file_name, card_name):
        """Writes the StamperIo class information to a file.

        Args:
            file_name (str): The input file.
            card_name (str): The card name to be written to the output file.
        """
        return self._instance.WriteToFile(file_name, card_name)

    def read_from_file(self, file_name):
        """Reads the StamperIo class information from a file.

        Args:
            file_name (str): The input file.
        """
        return self._instance.ReadFromFile(file_name)

    def set_precision_for_output(self, precision):
        """Set precision of the output.

        Args:
            precision (int): The output precision
        """
        self._instance.SetPrecisionForOutput(precision)
