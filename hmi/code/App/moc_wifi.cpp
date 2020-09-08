/****************************************************************************
** Meta object code from reading C++ file 'wifi.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "WIFI/wifi.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wifi.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WIFI_t {
    QByteArrayData data[9];
    char stringdata0[110];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WIFI_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WIFI_t qt_meta_stringdata_WIFI = {
    {
QT_MOC_LITERAL(0, 0, 4), // "WIFI"
QT_MOC_LITERAL(1, 5, 12), // "sig_scanOver"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 15), // "sig_connectOver"
QT_MOC_LITERAL(4, 35, 17), // "sig_connectStatus"
QT_MOC_LITERAL(5, 53, 11), // "std::string"
QT_MOC_LITERAL(6, 65, 13), // "slot_scanWIFI"
QT_MOC_LITERAL(7, 79, 13), // "slot_scanOver"
QT_MOC_LITERAL(8, 93, 16) // "slot_wifiMonitor"

    },
    "WIFI\0sig_scanOver\0\0sig_connectOver\0"
    "sig_connectStatus\0std::string\0"
    "slot_scanWIFI\0slot_scanOver\0"
    "slot_wifiMonitor"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WIFI[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       3,    1,   47,    2, 0x06 /* Public */,
       4,    1,   50,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   53,    2, 0x0a /* Public */,
       7,    0,   54,    2, 0x0a /* Public */,
       8,    0,   55,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, 0x80000000 | 5,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void WIFI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WIFI *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sig_scanOver((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->sig_connectOver((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->sig_connectStatus((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        case 3: _t->slot_scanWIFI(); break;
        case 4: _t->slot_scanOver(); break;
        case 5: _t->slot_wifiMonitor(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (WIFI::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WIFI::sig_scanOver)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (WIFI::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WIFI::sig_connectOver)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (WIFI::*)(std::string );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WIFI::sig_connectStatus)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject WIFI::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_WIFI.data,
    qt_meta_data_WIFI,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *WIFI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WIFI::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WIFI.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int WIFI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void WIFI::sig_scanOver(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void WIFI::sig_connectOver(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void WIFI::sig_connectStatus(std::string _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
