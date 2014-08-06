#include <ctime>
#include "irrlicht.h"
#include "Parser.h"
#include "Level.h"
#include "Element.h"
#include "PlayerElement.h"
#include "ParticleElement.h"
#include "ExplosiveBehavior.h"

#define PI 3.14159265358979323846

using namespace irr;

ExplosiveBehavior::ExplosiveBehavior(Element* element, std::istream& stream)
 : ExplosiveBehavior(element, Parser(stream, ',').getArgs<unsigned, unsigned, float>())
{
}

ExplosiveBehavior::ExplosiveBehavior(Element* element, std::tuple<unsigned, unsigned, float> data)
 : ExplosiveBehavior(element,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data))
{
}

ExplosiveBehavior::ExplosiveBehavior(Element* element, unsigned damage, unsigned timeout, float range)
 : Behavior(element, Type::UNSTABLE)
 , m_triggered(false)
 , m_explosionTimer(0)
 , m_damage(damage)
 , m_timeout(timeout)
 , m_range(range)
{
}

ExplosiveBehavior::~ExplosiveBehavior()
{
}

void ExplosiveBehavior::update(uint32_t elapsedMs)
{
    if (m_element == nullptr) return; // no element or already triggered (fallen down)

    if (m_triggered)
    {
        m_explosionTimer += elapsedMs;

        if (m_explosionTimer >= m_timeout)
        {
            auto explosionPos = m_element->getPosition();

            // damaging player
            PlayerElement* player = m_element->getLevel()->getPlayerElement();
            if (player && explosionPos.getDistanceFrom(player->getPosition()) <= m_range)
            {
                player->takeDamage(m_damage);
            }

            // explosion effect
            for (int deg = 0; deg <= 360; deg += 10)
            {
                unsigned intensity = rand() % 50;
                video::SColor color(255, rand() % 256, 0, 0);

                Element* particle = new ParticleElement(m_element->getLevel(), color, explosionPos, 3000 + rand()%500);
                particle->setMovementX( sin((float)deg / (2 * PI)) * intensity );
                particle->setMovementX( cos((float)deg / (2 * PI)) * intensity );
            }

            // remove exploded element from scene
            m_element->remove();
        }
    }
    else
    {
        m_element->updateCollisions();
        auto collisions = m_element->getCollisions();

        for (auto collision : collisions)
        {
            Element* contactElem = collision.getOtherElement();

            if (contactElem->getType() == Element::Type::PLAYER)
            {
                m_triggered = true;
                return;
            }
        }
    }
}
