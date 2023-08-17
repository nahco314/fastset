from setuptools import setup, Extension
from os import path, system

DIR = path.dirname(__file__)
setup(
    name="fastset",
    hpy_ext_modules=[
        Extension("fastset", sources=[path.join(DIR, "src/fastset_impl/fastset.cpp")]),
    ],
    setup_requires=["hpy"],
)
