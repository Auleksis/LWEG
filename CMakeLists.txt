cmake_minimum_required(VERSION 3.16)

project(LWEG LANGUAGES CXX)

# ---- C++ стандарт ----
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Тип сборки ----
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# ===============================
#       OpenSSL 3.6
# ===============================
set(OPENSSL_ROOT_DIR /opt/openssl-3.6)
set(OPENSSL_USE_STATIC_LIBS FALSE)
find_package(OpenSSL REQUIRED COMPONENTS Crypto)

message(STATUS "Using OpenSSL version: ${OpenSSL_VERSION}")
message(STATUS "OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")

# ===============================
#       Исходные файлы
# ===============================
set(SOURCES
    Algebra/Poly/poly.cpp
    Algebra/Poly/polyMatrix.cpp
    Algebra/Poly/polyVector.cpp
    Algebra/Ring/polyRing.cpp
    HashUtils/shake128.cpp
    HashUtils/shake256.cpp
    Math/functions.cpp
    utils/bytesUtils/bytesUtils.cpp
    utils/sample/sample.cpp
    LWEG/LWEG.cpp
    Tests/tests.cpp
    main.cpp
)

# Если есть дополнительные .cpp (например, те, что ещё не попали в список),
# можно добавить их сюда. Либо воспользоваться GLOB:
# file(GLOB_RECURSE SOURCES "*.cpp")

# ---- Заголовочные файлы (опционально, для удобства IDE) ----
# set(HEADERS
#     Algebra/Poly/poly.h
#     Algebra/Poly/polyMatrix.h
#     Algebra/Poly/polyVector.h
#     Algebra/Ring/polyRing.h
#     HashUtils/shake128.h
#     HashUtils/shake256.h
#     LWEG/LWEG.h
#     LWEG/LWEGContext.h
#     Math/functions.h
#     utils/bytesUtils/bytesUtils.h
#     utils/sample/sample.h
# )

# ===============================
#       Исполняемый файл
# ===============================
add_executable(lweg ${SOURCES})  # заголовки можно добавить для отображения в IDE

# ---- Подключаем директории с заголовками ----
# target_include_directories(LWEG PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}
#     ${CMAKE_CURRENT_SOURCE_DIR}/Algebra
#     ${CMAKE_CURRENT_SOURCE_DIR}/Algebra/Poly
#     ${CMAKE_CURRENT_SOURCE_DIR}/Algebra/Ring
#     ${CMAKE_CURRENT_SOURCE_DIR}/HashUtils
#     ${CMAKE_CURRENT_SOURCE_DIR}/LWEG
#     ${CMAKE_CURRENT_SOURCE_DIR}/Math
#     ${CMAKE_CURRENT_SOURCE_DIR}/utils
#     ${CMAKE_CURRENT_SOURCE_DIR}/utils/bytesUtils
#     ${CMAKE_CURRENT_SOURCE_DIR}/utils/sample
# )

# ---- Линковка с OpenSSL ----
target_link_libraries(lweg PRIVATE OpenSSL::Crypto)

# ---- RPATH для нестандартного пути OpenSSL ----
set_target_properties(lweg PROPERTIES
    BUILD_RPATH "/opt/openssl-3.6/lib64"
    INSTALL_RPATH "/opt/openssl-3.6/lib64"
)