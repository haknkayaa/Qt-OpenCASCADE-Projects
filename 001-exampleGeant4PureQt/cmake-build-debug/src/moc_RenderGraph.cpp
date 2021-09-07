/****************************************************************************
** Meta object code from reading C++ file 'RenderGraph.hh'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/GDML/RenderGraph.hh"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QSharedPointer>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RenderGraph.hh' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RenderGraph_t {
    QByteArrayData data[17];
    char stringdata0[148];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RenderGraph_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RenderGraph_t qt_meta_stringdata_RenderGraph = {
    {
QT_MOC_LITERAL(0, 0, 11), // "RenderGraph"
QT_MOC_LITERAL(1, 12, 4), // "done"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 12), // "elapsed_secs"
QT_MOC_LITERAL(4, 31, 7), // "aborted"
QT_MOC_LITERAL(5, 39, 10), // "progressed"
QT_MOC_LITERAL(6, 50, 5), // "start"
QT_MOC_LITERAL(7, 56, 22), // "QSharedPointer<QImage>"
QT_MOC_LITERAL(8, 79, 1), // "i"
QT_MOC_LITERAL(9, 81, 8), // "GridSpec"
QT_MOC_LITERAL(10, 90, 4), // "grid"
QT_MOC_LITERAL(11, 95, 8), // "ViewData"
QT_MOC_LITERAL(12, 104, 2), // "vd"
QT_MOC_LITERAL(13, 107, 7), // "changed"
QT_MOC_LITERAL(14, 115, 5), // "abort"
QT_MOC_LITERAL(15, 121, 9), // "queueNext"
QT_MOC_LITERAL(16, 131, 16) // "RenderGraphNode*"

    },
    "RenderGraph\0done\0\0elapsed_secs\0aborted\0"
    "progressed\0start\0QSharedPointer<QImage>\0"
    "i\0GridSpec\0grid\0ViewData\0vd\0changed\0"
    "abort\0queueNext\0RenderGraphNode*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RenderGraph[] = {

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
       4,    0,   47,    2, 0x06 /* Public */,
       5,    1,   48,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    4,   51,    2, 0x0a /* Public */,
      14,    0,   60,    2, 0x0a /* Public */,
      15,    1,   61,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::QReal,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 7, 0x80000000 | 9, 0x80000000 | 11, QMetaType::Int,    8,   10,   12,   13,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 16,    2,

       0        // eod
};

void RenderGraph::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RenderGraph *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->done((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 1: _t->aborted(); break;
        case 2: _t->progressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->start((*reinterpret_cast< QSharedPointer<QImage>(*)>(_a[1])),(*reinterpret_cast< const GridSpec(*)>(_a[2])),(*reinterpret_cast< const ViewData(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 4: _t->abort(); break;
        case 5: _t->queueNext((*reinterpret_cast< RenderGraphNode*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RenderGraph::*)(qreal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RenderGraph::done)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (RenderGraph::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RenderGraph::aborted)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (RenderGraph::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RenderGraph::progressed)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject RenderGraph::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_RenderGraph.data,
    qt_meta_data_RenderGraph,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RenderGraph::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RenderGraph::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RenderGraph.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int RenderGraph::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void RenderGraph::done(qreal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RenderGraph::aborted()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void RenderGraph::progressed(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
