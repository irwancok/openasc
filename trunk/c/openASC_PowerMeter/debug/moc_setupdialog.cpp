/****************************************************************************
** Meta object code from reading C++ file 'setupdialog.h'
**
** Created: Sun Mar 21 20:56:07 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../setupdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'setupdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SetupDialog[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

       0        // eod
};

static const char qt_meta_stringdata_SetupDialog[] = {
    "SetupDialog\0"
};

const QMetaObject SetupDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SetupDialog,
      qt_meta_data_SetupDialog, 0 }
};

const QMetaObject *SetupDialog::metaObject() const
{
    return &staticMetaObject;
}

void *SetupDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SetupDialog))
        return static_cast<void*>(const_cast< SetupDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int SetupDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
