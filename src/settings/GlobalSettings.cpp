#include "GlobalSettings.h"
#include <QtCore/QtCore>

GlobalSettings& GlobalSettings::Instance()
{
	static GlobalSettings instance;
	return instance;
}

void GlobalSettings::SetSearchCodeRegionEnable(bool enabled)
{
	Save("Search", "CodeRegionEnable", enabled);
}

bool GlobalSettings::IsEnableSearchCodeRegin()
{
	return Load("Search", "CodeRegionEnable", false).toBool();
}

void GlobalSettings::SetSearchBufferSize(long nSize)
{
	Save("Search", "BuffSize", nSize);
}

long GlobalSettings::GetSearchBufferSize()
{
	return Load("Search", "BuffSize", 0x2000).toLongLong();
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

void GlobalSettings::Save(const QString& qsGroupName, const QString& qsKey, const QVariant& value)
{
	mSettings->beginGroup(qsGroupName);
	mSettings->setValue(qsKey, value);
	mSettings->endGroup();
	mSettings->sync();
}

QVariant GlobalSettings::Load(const QString& qsGroupName, const QString& qsKey, const QVariant& default)
{
	mSettings->beginGroup(qsGroupName);
	auto& ret = mSettings->value(qsKey, default);
	mSettings->endGroup();
	return ret;
}

