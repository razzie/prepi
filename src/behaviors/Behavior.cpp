#include "Parser.h"
#include "elements\Element.h"
#include "behaviors\Behavior.h"
#include "behaviors\UnstableBehavior.h"
#include "behaviors\ExplosiveBehavior.h"
#include "behaviors\TeleportBehavior.h"
//#include "behaviors\ResizerBehavior.h"

Behavior::Behavior(Element* element, Type type)
 : m_type(type)
 , m_element(nullptr)
{
    Behavior* behavior = this; // this is for calling setElement virtual method of derived class
    if (element) behavior->setElement(element);
}

Behavior::~Behavior()
{
    if (m_element != nullptr) m_element->m_behavior = nullptr;
}

Behavior::Type Behavior::getType() const
{
    return m_type;
}

void Behavior::setElement(Element* element)
{
    if (m_element != nullptr) m_element->m_behavior = nullptr;
    if (element != nullptr) element->m_behavior = this;
    m_element = element;
}

Element* Behavior::getElement()
{
    return m_element;
}

const Element* Behavior::getElement() const
{
    return m_element;
}

void Behavior::update(uint32_t)
{
}

Behavior* CreateBehavior(Element* element, std::istream& stream)
{
    Behavior::Type type = Parser(stream, ';').getArg<Behavior::Type>();

    switch (type)
    {
        case Behavior::Type::UNSTABLE:
            return new UnstableBehavior(element, stream);

        case Behavior::Type::EXPLOSIVE:
            return new ExplosiveBehavior(element, stream);

        case Behavior::Type::TELEPORT:
            return new TeleportBehavior(element, stream);

        /*case Behavior::Type::RESIZER:
            return new ResizerBehavior(element, stream);*/

        case Behavior::Type::NONE:
        default:
            return nullptr;
    }
}

std::istream& operator>> (std::istream& stream, Behavior::Type& m)
{
    unsigned behavior;
    stream >> behavior;

    switch (behavior)
    {
        case 0: // NONE
        case 1: // UNSTABLE
        case 2: // EXPLOSIVE
        case 3: // TELEPORT
        case 4: // RESIZER
            m = static_cast<Behavior::Type>(behavior);
            break;

        default:
            m = Behavior::Type::NONE;
            throw std::runtime_error("unknown behavior");
    }

    return stream;
}

std::istream& operator>> (std::istream& stream, BehaviorWrapper& wrapper)
{
    wrapper.behavior = CreateBehavior(wrapper.element, stream);
    return stream;
}
