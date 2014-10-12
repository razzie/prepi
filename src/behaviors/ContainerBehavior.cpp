#include <sstream>
#include "Parser.h"
#include "elements\Element.h"
#include "level\Level.h"
#include "effects\EffectManager.h"
#include "behaviors\ContainerBehavior.h"

using namespace irr;

ContainerBehavior::ContainerBehavior(Element* element, std::istream& stream)
 : Behavior(element, Type::CONTAINER)
 , m_elemData(Parser(stream, '"').getString())
{
}

ContainerBehavior::ContainerBehavior(Element* element, std::string elemData)
 : Behavior(element, Type::CONTAINER)
 , m_elemData(elemData)
{
}

ContainerBehavior::~ContainerBehavior()
{
}

void ContainerBehavior::update(uint32_t)
{
    if (m_element == nullptr) return;

    if (m_element->isPlayerCollided())
    {
        try
        {
            std::stringstream ss(m_elemData);
            CreateElement(m_element->getLevel(), ss);
        }
        catch (std::exception& e)
        {
            std::cout << "Failed to create contained element: " << m_elemData << " (" << e.what() << ")" << std::endl;
        }

        m_element->getLevel()->getEffectManager()->fall(m_element);
        m_element->remove();
    }
}
