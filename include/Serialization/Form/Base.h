#pragma once


namespace Serialization
{
	namespace Form
	{
		enum : std::uint32_t
		{
			kInvalid = static_cast<std::uint32_t>(-1),
			kRemove = 0,
			kAdd = 1
		};


		class Base
		{
		public:
			using FormData = std::pair<RE::FormID, RE::FormID>;

			Base();
			Base(Base&) = default;
			Base(Base&&) = default;
			virtual ~Base() = default;

			Base& operator=(const Base&) = default;
			Base& operator=(Base&&) = default;

			virtual std::set<FormData>& GetData(std::uint32_t a_add);
			virtual void LoadData(std::uint32_t a_add) = 0;

			void SaveData(FormData a_newData, std::uint32_t a_add);

			void Clear(std::uint32_t a_add);
			void ClearAll();
			bool Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_type, std::uint32_t a_version, std::uint32_t a_add);
			bool Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_add);
			bool Load(SKSE::SerializationInterface* a_intfc, std::uint32_t a_add);

		protected:
			using Lock = std::recursive_mutex;
			using Locker = std::lock_guard<Lock>;

			std::set<FormData> _add;
			std::set<FormData> _remove;
			mutable Lock _lock;
		};

	}
}
