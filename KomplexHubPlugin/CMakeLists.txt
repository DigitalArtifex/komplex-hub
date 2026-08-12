add_library(
    KomplexHubPlugin
    STATIC
)

qt_add_qml_module(
    KomplexHubPlugin
    URI
        KomplexHubPlugin
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
        common/paginator.h
        common/coreservices.h
        common/coreservices.cpp
        common/logging.h
        common/fetchresult.h
        paginators/newestpackspaginator.h
        paginators/featuredpackspaginator.h
        paginators/featuredimagespaginator.h
        paginators/wallpaperpaginator.h
        paginators/packsearchpaginator.h
        paginators/imagesearchpaginator.h
        paginators/newestshaderspaginator.h
        paginators/newestcubemapspaginator.h
        paginators/featuredshaderspaginator.h
        paginators/featuredcubemapspaginator.h
        paginators/searchshaderspaginator.h
        paginators/searchcubemapspaginator.h
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
        SOURCES packcompiler.h packcompiler.cpp
)

target_compile_definitions(
    KomplexHubPlugin
    PUBLIC
        KOMPLEX_PLUGIN
)

target_link_libraries(KomplexHubPlugin PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
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