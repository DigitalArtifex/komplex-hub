/****************************************************************************
** Generated QML type registration code
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtQml/qqml.h>
#include <QtQml/qqmlmoduleregistration.h>

#if __has_include(<quickstudiocsvtablemodel.h>)
#  include <quickstudiocsvtablemodel.h>
#endif
#if __has_include(<quickstudiofilereader.h>)
#  include <quickstudiofilereader.h>
#endif


#if !defined(QT_STATIC)
#define Q_QMLTYPE_EXPORT Q_DECL_EXPORT
#else
#define Q_QMLTYPE_EXPORT
#endif
Q_QMLTYPE_EXPORT void qml_register_types_QtQuick_Studio_Utils()
{
    qmlRegisterModule("QtQuick.Studio.Utils", 1, 0);
    qmlRegisterModule("QtQuick.Studio.Utils", 1, 254);
    qmlRegisterModule("QtQuick.Studio.Utils", 254, 0);
    QT_WARNING_PUSH QT_WARNING_DISABLE_DEPRECATED
    QMetaType::fromType<QAbstractItemModel *>().id();
    qmlRegisterEnum<QAbstractItemModel::LayoutChangeHint>("QAbstractItemModel::LayoutChangeHint");
    qmlRegisterEnum<QAbstractItemModel::CheckIndexOption>("QAbstractItemModel::CheckIndexOption");
    QMetaType::fromType<QAbstractTableModel *>().id();
    qmlRegisterTypesAndRevisions<QuickStudioCsvTableModel>("QtQuick.Studio.Utils", 254);
    qmlRegisterTypesAndRevisions<QuickStudioFileReader>("QtQuick.Studio.Utils", 254);
    QT_WARNING_POP
    qmlRegisterModule("QtQuick.Studio.Utils", 254, 254);
}

static const QQmlModuleRegistration qtQuickStudioUtilsRegistration("QtQuick.Studio.Utils", qml_register_types_QtQuick_Studio_Utils);
