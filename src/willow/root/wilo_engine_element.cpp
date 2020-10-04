#include"willow/root/wilo_engine_element.hpp"

namespace wlo {

	EngineElement::EngineElement()
	{
		m_ID = ObjectCounter::getCoreElementUniqueID();
	}

	EngineElement::EngineElement(ID_type id)
	{
		m_ID = id;
        //this comment should be in all of these files!
	}

	EngineElement::~EngineElement() {}
}
