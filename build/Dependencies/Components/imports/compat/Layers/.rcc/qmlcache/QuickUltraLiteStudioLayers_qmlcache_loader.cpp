#include <QtQml/qqmlprivate.h>
#include <QtCore/qdir.h>
#include <QtCore/qurl.h>
#include <QtCore/qhash.h>
#include <QtCore/qstring.h>

namespace QmlCacheGeneratedCode {
namespace _qt_0x2d_project_0x2e_org_imports_QtQuickUltralite_Layers_ApplicationScreens_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_0x2d_project_0x2e_org_imports_QtQuickUltralite_Layers_ImageLayer_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_0x2d_project_0x2e_org_imports_QtQuickUltralite_Layers_ItemLayer_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_0x2d_project_0x2e_org_imports_QtQuickUltralite_Layers_Screen_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_0x2d_project_0x2e_org_imports_QtQuickUltralite_Layers_SpriteLayer_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_0x2d_project_0x2e_org_imports_QtQuickUltralite_Layers__0x2b_DesignMode_Screen_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}

}
namespace {
struct Registry {
    Registry();
    ~Registry();
    QHash<QString, const QQmlPrivate::CachedQmlUnit*> resourcePathToCachedUnit;
    static const QQmlPrivate::CachedQmlUnit *lookupCachedUnit(const QUrl &url);
};

Q_GLOBAL_STATIC(Registry, unitRegistry)


Registry::Registry() {
    resourcePathToCachedUnit.insert(QStringLiteral("/qt-project.org/imports/QtQuickUltralite/Layers/ApplicationScreens.qml"), &QmlCacheGeneratedCode::_qt_0x2d_project_0x2e_org_imports_QtQuickUltralite_Layers_ApplicationScreens_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt-project.org/imports/QtQuickUltralite/Layers/ImageLayer.qml"), &QmlCacheGeneratedCode::_qt_0x2d_project_0x2e_org_imports_QtQuickUltralite_Layers_ImageLayer_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt-project.org/imports/QtQuickUltralite/Layers/ItemLayer.qml"), &QmlCacheGeneratedCode::_qt_0x2d_project_0x2e_org_imports_QtQuickUltralite_Layers_ItemLayer_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt-project.org/imports/QtQuickUltralite/Layers/Screen.qml"), &QmlCacheGeneratedCode::_qt_0x2d_project_0x2e_org_imports_QtQuickUltralite_Layers_Screen_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt-project.org/imports/QtQuickUltralite/Layers/SpriteLayer.qml"), &QmlCacheGeneratedCode::_qt_0x2d_project_0x2e_org_imports_QtQuickUltralite_Layers_SpriteLayer_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt-project.org/imports/QtQuickUltralite/Layers/+DesignMode/Screen.qml"), &QmlCacheGeneratedCode::_qt_0x2d_project_0x2e_org_imports_QtQuickUltralite_Layers__0x2b_DesignMode_Screen_qml::unit);
    QQmlPrivate::RegisterQmlUnitCacheHook registration;
    registration.structVersion = 0;
    registration.lookupCachedQmlUnit = &lookupCachedUnit;
    QQmlPrivate::qmlregister(QQmlPrivate::QmlUnitCacheHookRegistration, &registration);
}

Registry::~Registry() {
    QQmlPrivate::qmlunregister(QQmlPrivate::QmlUnitCacheHookRegistration, quintptr(&lookupCachedUnit));
}

const QQmlPrivate::CachedQmlUnit *Registry::lookupCachedUnit(const QUrl &url) {
    if (url.scheme() != QLatin1String("qrc"))
        return nullptr;
    QString resourcePath = QDir::cleanPath(url.path());
    if (resourcePath.isEmpty())
        return nullptr;
    if (!resourcePath.startsWith(QLatin1Char('/')))
        resourcePath.prepend(QLatin1Char('/'));
    return unitRegistry()->resourcePathToCachedUnit.value(resourcePath, nullptr);
}
}
int QT_MANGLE_NAMESPACE(qInitResources_qmlcache_QuickUltraLiteStudioLayers)() {
    ::unitRegistry();
    return 1;
}
Q_CONSTRUCTOR_FUNCTION(QT_MANGLE_NAMESPACE(qInitResources_qmlcache_QuickUltraLiteStudioLayers))
int QT_MANGLE_NAMESPACE(qCleanupResources_qmlcache_QuickUltraLiteStudioLayers)() {
    return 1;
}
