echo off

echo.
echo COPY PYTHON LIBS .pyd FROM .conan AREA TO build_py\lib\. :
echo xmsstamper_py.cp35-win_amd64.pyd
pause

echo on
python -m pip install numpy
pushd .
cd ..\build_py\lib
python -m unittest discover -v -p *_pyt.py -s ../../xmsstamper/python/xmsstamper
popd
echo off

echo.
echo Look for tests ran OK
pause
