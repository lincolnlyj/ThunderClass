/****************************************************************************
** Meta object code from reading C++ file 'StudentWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../StudentWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'StudentWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_StudentWindow_t {
    QByteArrayData data[24];
    char stringdata0[273];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_StudentWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_StudentWindow_t qt_meta_stringdata_StudentWindow = {
    {
QT_MOC_LITERAL(0, 0, 13), // "StudentWindow"
QT_MOC_LITERAL(1, 14, 24), // "on_btnEnterClass_clicked"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 23), // "on_btnExitClass_clicked"
QT_MOC_LITERAL(4, 64, 18), // "on_btnSend_clicked"
QT_MOC_LITERAL(5, 83, 10), // "TimerEvent"
QT_MOC_LITERAL(6, 94, 12), // "ClassEntered"
QT_MOC_LITERAL(7, 107, 13), // "FailToConnect"
QT_MOC_LITERAL(8, 121, 11), // "ClassExited"
QT_MOC_LITERAL(9, 133, 8), // "RecvChat"
QT_MOC_LITERAL(10, 142, 3), // "Msg"
QT_MOC_LITERAL(11, 146, 10), // "RecvScreen"
QT_MOC_LITERAL(12, 157, 3), // "Img"
QT_MOC_LITERAL(13, 161, 11), // "eventFilter"
QT_MOC_LITERAL(14, 173, 6), // "Target"
QT_MOC_LITERAL(15, 180, 7), // "QEvent*"
QT_MOC_LITERAL(16, 188, 2), // "Ev"
QT_MOC_LITERAL(17, 191, 12), // "HideQuestion"
QT_MOC_LITERAL(18, 204, 11), // "StartAnswer"
QT_MOC_LITERAL(19, 216, 4), // "Ques"
QT_MOC_LITERAL(20, 221, 14), // "ShowAnswerTime"
QT_MOC_LITERAL(21, 236, 10), // "SendAnswer"
QT_MOC_LITERAL(22, 247, 10), // "ShareVoice"
QT_MOC_LITERAL(23, 258, 14) // "StopVoiceShare"

    },
    "StudentWindow\0on_btnEnterClass_clicked\0"
    "\0on_btnExitClass_clicked\0on_btnSend_clicked\0"
    "TimerEvent\0ClassEntered\0FailToConnect\0"
    "ClassExited\0RecvChat\0Msg\0RecvScreen\0"
    "Img\0eventFilter\0Target\0QEvent*\0Ev\0"
    "HideQuestion\0StartAnswer\0Ques\0"
    "ShowAnswerTime\0SendAnswer\0ShareVoice\0"
    "StopVoiceShare"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_StudentWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x08 /* Private */,
       3,    0,   95,    2, 0x08 /* Private */,
       4,    0,   96,    2, 0x08 /* Private */,
       5,    0,   97,    2, 0x08 /* Private */,
       6,    0,   98,    2, 0x08 /* Private */,
       7,    0,   99,    2, 0x08 /* Private */,
       8,    0,  100,    2, 0x08 /* Private */,
       9,    1,  101,    2, 0x08 /* Private */,
      11,    1,  104,    2, 0x08 /* Private */,
      13,    2,  107,    2, 0x08 /* Private */,
      17,    0,  112,    2, 0x08 /* Private */,
      18,    1,  113,    2, 0x08 /* Private */,
      20,    0,  116,    2, 0x08 /* Private */,
      21,    0,  117,    2, 0x08 /* Private */,
      22,    0,  118,    2, 0x08 /* Private */,
      23,    0,  119,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QImage,   12,
    QMetaType::Bool, QMetaType::QObjectStar, 0x80000000 | 15,   14,   16,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   19,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void StudentWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<StudentWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_btnEnterClass_clicked(); break;
        case 1: _t->on_btnExitClass_clicked(); break;
        case 2: _t->on_btnSend_clicked(); break;
        case 3: _t->TimerEvent(); break;
        case 4: _t->ClassEntered(); break;
        case 5: _t->FailToConnect(); break;
        case 6: _t->ClassExited(); break;
        case 7: _t->RecvChat((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->RecvScreen((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 9: { bool _r = _t->eventFilter((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< QEvent*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 10: _t->HideQuestion(); break;
        case 11: _t->StartAnswer((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->ShowAnswerTime(); break;
        case 13: _t->SendAnswer(); break;
        case 14: _t->ShareVoice(); break;
        case 15: _t->StopVoiceShare(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject StudentWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_StudentWindow.data,
    qt_meta_data_StudentWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *StudentWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *StudentWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_StudentWindow.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int StudentWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
