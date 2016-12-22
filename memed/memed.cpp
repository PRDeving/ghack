#include "stdafx.h"
#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <iostream>
#include <conio.h>
#include <comdef.h>

#include "memory.h"


int GetProcessId(const char* ProcName) {
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	_bstr_t f;
	if (Process32First(hSnapshot, &pe32)) {
		do {
			f = _bstr_t(pe32.szExeFile);
			if (strcmp((char*)f, ProcName) == 0) break;
		} while (Process32Next(hSnapshot, &pe32));
	}

	if (hSnapshot != INVALID_HANDLE_VALUE) CloseHandle(hSnapshot);
	return pe32.th32ProcessID;
}


int main() {
	const char * processName = "dummy.exe";
	DWORD addr = 0x118FDFC;

	int processID = GetProcessId(processName);
	if (!processID) {
		std::cout << "Error getting process ID" << std::endl;
		return -1;
	}

	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, false, processID);
	if (process == 0) {
		std::cout << "Error oppening process " << processID << std::endl;
		return -1;
	}
	else
		std::cout << "Process " << processID << " handler is " << process << std::endl;

	int val = memory::read<int>(process, addr);
	std::cout << "mem 0x" << std::hex << (int)addr << " is " << std::dec << val << std::endl;

	BOOL error = FALSE;
	while (!error) {
		error = !memory::write<int>(process, addr, 15);
		Sleep(100);
	}
	CloseHandle(process);
	std::cin >> val;
    return 0;
}

