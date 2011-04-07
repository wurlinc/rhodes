/****************************************************************************
** Meta object code from reading C++ file 'QtMainWindow.h'
**
** Created: Fri 8. Apr 02:30:25 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QtMainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtMainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtMainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      42,   13,   13,   13, 0x08,
      72,   13,   13,   13, 0x08,
      97,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtMainWindow[] = {
    "QtMainWindow\0\0on_webView_urlChanged(QUrl)\0"
    "on_webView_loadFinished(bool)\0"
    "on_webView_loadStarted()\0"
    "on_actionExit_triggered()\0"
};

const QMetaObject QtMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_QtMainWindow,
      qt_meta_data_QtMainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QtMainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QtMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QtMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtMainWindow))
        return static_cast<void*>(const_cast< QtMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int QtMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_webView_urlChanged((*reinterpret_cast< QUrl(*)>(_a[1]))); break;
        case 1: on_webView_loadFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: on_webView_loadStarted(); break;
        case 3: on_actionExit_triggered(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
