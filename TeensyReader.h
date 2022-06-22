#pragma once
#include <Windows.h>
#include <bitset>

class TeensyReader {
public:
	TeensyReader();
	~TeensyReader();
	std::bitset<5> read();

private:
	HANDLE serialHandle;
	DCB serialParams;
	COMMTIMEOUTS timeout;
};
