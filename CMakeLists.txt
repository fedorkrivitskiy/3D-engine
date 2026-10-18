cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(3D_Engine VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# -----------------------------------------------------------------------------
# Qt6 Packages
# -----------------------------------------------------------------------------
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets OpenGLWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGLWidgets)

# -----------------------------------------------------------------------------
# Conan2 Packages
# -----------------------------------------------------------------------------
find_package(nlohmann_json REQUIRED)
find_package(glm REQUIRED)
find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(tinyobjloader REQUIRED)

# -----------------------------------------------------------------------------
# Source files
# -----------------------------------------------------------------------------
file(GLOB_RECURSE SRC_HEADERS CONFIGURE_DEPENDS src/*.hpp)
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE UI_HEADERS CONFIGURE_DEPENDS ui/*.h ui/*.hpp)
file(GLOB_RECURSE UI_FILES CONFIGURE_DEPENDS ui/*.cpp)
file(GLOB_RECURSE UI_XML CONFIGURE_DEPENDS ui/*.ui)
file(GLOB_RECURSE RES CONFIGURE_DEPENDS resources/*)

set(PROJECT_SOURCES
    ${SRC_FILES}
    ${SRC_HEADERS}
    ${UI_FILES}
    ${UI_HEADERS}
    ${UI_XML}
)

if(UI_HEADERS OR SRC_HEADERS)
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Header Files" FILES ${SRC_HEADERS})
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/ui PREFIX "Header Files" FILES ${UI_HEADERS})
endif()

if(SRC_FILES OR UI_FILES)
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Source Files" FILES ${SRC_FILES})
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/ui PREFIX "Source Files" FILES ${UI_FILES})
endif()

if(UI_XML)
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/ui PREFIX "Form Files" FILES ${UI_XML})
endif()

if(RES)
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/resources PREFIX "Resources" FILES ${RES})
endif()

qt_add_executable(${PROJECT_NAME}
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    resources/resources.qrc
    ui/widgets/SettingsWidget.hpp ui/widgets/SettingsWidget.cpp ui/widgets/SettingsWidget.ui
)

target_include_directories(
    ${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    src
    ui
)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    Qt6::Widgets
    Qt6::OpenGLWidgets
)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    nlohmann_json::nlohmann_json
    glm::glm
    opengl::opengl
    glfw
    glad::glad
    tinyobjloader::tinyobjloader
)


if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${PROJECT_NAME})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()