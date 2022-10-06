#pragma once

namespace Cache::EditorID
{
	using _GetFormEditorID = const char* (*)(std::uint32_t);

	std::string GetFormEditorID(const RE::TESForm* a_form);
}
