/****************************************************************************
** Meta object code from reading C++ file 'sidebar.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../sidebar.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sidebar.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN7SidebarE_t {};
} // unnamed namespace

template <> constexpr inline auto Sidebar::qt_create_metaobjectdata<qt_meta_tag_ZN7SidebarE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Sidebar",
        "weatherConditionAdded",
        "",
        "WeatherCondition",
        "condition",
        "weatherConditionRemoved",
        "index",
        "dtedFilesAdded",
        "fileNames",
        "targetAdded",
        "Target",
        "target",
        "targetRemoved",
        "targetName",
        "initialPositionChanged",
        "lat",
        "lon",
        "alt",
        "targetTrajectoryChanged",
        "QList<std::pair<double,double>>",
        "latLon",
        "radarRouteChanged",
        "onTabChanged",
        "onAddWeatherCondition",
        "onDeleteWeatherCondition",
        "onBrowseDTEDFiles",
        "onRadarModeChanged",
        "onPrfCountChanged",
        "onPrfIndexChanged",
        "onPrfValueChanged",
        "value",
        "onWaveformTypeChanged",
        "onPfaValueChanged",
        "onFixedGainToggled",
        "checked",
        "onBeamPatternToggled",
        "onBrowseAzPattern",
        "onBrowseElPattern",
        "onShowRadarConfigs",
        "onTargetSelectionChanged",
        "onAddTarget",
        "onDeleteTarget",
        "onAppendWaypoint",
        "onDeleteWaypoint",
        "onShowWaypointsPage",
        "onBackToTargetsPage",
        "onInitialPositionChanged",
        "onShowRadarRoutePage",
        "onBackFromRadarRoutePage",
        "onAppendRadarWaypoint",
        "onDeleteRadarWaypoint"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'weatherConditionAdded'
        QtMocHelpers::SignalData<void(const WeatherCondition &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'weatherConditionRemoved'
        QtMocHelpers::SignalData<void(int)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Signal 'dtedFilesAdded'
        QtMocHelpers::SignalData<void(const QStringList &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QStringList, 8 },
        }}),
        // Signal 'targetAdded'
        QtMocHelpers::SignalData<void(const Target &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 10, 11 },
        }}),
        // Signal 'targetRemoved'
        QtMocHelpers::SignalData<void(const QString &)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 13 },
        }}),
        // Signal 'initialPositionChanged'
        QtMocHelpers::SignalData<void(double, double, double)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 15 }, { QMetaType::Double, 16 }, { QMetaType::Double, 17 },
        }}),
        // Signal 'targetTrajectoryChanged'
        QtMocHelpers::SignalData<void(const QString &, const QVector<QPair<double,double>> &)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 13 }, { 0x80000000 | 19, 20 },
        }}),
        // Signal 'radarRouteChanged'
        QtMocHelpers::SignalData<void(const QVector<QPair<double,double>> &)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 19, 20 },
        }}),
        // Slot 'onTabChanged'
        QtMocHelpers::SlotData<void(int)>(22, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Slot 'onAddWeatherCondition'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDeleteWeatherCondition'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBrowseDTEDFiles'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onRadarModeChanged'
        QtMocHelpers::SlotData<void(int)>(26, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Slot 'onPrfCountChanged'
        QtMocHelpers::SlotData<void(int)>(27, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Slot 'onPrfIndexChanged'
        QtMocHelpers::SlotData<void(int)>(28, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Slot 'onPrfValueChanged'
        QtMocHelpers::SlotData<void(double)>(29, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 30 },
        }}),
        // Slot 'onWaveformTypeChanged'
        QtMocHelpers::SlotData<void(int)>(31, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Slot 'onPfaValueChanged'
        QtMocHelpers::SlotData<void(double)>(32, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 30 },
        }}),
        // Slot 'onFixedGainToggled'
        QtMocHelpers::SlotData<void(bool)>(33, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 34 },
        }}),
        // Slot 'onBeamPatternToggled'
        QtMocHelpers::SlotData<void(bool)>(35, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 34 },
        }}),
        // Slot 'onBrowseAzPattern'
        QtMocHelpers::SlotData<void()>(36, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBrowseElPattern'
        QtMocHelpers::SlotData<void()>(37, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onShowRadarConfigs'
        QtMocHelpers::SlotData<void()>(38, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onTargetSelectionChanged'
        QtMocHelpers::SlotData<void()>(39, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onAddTarget'
        QtMocHelpers::SlotData<void()>(40, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDeleteTarget'
        QtMocHelpers::SlotData<void()>(41, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onAppendWaypoint'
        QtMocHelpers::SlotData<void()>(42, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDeleteWaypoint'
        QtMocHelpers::SlotData<void()>(43, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onShowWaypointsPage'
        QtMocHelpers::SlotData<void()>(44, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBackToTargetsPage'
        QtMocHelpers::SlotData<void()>(45, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onInitialPositionChanged'
        QtMocHelpers::SlotData<void()>(46, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onShowRadarRoutePage'
        QtMocHelpers::SlotData<void()>(47, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBackFromRadarRoutePage'
        QtMocHelpers::SlotData<void()>(48, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onAppendRadarWaypoint'
        QtMocHelpers::SlotData<void()>(49, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDeleteRadarWaypoint'
        QtMocHelpers::SlotData<void()>(50, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Sidebar, qt_meta_tag_ZN7SidebarE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Sidebar::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7SidebarE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7SidebarE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7SidebarE_t>.metaTypes,
    nullptr
} };

void Sidebar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Sidebar *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->weatherConditionAdded((*reinterpret_cast< std::add_pointer_t<WeatherCondition>>(_a[1]))); break;
        case 1: _t->weatherConditionRemoved((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->dtedFilesAdded((*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[1]))); break;
        case 3: _t->targetAdded((*reinterpret_cast< std::add_pointer_t<Target>>(_a[1]))); break;
        case 4: _t->targetRemoved((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->initialPositionChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[3]))); break;
        case 6: _t->targetTrajectoryChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QList<std::pair<double,double>>>>(_a[2]))); break;
        case 7: _t->radarRouteChanged((*reinterpret_cast< std::add_pointer_t<QList<std::pair<double,double>>>>(_a[1]))); break;
        case 8: _t->onTabChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 9: _t->onAddWeatherCondition(); break;
        case 10: _t->onDeleteWeatherCondition(); break;
        case 11: _t->onBrowseDTEDFiles(); break;
        case 12: _t->onRadarModeChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 13: _t->onPrfCountChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 14: _t->onPrfIndexChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 15: _t->onPrfValueChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 16: _t->onWaveformTypeChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 17: _t->onPfaValueChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 18: _t->onFixedGainToggled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 19: _t->onBeamPatternToggled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 20: _t->onBrowseAzPattern(); break;
        case 21: _t->onBrowseElPattern(); break;
        case 22: _t->onShowRadarConfigs(); break;
        case 23: _t->onTargetSelectionChanged(); break;
        case 24: _t->onAddTarget(); break;
        case 25: _t->onDeleteTarget(); break;
        case 26: _t->onAppendWaypoint(); break;
        case 27: _t->onDeleteWaypoint(); break;
        case 28: _t->onShowWaypointsPage(); break;
        case 29: _t->onBackToTargetsPage(); break;
        case 30: _t->onInitialPositionChanged(); break;
        case 31: _t->onShowRadarRoutePage(); break;
        case 32: _t->onBackFromRadarRoutePage(); break;
        case 33: _t->onAppendRadarWaypoint(); break;
        case 34: _t->onDeleteRadarWaypoint(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Sidebar::*)(const WeatherCondition & )>(_a, &Sidebar::weatherConditionAdded, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Sidebar::*)(int )>(_a, &Sidebar::weatherConditionRemoved, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Sidebar::*)(const QStringList & )>(_a, &Sidebar::dtedFilesAdded, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Sidebar::*)(const Target & )>(_a, &Sidebar::targetAdded, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (Sidebar::*)(const QString & )>(_a, &Sidebar::targetRemoved, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (Sidebar::*)(double , double , double )>(_a, &Sidebar::initialPositionChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (Sidebar::*)(const QString & , const QVector<QPair<double,double>> & )>(_a, &Sidebar::targetTrajectoryChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (Sidebar::*)(const QVector<QPair<double,double>> & )>(_a, &Sidebar::radarRouteChanged, 7))
            return;
    }
}

const QMetaObject *Sidebar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Sidebar::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7SidebarE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Sidebar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 35)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 35;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 35)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 35;
    }
    return _id;
}

// SIGNAL 0
void Sidebar::weatherConditionAdded(const WeatherCondition & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void Sidebar::weatherConditionRemoved(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void Sidebar::dtedFilesAdded(const QStringList & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void Sidebar::targetAdded(const Target & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void Sidebar::targetRemoved(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void Sidebar::initialPositionChanged(double _t1, double _t2, double _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1, _t2, _t3);
}

// SIGNAL 6
void Sidebar::targetTrajectoryChanged(const QString & _t1, const QVector<QPair<double,double>> & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1, _t2);
}

// SIGNAL 7
void Sidebar::radarRouteChanged(const QVector<QPair<double,double>> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1);
}
QT_WARNING_POP
