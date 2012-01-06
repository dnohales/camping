#ifndef MAIN_H
#define MAIN_H

#define Db() QSqlDatabase::database("main")
#define App() (static_cast<CampingApplication *>(QCoreApplication::instance()))

#endif // MAIN_H
