#pragma once

#define WIN32_LEAN_AND_MEAN

#define NOGDICAPMASKS
#define NOVIRTUALKEYCODES
#define NOWINMESSAGES
#define NOWINSTYLES
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTEROPS
#define NOSHOWWINDOW
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOGDI
#define NOKERNEL
#define NOUSER
#define NONLS
#define NOMB
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
#define NOMSG
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOWINOFFSETS
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX

#include <ranges>

#include "RE/Skyrim.h"
#include "REX/REX.h"
#include "SKSE/SKSE.h"

#include "Version.h"

#include <boost/regex.hpp>
#include <boost/unordered/unordered_flat_map.hpp>
#include <boost/unordered/unordered_flat_set.hpp>
#include <frozen/bits/elsa_std.h>
#include <frozen/unordered_map.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <xbyak/xbyak.h>

#include <ClibUtil/distribution.hpp>
#include <ClibUtil/editorID.hpp>
#define DLLEXPORT __declspec(dllexport)

namespace dist = clib_util::distribution;
namespace editorID = clib_util::editorID;

using namespace std::literals;
using namespace RE::literals;
using namespace REX::STR::literals;

template <class K, class D, class H = boost::hash<K>, class KEqual = std::equal_to<K>>
using Map = boost::unordered_flat_map<K, D, H, KEqual>;

template <class K, class H = boost::hash<K>, class KEqual = std::equal_to<K>>
using Set = boost::unordered_flat_set<K, H, KEqual>;

namespace frozen
{
	template <typename K, typename V, std::size_t N>
	struct bidirectional_map
	{
		constexpr bidirectional_map() = delete;

		constexpr bidirectional_map(std::initializer_list<std::pair<K, V>> list) :
			_map(list),
			_reverseMap(reverse(list))
		{}

		constexpr const V* find(const K& key) const
		{
			auto it = _map.find(key);
			return (it != _map.end()) ? &it->second : nullptr;
		}

		constexpr const K* find(const V& value) const
		{
			auto it = _reverseMap.find(value);
			return (it != _reverseMap.end()) ? &it->second : nullptr;
		}

	private:
		static constexpr std::array<std::pair<V, K>, N> reverse(const std::initializer_list<std::pair<K, V>>& list)
		{
			std::array<std::pair<V, K>, N> reversed{};
			std::size_t                    i = 0;
			for (auto it = list.begin(); it != list.end(); it++) {
				reversed[i] = { it->second, it->first };
				i++;
			}
			return reversed;
		}

		const frozen::unordered_map<K, V, N> _map;
		const frozen::unordered_map<V, K, N> _reverseMap;
	};
}

namespace stl
{
	template <class T>
	void write_thunk_call(std::uintptr_t a_src)
	{
		auto& trampoline = REL::GetTrampoline();
		T::func = trampoline.write_call<5>(a_src, T::thunk);
	}

	template <class F, std::size_t idx, class T>
	void write_vfunc()
	{
		REL::Relocation<std::uintptr_t> vtbl{ F::VTABLE[0] };
		T::func = vtbl.write_vfunc(idx, T::thunk);
	}

	template <class T, std::size_t BYTES>
	void hook_function_prologue(std::uintptr_t a_src)
	{
		struct Patch : Xbyak::CodeGenerator
		{
			Patch(std::uintptr_t a_originalFuncAddr, std::size_t a_originalByteLength)
			{
				// Hook returns here. Execute the restored bytes and jump back to the original function.
				for (size_t i = 0; i < a_originalByteLength; ++i) {
					db(*reinterpret_cast<std::uint8_t*>(a_originalFuncAddr + i));
				}

				jmp(ptr[rip]);
				dq(a_originalFuncAddr + a_originalByteLength);
			}
		};

		Patch p(a_src, BYTES);
		p.ready();

		auto& trampoline = REL::GetTrampoline();
		trampoline.write_jmp<5>(a_src, T::thunk);

		auto alloc = trampoline.allocate(p.getSize());
		std::memcpy(alloc, p.getCode(), p.getSize());

		T::func = reinterpret_cast<std::uintptr_t>(alloc);
	}

	template <typename First, typename... T>
	[[nodiscard]] bool is_in(First&& first, T&&... t)
	{
		return ((first == t) || ...);
	}

	constexpr inline auto enum_range(auto first, auto last)
	{
		auto enum_range =
			std::views::iota(
				std::to_underlying(first),
				std::to_underlying(last)) |
			std::views::transform([](auto enum_val) {
				return (decltype(first))enum_val;
			});

		return enum_range;
	};
}

namespace Runtime
{
	inline constexpr REL::Version SSE_1_7_99(1, 7, 99, 0);
	inline constexpr REL::Version MIN_ADDRESS_LIBRARY_V5 = SSE_1_7_99;

	inline REL::Version version{};

	[[nodiscard]] inline bool IsAtLeast1_7_99() noexcept
	{
		return version >= Runtime::SSE_1_7_99;
	}

	inline void Init(const REL::Version& a_runtimeVersion)
	{
		version = a_runtimeVersion;

		REX::INFO("Game version : {}", version);

#ifdef SKYRIM_SUPPORT_AE
		if constexpr (SKSE::RUNTIME_SSE_LATEST < MIN_ADDRESS_LIBRARY_V5) {
			if (version >= MIN_ADDRESS_LIBRARY_V5) {
				REX::FAIL(
					"You are using a newer version of Skyrim than this version of {0} supports.\n"
					"Install the correct version of {0} for your game version.\n"
					"Runtime: {1}\n"
					"Supported: 1.6.1170 (Steam) / 1.6.1179 (GOG)",
					Version::PROJECT, version);
			}
		}
#endif
	}
}

#ifdef SKYRIM_AE
#	define OFFSET(se, ae) ae
#	define OFFSET_3(se, ae, vr) ae
#	define OFFSET_VERSIONED(se, ae, ae1799, vr) \
		(Runtime::IsAtLeast1_7_99() ? ae1799 : ae)
#elif SKYRIMVR
#	define OFFSET(se, ae) se
#	define OFFSET_3(se, ae, vr) vr
#	define OFFSET_VERSIONED(se, ae, ae1799, vr) vr
#else
#	define OFFSET(se, ae) se
#	define OFFSET_3(se, ae, vr) se
#	define OFFSET_VERSIONED(se, ae, ae1799, vr) se
#endif

#include "API/API.h"
#include "Common.h"
