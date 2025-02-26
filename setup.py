import glob
import subprocess
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

def get_include_path(pkg_name):
    output = subprocess.run(["pkg-config", "--cflags", pkg_name], encoding="utf-8", capture_output=True, text=True).stdout
    return output.strip().replace("-I", "")

def get_library_path(pkg_name):
    output = subprocess.run(["pkg-config", "--libs", pkg_name], encoding="utf-8", capture_output=True, text=True).stdout
    lib_path = [x.replace("-L", "") for x in output.strip().split(" ") if x[:2] == "-L"]
    return lib_path[0] if len(lib_path) != 0 else ""

print(get_include_path("opencv4"))
print(get_include_path("fmt") + "/fmt")
print(get_library_path("fmt"))

ext_modules = [
    Pybind11Extension(
        "libshutoh",
        sorted([x for x in glob.glob("src/**/*.cpp", recursive=True) if not x in exclude_files]),
        include_dirs=["include/", get_include_path("opencv4"), get_include_path("fmt") + "/fmt"],
        library_dirs=[get_library_path("fmt")],
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
)
