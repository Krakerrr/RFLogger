#ifndef RFSTRUCT_H
#define RFSTRUCT_H

#include <stdint.h>

#define RFPayloadSize 56

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
};

#endif // RFSTRUCT_H
