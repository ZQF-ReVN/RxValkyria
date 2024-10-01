include(FetchContent)

FetchContent_Declare(
  ZxFile
  GIT_REPOSITORY https://github.com/ZQF-Zut/ZxFile.git
  GIT_TAG fe4912736343c69bedd77b4ca0c10e9e4b962bec)

FetchContent_MakeAvailable(ZxFile)

FetchContent_Declare(
  ZxMem
  GIT_REPOSITORY https://github.com/ZQF-Zut/ZxMem.git
  GIT_TAG 873edf0a29e0c54c1003d3544a02010769cea7b8)

FetchContent_MakeAvailable(ZxMem)

FetchContent_Declare(
  ZxCvt
  GIT_REPOSITORY https://github.com/ZQF-Zut/ZxCvt.git
  GIT_TAG cd5d3967a1c7552fdd054215a2961e3cb43d1312)

FetchContent_MakeAvailable(ZxCvt)

FetchContent_Declare(
  ZxJson
  GIT_REPOSITORY https://github.com/ZQF-Zut/ZxJson.git
  GIT_TAG b0784bc00010e13652976e3c7edb34f1ed5e1b3d)

FetchContent_MakeAvailable(ZxJson)

FetchContent_Declare(
  ZxFS
  GIT_REPOSITORY https://github.com/ZQF-Zut/ZxFS.git
  GIT_TAG f7c3d1082b050d203246ca259ea07893c5033494)

FetchContent_MakeAvailable(ZxFS)

if((CMAKE_SYSTEM_NAME STREQUAL "Windows") AND MSVC)
    FetchContent_Declare(
        ZxHook
        GIT_REPOSITORY https://github.com/ZQF-Zut/ZxHook.git
        GIT_TAG f2bdbc8c4b33fc31dd53fcb78f2d2ac7778e2854)

    FetchContent_MakeAvailable(ZxHook)
endif()