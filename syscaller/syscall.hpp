#pragma once
#include <Windows.h>
#include <string_view>

extern "C" void* syscall_stub();

namespace system_call
{
	template<typename T, typename First = void*, typename Second = void*, typename Third = void*, typename Fourth = void*>
	T call(std::uint32_t idx, First first = 0, Second second = 0, Third third = 0, Fourth fourth = 0)
	{
		auto fn = reinterpret_cast<T(*)(First, Second, Third, Fourth, std::uint32_t)>(&syscall_stub);
		return fn(first, second, third, fourth, idx);
	}

	template<typename T, typename First, typename Second, typename Third, typename Fourth, typename... Args>
	T call(std::uint32_t idx, First first, Second second, Third third, Fourth fourth, Args... args)
	{
		auto fn = reinterpret_cast<T(*)(First, Second, Third, Fourth, std::uint32_t, Args...)>(&syscall_stub);
		return fn(first, second, third, fourth, idx, args...);
	}

	template<typename T>
	class func
	{
	public:
		explicit func(std::string_view func_name, std::wstring module_name = L"ntdll.dll")
			: func(func_name, ::GetModuleHandle(module_name.data()))
		{ }

		explicit func(std::string_view func_name, HMODULE module)
		{
			auto fn = reinterpret_cast<std::uint64_t>(::GetProcAddress(module, func_name.data()));
			_syscall_idx = *reinterpret_cast<std::uint32_t*>(fn + 4);
		}

		template<typename... Args>
		T operator()(Args... args)
		{
			return call<T>(_syscall_idx, std::forward<Args>(args)...);
		}
	private:
		std::uint32_t _syscall_idx;
	};
}