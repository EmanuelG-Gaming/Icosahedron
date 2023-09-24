set(FREETYPE2_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include")

if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
  set(FREETYPE2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/libfreetype.a")
else ()
  set(FREETYPE2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/libfreetype.a")
endif ()

string(STRIP "${FREETYPE2_LIBRARIES}" FREETYPE2_LIBRARIES)