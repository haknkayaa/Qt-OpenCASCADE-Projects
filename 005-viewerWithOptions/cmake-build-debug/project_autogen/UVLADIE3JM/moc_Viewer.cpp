/****************************************************************************
** Meta object code from reading C++ file 'Viewer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/Viewer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Viewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Viewer_t {
    QByteArrayData data[12];
    char stringdata0[193];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Viewer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Viewer_t qt_meta_stringdata_Viewer = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Viewer"
QT_MOC_LITERAL(1, 7, 15), // "mousePosChanged"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 6), // "fitAll"
QT_MOC_LITERAL(4, 31, 25), // "slot_changeProjectionMode"
QT_MOC_LITERAL(5, 57, 22), // "slot_showTrihedronCube"
QT_MOC_LITERAL(6, 80, 25), // "slot_showPerformanceStats"
QT_MOC_LITERAL(7, 106, 15), // "slot_show3DGrid"
QT_MOC_LITERAL(8, 122, 25), // "slot_changeProjectionAxis"
QT_MOC_LITERAL(9, 148, 14), // "slot_showShape"
QT_MOC_LITERAL(10, 163, 16), // "Handle_AIS_Shape"
QT_MOC_LITERAL(11, 180, 12) // "slot_explode"

    },
    "Viewer\0mousePosChanged\0\0fitAll\0"
    "slot_changeProjectionMode\0"
    "slot_showTrihedronCube\0slot_showPerformanceStats\0"
    "slot_show3DGrid\0slot_changeProjectionAxis\0"
    "slot_showShape\0Handle_AIS_Shape\0"
    "slot_explode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Viewer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   62,    2, 0x0a /* Public */,
       4,    1,   63,    2, 0x0a /* Public */,
       5,    1,   66,    2, 0x0a /* Public */,
       6,    1,   69,    2, 0x0a /* Public */,
       7,    1,   72,    2, 0x0a /* Public */,
       8,    1,   75,    2, 0x0a /* Public */,
       9,    1,   78,    2, 0x0a /* Public */,
      11,    2,   81,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QPoint,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, 0x80000000 | 10,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,

       0        // eod
};

void Viewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Viewer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->mousePosChanged((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 1: _t->fitAll(); break;
        case 2: _t->slot_changeProjectionMode((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->slot_showTrihedronCube((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->slot_showPerformanceStats((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->slot_show3DGrid((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->slot_changeProjectionAxis((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->slot_showShape((*reinterpret_cast< Handle_AIS_Shape(*)>(_a[1]))); break;
        case 8: _t->slot_explode((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Viewer::*)(QPoint );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Viewer::mousePosChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Viewer::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_Viewer.data,
    qt_meta_data_Viewer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Viewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Viewer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Viewer.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Viewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void Viewer::mousePosChanged(QPoint _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
