#include "Parser.h"
#include "Color.h"
#include "elements\Element.h"
#include "level\Level.h"
#include "effects\EffectManager.h"
#include "behaviors\TextBehavior.h"

using namespace irr;

TextBehavior::TextBehavior(Element* element, std::istream& stream)
// : TextBehavior(element, Parser(stream, ',').getArgs<std::string, video::SColor>())
 : Behavior(element, Type::TEXT)
{
    Parser p(stream, ',');
    m_text = p.getString();
    m_color = p.getArg<video::SColor>();
}

/*TextBehavior::TextBehavior(Element* element, std::tuple<std::string, video::SColor> data)
 : TextBehavior(element,
    std::get<0>(data),
    std::get<1>(data))
{
}*/

TextBehavior::TextBehavior(Element* element, std::string text, video::SColor color)
 : Behavior(element, Type::TEXT)
 , m_text(text)
 , m_color(color)
 , m_triggered(false)
{
}

TextBehavior::~TextBehavior()
{
}

void TextBehavior::update(uint32_t)
{
    if (m_element == nullptr) return;

    if (m_element->isPlayerCollided())
    {
        if (!m_triggered)
        {
            m_element->getLevel()->getEffectManager()->text(m_text.c_str(), m_element, m_color);
        }

        m_triggered = true;
    }
    else
    {
        m_triggered = false;
    }
}
