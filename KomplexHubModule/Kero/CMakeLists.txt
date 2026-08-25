qt_add_library(KomplexHubKero STATIC)
qt6_add_qml_module(KomplexHubKero
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
        "KeroHeader.qml"
        "KeroSuccessAvatar.qml"
)
#set_target_properties(KomplexHubKero PROPERTIES VERSION "1.0")