#ifndef MAIN_H
#define MAIN_H

#include "CampingApplication.h"

#define Db() QSqlDatabase::database("main")
#define App() (static_cast<CampingApplication *>(QCoreApplication::instance()))

#endif // MAIN_H
