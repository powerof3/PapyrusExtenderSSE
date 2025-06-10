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
#include "REX/REX/Singleton.h"
#include "SKSE/SKSE.h"

#include <ankerl/unordered_dense.h>
#include <frozen/bits/elsa_std.h>
#include <frozen/unordered_map.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <srell.hpp>
#include <xbyak/xbyak.h>

#include <ClibUtil/distribution.hpp>
#include <ClibUtil/editorID.hpp>
#include <ClibUtil/numeric.hpp>
#include <ClibUtil/rng.hpp>
#include <ClibUtil/string.hpp>

#define DLLEXPORT __declspec(dllexport)

namespace logger = SKSE::log;
namespace numeric = clib_util::numeric;
namespace string = clib_util::string;
namespace dist = clib_util::distribution;
namespace editorID = clib_util::editorID;

using namespace std::literals;
using namespace string::literals;

template <class D>
using Set = ankerl::unordered_dense::set<D>;
template <class K, class D>
using Map = ankerl::unordered_dense::map<K, D>;

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
	using namespace SKSE::stl;

	template <class T>
	void write_thunk_call(std::uintptr_t a_src)
	{
		auto& trampoline = SKSE::GetTrampoline();
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

				jmp(qword[rip]);
				dq(a_originalFuncAddr + a_originalByteLength);
			}
		};

		Patch p(a_src, BYTES);
		p.ready();

		auto& trampoline = SKSE::GetTrampoline();
		trampoline.write_branch<5>(a_src, T::thunk);

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

#ifdef SKYRIM_AE
#	define OFFSET(se, ae) ae
#	define OFFSET_3(se, ae, vr) ae
#elif SKYRIMVR
#	define OFFSET(se, ae) se
#	define OFFSET_3(se, ae, vr) vr
#else
#	define OFFSET(se, ae) se
#	define OFFSET_3(se, ae, vr) se
#endif

#include "API/API.h"
#include "Common.h"
#include "Version.h"
