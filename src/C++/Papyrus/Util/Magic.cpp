#include "Magic.h"

namespace MAGIC
{
	std::string_view get_archetype_as_string(Archetype a_archetype)
	{
		auto it = archetypeMap.find(a_archetype);
		return it != archetypeMap.end() ? it->second : "";
	}
}
