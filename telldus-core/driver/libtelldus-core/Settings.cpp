#include "Settings.h"
#include "DeviceNexa.h"
#include "DeviceWaveman.h"
#include "DeviceSartano.h"
#include "DeviceIkea.h"
#include <string.h>
#include <stdlib.h>

/*
* Get the requested device
* Note that the returned Device should be deleted when not in use anymore
*/
Device* Settings::getDevice(int intDeviceId){

	Device* dev = NULL;

	try{
		char* protocol = getProtocol(intDeviceId);
		if (protocol == NULL) {
			return NULL;
		}

		//each new brand must be added here
		if (strcasecmp(protocol, "arctech") == 0){
			char *strHouse = getDeviceParameter(intDeviceId, "nexa_house");
			char *strCode = getDeviceParameter(intDeviceId, "nexa_unit");
			dev = new DeviceNexa(strHouse, strCode);
			free(strHouse);
			free(strCode);
		
		} else if (strcasecmp(protocol, "Waveman") == 0) {
			char *strHouse = getDeviceParameter(intDeviceId, "nexa_house");
			char *strCode = getDeviceParameter(intDeviceId, "nexa_unit");
			dev = new DeviceWaveman(strHouse, strCode);
			free(strHouse);
			free(strCode);

		} else if (strcasecmp(protocol, "Sartano") == 0) {
			char *strCode = getDeviceParameter(intDeviceId, "sartano_code");
			dev = new DeviceSartano(strCode);
			free(strCode);

		} else if (strcasecmp(protocol, "Ikea") == 0) {
			char *strSystem = getDeviceParameter(intDeviceId, "ikea_system");
			char *strUnits = getDeviceParameter(intDeviceId, "ikea_units");
			char *strFade = getDeviceParameter(intDeviceId, "ikea_fade");
			dev = new DeviceIkea(strSystem, strUnits, strFade);
			free(strSystem);
			free(strUnits);
			free(strFade);

		} else {
			free(protocol);
			return NULL;
		}
		free(protocol);

#ifdef _LINUX
		dev->setDevice( getSetting("deviceNode") );
#endif

	}
	catch(...){
		throw;
	}
	return dev;
}

/*
* Get the name of the device
*/
char* Settings::getName(int intDeviceId){
	return getStringSetting(intDeviceId, "name", false);
}

/*
* Set the name of the device
*/
bool Settings::setName(int intDeviceId, const char* strNewName){
	return setStringSetting(intDeviceId, "name", strNewName, false);
}

/*
* Get the device vendor
*/
char* Settings::getProtocol(int intDeviceId){
	return getStringSetting(intDeviceId, "protocol", false);
}

/*
* Set the device vendor
*/
bool Settings::setProtocol(int intDeviceId, const char* strVendor){
	return setStringSetting(intDeviceId, "protocol", strVendor, false);
}

/*
* Get the device model
*/
int Settings::getModel(int intDeviceId){
	return getIntSetting(intDeviceId, "model", false);
}

/*
* Set the device model
*/
bool Settings::setModel(int intDeviceId, int intModel){
	return setIntSetting(intDeviceId, "model", intModel, false);
}

/*
* Set device argument
*/
bool Settings::setDeviceParameter(int intDeviceId, const char *strName, const char *strValue){
	return setStringSetting(intDeviceId, strName, strValue, true);
}

/*
* Get device argument
*/
char* Settings::getDeviceParameter(int intDeviceId, const char *strName) {
	return getStringSetting(intDeviceId, strName, true);
}
