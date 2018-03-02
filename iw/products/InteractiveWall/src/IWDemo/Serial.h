#pragma once
#include <windows.h>

namespace Serial {
	int writeBytes (HANDLE port, char * bytes, int num );
	void readByte  (HANDLE port, char* byte);
	void readData  (HANDLE port, char start, char* buffer, int data_len);
	void closePort (HANDLE port);
	void flush	   (HANDLE port);
	HANDLE openPort(int portNum, long baud, bool* connected);
}
