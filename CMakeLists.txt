
cmake_minimum_required(VERSION 3.21.1)

option(LINK_INSIGHT "Link Qt Insight Tracker library" ON)
option(BUILD_QDS_COMPONENTS "Build design studio components" ON)
 
project(KomplexHub LANGUAGES CXX) 

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml)
set(QML_IMPORT_PATH ${QT_QML_OUTPUT_DIRECTORY}
    CACHE STRING "Import paths for Qt Creator's code model"
    FORCE
)

find_package(Qt6 6.8 REQUIRED COMPONENTS Core Gui Widgets Qml Quick QuickControls2 QuickTimeline ShaderTools)
qt_standard_project_setup()

qt_add_executable(${CMAKE_PROJECT_NAME}
  README.md
  LICENSE
  images/icons/icons8-video.svg
)
qt_add_resources(${CMAKE_PROJECT_NAME} "configuration"
    PREFIX "/"
    FILES
        qtquickcontrols2.conf
)

qt_add_resources(${CMAKE_PROJECT_NAME} "app_images"
    FILES
        "images/icons/icons8-back.svg"
        "images/icons/icons8-bill.svg"
        "images/icons/icons8-biotech.svg"
        "images/icons/icons8-cancel.svg"
        "images/icons/icons8-check-mark.svg"
        "images/icons/icons8-close-window.svg"
        "images/icons/icons8-famous.svg"
        "images/icons/icons8-filter.svg"
        "images/icons/icons8-forward.svg"
        "images/icons/icons8-hdd.svg"
        "images/icons/icons8-image.svg"
        "images/icons/icons8-login.svg"
        "images/icons/icons8-logout-rounded-left.svg"
        "images/icons/icons8-next.svg"
        "images/icons/icons8-reply-arrow.svg"
        "images/icons/icons8-search.svg"
        "images/icons/icons8-settings.svg"
        "images/icons/icons8-shopping-cart.svg"
        "images/icons/icons8-star-filled.svg"
        "images/icons/icons8-store.svg"
        "images/icons/icons8-user-account.svg"
        "images/icons/icons8-view-more.svg"
        "images/icons/icons8-video.svg"
        "images/kero/kero_build_1.png"
        "images/kero/kero_build_2.png"
        "images/kero/kero_build_3.png"
        "images/kero/kero_build_4.png"
        "images/kero/kero_build_error.png"
        "images/kero/kero_choice.png"
        "images/kero/kero_error.png"
        "images/kero/kero_hello.png"
        "images/kero/kero_login.png"
        "images/kero/kero_no_results.png"
        "images/kero/kero_run_1.png"
        "images/kero/kero_run_2.png"
        "images/kero/kero_run_3.png"
        "images/kero/kero_run_4.png"
        "images/kero/kero_success.png"
        "images/kero/kero_warning.png"
        "images/icons/icons8-trash.svg"
    )

include(qds)

option(ENABLE_ASAN "Enable AddressSanitizer" OFF)

if(ENABLE_ASAN)
  message(STATUS "Building with AddressSanitizer")
  if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    message(FATAL_ERROR "ASan supported only with Clang/GCC")
  endif()

  target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
  target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -fsanitize=address)
endif()

# this wont produce a binary due to a feature mismatch issue, but will lint

option(ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)

if(ENABLE_CLANG_TIDY)
  if(NOT DEFINED CLANG_TIDY)
    if(DEFINED ENV{CLANG_TIDY})
      set(CLANG_TIDY $ENV{CLANG_TIDY})
    endif()
  endif()

  find_program(CLANG_TIDY_EXE NAMES ${CLANG_TIDY} clang-tidy)
  if(CLANG_TIDY_EXE)
    message(STATUS ${CMAKE_CXX_FLAGS})
    message(STATUS "Found clang-tidy: ${CLANG_TIDY_EXE}")
  else()
    message(STATUS "clang-tidy not found")
  endif()
  if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-checks=modernize-*,performance-*,readability-*,header-filter=.*;--warnings-as-errors=*;--extra-arg=-Qunused-arguments")
  endif()
endif()

if (BUILD_QDS_COMPONENTS)
    include(qmlcomponents OPTIONAL)
endif()

if (LINK_INSIGHT)
    include(insight OPTIONAL)
endif ()

include(GNUInstallDirs)
install(TARGETS ${CMAKE_PROJECT_NAME}
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
