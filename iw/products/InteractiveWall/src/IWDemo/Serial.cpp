#include "Serial.h"
#include <stdexcept>

/*
 * Based on code by Anthony Scavarelli:
 * https://gist.github.com/PlumCantaloupe/8939224
 */

using namespace std;

namespace Serial {

	int writeBytes(HANDLE port, char * bytes, int num)
	{
		DWORD dwBytesWritten = 0;
		if (!WriteFile(port, bytes, num, &dwBytesWritten, NULL))
		{
			throw runtime_error("Error writing serial");
		}
		return dwBytesWritten;
	}

	void readByte(HANDLE port, char* byte) {
		COMMTIMEOUTS timeout;
		GetCommTimeouts(port, &timeout);

		timeout.ReadIntervalTimeout        = 10;
		timeout.ReadTotalTimeoutMultiplier = 10;
		timeout.ReadTotalTimeoutConstant   = 10;

		// block until a byte is available
		SetCommTimeouts(port, &timeout);

		if (!ReadFile(port, byte, 1, NULL, NULL)) {
			throw runtime_error("Error reading serial");
		}
	}

	void readData(HANDLE port, char start, char* buffer, int dataLen) {
		char byte;
		int dataPos = 0;

		while (dataPos < dataLen) {
			readByte(port, &byte);

			if (byte == '>') {
				dataPos = 0;
			} else {
				buffer[dataPos] = byte;
				dataPos++;
			}
		}
	}

	void closePort(HANDLE port) {
		CloseHandle(port);
	}

	void flush(HANDLE port)
	{
		PurgeComm(port, PURGE_RXCLEAR | PURGE_TXCLEAR);
	}

	wstring utf8Decode(const string &str) {
		int resultSize = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
		wstring result(resultSize, 0);

		MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &result[0], resultSize);
		return result;
	}

	HANDLE openPort(int portNum, long baud, bool* connected) {
		HANDLE port;
		char portName[256];

		if (portNum == 1) {
			sprintf(portName, "\\\\.\\COM%d", portNum); // Microsoft KB115831
		} else {
			sprintf(portName, "COM%d", portNum);
			portName[sizeof(portName) - 1] = 0;
		}
		auto pName = string(portName);
		port = CreateFile(
			utf8Decode(pName).c_str(), 
			GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, NULL);

		if (port == INVALID_HANDLE_VALUE) {
			*connected = false;
			return NULL;
		}

		COMMCONFIG config;
		DWORD      configSize;
		GetCommConfig(port, &config, &configSize);

		config.dcb.BaudRate      = baud;
		config.dcb.fBinary       = TRUE;
		config.dcb.fParity       = FALSE;
		config.dcb.fOutxCtsFlow  = FALSE;
		config.dcb.fOutxDsrFlow  = FALSE;
		config.dcb.fOutX         = FALSE;
		config.dcb.fInX          = FALSE;
		config.dcb.fErrorChar    = FALSE;
		config.dcb.fNull         = FALSE;
		config.dcb.fRtsControl   = RTS_CONTROL_ENABLE;
		config.dcb.fAbortOnError = FALSE;
		config.dcb.ByteSize      = 8;
		config.dcb.Parity        = NOPARITY;
		config.dcb.StopBits      = ONESTOPBIT;
		config.dcb.fDtrControl   = DTR_CONTROL_ENABLE;

		SetCommConfig(port, &config, configSize);

		*connected = true;
		return port;
	}
}
