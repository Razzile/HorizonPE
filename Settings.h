/*                   __  __ 
 *|__| _  _._  _  _ |__)|_  
 *|  |(_)| |/_(_)| )|   |__ 
 *                       
 * File: Settings.h
 * Created by Razzile & AlphaMATTER
 */
#ifndef __Settings__
#define __Settings__

#include <iostream>
#include <string>

extern const char *path;
#define Settings(p) const char* path = p; settings

using namespace std;

class settings {
public:

	static int GetPrefInt(const char* key);
	static float GetPrefFloat(const char* key);
	static bool GetPrefBool(const char* key);

	class settings_proxy {
	public:
		char* key;

		union Value {
			int asInt;
			bool asBool;
			float asFloat;
		} value;

		enum ValueType {
			Int,
			Bool,
			Float
		} valueType;

		settings_proxy(const char* _key) {
			key = (char*)malloc(strlen(_key));
			strcpy(key, _key);
		 }

		 settings_proxy(int val) {
		 	value.asInt = val; 
		 	valueType = Int;
		 }

		 settings_proxy(float val) {
		 	value.asFloat = val;
		 	valueType = Float;
		 }

		settings_proxy(bool val) {
		 	 value.asBool = val;
		 	 valueType = Bool;
		 }
 

		 __attribute__((noinline))
		operator int() {
			return settings::GetPrefInt(key);
		}

		 __attribute__((noinline))
		operator float() {
			return settings::GetPrefFloat(key);
		}

		__attribute__((noinline))
		operator bool() {
			return settings::GetPrefBool(key);
		}

		__attribute__((noinline))
		settings_proxy& operator= (const settings_proxy &source) {
			switch(source.valueType)
			{
				case Int: {
					set(source.value.asInt);
					break;
				}
				case Bool: {
					set(source.value.asBool);
					break;
				}
				case Float: {
					set(source.value.asFloat);
					break;
				}
			}
			return *this;
		}
		void set(bool value);
		void set(int value);
		void set(float value);

		~settings_proxy() {
			free(key);
		}
	};
	__attribute__((noinline))
	settings_proxy operator[] (const char* key) {
		settings_proxy proxy(key);
		return proxy;
	}

};

extern settings horizonSettings;
#endif
