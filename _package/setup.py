"""Install the xmsstamper package."""
import os

from setuptools import setup

from xms.stamper import __version__


# allow setup.py to be run from any path
os.chdir(os.path.normpath(os.path.join(os.path.abspath(__file__), os.pardir)))

requires = [
    'numpy',
    'xmscore>=6.0.0',
    'xmsgrid>=7.0.0, <8.0.0',
]

version = __version__

setup(
    python_requires='>=3.10',
    name='xmsstamper',
    version=version,
    packages=['xms.stamper', 'xms.stamper.stamping', 'xms.stamper.api'],
    include_package_data=True,
    license='BSD 2-Clause License',
    description='',
    author='Aquaveo',
    install_requires=requires,
    package_data={'': ['*.pyd', '*.so']},
    test_suite="tests",
    has_ext_modules=lambda: True,
)
