#include "Papyrus/Functions/Book.h"

namespace Papyrus::Book
{
	void ClearBookCantBeTakenFlag(STATIC_ARGS, RE::TESObjectBOOK* a_book)
	{
		if (!a_book) {
			a_vm->TraceStack("Book is None", a_stackID);
			return;
		}

		a_book->data.flags.reset(RE::OBJ_BOOK::Flag::kCantTake);
	}

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

	void SetBookCantBeTakenFlag(STATIC_ARGS, RE::TESObjectBOOK* a_book)
	{
		if (!a_book) {
			a_vm->TraceStack("Book is None", a_stackID);
			return;
		}

		a_book->data.flags.reset(RE::OBJ_BOOK::Flag::kCantTake);
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
		std::uint32_t count = 0;

		BIND(ClearBookCantBeTakenFlag);
		BIND(ClearReadFlag);
		BIND(SetBookCantBeTakenFlag);
		BIND(SetReadFlag);

		logger::info("Registered {} book functions"sv, count);
	}
}
