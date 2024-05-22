#pragma once
struct deviceRaw
{
    int m_deviceID;
    deviceRaw(int a) : m_deviceID{a} {};
    ~deviceRaw() = default;
};