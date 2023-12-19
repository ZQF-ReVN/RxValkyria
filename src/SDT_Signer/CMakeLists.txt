# Value Define
set(PROJECT_NAME SDT_Signer)

# Cpp Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add Library
link_libraries(Valkyria Rut)

# Current Project
project("${PROJECT_NAME}")
add_executable(${PROJECT_NAME} "main.cpp")
