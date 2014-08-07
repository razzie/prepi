#include "Parser.h"
#include "elements\Element.h"
#include "motions\Motion.h"
#include "motions\AutoMotion.h"
#include "motions\StraightMotion.h"
#include "motions\CircularMotion.h"

Motion::Motion(Element* element, Type type)
 : m_type(type)
 , m_element(nullptr)
{
    Motion* motion = this; // this is for calling setElement virtual method of derived class
    if (element) motion->setElement(element);
}

Motion::~Motion()
{
    if (m_element != nullptr) m_element->m_motion = nullptr;
}

Motion::Type Motion::getType() const
{
    return m_type;
}

void Motion::setElement(Element* element)
{
    if (m_element != nullptr) m_element->m_motion = nullptr;
    if (element != nullptr) element->m_motion = this;
    m_element = element;
}

Element* Motion::getElement()
{
    return m_element;
}

const Element* Motion::getElement() const
{
    return m_element;
}

void Motion::update(uint32_t)
{
}

Motion* CreateMotion(Element* element, std::istream& stream)
{
    Motion::Type type = Parser(stream, ';').getArg<Motion::Type>();

    switch (type)
    {
        case Motion::Type::STATIC:
        case Motion::Type::DYNAMIC:
            return new Motion(element, type);

        case Motion::Type::AUTO:
            return new AutoMotion(element, stream);

        case Motion::Type::STRAIGHT:
            return new StraightMotion(element, stream);

        case Motion::Type::CIRCULAR:
            return new CircularMotion(element, stream);

        case Motion::Type::NONE:
        default:
            return nullptr;
    }
}

std::istream& operator>> (std::istream& stream, Motion::Type& m)
{
    unsigned motion;
    stream >> motion;

    switch (motion)
    {
        case 0: // BACK
        case 2: // FORE
            m = Motion::Type::NONE;
            break;

        case 3: // STATIC
        case 4: // DYNAMIC
        case 5: // AUTO
        case 6: // STRAIGHT
        case 7: // CIRCULAR
            m = static_cast<Motion::Type>(motion);
            break;

        default:
            m = Motion::Type::STATIC;
    }

    return stream;
}

std::istream& operator>> (std::istream& stream, MotionWrapper& wrapper)
{
    wrapper.motion = CreateMotion(wrapper.element, stream);
    return stream;
}