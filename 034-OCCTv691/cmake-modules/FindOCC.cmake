# Try to find OCE / OCC
# Once done this will define
#
# OCC_FOUND          - system has OCC - OpenCASCADE
# OCC_INCLUDE_DIR    - where the OCC include directory can be found
# OCC_LIBRARY_DIR    - where the OCC library directory can be found
# OCC_LIBRARIES      - Link this to use OCC
# OCC_OCAF_LIBRARIES - Link this to use OCC OCAF framework


set(OCC_INCLUDE_DIR "/opt/OpenCascade/inc")
set(OCC_LIBRARY_DIR "/opt/OpenCascade/lin64/gcc/lib")


file(GLOB OCC_HEADERS ${OCC_INCLUDE_DIR}/*.hxx *.gxx *.lxx)

foreach(_file ${OCC_HEADERS})
  message("Found Header: ${_file}")
endforeach()

file(GLOB OCC_LIBRARIES ${OCC_LIBRARY_DIR}/*.so)

foreach(_file ${OCC_LIBRARIES})
  message("Found library: ${_file}")
endforeach()
