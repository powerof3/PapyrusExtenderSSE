#include "Papyrus/Functions/Book.h"

namespace Papyrus::Book
{
	void ClearReadFlag(STATIC_ARGS, RE::TESObjectBOOK* a_book)
	{
		if (!a_book) {
			a_vm->TraceStack("Book is None", a_stackID);
			return;
		}

		if (a_book->IsRead()) {
			a_book->data.flags.reset(RE::OBJ_BOOK::Flag::kHasBeenRead);
			a_book->RemoveChange(RE::TESObjectBOOK::ChangeFlags::kRead);
		}
	}

	void SetReadFlag(STATIC_ARGS, RE::TESObjectBOOK* a_book)
	{
		if (!a_book) {
			a_vm->TraceStack("Book is None", a_stackID);
			return;
		}

		if (!a_book->IsRead()) {
			a_book->data.flags.set(RE::OBJ_BOOK::Flag::kHasBeenRead);
			a_book->AddChange(RE::TESObjectBOOK::ChangeFlags::kRead);
		}
	}

	void Bind(VM& a_vm)
	{
		BIND(ClearReadFlag);
		BIND(SetReadFlag);

		logger::info("Registered book functions"sv);
	}
}
