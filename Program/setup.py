from distutils.core import setup, Extension

mod = Extension('dbscan', sources=['CExtensionFiles/extension.c',\
                                    'CExtensionFiles/kdtree.c', \
                                    'CExtensionFiles/dynamic_array.c'])

mod.extra_compile_args = ['-std=c11']

setup(ext_modules=[mod])
