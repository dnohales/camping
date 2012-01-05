#include <QTextCodec>
#include "CampingApplication.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    CampingApplication a(argc, argv);
	
	QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );
	
    MainWindow w;
    w.show();

    return a.exec();
}
