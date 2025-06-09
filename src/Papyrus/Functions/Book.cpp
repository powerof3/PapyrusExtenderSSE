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

	void ShowBookMenu(STATIC_ARGS, RE::TESObjectBOOK* a_book)
	{
		if (!a_book) {
			a_vm->TraceStack("Book is None", a_stackID);
			return;
		}
		
		RE::NiMatrix3 rot{};
		rot.SetEulerAnglesXYZ(-0.05f, -0.05f, 1.50f);

		RE::BSString str;
		a_book->GetDescription(str, nullptr);
		logger::info("{}", str.c_str());

		RE::BookMenu::OpenBookMenu(str, nullptr, nullptr, a_book, RE::NiPoint3(), rot, 1.0f, true);
	}

	void Bind(VM& a_vm)
	{
		BIND(ClearBookCantBeTakenFlag);
		BIND(ClearReadFlag);
		BIND(SetBookCantBeTakenFlag);
		BIND(SetReadFlag);
		BIND(ShowBookMenu);

		logger::info("Registered book functions"sv);
	}
}
