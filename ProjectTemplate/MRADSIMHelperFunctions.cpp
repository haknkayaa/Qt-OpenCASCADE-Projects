//
// Created by sufuk on 9.12.2021.
//
#include  "MRADSIMHelperFunctions.h"

int indexOfLineStartingWith(const QStringList &list, const QString &textToFind) {
    return list.indexOf(QRegExp("^" + QRegExp::escape(textToFind) + ".+"));
}

OCCData *getNodeData(QTreeWidgetItem *arg_item) {
    return qvariant_cast<OCCData *>(arg_item->data(0, Qt::UserRole));
}
