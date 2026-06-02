/****************************************************************************
** Generated QML type registration code
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtQml/qqml.h>
#include <QtQml/qqmlmoduleregistration.h>

#if __has_include(<quickstudioapplication.h>)
#  include <quickstudioapplication.h>
#endif


#if !defined(QT_STATIC)
#define Q_QMLTYPE_EXPORT Q_DECL_EXPORT
#else
#define Q_QMLTYPE_EXPORT
#endif
Q_QMLTYPE_EXPORT void qml_register_types_QtQuick_Studio_Application()
{
    qmlRegisterModule("QtQuick.Studio.Application", 254, 0);
    QT_WARNING_PUSH QT_WARNING_DISABLE_DEPRECATED
    qmlRegisterTypesAndRevisions<QuickStudioApplication>("QtQuick.Studio.Application", 254);
    QT_WARNING_POP
    qmlRegisterModule("QtQuick.Studio.Application", 254, 254);
}

static const QQmlModuleRegistration qtQuickStudioApplicationRegistration("QtQuick.Studio.Application", qml_register_types_QtQuick_Studio_Application);
