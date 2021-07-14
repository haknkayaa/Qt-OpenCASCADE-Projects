//
// Created by hakan on 9.07.2021.
//

#ifndef PROJECT_DATASTRUCTS_H
#define PROJECT_DATASTRUCTS_H


typedef struct {
    QString Index;
    QString Name;
    QString Formula;
    QString DensityValue;
    QString DensityUnit;
    QString SubElements;
} table_material;

typedef struct {
    QString type;
    QString elementName;
    QString elementRate;
} t_subElement;



#endif //PROJECT_DATASTRUCTS_H
