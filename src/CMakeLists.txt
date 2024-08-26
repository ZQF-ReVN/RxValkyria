# RxValkyria
cmake_minimum_required(VERSION 3.25)

# Project Name
project(ReVN_RxValkyria)

# Export Symbols
if(BUILD_SHARED_LIBS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Set Source File Path
list(APPEND ReVN_RxValkyria_Core_Src_Files
    "src/RxValkyria/Core/Pack.cpp"
    "src/RxValkyria/Core/SDT_Code.cpp"
    "src/RxValkyria/Core/SDT_View.cpp"
    "src/RxValkyria/Core/SDT_HDR.cpp"
    "src/RxValkyria/Core/SDT_Signer.cpp"
    "src/RxValkyria/Core/SDT_String.cpp"
    "src/RxValkyria/Core/SDT_TextCode.cpp"
    "src/RxValkyria/Core/SDT_TextParser.cpp")

# Set Library
add_library(ReVN_RxValkyria_Core ${ReVN_RxValkyria_Core_Src_Files})
add_library(ReVN::RxValkyria::Core ALIAS ReVN_RxValkyria_Core)
target_include_directories(ReVN_RxValkyria_Core PUBLIC "src")
target_compile_features(ReVN_RxValkyria_Core PRIVATE cxx_std_23)
target_compile_options(ReVN_RxValkyria_Core PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
target_link_libraries(ReVN_RxValkyria_Core PUBLIC Zut::ZxMem Zut::ZxJson Zut::ZxCvt Zut::ZxFile Zut::ZxFS)

# Warning
if(MSVC)
    target_compile_options(ReVN_RxValkyria_Core PRIVATE /W4)
else()
    target_compile_options(ReVN_RxValkyria_Core PRIVATE -Wall -Wextra)
endif()


# Hook
if((MSVC) AND (CMAKE_SIZEOF_VOID_P EQUAL 4))
    # Set Source File Path
    list(APPEND ReVN_RxValkyria_Hook_Src_Files
        "src/RxValkyria/Hook/VFS_Hook.cpp")

    # Set Library
    add_library(ReVN_RxValkyria_Hook ${ReVN_RxValkyria_Hook_Src_Files})
    add_library(ReVN::RxValkyria::Hook ALIAS ReVN_RxValkyria_Hook)
    target_include_directories(ReVN_RxValkyria_Hook PUBLIC "src")
    target_compile_options(ReVN_RxValkyria_Hook PRIVATE /W3)
    target_compile_features(ReVN_RxValkyria_Hook PRIVATE cxx_std_23)
    target_compile_options(ReVN_RxValkyria_Hook PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    target_link_libraries(ReVN_RxValkyria_Hook PRIVATE Zut::ZxHook)
endif()
