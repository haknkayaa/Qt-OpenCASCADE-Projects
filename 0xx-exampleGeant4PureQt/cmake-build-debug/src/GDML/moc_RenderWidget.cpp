/****************************************************************************
** Meta object code from reading C++ file 'RenderWidget.hh'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/GDML/RenderWidget.hh"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RenderWidget.hh' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RenderWidget_t {
    QByteArrayData data[20];
    char stringdata0[219];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RenderWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RenderWidget_t qt_meta_stringdata_RenderWidget = {
    {
QT_MOC_LITERAL(0, 0, 12), // "RenderWidget"
QT_MOC_LITERAL(1, 13, 9), // "frameTime"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 10), // "forwardKey"
QT_MOC_LITERAL(4, 35, 10), // "QKeyEvent*"
QT_MOC_LITERAL(5, 46, 1), // "e"
QT_MOC_LITERAL(6, 48, 12), // "forwardMouse"
QT_MOC_LITERAL(7, 61, 12), // "QMouseEvent*"
QT_MOC_LITERAL(8, 74, 12), // "forwardWheel"
QT_MOC_LITERAL(9, 87, 12), // "QWheelEvent*"
QT_MOC_LITERAL(10, 100, 18), // "forwardContextMenu"
QT_MOC_LITERAL(11, 119, 18), // "QContextMenuEvent*"
QT_MOC_LITERAL(12, 138, 13), // "forwardResize"
QT_MOC_LITERAL(13, 152, 13), // "QResizeEvent*"
QT_MOC_LITERAL(14, 166, 3), // "evt"
QT_MOC_LITERAL(15, 170, 13), // "setFullDetail"
QT_MOC_LITERAL(16, 184, 9), // "completed"
QT_MOC_LITERAL(17, 194, 7), // "aborted"
QT_MOC_LITERAL(18, 202, 8), // "rerender"
QT_MOC_LITERAL(19, 211, 7) // "changes"

    },
    "RenderWidget\0frameTime\0\0forwardKey\0"
    "QKeyEvent*\0e\0forwardMouse\0QMouseEvent*\0"
    "forwardWheel\0QWheelEvent*\0forwardContextMenu\0"
    "QContextMenuEvent*\0forwardResize\0"
    "QResizeEvent*\0evt\0setFullDetail\0"
    "completed\0aborted\0rerender\0changes"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RenderWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       3,    1,   67,    2, 0x06 /* Public */,
       6,    1,   70,    2, 0x06 /* Public */,
       8,    1,   73,    2, 0x06 /* Public */,
      10,    1,   76,    2, 0x06 /* Public */,
      12,    1,   79,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      15,    1,   82,    2, 0x0a /* Public */,
      16,    1,   85,    2, 0x0a /* Public */,
      17,    0,   88,    2, 0x0a /* Public */,
      18,    1,   89,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QReal,    2,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 7,    2,
    QMetaType::Void, 0x80000000 | 9,    2,
    QMetaType::Void, 0x80000000 | 11,    2,
    QMetaType::Void, 0x80000000 | 13,   14,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::QReal,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   19,

       0        // eod
};

void RenderWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RenderWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->frameTime((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 1: _t->forwardKey((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
        case 2: _t->forwardMouse((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 3: _t->forwardWheel((*reinterpret_cast< QWheelEvent*(*)>(_a[1]))); break;
        case 4: _t->forwardContextMenu((*reinterpret_cast< QContextMenuEvent*(*)>(_a[1]))); break;
        case 5: _t->forwardResize((*reinterpret_cast< QResizeEvent*(*)>(_a[1]))); break;
        case 6: _t->setFullDetail((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->completed((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 8: _t->aborted(); break;
        case 9: _t->rerender((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RenderWidget::*)(qreal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RenderWidget::frameTime)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (RenderWidget::*)(QKeyEvent * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RenderWidget::forwardKey)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (RenderWidget::*)(QMouseEvent * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RenderWidget::forwardMouse)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (RenderWidget::*)(QWheelEvent * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RenderWidget::forwardWheel)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (RenderWidget::*)(QContextMenuEvent * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RenderWidget::forwardContextMenu)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (RenderWidget::*)(QResizeEvent * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RenderWidget::forwardResize)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject RenderWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_RenderWidget.data,
    qt_meta_data_RenderWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RenderWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RenderWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RenderWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int RenderWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void RenderWidget::frameTime(qreal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RenderWidget::forwardKey(QKeyEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void RenderWidget::forwardMouse(QMouseEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void RenderWidget::forwardWheel(QWheelEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void RenderWidget::forwardContextMenu(QContextMenuEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void RenderWidget::forwardResize(QResizeEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
struct qt_meta_stringdata_RenderSaveObject_t {
    QByteArrayData data[5];
    char stringdata0[42];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RenderSaveObject_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RenderSaveObject_t qt_meta_stringdata_RenderSaveObject = {
    {
QT_MOC_LITERAL(0, 0, 16), // "RenderSaveObject"
QT_MOC_LITERAL(1, 17, 5), // "abort"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 7), // "aborted"
QT_MOC_LITERAL(4, 32, 9) // "completed"

    },
    "RenderSaveObject\0abort\0\0aborted\0"
    "completed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RenderSaveObject[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x0a /* Public */,
       3,    0,   30,    2, 0x0a /* Public */,
       4,    0,   31,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void RenderSaveObject::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RenderSaveObject *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->abort(); break;
        case 1: _t->aborted(); break;
        case 2: _t->completed(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject RenderSaveObject::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_RenderSaveObject.data,
    qt_meta_data_RenderSaveObject,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RenderSaveObject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RenderSaveObject::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RenderSaveObject.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int RenderSaveObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
