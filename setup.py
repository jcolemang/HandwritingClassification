from distutils.core import setup, Extension

mod = Extension('dbscan', sources=['dbscan.cpp'])

setup(ext_modules=[mod])
