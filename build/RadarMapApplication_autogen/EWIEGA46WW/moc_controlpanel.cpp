/****************************************************************************
** Meta object code from reading C++ file 'controlpanel.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../controlpanel.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'controlpanel.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN12ControlPanelE_t {};
} // unnamed namespace

template <> constexpr inline auto ControlPanel::qt_create_metaobjectdata<qt_meta_tag_ZN12ControlPanelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ControlPanel",
        "startClicked",
        "",
        "stopClicked",
        "showWeatherConditionsChanged",
        "checked",
        "showDTEDAreasChanged",
        "showTargetsTrajChanged",
        "targetPositionUpdated",
        "targetName",
        "lat",
        "lon",
        "alt",
        "radarPositionUpdated",
        "namedRadarPositionUpdated",
        "radarName",
        "onStartClicked",
        "onStopClicked",
        "onShowDTEDAreasChanged",
        "onShowWeatherConditionsChanged",
        "onShowTargetsTrajChanged",
        "updateElapsedTime",
        "updateTargetPositions"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'startClicked'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'stopClicked'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'showWeatherConditionsChanged'
        QtMocHelpers::SignalData<void(bool)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 5 },
        }}),
        // Signal 'showDTEDAreasChanged'
        QtMocHelpers::SignalData<void(bool)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 5 },
        }}),
        // Signal 'showTargetsTrajChanged'
        QtMocHelpers::SignalData<void(bool)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 5 },
        }}),
        // Signal 'targetPositionUpdated'
        QtMocHelpers::SignalData<void(const QString &, double, double, double)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 }, { QMetaType::Double, 10 }, { QMetaType::Double, 11 }, { QMetaType::Double, 12 },
        }}),
        // Signal 'radarPositionUpdated'
        QtMocHelpers::SignalData<void(double, double, double)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 10 }, { QMetaType::Double, 11 }, { QMetaType::Double, 12 },
        }}),
        // Signal 'namedRadarPositionUpdated'
        QtMocHelpers::SignalData<void(const QString &, double, double, double)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 15 }, { QMetaType::Double, 10 }, { QMetaType::Double, 11 }, { QMetaType::Double, 12 },
        }}),
        // Slot 'onStartClicked'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onStopClicked'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onShowDTEDAreasChanged'
        QtMocHelpers::SlotData<void(bool)>(18, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 5 },
        }}),
        // Slot 'onShowWeatherConditionsChanged'
        QtMocHelpers::SlotData<void(bool)>(19, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 5 },
        }}),
        // Slot 'onShowTargetsTrajChanged'
        QtMocHelpers::SlotData<void(bool)>(20, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 5 },
        }}),
        // Slot 'updateElapsedTime'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'updateTargetPositions'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ControlPanel, qt_meta_tag_ZN12ControlPanelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ControlPanel::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ControlPanelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ControlPanelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12ControlPanelE_t>.metaTypes,
    nullptr
} };

void ControlPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ControlPanel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->startClicked(); break;
        case 1: _t->stopClicked(); break;
        case 2: _t->showWeatherConditionsChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->showDTEDAreasChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 4: _t->showTargetsTrajChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 5: _t->targetPositionUpdated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[4]))); break;
        case 6: _t->radarPositionUpdated((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[3]))); break;
        case 7: _t->namedRadarPositionUpdated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[4]))); break;
        case 8: _t->onStartClicked(); break;
        case 9: _t->onStopClicked(); break;
        case 10: _t->onShowDTEDAreasChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 11: _t->onShowWeatherConditionsChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 12: _t->onShowTargetsTrajChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 13: _t->updateElapsedTime(); break;
        case 14: _t->updateTargetPositions(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ControlPanel::*)()>(_a, &ControlPanel::startClicked, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ControlPanel::*)()>(_a, &ControlPanel::stopClicked, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ControlPanel::*)(bool )>(_a, &ControlPanel::showWeatherConditionsChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (ControlPanel::*)(bool )>(_a, &ControlPanel::showDTEDAreasChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (ControlPanel::*)(bool )>(_a, &ControlPanel::showTargetsTrajChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (ControlPanel::*)(const QString & , double , double , double )>(_a, &ControlPanel::targetPositionUpdated, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (ControlPanel::*)(double , double , double )>(_a, &ControlPanel::radarPositionUpdated, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (ControlPanel::*)(const QString & , double , double , double )>(_a, &ControlPanel::namedRadarPositionUpdated, 7))
            return;
    }
}

const QMetaObject *ControlPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ControlPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ControlPanelE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ControlPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void ControlPanel::startClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ControlPanel::stopClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ControlPanel::showWeatherConditionsChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void ControlPanel::showDTEDAreasChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void ControlPanel::showTargetsTrajChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void ControlPanel::targetPositionUpdated(const QString & _t1, double _t2, double _t3, double _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1, _t2, _t3, _t4);
}

// SIGNAL 6
void ControlPanel::radarPositionUpdated(double _t1, double _t2, double _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1, _t2, _t3);
}

// SIGNAL 7
void ControlPanel::namedRadarPositionUpdated(const QString & _t1, double _t2, double _t3, double _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1, _t2, _t3, _t4);
}
QT_WARNING_POP
