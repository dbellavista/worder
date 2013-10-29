#CMakeLists.txt
#
#:Author: Daniele Bellavista
#:Email: Daniele_Bellavista@iprel.it
#:Date: 2013-10-29 13:26

cmake_minimum_required (VERSION 2.6)
project (Worder)
# The version number.
set (Worder_VERSION_MAJOR 1)
set (Worder_VERSION_MINOR 0)

set (Worder_SRC_DIR "${Worder_SOURCE_DIR}/src")
set (Worder_TEST_DIR "${Worder_SRC_DIR}/test")
set (Worder_LIB_DIR "lib")
set (Worder_INCLUDE_DIR "include")
set (Worder_BIN_DIR "bin")

add_subdirectory(src)
