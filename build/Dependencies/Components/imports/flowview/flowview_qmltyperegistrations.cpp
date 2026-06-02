/****************************************************************************
** Generated QML type registration code
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtQml/qqml.h>
#include <QtQml/qqmlmoduleregistration.h>



#if !defined(QT_STATIC)
#define Q_QMLTYPE_EXPORT Q_DECL_EXPORT
#else
#define Q_QMLTYPE_EXPORT
#endif
Q_QMLTYPE_EXPORT void qml_register_types_FlowView()
{
    qmlRegisterModule("FlowView", 1, 0);
    qmlRegisterModule("FlowView", 1, 254);
    qmlRegisterModule("FlowView", 254, 0);
    QT_WARNING_PUSH QT_WARNING_DISABLE_DEPRECATED
    QT_WARNING_POP
    qmlRegisterModule("FlowView", 254, 254);
}

static const QQmlModuleRegistration flowViewRegistration("FlowView", qml_register_types_FlowView);
