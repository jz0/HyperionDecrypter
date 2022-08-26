#include "Hyperion.hpp"

#define IN_RANGE(x, a, b) (x >= a && x <= b)
#define GET_BITS(x) (IN_RANGE((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xA): (IN_RANGE(x, '0', '9') ? x - '0': 0))
#define GET_BYTE(x) (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))

uintptr_t FindPattern(uintptr_t Base, const char* Sig)
{
	const auto DOSHeader = PIMAGE_DOS_HEADER(Base);
	const auto NTHeaders = PIMAGE_NT_HEADERS(Base + DOSHeader->e_lfanew);
	uintptr_t Size = (Base + (uintptr_t)NTHeaders->OptionalHeader.SizeOfImage);
	const char* Pat = Sig;
	uintptr_t FirstMatch = 0;

	for (uintptr_t Cur = Base; Cur < Size; Cur++)
	{

		if (!*Pat)
			return FirstMatch;

		if (*(PBYTE)Pat == ('\?') || *(BYTE*)Cur == GET_BYTE(Pat))
		{
			if (!FirstMatch)
				FirstMatch = Cur;

			if (!Pat[2])
				return FirstMatch;

			if (*(PWORD)Pat == ('\?\?') || *(PBYTE)Pat != ('\?'))
				Pat += 3;

			else Pat += 2;
		}
		else
		{
			Pat = Sig;
			FirstMatch = 0;
		}
	}

	return NULL;
}

void Init()
{
	auto PEB = reinterpret_cast<PPEB>(__readgsqword(0x60));
	Hyperion::ImageBase = reinterpret_cast<uintptr_t>(PEB->ImageBaseAddress);
	Hyperion::GadgetPtr = FindPattern(reinterpret_cast<uintptr_t>(GetModuleHandleA("loader.dll")), "8A 01 C3 ??"); //mov al, byte ptr [rcx] ret, trap to debugger d(-_____-)b
	Hyperion::DecryptMemory();
}

bool __stdcall DllMain(void* Instance, unsigned long Reason, void* Reserved)
{
	if (Reason == DLL_PROCESS_ATTACH)
		Init();

	return true;
}