#include "tire.h"
#include "string.h"

void TireInit(Tire* this) {
	(void)this;
}

void TireGetDataInFlash(Tire* this, uint32_t address) {
	uint8_t data[TIRE_DATA_FLASH_BUFFER_SIZE];
	uint8_t* buff = &data[0];
	memset(data, 0, TIRE_DATA_FLASH_BUFFER_SIZE);

	memcpy(data, (uint8_t *)address, TIRE_DATA_FLASH_BUFFER_SIZE);
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
