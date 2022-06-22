#include "TeensyReader.h"

TeensyReader::TeensyReader() {
	serialHandle = CreateFile(L"COM3", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	serialParams = { 0 };
	serialParams.DCBlength = sizeof(serialParams);
	
	GetCommState(serialHandle, &serialParams);
	serialParams.BaudRate = CBR_9600;
	serialParams.ByteSize = 8;
	serialParams.Parity = NOPARITY;
	serialParams.StopBits = TWOSTOPBITS;

	SetCommState(serialHandle, &serialParams);
	
	timeout = { 0 };
	timeout.ReadIntervalTimeout = 1000;
	timeout.ReadTotalTimeoutConstant = 1000;
	timeout.ReadTotalTimeoutMultiplier = 1000;
	timeout.WriteTotalTimeoutConstant = 1000;
	timeout.WriteTotalTimeoutMultiplier = 1000;

	SetCommTimeouts(serialHandle, &timeout);
	SetCommMask(serialHandle, EV_RXCHAR);
}

TeensyReader::~TeensyReader() {
	CloseHandle (serialHandle);
}

std::bitset<5> TeensyReader::read() {
	int input = 0;
	DWORD br = 0;
	DWORD dwCMS = 0;

	WaitCommEvent(serialHandle, &dwCMS, 0);
	
	ReadFile(serialHandle, &input, 1, &br, NULL);
	
	return std::bitset<5>(input);
}
