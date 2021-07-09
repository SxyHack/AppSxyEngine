#pragma once

#include <QSettings>

class GlobalSettings
{
public:
	static GlobalSettings& Instance();

	void SetSearchCodeRegionEnable(bool enabled);
	bool IsEnableSearchCodeRegin();

protected:
	GlobalSettings();

	void Save(const QString& qsGroupName, const QString& qsKey, const QVariant& value);
	QVariant Load(const QString& qsGroupName, const QString& qsKey, const QVariant& default);
private:
	QSettings* mSettings;
};


#define Settings GlobalSettings::Instance()