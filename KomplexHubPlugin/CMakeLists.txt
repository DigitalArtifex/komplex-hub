find_package(
  Qt6
    REQUIRED COMPONENTS
      Core
      Gui
      GuiPrivate
      Widgets
      Qml
      Quick
      QuickControls2
      QuickTimeline
      ShaderTools
      ShaderToolsPrivate
      Concurrent
)

find_package(
  OpenGL REQUIRED COMPONENTS OpenGL
)

find_package(
  Vulkan REQUIRED COMPONENTS glslang SPIRV-Tools
)

add_library(
    KomplexHubPlugin
    STATIC
)

set_target_properties(KomplexHubPlugin PROPERTIES VERSION "1.0")

qt_add_qml_module(
    KomplexHubPlugin
    URI
        "KomplexHub.Plugin"
    VERSION
        1.0
    SOURCES
      settingsmanager.h
      settingsmanager.cpp
      installedpackmanager.h
      installedpackmanager.cpp
      common/wallpapercache.h
      newestpacksmodel.h
      newestpacksmodel.cpp
      common/komplex_global.h
      common/slidingcachecontroller.h
      common/coreservices.h
      common/coreservices.cpp
      common/logging.h
      common/fetchresult.h
      paginators/paginator.h
      paginators/newestpackspaginator.h
      paginators/featuredpackspaginator.h
      paginators/featuredimagespaginator.h
      paginators/wallpaperpaginator.h
      paginators/packsearchpaginator.h
      paginators/imagesearchpaginator.h
      paginators/videosearchpaginator.h
      featuredpacksmodel.h
      featuredpacksmodel.cpp
      featuredimagesmodel.h
      featuredimagesmodel.cpp
      featuredvideosmodel.h
      featuredvideosmodel.cpp
      paginators/featuredvideospaginator.h
      common/imagecache.h
      paginators/imagepaginator.h
      common/videocache.h
      paginators/videopaginator.h
      imagesearchmodel.cpp
      imagesearchmodel.h
      videosearchmodel.h
      videosearchmodel.cpp
      packsearchmodel.cpp
      packsearchmodel.h
      videoitemmodel.h
      videoitemmodel.cpp
      downloadmanager.h downloadmanager.cpp
      common/exceptions.h
      common/shadertoymetadata.h
      common/shaderpack.h
      packcompiler.h
      packcompiler.cpp
      common/userservices.h
      common/userservices.cpp
      common/qwallet.h
      common/qwallet.cpp
      usermodel.h 
      usermodel.cpp
      SOURCES
)

target_compile_definitions(
    KomplexHubPlugin
    PUBLIC
        KOMPLEX_PLUGIN
)

add_subdirectory(3rdparty/qtkeychain)

target_link_libraries(KomplexHubPlugin PRIVATE
    Qt6::Core
    Qt6::Concurrent
    Qt6::Gui
    Qt6::GuiPrivate
    Qt6::ShaderTools
    Qt6::ShaderToolsPrivate
    Vulkan::Vulkan
    qt6keychain
)

if(ENABLE_ASAN)
  message(STATUS "Building with AddressSanitizer")
  if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    message(FATAL_ERROR "ASan supported only with Clang/GCC")
  endif()

  target_compile_options(KomplexHubPlugin PRIVATE -fsanitize=address -fno-omit-frame-pointer)
  target_link_options(KomplexHubPlugin PRIVATE -fsanitize=address)
endif()

if(ENABLE_CLANG_TIDY)
  if(NOT DEFINED CLANG_TIDY)
    if(DEFINED ENV{CLANG_TIDY})
      set(CLANG_TIDY $ENV{CLANG_TIDY})
    endif()
  endif()

  find_program(CLANG_TIDY_EXE NAMES ${CLANG_TIDY} clang-tidy)
  if(CLANG_TIDY_EXE)
    message(STATUS "Plugin Found clang-tidy: ${CLANG_TIDY_EXE}")
  else()
    message(STATUS "clang-tidy not found")
  endif()
  if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-checks=modernize-*,performance-*,readability-*,header-filter=.*;--warnings-as-errors=*;--extra-arg=-Qunused-arguments")
  endif()
endif()