#pragma once


namespace Serialize
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
		struct FormData
		{
			RE::FormID _formID;
			RE::FormID _dataID;
		};

		Base();
		Base(Base&) = default;
		Base(Base&&) = default;
		~Base() = default;

		Base& operator=(const Base&) = default;
		Base& operator=(Base&&) = default;


		virtual std::vector<FormData>& GetData(std::uint32_t a_add);
		virtual const char* GetType(std::uint32_t a_add) = 0;
		virtual void LoadData(FormData a_data, std::uint32_t a_add) = 0;
		void SaveData(FormData a_newData, std::uint32_t a_add);

		void Clear(std::uint32_t a_add);
		bool Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_type, std::uint32_t a_version, std::uint32_t a_add);
		bool Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_add);
		bool Load(SKSE::SerializationInterface* a_intfc, std::uint32_t a_add);

	protected:
		std::vector<FormData> _add;
		std::vector<FormData> _remove;
	};
}
