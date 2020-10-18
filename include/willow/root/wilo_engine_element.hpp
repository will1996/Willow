#ifndef WILO_ENGINE_COMPONENT_H
#define WILO_ENGINE_COMPONENT_H
#include "wilo_object_counter.hpp"
namespace wlo {

	class EngineElement{
		/*base engine component for , contains an ID, from the object counter as well as the pure virtual methods initiaze and reclaim */
	public:
		EngineElement();
		EngineElement(ID_type);

		virtual ~EngineElement();
		ID_type m_ID;
		};


}
#endif