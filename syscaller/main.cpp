#include <iostream>
#include "syscall.hpp"

int main()
{
	int dest = 0;
	int src = 5;

	std::cout << "value (before) : " << std::dec << dest << std::endl;

	auto write_virtual_memory = system_call::func<NTSTATUS>("NtWriteVirtualMemory");
	write_virtual_memory(GetCurrentProcess(), &dest, &src, sizeof(int), nullptr);

	std::cout << "value (after)  : " << std::dec << dest << std::endl;
	getchar();
}