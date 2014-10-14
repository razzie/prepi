#include "Parser.h"
#include "elements\Element.h"
#include "behaviors\Behavior.h"
#include "behaviors\UnstableBehavior.h"
#include "behaviors\ExplosiveBehavior.h"
#include "behaviors\TeleportBehavior.h"
#include "behaviors\ResizerBehavior.h"
#include "behaviors\LeafEffectBehavior.h"
#include "behaviors\ContainerBehavior.h"
//#include "behaviors\TrapBehavior.h"
#include "behaviors\LadderBehavior.h"
#include "behaviors\TextBehavior.h"
#include "behaviors\FanBehavior.h"

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

        case Behavior::Type::RESIZER:
            return new ResizerBehavior(element, stream);

        case Behavior::Type::LEAF_EFFECT:
            return new LeafEffectBehavior(element, stream);

        case Behavior::Type::CONTAINER:
            return new ContainerBehavior(element, stream);

        //case Behavior::Type::TRAP:
        //    return new TrapBehavior(element, stream);

        case Behavior::Type::LADDER:
            return new LadderBehavior(element);

        case Behavior::Type::CLIMBING:
            return new Behavior(element, Behavior::Type::CLIMBING);

        case Behavior::Type::TEXT:
            return new TextBehavior(element, stream);

        case Behavior::Type::FAN:
            return new FanBehavior(element, stream);

        case Behavior::Type::CHECKPOINT:
            return new Behavior(element, Behavior::Type::CHECKPOINT);

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
        case 5: // LEAF_EFFECT
        case 6: // CONTAINER
        case 7: // TRAP
        case 8: // LADDER
        case 9: // CLIMBING
        case 10: // TEXT
        case 11: // FAN
        case 12: // CHECKPOINT
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
