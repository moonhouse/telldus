#include "ProtocolEverflourish.h"

std::string ProtocolEverflourish::getStringForMethod(int method, const std::string &, Controller *) {
	unsigned int deviceCode = this->getIntParameter(L"house", 0, 16383);
	unsigned int intCode = this->getIntParameter(L"unit", 1, 4)-1;
	unsigned char action;

	if (method == TELLSTICK_TURNON) {
		action = 15;
	} else if (method == TELLSTICK_TURNOFF) {
		action = 0;
	} else if (method == TELLSTICK_LEARN) {
		action = 10;
	} else {
		return "";
	}
	
	const char ssss = 85;
	const char sssl = 84; // 0
	const char slss = 69; // 1

	const char bits[2] = {sssl,slss};
	int i, check;

	std::string strCode;

	deviceCode = (deviceCode << 2) | intCode;
	
	check = calculateChecksum(deviceCode);

	char preamble[] = {'R', 5, 'T', 114,60,1,1,105,ssss,ssss,0};
	strCode.append(preamble);

	for(i=15;i>=0;i--) {
		strCode.append(1, bits[(deviceCode>>i)&0x01]);
	}
	for(i=3;i>=0;i--) {
		strCode.append(1, bits[(check>>i)&0x01]);
	}
	for(i=3;i>=0;i--) {
		strCode.append(1, bits[(action>>i)&0x01]);
	}
	
	strCode.append(1, ssss);
	strCode.append(1, '+');

	return strCode;
}

// The calculation used in this function is provided by Frank Stevenson
unsigned int ProtocolEverflourish::calculateChecksum(unsigned int x) {
	unsigned int bits[16] = {
		0xf ,0xa ,0x7 ,0xe,
		0xf ,0xd ,0x9 ,0x1,
		0x1 ,0x2 ,0x4 ,0x8,
		0x3 ,0x6 ,0xc ,0xb
	};
	unsigned int bit = 1;
	unsigned int res = 0x5;
	int i;
	unsigned int lo,hi;

	if ((x&0x3)==3) {
		lo = x & 0x00ff;
		hi = x & 0xff00;
		lo += 4;
		if (lo>0x100) {
			lo = 0x12;
		}
		x = lo | hi;
	}

	for(i=0;i<16;i++) {
		if (x&bit) {
			res = res ^ bits[i];
		}
		bit = bit << 1;
	}

	return res; 
}