//
// Created by sufuk on 9.12.2021.
//

#ifndef MRADSIM_SPACE_MRADSIMHELPERFUNCTIONS_H
#define MRADSIM_SPACE_MRADSIMHELPERFUNCTIONS_H
#include "DataStructs.h"

int indexOfLineStartingWith(const QStringList &list, const QString &textToFind);
OCCData* getNodeData(QTreeWidgetItem*);

#endif //MRADSIM_SPACE_MRADSIMHELPERFUNCTIONS_H
