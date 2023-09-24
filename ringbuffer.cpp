#include "ringbuffer.h"

RingBuffer::RingBuffer()
{

}

RingBuffer::~RingBuffer()
{

}

void RingBuffer::Insert(uint8_t data)
{
    // insert data
    buffer.lastInsert = (buffer.lastInsert + 1) % buffer.size;
    buffer.data[buffer.lastInsert] = data;
    //check read position
    if (buffer.availabeData && buffer.nextRead == buffer.lastInsert)
    {
        buffer.nextRead = (buffer.nextRead+1) % buffer.size;
    }
    //increase available data
    buffer.availabeData++;
    if (buffer.availabeData > buffer.size)
        buffer.availabeData = buffer.size;
}


void RingBuffer::InsertData(uint8_t* data, uint16_t datasize)
{
    for (int i = 0; i < datasize; ++i) {
        Insert(*data);
        data++;
    }
}


uint8_t RingBuffer::Recieve(bool* recvstatus)
{
    uint8_t recvdata = 0;
    *recvstatus = false;
    if (buffer.availabeData)
    {
        *recvstatus = true;
        buffer.availabeData--;
        recvdata = buffer.data[buffer.nextRead];
        buffer.nextRead = (buffer.nextRead + 1) % buffer.size;
    }
    return recvdata;
}


uint8_t RingBuffer::Recieve(void)
{
    uint8_t recvdata = 0;
    if (buffer.availabeData)
    {
        buffer.availabeData--;
        recvdata = buffer.data[buffer.nextRead];
        buffer.nextRead = (buffer.nextRead + 1) % buffer.size;
    }
    return recvdata;
}

void RingBuffer::MoveReadIndex(int16_t decrement)
{
    buffer.nextRead = (buffer.nextRead + decrement + buffer.size) % buffer.size;
}


std::vector<uint8_t> RingBuffer::RecieveAll()
{
    std::vector<uint8_t> recvdata;
    while (buffer.availabeData)
    {
        recvdata.push_back(Recieve());
    }
    return recvdata;

}

uint16_t RingBuffer::GetAvailabeData()
{
    return buffer.availabeData;
}


