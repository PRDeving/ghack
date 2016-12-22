#pragma once

namespace memory {
	template <class T>
	T read(HANDLE process, DWORD addr) {
		T result;
		SIZE_T bs = sizeof(T);
		SIZE_T r;

		BOOL err = ReadProcessMemory(process, reinterpret_cast<LPCVOID>(addr), &result, bs, &r);
		if (!err || r != bs)
			std::cout << "Error reading memory block 0x" << std::hex << (int)addr << std::endl;
		return result;
	}

	template <class T>
	T write(HANDLE process, DWORD addr, T val) {
		DWORD old;
		SIZE_T bs = sizeof(T);
		SIZE_T r;

		VirtualProtectEx(process, reinterpret_cast<LPVOID>(addr), bs, PAGE_EXECUTE_READWRITE, &old);
		BOOL err = WriteProcessMemory(process, reinterpret_cast<LPVOID>(addr), &val, bs, &r);
		VirtualProtectEx(process, reinterpret_cast<LPVOID>(addr), bs, old, &old);

		if (!err || r != bs) {
			std::cout << "Error writting memory 0x" << std::hex << addr << ": " << std::dec << r << "/" << bs << " bytes written" << std::endl;
		}
		return err;
	}
}