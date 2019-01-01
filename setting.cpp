#include "setting.h"

CSetting CSetting::MSetting;

CSetting::CSetting()
{
	ini.SetUnicode();
	LoadSettings();
}

bool CSetting::FileExists(const std::string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}

void CSetting::LoadSettings()
{
	ini.Reset();
	if (FileExists("tycho.ini"))
	{
		ini.LoadFile("tycho.ini");
		volume = ini.GetDoubleValue("setting", "volume", 1.0f, false);
		fullscreen = ini.GetBoolValue("setting", "fullscreen", true, false);
		FPS = ini.GetBoolValue("debug", "fps", false, false);
	}
	else
	{
		volume = 1.0f;
		fullscreen = true;
		FPS = false;
		SaveSettings();
	}
}

void CSetting::SaveSettings()
{
	ini.SetDoubleValue("setting", "volume", volume);
	ini.SetBoolValue("setting", "fullscreen", fullscreen);
	ini.SetBoolValue("debug", "fps", FPS);
	ini.SaveFile("tycho.ini", false);
}