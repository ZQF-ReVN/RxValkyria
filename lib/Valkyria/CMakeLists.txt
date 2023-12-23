# Library Name
set(PROJECT_NAME Valkyria)

# Cpp Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add Library
link_libraries(Rut RxHook)

# Set Source File Path
list(APPEND SRC_FILE 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/DAT_Editor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SDT_Signer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SDT_Parser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SDT_Text_Parser.cpp")

# Source File Only x32
if (CMAKE_SIZEOF_VOID_P EQUAL 4)
list(APPEND SRC_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/VFS_Hook.cpp")
endif()

# Set Library
add_library(${PROJECT_NAME} "${SRC_FILE}")
