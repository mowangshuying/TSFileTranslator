cmake_minimum_required(VERSION 3.20)
project(TSFileTranslator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}/bin")

set(CMAKE_PREFIX_PATH "C:\\Qt\\6.5.1\\msvc2019_64")

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
if(QT_VERSION VERSION_GREATER_EQUAL "6.2")
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Svg Charts Core5Compat Xml)
find_package(Qt6 REQUIRED COMPONENTS Widgets Svg Charts Core5Compat Xml)
if (USE_QRC)
if (MSVC_IDE)
	message(STATUS "Qt6Widgets_LIBRARIES: ${Qt6Widgets_LIBRARIES}")
	get_target_property(V_LOCATION ${Qt6Widgets_LIBRARIES} IMPORTED_LOCATION)
	message(STATUS "V_LOCATION: ${V_LOCATION}")
	cmake_path(GET V_LOCATION PARENT_PATH V_RUN_PATH)
	message(STATUS "V_RUN_PATH: ${V_RUN_PATH}")
	set(CMAKE_VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${V_RUN_PATH}")
	message(STATUS "CMAKE_VS_DEBUGGER_ENVIRONMENT: ${CMAKE_VS_DEBUGGER_ENVIRONMENT}")
endif()
endif()
else()
find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets Svg Charts)
find_package(Qt5 REQUIRED COMPONENTS Widgets Svg Charts)
endif()

add_subdirectory(3rdparty/FluentUI)
add_subdirectory(3rdparty/TongYiOpenAi)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/FluentUI/FluControls
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/FluentUI/FluUtils
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/TongYiOpenAi/include/TongYiOpenAi
)

set(TGT "TSFileTranslator")
FILE(GLOB DIR_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "*.h"
    "*.cpp"
)

if (${QT_VERSION_MAJOR} LESS 6)
    add_executable(${TGT}
        ${DIR_SRCS}
    )
else()
    qt_add_executable(${TGT}
        ${DIR_SRCS}
    )
endif()

set_property(TARGET ${TGT} PROPERTY WIN32_EXECUTABLE true)
# set(QSS_FILES
#     # "StyleSheet/light/StatsWindow.qss"
#     # "StyleSheet/dark/StatsWindow.qss"
#     # "StyleSheet/atomOneDark/StatsWindow.qss"
#     # "StyleSheet/light/InfoCard.qss"
#     # "StyleSheet/dark/InfoCard.qss"
#     # "StyleSheet/atomOneDark/InfoCard.qss"
# )
# source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${QSS_FILES})

# qt_add_resources(${TGT} "Qss"
#     PREFIX "/"
#     BASE "."
#     FILES
#     ${QSS_FILES}
# )

# add_executable(GithubReleaseStatus App.cpp)
# target_link_libraries(${TGT}
#     PRIVATE 
#     FluControls
#     FluUtils
#     Qt${QT_VERSION_MAJOR}::Network
#     qtadvanceddocking-qt6
#     )

if(QT_VERSION VERSION_GREATER_EQUAL "6.2")
target_link_libraries(${TGT}
    PRIVATE
    FluControls
    FluUtils
    qscintilla
    qrcode
    Qt6::Charts
    Qt6::Core5Compat
    Qt6::Xml
    qwindowkit
    qtadvanceddocking-qt6
)

else()
target_link_libraries(${TGT}
    PRIVATE
    FluControls
    FluUtils
    qscintilla
    qrcode
    Qt5::Charts
    Qt5::Xml
    qwindowkit
    qtadvanceddocking-qt5
)
endif()

