#include "main.h"
#include "common.h"
#include "MainWindow.h"

#define TEST 0

int test();

int main(int argc, char *argv[])
{
    CampingApplication a(argc, argv);
	
	QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );
	QTextCodec::setCodecForCStrings( QTextCodec::codecForName("utf8") );
	
	if(!TEST){
		MainWindow w;
		w.show();
	
		return a.exec();
	} else{
		return test();
	}
}

int test()
{
	App()->initExistentDatabase("jaja.camp");
	//LocationCollection loc = Location().findAll();
	//qDebug() << Location().findAllByType(Location::TENT).at(0).getName();
	
	//qDebug() << loc.at(0).getName();
	/*try{
		loc.setName("Carpa 1");
		loc.save();
	} catch(ActiveRecordException &e){
		qDebug() << e.message();
	}*/
	
	return 0;
}
