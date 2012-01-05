#ifndef MAIN_H
#define MAIN_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QMapIterator>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QDebug>

#include "CampingApplication.h"
#include "CampingConfig.h"
#include "CampingException.h"
#include "ActiveRecord.h"
#include "Client.h"
#include "Location.h"

#define Db() QSqlDatabase::database("main")
#define App() (static_cast<CampingApplication *>(QCoreApplication::instance()))

#endif // MAIN_H
