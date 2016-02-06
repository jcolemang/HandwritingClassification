from distutils.core import setup, Extension

mod = Extension('dbscan', sources=['extension.c', 'kdtree.c'])
mod.extra_compile_args = ['-std=c11']

setup(ext_modules=[mod])
