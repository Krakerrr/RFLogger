#ifndef RFSTRUCT_H
#define RFSTRUCT_H

#include <QMetaType>
#include <stdint.h>

#define RFPAYLOADSIZE   56
#define RFDATASIZE      60
#define RFHEADER        96
#define RFLENGTH        60

struct RFCounter
{
    uint32_t recievedPacketCounter = 0;
    uint16_t recievedPacketFreq;
    uint32_t CRCnotOKCounter = 0;
};

struct __attribute__((__packed__)) RFData
{
    uint32_t	FCC_Clock;
    uint8_t     IMU_freq_status;
    float       IMU_acc_x;
    float       IMU_acc_y;
    float       IMU_acc_z;
    float       IMU_gyro_x;
    float       IMU_gyro_y;
    float       IMU_gyro_z;
    float       IMU_temp;
    float       CMD_throttle;
    float       CMD_de;
    float       CMD_da;
    float       CMD_dr;
    uint8_t     Rezerve[7];
    struct RFCounter Counter;
};
Q_DECLARE_METATYPE(RFData);

#endif // RFSTRUCT_H
