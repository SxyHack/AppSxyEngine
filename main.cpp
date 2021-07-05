#include "MainWindow.h"
#include "DarkStyle.h"
#include "framelesswindow.h"
#include "MainWindow.h"
#include "src/log/CustomLogMessageHandler.h"
#include "utility/Random.h"

#include <QtWidgets/QApplication>
#include <QtDebug>


const QString gWelcome = R"(
 _______ __   __ __   __   _______ __    _ _______ ___ __    _ _______ 
|       |  |_|  |  | |  | |       |  |  | |       |   |  |  | |       |
|  _____|       |  |_|  | |    ___|   |_| |    ___|   |   |_| |    ___|
| |_____|       |       | |   |___|       |   | __|   |       |   |___ 
|_____  ||     ||_     _| |    ___|  _    |   ||  |   |  _    |    ___|
 _____| |   _   | |   |   |   |___| | |   |   |_| |   | | |   |   |___ 
|_______|__| |__| |___|   |_______|_|  |__|_______|___|_|  |__|_______|
)";

int main(int argc, char *argv[])
{
	CustomLogMessageHandler::Instance();
	qInstallMessageHandler(CustomLogMessageHandler::handle);
	qDebug() << gWelcome.toUtf8().data();

	QApplication a(argc, argv);
	QApplication::setStyle(new DarkStyle);
	QApplication::setPalette(QApplication::style()->standardPalette());

	// create frameless window (and set windowState or title)
	FramelessWindow framelessWindow;
	//framelessWindow.setWindowState(Qt::WindowFullScreen);
	framelessWindow.setWindowTitle(Random::GenerateString(16));
	framelessWindow.setMinimumWidth(1024);
	framelessWindow.setMinimumHeight(1000);
	framelessWindow.setWindowIcon(a.style()->standardIcon(QStyle::SP_DesktopIcon));

	MainWindow mainWindow;
	mainWindow.setWindowTitle(Random::GenerateString(32));
	mainWindow.setMinimumWidth(1024);
	mainWindow.setMinimumHeight(1000);
	mainWindow.show();

	//mainWindow->showMaximized();
	// add the mainwindow to our custom frameless window
	//framelessWindow.setContent(mainWindow);
	//framelessWindow.showMaximized();
	return a.exec();
}
