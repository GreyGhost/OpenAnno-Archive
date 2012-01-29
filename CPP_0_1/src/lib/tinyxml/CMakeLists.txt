set(tinyxmlSrcs tinystr.cpp tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp)
add_definitions(-DTIXML_USE_STL)

find_package(PhysFS REQUIRED)
include_directories(${PHYSFS_INCLUDE_DIR})


add_library(tinyxml STATIC ${tinyxmlSrcs})
