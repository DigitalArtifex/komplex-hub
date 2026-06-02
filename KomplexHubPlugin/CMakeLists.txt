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
        SettingsManager.h
        SettingsManager.cpp
        InstalledPackManager.h
        InstalledPackManager.cpp
        SOURCES WallpaperMetaData.h
)

target_compile_definitions(
    KomplexHubPlugin
    PUBLIC
        KOMPLEX_PLUGIN
)