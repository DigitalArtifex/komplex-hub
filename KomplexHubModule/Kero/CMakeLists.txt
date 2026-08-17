qt_add_library(KomplexHubModule_Kero STATIC)
qt6_add_qml_module(KomplexHubModule_Kero
    URI "KomplexHub.Kero"
    VERSION 1.0
    RESOURCE_PREFIX "/qt/qml"
    QML_FILES
        "KeroBuildingAnimation.qml"
        "KeroChoiceAvatar.qml"
        "KeroErrorAvatar.qml"
        "KeroHelloAvatar.qml"
        "KeroLoadingAnimation.qml"
        "KeroNoResultsAvatar.qml"
        "KeroWarningAvatar.qml"
        QML_FILES KeroHeader.qml
)