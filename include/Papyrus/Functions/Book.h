#pragma once

namespace Papyrus::Book
{
	void ClearReadFlag(STATIC_ARGS, RE::TESObjectBOOK* a_book);
	void SetReadFlag(STATIC_ARGS, RE::TESObjectBOOK* a_book);

	void Bind(VM& a_vm);
}
