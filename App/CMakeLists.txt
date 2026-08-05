
target_sources(${CMAKE_PROJECT_NAME} PUBLIC
    "main.cpp"
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Qml
)

if(ENABLE_ASAN)
  message(STATUS "Building with AddressSanitizer")
  if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    message(FATAL_ERROR "ASan supported only with Clang/GCC")
  endif()

  target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
  target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -fsanitize=address)
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