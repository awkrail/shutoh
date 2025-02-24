import glob
from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup

__version__ = "0.0.1"

exclude_files = [
    "src/main.cpp",
    "src/command_runner.cpp",
    "src/config.cpp",
    "src/csv_writer.cpp",
    "src/image_extractor.cpp",
    "src/parameters.cpp",
    "src/video_splitter.cpp"
]

ext_modules = [
    Pybind11Extension(
        "libshutoh",
        sorted([x for x in glob.glob("src/**/*.cpp", recursive=True) if not x in exclude_files]),
        include_dirs=["include/", "/usr/include/opencv4", "/usr/local/include/fmt"],
        library_dirs=["/usr/local/lib"],
        libraries=["opencv_core", "opencv_videoio", "fmt"],
        language="c++",
        cxx_std=20,
        define_macros=[("SHUTOH_VERSION_INFO", f'"{__version__}"')],
    ),
]

with open("README.md", "r", encoding="utf-8") as f:
    long_description = f.read()

setup(
    name="libshutoh",
    version=__version__,
    author="Taichi Nishimura",
    author_email="taichitary@gmail.com",
    url="https://github.com/awkrail/shutoh",
    description="Yet another shot detector implemented in C++, designed as a faster alternative to PySceneDetect",
    long_description=long_description,
    long_description_content_type="text/markdown",
    ext_modules=ext_modules,
    extras_require={"test": "pytest"},
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.8",
    install_requires=['scenedetect']
)
