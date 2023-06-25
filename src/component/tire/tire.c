#include "tire.h"
#include "string.h"
#include "stdlib.h"

void TireInit(Tire* this) {
	(void)this;
}

bool IsNeedBackupTireData(Tire* tire, Tire* tempTire) {
	// 2.2 -> 2.4
	if (abs(tire->press.value-tempTire->press.value) > 200) {
		return true;
	}

	// Position changed
	if (memcmp(tire->pos, tempTire->pos, 3) != 0) {
		return true;
	}

	// ID changed
	if (memcmp(tire->id, tempTire->id, 8) != 0) {
		return true;
	}

	return false;
}

void TireParseByteBuffer(Tire* this, uint8_t* buff) {
	memcpy(this->id, buff, 8);
	buff+=8;
	memcpy(this->pos, buff, 3);
	buff+=3;
	this->press.value = (int32_t)GetUint32(buff);
	buff+=4;
	this->temp.value = (int32_t)GetUint16(buff);
	buff+=2;
	this->soc = (uint32_t)*buff;
}

void TirePackageByteBuffer(Tire* this, uint8_t* buff){
	memcpy(buff, this->id, 8);
	buff+=8;
	memcpy(buff,this->pos,3);
	buff+=3;
	SetUint32(buff, (uint32_t)this->press.value);
	buff+=4;
	SetUint16(buff, (uint32_t)this->temp.value);
	buff+=2;
	*buff = (uint8_t)this->soc;
}

char* TirePackageJsonMessage(Tire* this, char* buff) {
	buff = JsonOpen(buff);
	buff = JsonFromString(buff, "id", this->id);
	*buff++ = ',';
	buff = JsonFromString(buff, "pos", this->pos);
	*buff++ = ',';
	// buff = JsonFromInt(buff, "press", this->press.value);
	buff = JsonFromFloat(buff, "press", (float)this->press.value / 1000.0, 3);
	*buff++ = ',';
	//    buff = JsonFromFloat(buff, "bat", (float)this->soc / 100.0, 1);
	buff = JsonFromInt(buff, "bat", this->soc);
	*buff++ = ',';
	buff = JsonFromInt(buff, "temp", this->temp.value);
	buff = JsonClose(buff);

	return buff;
}

void TireSetId(Tire* this, char* id) {
	memset(this->id, 0, 10);
	memcpy(this->id, id, strlen(id));
}

// All Parameters were multiple by 1000
uint32_t TireGetPressure(uint32_t press) {
	// return (120000 + (824 * press + 88353)) / 98;
	return (0 + (824 * press + 88353)) / 98;
}

uint32_t TireGetSoc(uint32_t volx100) {
	uint32_t mVol = volx100 * 10;
	uint32_t soc = 0;

	if (mVol >= 3150)
	{
		soc = 100;
	}
	else if ((mVol < 3150) && (mVol >= 2910))
	{
		soc = 100 - (3150 - mVol) / 6;
	}
	else if ((mVol < 2910) && (mVol >= 2510))
	{
		soc = 60 - (2910 - mVol) / 10;
	}
	else if ((mVol < 2510) && (mVol >= 2285))
	{
		soc = 20 - (2510 - mVol) / 15;
	}
	else if ((mVol < 2285) && (mVol >= 2185))
	{
		soc = 5 - (2285 - mVol) / 20;
	}
	else if (mVol < 2180)
	{
		soc = 0;
	}

	return soc;
}
