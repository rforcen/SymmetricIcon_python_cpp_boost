# https://docs.python.org/3/distutils/apiref.html
from distutils.core import setup
from distutils.extension import Extension

import os


SymmetricIcon = Extension(
    'SymmetricIcon',
    sources=['symmetricIcon.cpp'],
    libraries=['boost_python37-mt', 'boost_numpy37-mt'],
    extra_compile_args=['-std=c++11']  # lambda support required
)

setup(
    name='SymmetricIcon',
    version='0.1',
    ext_modules=[SymmetricIcon])

# call with: python3.7 setup.py build_ext --inplace
