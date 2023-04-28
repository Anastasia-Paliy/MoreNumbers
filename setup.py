from distutils.core import setup, Extension

sources = ['main.c']
name = 'more_numbers'
description = 'A module for working with numbers in different number systems with long arithmetic.'
module1 = Extension(name, sources=sources)

setup(
    name=name,
    version='1.0',
    description=description,
    ext_modules=[module1]
)

# python setup.py install
