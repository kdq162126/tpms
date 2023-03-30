#include "tire.h"
#include "string.h"

void TireInit(Tire* this) {
    (void)this;
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
    buff = JsonFromFloat(buff, "bat", (float)this->bat / 100.0, 1);
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
    return (100000 + (824 * press + 88353)) / 98;
}
