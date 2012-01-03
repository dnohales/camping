#ifndef MAIN_H
#define MAIN_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QMapIterator>

#include "CampingApplication.h"
#include "CampingConfig.h"

#define Db() QSqlDatabase::database()
#define App() (static_cast<CampingApplication *>(QCoreApplication::instance()))

#endif // MAIN_H
