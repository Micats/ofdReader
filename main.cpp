#pragma execution_character_set("utf-8")
#include "OfdReader.h"
#include <QtWidgets/QApplication>
#include <QDebug>
#include <QDialog>
#include "YOfdParser.h"
#include "OfdExplore.h"
#include "freetype/freetype.h"
#include "freetype/ftoutln.h"
#include "freetype/ftglyph.h"
//#include "Third/FreeType/include/ft2build.h"
#include <QPainter>
#include <QLibraryInfo>
#include "YFont.h"


int main(int argc, char *argv[])
{
	int argCount = argc;
	char** argVar = new char* [argCount + 3];
	for (int i=0;i<argc;i++)
	{
		int size = strlen(argv[i]) + 1;
		argVar[i] = new char[size];
		memcpy(argVar[i], argv[i], size);
	}
	argVar[argCount++] = new char[10] {"-platform"};
	argVar[argCount++] = new char[28] {"windows:fontengine=freetype"};
	argVar[argCount] = nullptr;
	
	QApplication a(argCount, argVar);

	for (int i = 0; (i != argCount) && (argVar[i] != nullptr); ++i)
		delete[] argVar[i];
	delete[] argVar;


	QFontDatabase database;
	foreach(const QString & family, database.families()) {
		qDebug() << family;
	}
	qDebug() << "system fonts family;";
	
	OfdExplore w2;
	w2.show();

	w2.openFile("C:\\Users\\YZQ\\Desktop\\2M - ¸±±¾ (1).ofd");

    return a.exec();
}
