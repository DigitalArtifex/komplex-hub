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
        SOURCES paginators/featuredpackspaginator.h
        SOURCES paginators/featuredimagespaginator.h
        SOURCES paginators/wallpaperpaginator.h
        SOURCES paginators/searchpackspaginator.h
        SOURCES paginators/searchimagespaginator.h
        SOURCES paginators/newestshaderspaginator.h
        SOURCES paginators/newestcubemapspaginator.h
        SOURCES paginators/featuredshaderspaginator.h
        SOURCES paginators/featuredcubemapspaginator.h
        SOURCES paginators/searchshaderspaginator.h
        SOURCES paginators/searchcubemapspaginator.h
        SOURCES paginators/searchvideospaginator.h
        SOURCES featuredpacksmodel.h
        SOURCES featuredpacksmodel.cpp
        SOURCES featuredimagesmodel.h
        SOURCES featuredimagesmodel.cpp
        SOURCES featuredvideosmodel.h
        SOURCES featuredvideosmodel.cpp
        SOURCES paginators/featuredvideospaginator.h
        SOURCES common/imagecache.h
        SOURCES paginators/imagepaginator.h
        SOURCES common/videocache.h
        SOURCES paginators/videopaginator.h

)

target_compile_definitions(
    KomplexHubPlugin
    PUBLIC
        KOMPLEX_PLUGIN
        KOMPLEX_LOCAL_DEV
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