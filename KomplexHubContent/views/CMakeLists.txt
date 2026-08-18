qt_add_library(KomplexHubViews STATIC)
qt6_add_qml_module(KomplexHubViews
    URI "KomplexHub.Views"
    VERSION 1.0
    RESOURCE_PREFIX "/qt/qml"
    QML_FILES
        "CompletedView.qml"
        "DownloadView.qml"
        "ErrorView.qml"
        "ImageView.qml"
        "PackView.qml"
        "VideoView.qml"
)