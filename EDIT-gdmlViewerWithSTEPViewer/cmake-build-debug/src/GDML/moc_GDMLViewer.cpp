/****************************************************************************
** Meta object code from reading C++ file 'GDMLViewer.hh'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/GDML/GDMLViewer.hh"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GDMLViewer.hh' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GDMLViewer_t {
    QByteArrayData data[10];
    char stringdata0[104];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GDMLViewer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GDMLViewer_t qt_meta_stringdata_GDMLViewer = {
    {
QT_MOC_LITERAL(0, 0, 10), // "GDMLViewer"
QT_MOC_LITERAL(1, 11, 10), // "processKey"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 10), // "QKeyEvent*"
QT_MOC_LITERAL(4, 34, 1), // "e"
QT_MOC_LITERAL(5, 36, 12), // "processMouse"
QT_MOC_LITERAL(6, 49, 12), // "QMouseEvent*"
QT_MOC_LITERAL(7, 62, 12), // "processWheel"
QT_MOC_LITERAL(8, 75, 12), // "QWheelEvent*"
QT_MOC_LITERAL(9, 88, 15) // "setViewRotation"

    },
    "GDMLViewer\0processKey\0\0QKeyEvent*\0e\0"
    "processMouse\0QMouseEvent*\0processWheel\0"
    "QWheelEvent*\0setViewRotation"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GDMLViewer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x0a /* Public */,
       5,    1,   37,    2, 0x0a /* Public */,
       7,    1,   40,    2, 0x0a /* Public */,
       9,    1,   43,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void, 0x80000000 | 8,    2,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void GDMLViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GDMLViewer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->processKey((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
        case 1: _t->processMouse((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 2: _t->processWheel((*reinterpret_cast< QWheelEvent*(*)>(_a[1]))); break;
        case 3: _t->setViewRotation((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GDMLViewer::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_GDMLViewer.data,
    qt_meta_data_GDMLViewer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GDMLViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GDMLViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GDMLViewer.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int GDMLViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
