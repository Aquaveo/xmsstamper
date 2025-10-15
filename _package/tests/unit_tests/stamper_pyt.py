"""Test Stamper_py.cpp."""
import os
import unittest

import numpy as np

import xms.grid as xmsgrid

import xms.stamper
from xms.stamper import stamping


class TestStamper(unittest.TestCase):
    """Stamper tests."""
    def setUp(self):
        """Runs before each test."""
        script_file = os.path.abspath(os.path.realpath(__file__))
        script_path = os.path.dirname(script_file)
        self.base_file_path = os.path.join(
            os.path.abspath(
                os.path.join(script_path, '..', '..', '..', '_package', 'tests', 'test_files')
            )
        )
        self.assertTrue(os.path.isdir(os.path.join(script_path, '..', '..', '..')))
        self.assertEqual([], os.listdir(os.path.join(script_path, '..', '..', '..')))
        self.assertTrue(os.path.isdir(os.path.join(script_path, '..', '..', '..', '_package')))
        self.assertTrue(os.path.isdir(os.path.join(script_path, '..', '..', '..', '_package', 'tests')))
        self.assertTrue(os.path.isdir(self.base_file_path),
                        f"Input file directory does not exist {self.base_file_path}")
        self.output_file_path = os.path.join(
            os.path.abspath(
                os.path.join(script_path, '..', '..', '..', 'output_files')
            )
        )
        if not os.path.isdir(self.output_file_path):
            os.mkdir(self.output_file_path)
        self.assertTrue(os.path.isdir(self.output_file_path),
                        "Could not create output file directory")

    def assert_files_equal(self, base, out):
        """Check files are equal.

        Args:
            base (str): Path to the baseline file
            out (str): Path to the output file
        """
        self.assertTrue(os.path.isfile(base), "Base file '{}' does not exist".format(base))
        self.assertTrue(os.path.isfile(out), "Out file '{}' does not exist".format(out))

        with open(base, 'r') as b:
            base_file = ''.join(b.readlines())

        with open(out, 'r') as o:
            out_file = ''.join(o.readlines())

        self.assertEqual(base_file, out_file)

    def test_stamp_fill_embankment(self):
        """Test stamp fill embankment."""
        # Type and centerline
        io_stamping_type = "fill"
        io_centerline = ((0, 0, 15), (0, 10, 15))

        # Cross sections
        left = right = ((0, 15), (5, 15), (6, 14))
        left_max = right_max = 20
        index_left_shoulder = index_right_shoulder = 1
        cs1 = xms.stamper.stamping.CrossSection(
            left=left,
            right=right,
            left_max=left_max,
            right_max=right_max,
            index_left_shoulder=index_left_shoulder,
            index_right_shoulder=index_right_shoulder,
        )
        cs2 = xms.stamper.stamping.CrossSection(
            left=left,
            right=right,
            left_max=left_max,
            right_max=right_max,
            index_left_shoulder=index_left_shoulder,
            index_right_shoulder=index_right_shoulder,
        )
        io_cs = (cs1, cs2)

        # Raster to stamp
        io_raster = xms.stamper.stamping.StampRaster(
            num_pixels_x=41,
            num_pixels_y=11,
            pixel_size_x=1,
            pixel_size_y=1,
            min_point=(-20, 0.0),
            vals=[5] * (41 * 11),
            no_data=float('nan'),
        )

        io = xms.stamper.stamping.StamperIo(
            center_line=io_centerline,
            stamping_type=io_stamping_type,
            cs=io_cs,
            raster=io_raster,
        )

        stamping.stamp(io)

        base_file = os.path.join(
            self.base_file_path,
            "testFillEmbankment_base.asc"
        )
        output_file = os.path.join(
            self.output_file_path, "raster_stamp_fill_embankment_out.asc"
        )

        io.raster.write_grid_file(output_file, 'ascii')

        # Verify grid file
        self.assert_files_equal(base_file, output_file)

        # Verify outputs
        self.assertEqual(7, len(io.out_breaklines))

        breaklines = [
            (0, 1), (3, 2, 0, 6, 7), (5, 4, 1, 8, 9), (3, 5), (7, 9),
            (2, 4), (6, 8)
        ]

        for i in range(0, len(breaklines)):
            self.assertEqual(io.out_breaklines[i], breaklines[i])

    def test_stamp_cut_embankment(self):
        """Test stamp cut embankment."""
        # Type and centerline
        io_stamping_type = "cut"
        io_centerline = ((0, 0, 0), (0, 10, 0))

        # Cross sections
        left = right = ((0, 0), (5, 0), (6, 1))
        left_max = right_max = 20
        index_left_shoulder = index_right_shoulder = 1
        cs1 = xms.stamper.stamping.CrossSection(
            left=left,
            right=right,
            left_max=left_max,
            right_max=right_max,
            index_left_shoulder=index_left_shoulder,
            index_right_shoulder=index_right_shoulder,
        )
        cs2 = xms.stamper.stamping.CrossSection(
            left=left,
            right=right,
            left_max=left_max,
            right_max=right_max,
            index_left_shoulder=index_left_shoulder,
            index_right_shoulder=index_right_shoulder,
        )
        io_cs = (cs1, cs2)

        # Raster to stamp
        no_data = float('nan')
        num_pixels_x = 41
        num_pixels_y = 11
        pixel_size = 1
        min_pt = (-20.0, 0.0)
        raster_vals = [5] * (num_pixels_x * num_pixels_y)
        raster = xms.stamper.stamping.StampRaster(
            num_pixels_x=num_pixels_x,
            num_pixels_y=num_pixels_y,
            pixel_size_x=pixel_size,
            pixel_size_y=pixel_size,
            min_point=min_pt,
            vals=raster_vals,
            no_data=no_data
        )
        io_raster = raster

        io = xms.stamper.stamping.StamperIo(
            center_line=io_centerline,
            stamping_type=io_stamping_type,
            cs=io_cs,
            raster=io_raster,
        )

        stamping.stamp(io)

        base_file = os.path.join(
            self.base_file_path,
            "testCutEmbankment_base.asc"
        )
        output_file = os.path.join(
            self.output_file_path, "raster_stamp_cut_embankment_out.asc"
        )

        io.raster.write_grid_file(output_file, 'ascii')

        # Verify grid file
        self.assert_files_equal(base_file, output_file)

        # Verify outputs
        self.assertEqual(7, len(io.out_breaklines))

        breaklines = [
            (0, 1), (3, 2, 0, 6, 7), (5, 4, 1, 8, 9), (3, 5), (7, 9),
            (2, 4), (6, 8)
        ]

        for i in range(0, len(breaklines)):
            self.assertEqual(io.out_breaklines[i], breaklines[i])

    def test_stamp_wing_wall(self):
        """Test stamp wing wall."""
        # Type and centerline
        io_stamping_type = 'fill'
        io_centerline = ((0, 0, 15), (20, 20, 15))

        # Cross sections
        left = right = ((0, 15), (5, 15), (6, 14))
        left_max = right_max = 20
        index_left_shoulder = index_right_shoulder = 1
        cs1 = xms.stamper.stamping.CrossSection(
            left=left,
            right=right,
            left_max=left_max,
            right_max=right_max,
            index_left_shoulder=index_left_shoulder,
            index_right_shoulder=index_right_shoulder,
        )
        cs2 = xms.stamper.stamping.CrossSection(
            left=left,
            right=right,
            left_max=left_max,
            right_max=right_max,
            index_left_shoulder=index_left_shoulder,
            index_right_shoulder=index_right_shoulder,
        )
        io_cs = (cs1, cs2)

        first_wing_wall = xms.stamper.stamping.WingWall(
            wing_wall_angle=15,
        )
        last_wing_wall = xms.stamper.stamping.WingWall(
            wing_wall_angle=10,
        )
        first_end_cap = xms.stamper.stamping.EndCap(
            angle=15,
            endcap=first_wing_wall,
        )
        last_end_cap = xms.stamper.stamping.EndCap(
            angle=-15,
            endcap=last_wing_wall,
        )

        # Raster to stamp
        num_pixels_x = 46
        num_pixels_y = 45
        pixel_size = 1
        min_pt = (-16.0, -8.0)
        raster_vals = [float('nan')] * (num_pixels_x * num_pixels_y)
        raster = xms.stamper.stamping.StampRaster(
            num_pixels_x=num_pixels_x,
            num_pixels_y=num_pixels_y,
            pixel_size_x=pixel_size,
            pixel_size_y=pixel_size,
            min_point=min_pt,
            vals=raster_vals,
            no_data=float('nan'),
        )
        io_raster = raster

        io = xms.stamper.stamping.StamperIo(
            center_line=io_centerline,
            stamping_type=io_stamping_type,
            cs=io_cs,
            first_end_cap=first_end_cap,
            last_end_cap=last_end_cap,
            raster=io_raster,
        )

        stamping.stamp(io)

        base_file = os.path.join(
            self.base_file_path,
            "testWingWall_base.asc"
        )
        output_file = os.path.join(
            self.output_file_path, "raster_wing_wall_out.asc"
        )

        io.raster.write_grid_file(output_file, 'ascii')

        # Verify grid file
        self.assert_files_equal(base_file, output_file)

        # Verify outputs
        self.assertEqual(7, len(io.out_breaklines))

        breaklines = [
            (0, 1), (3, 2, 0, 6, 7), (5, 4, 1, 8, 9), (3, 5), (7, 9),
            (2, 4), (6, 8)
        ]

        for i in range(0, len(breaklines)):
            self.assertEqual(io.out_breaklines[i], breaklines[i])

    def test_stamp_sloped_abutment(self):
        """Test stamp sloped abutment."""
        # Type and centerline
        io_stamping_type = 'fill'
        io_centerline = ((0, 0, 15), (20, 20, 15))

        # Cross sections
        left = right = ((0, 15), (5, 15), (6, 14))
        left_max = right_max = 20
        index_left_shoulder = index_right_shoulder = 1
        cs1 = xms.stamper.stamping.CrossSection(
            left=left,
            right=right,
            left_max=left_max,
            right_max=right_max,
            index_left_shoulder=index_left_shoulder,
            index_right_shoulder=index_right_shoulder,
        )
        cs2 = xms.stamper.stamping.CrossSection(
            left=left,
            right=right,
            left_max=left_max,
            right_max=right_max,
            index_left_shoulder=index_left_shoulder,
            index_right_shoulder=index_right_shoulder,
        )
        io_cs = (cs1, cs2)

        # Change to sloped abutment end cap
        sloped_abutment = xms.stamper.stamping.SlopedAbutment(
            max_x=10,
            slope=((0, 15), (1, 14)),
        )
        first_end_cap = xms.stamper.stamping.EndCap(
            endcap=sloped_abutment,
        )
        last_end_cap = xms.stamper.stamping.EndCap(
            endcap=sloped_abutment,
            angle=20,
        )

        # Raster to stamp
        num_pixels_x = 56
        num_pixels_y = 53
        pixel_size = 1
        min_pt = (-17.0, -17.0)
        raster_vals = [float('nan')] * (num_pixels_x * num_pixels_y)
        raster = xms.stamper.stamping.StampRaster(
            num_pixels_x=num_pixels_x,
            num_pixels_y=num_pixels_y,
            pixel_size_x=pixel_size,
            pixel_size_y=pixel_size,
            min_point=min_pt,
            vals=raster_vals,
            no_data=float('nan'),
        )
        io_raster = raster

        io = xms.stamper.stamping.StamperIo(
            center_line=io_centerline,
            stamping_type=io_stamping_type,
            first_end_cap=first_end_cap,
            last_end_cap=last_end_cap,
            cs=io_cs,
            raster=io_raster,
        )

        stamping.stamp(io)

        base_file = os.path.join(
            self.base_file_path,
            "testSlopedAbutment_base.asc"
        )
        output_file = os.path.join(
            self.output_file_path, "test_sloped_abutment_out.asc"
        )

        io.raster.write_grid_file(output_file, 'ascii')

        # Verify grid file
        self.assert_files_equal(base_file, output_file)

        base_pts = ((0, 0, 15), (20, 20, 15), (-3.5, 3.5, 15), (-14.1, 14.1, 0),
                    (15.2, 22.2, 15), (4.6, 32.9, 0), (3.5, -3.5, 15),
                    (14.1, -14.1, 0), (24.8, 17.8, 15), (35.4, 7.1, 0),
                    (-10.6, -3.5, 5), (-12.9, -1.9, 4.2), (-14.8, 0.5, 3.3),
                    (-16, 3.5, 2.5), (-16.4, 7, 1.7), (-15.8, 10.6, 0.8),
                    (-3.5, -10.6, 5.0), (-1.9, -12.9, 4.2), (0.5, -14.8, 3.3),
                    (3.5, -16.0, 2.5), (7.0, -16.4, 1.7), (10.6, -15.8, 0.8),
                    (19.4, 31.3, 5), (16.6, 33.4, 3.8), (13, 34.6, 2.5),
                    (8.8, 34.4, 1.3), (29, 26.8, 5), (31.8, 25.9, 4.3),
                    (34.3, 24.1, 3.6), (36.4, 21.5, 2.9), (37.7, 18.2, 2.1),
                    (38, 14.6, 1.4), (37.3, 10.8, 0.7))

        np.testing.assert_array_almost_equal(base_pts, io.out_tin.points,
                                             decimal=1)

    def test_stamp_guide_bank(self):
        """Test stamp guidebank."""
        # Type and centerline
        io_stamping_type = 'fill'
        io_centerline = ((0, 0, 15), (50, 50, 15))

        # Cross sections
        left = right = ((0, 15), (5, 15), (6, 14))
        left_max = right_max = 10
        index_left_shoulder = index_right_shoulder = 1
        cs1 = xms.stamper.stamping.CrossSection(
            left=left,
            right=right,
            left_max=left_max,
            right_max=right_max,
            index_left_shoulder=index_left_shoulder,
            index_right_shoulder=index_right_shoulder,
        )
        cs2 = xms.stamper.stamping.CrossSection(
            left=left,
            right=right,
            left_max=left_max,
            right_max=right_max,
            index_left_shoulder=index_left_shoulder,
            index_right_shoulder=index_right_shoulder,
        )
        io_cs = (cs1, cs2)

        # change to guidebank end cap
        guidebank = xms.stamper.stamping.Guidebank(
            n_points=10,
            radius1=30,
            radius2=15,
            side='left',  # Left Side
            width=6,
        )
        first_endcap = xms.stamper.stamping.EndCap(
            endcap=guidebank,
        )
        last_endcap = xms.stamper.stamping.EndCap(
            endcap=guidebank,
            angle=10,
        )

        # Raster to stamp
        no_data = float('nan')
        num_pixels_x = 90
        num_pixels_y = 81
        pixel_size = 1
        min_pt = (-21.0, -12.0)
        raster_vals = [no_data] * (num_pixels_x * num_pixels_y)
        raster = xms.stamper.stamping.StampRaster(
            num_pixels_x=num_pixels_x,
            num_pixels_y=num_pixels_y,
            pixel_size_x=pixel_size,
            pixel_size_y=pixel_size,
            min_point=min_pt,
            vals=raster_vals,
            no_data=no_data
        )
        io_raster = raster

        io = xms.stamper.stamping.StamperIo(
            center_line=io_centerline,
            stamping_type=io_stamping_type,
            cs=io_cs,
            first_end_cap=first_endcap,
            last_end_cap=last_endcap,
            raster=io_raster,
        )

        stamping.stamp(io)

        base_file = os.path.join(
            self.base_file_path,
            "testGuidebank_base.asc"
        )
        output_file = os.path.join(
            self.output_file_path, "test_guidebank_out.asc"
        )

        io.raster.write_grid_file(output_file, 'ascii')

        # Verify grid file
        self.assert_files_equal(base_file, output_file)

    def test_stamp_intersect_bathymetry(self):
        """Test stamp guidebank."""
        # Type and centerline
        io_stamping_type = 'fill'
        io_centerline = ((0, 0, 15), (10, 10, 15))

        # Cross sections
        left = right = ((0, 15), (5, 15), (6, 14))
        left_max = right_max = 20
        index_left_shoulder = index_right_shoulder = 1
        cs1 = xms.stamper.stamping.CrossSection(
            left=left,
            right=right,
            left_max=left_max,
            right_max=right_max,
            index_left_shoulder=index_left_shoulder,
            index_right_shoulder=index_right_shoulder,
        )
        cs2 = xms.stamper.stamping.CrossSection(
            left=left,
            right=right,
            left_max=left_max,
            right_max=right_max,
            index_left_shoulder=index_left_shoulder,
            index_right_shoulder=index_right_shoulder,
        )
        io_cs = (cs1, cs2)

        # Create a TIN to represent bathymetry
        pts = ((-1, 25, 6), (-15, 11, 6), (5, -11, 10), (20, 4, 10))
        tris = (0, 1, 2, 1, 3, 2)
        tin = xmsgrid.triangulate.Tin(pts, tris)
        io_bathymetry = tin

        # Raster to stamp
        no_data = float('nan')
        num_pixels_x = 29
        num_pixels_y = 34
        pixel_size = 1
        min_pt = (-10.0, -8.0)
        raster_vals = [no_data] * (num_pixels_x * num_pixels_y)
        raster = xms.stamper.stamping.StampRaster(
            num_pixels_x=num_pixels_x,
            num_pixels_y=num_pixels_y,
            pixel_size_x=pixel_size,
            pixel_size_y=pixel_size,
            min_point=min_pt,
            vals=raster_vals,
            no_data=no_data
        )
        io_raster = raster

        io = xms.stamper.stamping.StamperIo(
            center_line=io_centerline,
            stamping_type=io_stamping_type,
            cs=io_cs,
            raster=io_raster,
            bathymetry=io_bathymetry,
        )

        stamping.stamp(io)

        base_file = os.path.join(
            self.base_file_path,
            "testIntersectBathymetry_base.asc"
        )
        output_file = os.path.join(
            self.output_file_path, "test_intersect_bathymetry_out.asc"
        )

        io.raster.write_grid_file(output_file, 'ascii')

        # Verify grid file
        self.assert_files_equal(base_file, output_file)

        base_pts = ((0, 0, 15), (10, 10, 15), (-3.54, 3.54, 15),
                    (-9.42, 9.42, 6.68), (6.46, 13.54, 15), (-4.14, 24.14, 0),
                    (3.54, -3.54, 15), (7.18, -7.18, 9.84), (13.54, 6.46, 15),
                    (17.18, 2.82, 9.84))

        np.testing.assert_array_almost_equal(base_pts, io.out_tin.points,
                                             decimal=2)
