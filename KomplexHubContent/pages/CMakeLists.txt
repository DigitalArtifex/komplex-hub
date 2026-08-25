qt_add_library(KomplexHubPages STATIC)
qt6_add_qml_module(KomplexHubPages
    URI "KomplexHub.Pages"
    VERSION 1.0
    RESOURCE_PREFIX "/qt/qml"
    QML_FILES
        "HomePage.qml"
        "ImageSearchPage.qml"
        "InstalledWallpaperPage.qml"
        "LiveSearchPage.qml"
        "LoginPage.qml"
        "SettingsPage.qml"
        "UserProfilePage.qml"
        "WallpaperSettingsPage.qml"
        "VideoSearchPage.qml"
        "FeaturedVideosPage.qml"
        "FeaturedPacksPage.qml"
        "FeaturedImagesPage.qml"
        "NewestPacksPage.qml"
        QML_FILES KomplexSettingsPage.qml
)