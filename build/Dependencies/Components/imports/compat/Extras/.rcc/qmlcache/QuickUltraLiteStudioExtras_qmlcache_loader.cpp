#include <QtQml/qqmlprivate.h>
#include <QtCore/qdir.h>
#include <QtCore/qurl.h>
#include <QtCore/qhash.h>
#include <QtCore/qstring.h>

namespace QmlCacheGeneratedCode {
namespace _qt_0x2d_project_0x2e_org_imports_QtQuickUltralite_Extras_AnimatedSpriteDirectory_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_0x2d_project_0x2e_org_imports_QtQuickUltralite_Extras_ColorizedImage_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_0x2d_project_0x2e_org_imports_QtQuickUltralite_Extras_ItemBuffer_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_0x2d_project_0x2e_org_imports_QtQuickUltralite_Extras_StaticText_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_0x2d_project_0x2e_org_imports_QtQuickUltralite_Extras_QulPerf_qml { 
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
    resourcePathToCachedUnit.insert(QStringLiteral("/qt-project.org/imports/QtQuickUltralite/Extras/AnimatedSpriteDirectory.qml"), &QmlCacheGeneratedCode::_qt_0x2d_project_0x2e_org_imports_QtQuickUltralite_Extras_AnimatedSpriteDirectory_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt-project.org/imports/QtQuickUltralite/Extras/ColorizedImage.qml"), &QmlCacheGeneratedCode::_qt_0x2d_project_0x2e_org_imports_QtQuickUltralite_Extras_ColorizedImage_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt-project.org/imports/QtQuickUltralite/Extras/ItemBuffer.qml"), &QmlCacheGeneratedCode::_qt_0x2d_project_0x2e_org_imports_QtQuickUltralite_Extras_ItemBuffer_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt-project.org/imports/QtQuickUltralite/Extras/StaticText.qml"), &QmlCacheGeneratedCode::_qt_0x2d_project_0x2e_org_imports_QtQuickUltralite_Extras_StaticText_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt-project.org/imports/QtQuickUltralite/Extras/QulPerf.qml"), &QmlCacheGeneratedCode::_qt_0x2d_project_0x2e_org_imports_QtQuickUltralite_Extras_QulPerf_qml::unit);
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
int QT_MANGLE_NAMESPACE(qInitResources_qmlcache_QuickUltraLiteStudioExtras)() {
    ::unitRegistry();
    return 1;
}
Q_CONSTRUCTOR_FUNCTION(QT_MANGLE_NAMESPACE(qInitResources_qmlcache_QuickUltraLiteStudioExtras))
int QT_MANGLE_NAMESPACE(qCleanupResources_qmlcache_QuickUltraLiteStudioExtras)() {
    return 1;
}
