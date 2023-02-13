#include "at_modem.h"
#include "string.h"

#define AT_MODEM_BUILD_HEAD(buff) \
    *buff++ = 'A';                \
    *buff++ = 'T';                \
    *buff++ = '='
#define AT_MODEM_BUILD_TAIL(buff) \
    *buff++ = '\r';               \
    *buff++ = '\n';               \
    *buff++ = '\0'

static inline bool ATmodemCheckResponse(ATmodem* this, const char* resp);
static inline void ATmodemBuildCmd(char* str, const char* content);
static inline void ATmodemClean(ATmodem* this);

char rxBuffer[AT_MODEM_RX_BUFFER_LEN];

void ATmodemInit(ATmodem* this) {
    this->rxBuff = rxBuffer;
}

bool ATmodemExcutiveCmd(ATmodem* this, const char* cmd, const char* resp, uint32_t timeoutMs) {
    ATmodemClean(this);
    ClockEnableTimeout(&this->clock, timeoutMs);

    this->sendCmd(cmd);
    while (!this->isResponse && !ClockIsTimeout(&this->clock));

    if (ClockIsTimeout(&this->clock))
        return false;

    ClockDisableTimeout(&this->clock);
    return ATmodemCheckResponse(this, resp);
}

static inline bool ATmodemCheckResponse(ATmodem* this, const char* resp) {
    return strstr(this->rxBuff, resp);
}

static inline void ATmodemBuildCmd(char* str, const char* content) {
    char* buff = str;
    AT_MODEM_BUILD_HEAD(buff);
    strcat(buff, content);
    buff += strlen(content);
    AT_MODEM_BUILD_TAIL(buff);
}

static inline void ATmodemClean(ATmodem* this) {
    this->isResponse = false;
    memset(this->rxBuff, '\0', AT_MODEM_RX_BUFFER_LEN);
}
