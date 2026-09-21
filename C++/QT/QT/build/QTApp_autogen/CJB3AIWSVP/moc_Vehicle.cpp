/****************************************************************************
** Meta object code from reading C++ file 'Vehicle.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../includes/Vehicle.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Vehicle.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_Vehicle_t {
    uint offsetsAndSizes[22];
    char stringdata0[8];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[20];
    char stringdata4[19];
    char stringdata5[14];
    char stringdata6[14];
    char stringdata7[13];
    char stringdata8[6];
    char stringdata9[13];
    char stringdata10[12];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_Vehicle_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_Vehicle_t qt_meta_stringdata_Vehicle = {
    {
        QT_MOC_LITERAL(0, 7),  // "Vehicle"
        QT_MOC_LITERAL(8, 12),  // "speedChanged"
        QT_MOC_LITERAL(21, 0),  // ""
        QT_MOC_LITERAL(22, 19),  // "batteryLevelChanged"
        QT_MOC_LITERAL(42, 18),  // "temperatureChanged"
        QT_MOC_LITERAL(61, 13),  // "increaseSpeed"
        QT_MOC_LITERAL(75, 13),  // "decreaseSpeed"
        QT_MOC_LITERAL(89, 12),  // "openSettings"
        QT_MOC_LITERAL(102, 5),  // "speed"
        QT_MOC_LITERAL(108, 12),  // "batteryLevel"
        QT_MOC_LITERAL(121, 11)   // "temperature"
    },
    "Vehicle",
    "speedChanged",
    "",
    "batteryLevelChanged",
    "temperatureChanged",
    "increaseSpeed",
    "decreaseSpeed",
    "openSettings",
    "speed",
    "batteryLevel",
    "temperature"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_Vehicle[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       3,   56, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   50,    2, 0x06,    4 /* Public */,
       3,    0,   51,    2, 0x06,    5 /* Public */,
       4,    0,   52,    2, 0x06,    6 /* Public */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,   53,    2, 0x02,    7 /* Public */,
       6,    0,   54,    2, 0x02,    8 /* Public */,
       7,    0,   55,    2, 0x02,    9 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       8, QMetaType::Int, 0x00015103, uint(0), 0,
       9, QMetaType::Int, 0x00015103, uint(1), 0,
      10, QMetaType::Int, 0x00015103, uint(2), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject Vehicle::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_Vehicle.offsetsAndSizes,
    qt_meta_data_Vehicle,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_Vehicle_t,
        // property 'speed'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'batteryLevel'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'temperature'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Vehicle, std::true_type>,
        // method 'speedChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'batteryLevelChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'temperatureChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'increaseSpeed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'decreaseSpeed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'openSettings'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void Vehicle::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Vehicle *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->speedChanged(); break;
        case 1: _t->batteryLevelChanged(); break;
        case 2: _t->temperatureChanged(); break;
        case 3: _t->increaseSpeed(); break;
        case 4: _t->decreaseSpeed(); break;
        case 5: _t->openSettings(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Vehicle::*)();
            if (_t _q_method = &Vehicle::speedChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Vehicle::*)();
            if (_t _q_method = &Vehicle::batteryLevelChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Vehicle::*)();
            if (_t _q_method = &Vehicle::temperatureChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<Vehicle *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->speed(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->batteryLevel(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->temperature(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<Vehicle *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setSpeed(*reinterpret_cast< int*>(_v)); break;
        case 1: _t->setBatteryLevel(*reinterpret_cast< int*>(_v)); break;
        case 2: _t->setTemperature(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
    (void)_a;
}

const QMetaObject *Vehicle::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Vehicle::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Vehicle.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Vehicle::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void Vehicle::speedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Vehicle::batteryLevelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Vehicle::temperatureChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
