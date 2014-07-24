/*
 *     .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .-----------------. .----------------.  .----------------. 
 *    | .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. |
 *    | |  ____  ____  | || |     ____     | || |  _______     | || |     _____    | || |   ________   | || |     _____    | || |     ____     | || | ____  _____  | || |   ______     | || |  _________   | |
 *    | | |_   ||   _| | || |   .'    `.   | || | |_   __ \    | || |    |_   _|   | || |  |  __   _|  | || |    |_   _|   | || |   .'    `.   | || ||_   \|_   _| | || |  |_   __ \   | || | |_   ___  |  | |
 *    | |   | |__| |   | || |  /  .--.  \  | || |   | |__) |   | || |      | |     | || |  |_/  / /    | || |      | |     | || |  /  .--.  \  | || |  |   \ | |   | || |    | |__) |  | || |   | |_  \_|  | |
 *    | |   |  __  |   | || |  | |    | |  | || |   |  __ /    | || |      | |     | || |     .'.' _   | || |      | |     | || |  | |    | |  | || |  | |\ \| |   | || |    |  ___/   | || |   |  _|  _   | |
 *    | |  _| |  | |_  | || |  \  `--'  /  | || |  _| |  \ \_  | || |     _| |_    | || |   _/ /__/ |  | || |     _| |_    | || |  \  `--'  /  | || | _| |_\   |_  | || |   _| |_      | || |  _| |___/ |  | |
 *    | | |____||____| | || |   `.____.'   | || | |____| |___| | || |    |_____|   | || |  |________|  | || |    |_____|   | || |   `.____.'   | || ||_____|\____| | || |  |_____|     | || | |_________|  | |
 *    | |              | || |              | || |              | || |              | || |              | || |              | || |              | || |              | || |              | || |              | |
 *    | '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' |
 *     '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'                       
 * File: Hooks.cpp
 * Created by Razzile & AlphaMATTER
 */
#include "Settings.h"

Settings("/var/mobile/Library/Preferences/horizonpe.plist") horizonSettings;

NSMutableDictionary *prefs;

int settings::GetPrefInt(const char* key)
{
	return [[prefs valueForKey:[NSString stringWithUTF8String:key]] intValue];
}

float settings::GetPrefFloat(const char* key)
{
	return [[prefs valueForKey:[NSString stringWithUTF8String:key]] floatValue];
}

bool settings::GetPrefBool(const char* key) 
{
	return [[prefs valueForKey:[NSString stringWithUTF8String:key]] boolValue];
}

void settings::settings_proxy::set(bool value)
{
	[prefs setValue:[NSNumber numberWithBool:value] forKey:[NSString stringWithUTF8String:key]];
	[prefs writeToFile:[NSString stringWithUTF8String:path] atomically: YES];
}

void settings::settings_proxy::set(int value)
{
	[prefs setValue:[NSNumber numberWithInt:value] forKey:[NSString stringWithUTF8String:key]];
	[prefs writeToFile: [NSString stringWithUTF8String:path] atomically: YES];
}

void settings::settings_proxy::set(float value)
{
	[prefs setValue: [NSNumber numberWithFloat:value] forKey:[NSString stringWithUTF8String:key]];
	[prefs writeToFile: [NSString stringWithUTF8String:path] atomically: YES];
}


__attribute__((constructor))
void LoadPrefs(void)
{
	prefs =  [NSMutableDictionary dictionaryWithContentsOfFile:@"/var/mobile/Library/Preferences/horizonpe.plist"];
}