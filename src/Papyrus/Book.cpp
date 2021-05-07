#include "Papyrus/Book.h"


void papyrusBook::ClearReadFlag(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectBOOK* a_book)
{
	if (!a_book) {
		a_vm->TraceStack("Book is None", a_stackID, Severity::kWarning);
		return;
	}

	if (a_book->IsRead()) {
		a_book->data.flags.reset(RE::OBJ_BOOK::Flag::kHasBeenRead);
		a_book->RemoveChange(RE::TESObjectBOOK::ChangeFlags::kRead);
	}
}


void papyrusBook::SetReadFlag(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectBOOK* a_book)
{
	if (!a_book) {
		a_vm->TraceStack("Book is None", a_stackID, Severity::kWarning);
		return;
	}

	if (!a_book->IsRead()) {
		a_book->data.flags.set(RE::OBJ_BOOK::Flag::kHasBeenRead);
		a_book->AddChange(RE::TESObjectBOOK::ChangeFlags::kRead);
	}
}


bool papyrusBook::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		logger::critical("papyrusBook - couldn't get VMState"sv);
		return false;
	}

	auto constexpr Functions = "PO3_SKSEFunctions"sv;

	a_vm->RegisterFunction("ClearReadFlag"sv, Functions, ClearReadFlag);

	a_vm->RegisterFunction("SetReadFlag"sv, Functions, SetReadFlag);

	return true;
}
