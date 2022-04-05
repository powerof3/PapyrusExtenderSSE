#include "Cache.h"

namespace Cache
{
	EditorID* EditorID::GetSingleton()
	{
		static EditorID singleton;
		return std::addressof(singleton);
	}

	void EditorID::FillMap()
	{
		const auto& [map, lock] = RE::TESForm::GetAllFormsByEditorID();
		const RE::BSReadLockGuard locker{ lock };
		if (map) {
			for (auto& [id, form] : *map) {
				_formIDToEditorIDMap.emplace(form->GetFormID(), id.c_str());
			}
		}
	}

	std::string EditorID::GetEditorID(RE::FormID a_formID)
	{
		Locker locker(_lock);

		const auto it = _formIDToEditorIDMap.find(a_formID);
		return it != _formIDToEditorIDMap.end() ? it->second : std::string();
	}

	void Register()
	{
		EditorID::GetSingleton()->FillMap();
	}
}
