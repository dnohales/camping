#include "main.h"
#include "MainWindow.h"
#include "common.h"
#include <QLibraryInfo>
#include <QTranslator>

#define TEST 0

int test();

int main(int argc, char *argv[])
{
	CampingApplication a(argc, argv);

	QTranslator qtTranslator;
	qtTranslator.load("qt_" + QLocale::system().name(),
					  QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	a.installTranslator(&qtTranslator);

	if (!TEST) {
		MainWindow w;
		w.show();

		return a.exec();
	} else {
		return test();
	}
}

int test()
{
	return 0;
}
