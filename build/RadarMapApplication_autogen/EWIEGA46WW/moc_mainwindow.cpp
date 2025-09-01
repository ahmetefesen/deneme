/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN10MainWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto MainWindow::qt_create_metaobjectdata<qt_meta_tag_ZN10MainWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MainWindow",
        "newFile",
        "",
        "openFile",
        "closeFile",
        "saveFile",
        "saveAsFile",
        "showSettings",
        "showDocumentation",
        "showAbout",
        "startSimulation",
        "stopSimulation",
        "addWeatherCondition",
        "WeatherCondition",
        "condition",
        "removeWeatherCondition",
        "index",
        "addDTEDFiles",
        "fileNames",
        "addTargetToSimulation",
        "Target",
        "target",
        "removeTargetFromSimulation",
        "targetName",
        "updateTargetPositionOnMap",
        "lat",
        "lon",
        "alt",
        "updateInitialPositionOnMap",
        "onTargetTrajectoryChanged",
        "QList<std::pair<double,double>>",
        "latLon",
        "onShowTargetsTraj",
        "enabled",
        "onRadarRouteChanged"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'newFile'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'openFile'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'closeFile'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'saveFile'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'saveAsFile'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'showSettings'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'showDocumentation'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'showAbout'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'startSimulation'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'stopSimulation'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'addWeatherCondition'
        QtMocHelpers::SlotData<void(const WeatherCondition &)>(12, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 13, 14 },
        }}),
        // Slot 'removeWeatherCondition'
        QtMocHelpers::SlotData<void(int)>(15, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 16 },
        }}),
        // Slot 'addDTEDFiles'
        QtMocHelpers::SlotData<void(const QStringList &)>(17, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QStringList, 18 },
        }}),
        // Slot 'addTargetToSimulation'
        QtMocHelpers::SlotData<void(const Target &)>(19, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 20, 21 },
        }}),
        // Slot 'removeTargetFromSimulation'
        QtMocHelpers::SlotData<void(const QString &)>(22, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 23 },
        }}),
        // Slot 'updateTargetPositionOnMap'
        QtMocHelpers::SlotData<void(const QString &, double, double, double)>(24, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 23 }, { QMetaType::Double, 25 }, { QMetaType::Double, 26 }, { QMetaType::Double, 27 },
        }}),
        // Slot 'updateInitialPositionOnMap'
        QtMocHelpers::SlotData<void(double, double, double)>(28, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 25 }, { QMetaType::Double, 26 }, { QMetaType::Double, 27 },
        }}),
        // Slot 'onTargetTrajectoryChanged'
        QtMocHelpers::SlotData<void(const QString &, const QVector<QPair<double,double>> &)>(29, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 23 }, { 0x80000000 | 30, 31 },
        }}),
        // Slot 'onShowTargetsTraj'
        QtMocHelpers::SlotData<void(bool)>(32, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 33 },
        }}),
        // Slot 'onRadarRouteChanged'
        QtMocHelpers::SlotData<void(const QVector<QPair<double,double>> &)>(34, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 30, 31 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MainWindow, qt_meta_tag_ZN10MainWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10MainWindowE_t>.metaTypes,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->newFile(); break;
        case 1: _t->openFile(); break;
        case 2: _t->closeFile(); break;
        case 3: _t->saveFile(); break;
        case 4: _t->saveAsFile(); break;
        case 5: _t->showSettings(); break;
        case 6: _t->showDocumentation(); break;
        case 7: _t->showAbout(); break;
        case 8: _t->startSimulation(); break;
        case 9: _t->stopSimulation(); break;
        case 10: _t->addWeatherCondition((*reinterpret_cast< std::add_pointer_t<WeatherCondition>>(_a[1]))); break;
        case 11: _t->removeWeatherCondition((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 12: _t->addDTEDFiles((*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[1]))); break;
        case 13: _t->addTargetToSimulation((*reinterpret_cast< std::add_pointer_t<Target>>(_a[1]))); break;
        case 14: _t->removeTargetFromSimulation((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 15: _t->updateTargetPositionOnMap((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[4]))); break;
        case 16: _t->updateInitialPositionOnMap((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[3]))); break;
        case 17: _t->onTargetTrajectoryChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QList<std::pair<double,double>>>>(_a[2]))); break;
        case 18: _t->onShowTargetsTraj((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 19: _t->onRadarRouteChanged((*reinterpret_cast< std::add_pointer_t<QList<std::pair<double,double>>>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 20;
    }
    return _id;
}
QT_WARNING_POP
