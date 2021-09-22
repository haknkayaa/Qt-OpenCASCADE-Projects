/****************************************************************************
** Meta object code from reading C++ file 'VectorTrace.hh'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/GDML/VectorTrace.hh"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VectorTrace.hh' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VectorTracer_t {
    QByteArrayData data[16];
    char stringdata0[169];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VectorTracer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VectorTracer_t qt_meta_stringdata_VectorTracer = {
    {
QT_MOC_LITERAL(0, 0, 12), // "VectorTracer"
QT_MOC_LITERAL(1, 13, 17), // "produceImagePhase"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 7), // "message"
QT_MOC_LITERAL(4, 40, 8), // "nqueries"
QT_MOC_LITERAL(5, 49, 4), // "done"
QT_MOC_LITERAL(6, 54, 10), // "renderFull"
QT_MOC_LITERAL(7, 65, 10), // "renderStep"
QT_MOC_LITERAL(8, 76, 5), // "reset"
QT_MOC_LITERAL(9, 82, 9), // "grid_size"
QT_MOC_LITERAL(10, 92, 11), // "target_name"
QT_MOC_LITERAL(11, 104, 7), // "recolor"
QT_MOC_LITERAL(12, 112, 11), // "computeGrid"
QT_MOC_LITERAL(13, 124, 12), // "computeEdges"
QT_MOC_LITERAL(14, 137, 14), // "computeCreases"
QT_MOC_LITERAL(15, 152, 16) // "computeGradients"

    },
    "VectorTracer\0produceImagePhase\0\0message\0"
    "nqueries\0done\0renderFull\0renderStep\0"
    "reset\0grid_size\0target_name\0recolor\0"
    "computeGrid\0computeEdges\0computeCreases\0"
    "computeGradients"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VectorTracer[] = {

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
       1,    4,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   68,    2, 0x0a /* Public */,
       7,    0,   69,    2, 0x0a /* Public */,
       8,    2,   70,    2, 0x0a /* Public */,
      11,    0,   75,    2, 0x0a /* Public */,
      12,    0,   76,    2, 0x0a /* Public */,
      13,    0,   77,    2, 0x0a /* Public */,
      14,    0,   78,    2, 0x0a /* Public */,
      15,    0,   79,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QImage, QMetaType::QString, QMetaType::Int, QMetaType::Bool,    2,    3,    4,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QSize, QMetaType::QString,    9,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void VectorTracer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VectorTracer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->produceImagePhase((*reinterpret_cast< QImage(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 1: _t->renderFull(); break;
        case 2: _t->renderStep(); break;
        case 3: _t->reset((*reinterpret_cast< const QSize(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->recolor(); break;
        case 5: _t->computeGrid(); break;
        case 6: _t->computeEdges(); break;
        case 7: _t->computeCreases(); break;
        case 8: _t->computeGradients(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VectorTracer::*)(QImage , QString , int , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VectorTracer::produceImagePhase)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VectorTracer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_VectorTracer.data,
    qt_meta_data_VectorTracer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VectorTracer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VectorTracer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VectorTracer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int VectorTracer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void VectorTracer::produceImagePhase(QImage _t1, QString _t2, int _t3, bool _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
