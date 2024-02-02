cmake_minimum_required(VERSION 3.16)
project(hdf-expand VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(hdf-expand WIN32 MACOSX_BUNDLE
    hdf_expand.cpp hdf_expand.hpp


)
target_link_libraries(hdf-expand PRIVATE
    Qt::Core
    Qt::Gui
)

list(APPEND cxxflags -DH5Oget_info_by_idx_vers=1 -DH5O_info_t_vers=1)

# Resources:
set(hdf_expand_resource_files
    "images/document.png"
    "images/folder.png"
    "images/image_indexed.png"
    "images/image_true.png"
    "images/left.png"
    "images/matrix.png"
    "images/open.png"
    "images/open_dap.png"
    "images/right.png"
    "images/sample.png"
)

qt_add_resources(hdf-expand "hdf_expand"
    PREFIX
        "/"
    FILES
        ${hdf_expand_resource_files}
)

target_include_directories(hdf-expand PRIVATE
    /usr/local/include
)

target_link_directories(hdf-expand PRIVATE
    /usr/local/lib
)



target_link_libraries(hdf-expand PRIVATE
    libhdf5_cpp.a
    libhdf5_hl_cpp.a
    libhdf5.a
    libhdf5_hl.a
)


if(MACOSX)
    target_include_directories(hdf-expand PRIVATE
        /usr/local/include
    )

    target_link_libraries(hdf-expand PRIVATE
        /usr/local/lib/libhdf5.a
        /usr/local/lib/libhdf5_hl.a
        /usr/local/lib/libsz.a
        curl
        z
    )
endif()

install(TARGETS hdf-expand
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET hdf-expand
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
