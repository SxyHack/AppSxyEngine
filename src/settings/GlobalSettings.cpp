#include "GlobalSettings.h"
#include <QtCore/QtCore>

GlobalSettings& GlobalSettings::Instance()
{
	static GlobalSettings instance;
	return instance;
}

GlobalSettings::GlobalSettings()
	: mSettings(nullptr)
{
	auto sAppDir = QCoreApplication::applicationDirPath();
	QDir appDir(sAppDir);
	appDir.mkdir("config");
	appDir.cd("config");

	auto sPath = appDir.absolutePath() + "/global_settings.ini";
	mSettings = new QSettings(sPath, QSettings::IniFormat);
}

