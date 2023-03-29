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
    buff = JsonFromInt(buff, "press", this->press);
    *buff++ = ',';
    buff = JsonFromInt(buff, "bat", this->bat);
    *buff++ = ',';
    buff = JsonFromInt(buff, "temp", this->temp);
    buff = JsonClose(buff);

    return buff;
}

void TireSetId(Tire* this, char* id) {
    memset(this->id, 0, 10);
    memcpy(this->id, id, strlen(id));
}

uint32_t TireGetPressure(uint32_t press) {
    return 824 * press + 88353;
}
