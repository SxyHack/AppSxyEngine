#pragma once

#include <QSettings>

class GlobalSettings
{
public:
	static GlobalSettings& Instance();

protected:
	GlobalSettings();

private:
	QSettings* mSettings;
};
