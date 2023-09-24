#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdint.h>
#include <vector>
#define buffersize 180

class RingBuffer
{
public:
    RingBuffer();
    ~RingBuffer();
    void Insert(uint8_t data);
    void InsertData(uint8_t* data, uint16_t datasize);
    uint8_t Recieve(bool* recvstatus);
    uint8_t Recieve(void);
    void MoveReadIndex(int16_t decrement);
    std::vector<uint8_t> RecieveAll();
    uint16_t GetAvailabeData();
private:
    struct
    {
        uint8_t data[buffersize] = {0};
        uint16_t size = buffersize;
        uint16_t lastInsert = 0;
        uint16_t nextRead = 0;
        uint16_t availabeData = 0;
    }buffer;
};

#endif // RINGBUFFER_H
