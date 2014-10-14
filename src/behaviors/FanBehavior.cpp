//#include "Globals.h"
#include "Parser.h"
#include "elements\Element.h"
#include "elements\PlayerElement.h"
#include "level\Level.h"
#include "behaviors\FanBehavior.h"

using namespace irr;

#define RANGE 1.f

static bool isPointRight(core::line2df l, core::vector2df p)
{
    l.end -= l.start;
    float v = (l.end.Y * (p.X - l.start.X)) + (l.end.X * (l.start.Y - p.Y));
    return (v > 0);
}

static bool isRectRight(core::line2df l, core::rectf r)
{
    if (isPointRight(l, r.UpperLeftCorner) ||
        isPointRight(l, {r.LowerRightCorner.X, r.UpperLeftCorner.Y}) ||
        isPointRight(l, r.LowerRightCorner) ||
        isPointRight(l, {r.UpperLeftCorner.X, r.LowerRightCorner.Y}))
    {
        return true;
    }
    else
    {
        return false;
    }
}

FanBehavior::FanBehavior(Element* element, std::istream& stream)
 : FanBehavior(element, Parser(stream, ',').getArgs<core::vector2df, bool>())
{
}

FanBehavior::FanBehavior(Element* element, std::tuple<core::vector2df, bool> data)
 : FanBehavior(element,
    std::get<0>(data),
    std::get<1>(data))
{
}

FanBehavior::FanBehavior(Element* element, core::vector2df velocity, bool reverse)
 : Behavior(element, Type::FAN)
 , m_velocity(velocity)
 , m_reverse(reverse)
{
    core::vector2df norm = velocity;
    norm.normalize();

    float scale = (m_element == nullptr) ? 0.5f : (m_element->getScale() / 2);

    /*
     * p2_____p3
     *  |     |
     *  |     |
     * p1__.__p0
     *    0:0
     */

    m_points[0] = core::vector2df(norm.Y, -norm.X) * scale;
    m_points[1] = core::vector2df(-norm.Y, norm.X) * scale;
    m_points[2] = m_points[1] + velocity * RANGE;
    m_points[3] = m_points[0] + velocity * RANGE;
}

FanBehavior::~FanBehavior()
{
}

void FanBehavior::update(uint32_t elapsedMs)
{
    if (m_element == nullptr) return;

    PlayerElement* player = m_element->getLevel()->getPlayerElement();

    if (player == nullptr) return;

    core::vector2df fanPos = m_element->getPosition() + m_element->getBoundingBox().getCenter();
    core::vector2df playerPos = player->getPosition() + player->getBoundingBox().getCenter();
    core::rectf playerBox = player->getBoundingBox() + player->getPosition();

    /*Level* level = m_element->getLevel();
    video::IVideoDriver* driver = level->getGlobals()->driver;

    driver->draw2DLine(level->getScreenPosition(fanPos + m_points[0]), level->getScreenPosition(fanPos + m_points[1]));
    driver->draw2DLine(level->getScreenPosition(fanPos + m_points[1]), level->getScreenPosition(fanPos + m_points[2]));
    driver->draw2DLine(level->getScreenPosition(fanPos + m_points[2]), level->getScreenPosition(fanPos + m_points[3]));
    driver->draw2DLine(level->getScreenPosition(fanPos + m_points[3]), level->getScreenPosition(fanPos + m_points[0]));*/

    if (isRectRight({fanPos + m_points[0], fanPos + m_points[1]}, playerBox) &&
        isRectRight({fanPos + m_points[1], fanPos + m_points[2]}, playerBox) &&
        isRectRight({fanPos + m_points[2], fanPos + m_points[3]}, playerBox) &&
        isRectRight({fanPos + m_points[3], fanPos + m_points[0]}, playerBox))
    /*if (isPointRight({fanPos + m_points[0], fanPos + m_points[1]}, playerPos) &&
        isPointRight({fanPos + m_points[1], fanPos + m_points[2]}, playerPos) &&
        isPointRight({fanPos + m_points[2], fanPos + m_points[3]}, playerPos) &&
        isPointRight({fanPos + m_points[3], fanPos + m_points[0]}, playerPos))*/
    {
        std::cout << "Player in fan range" << std::endl;

        float speed = (float)elapsedMs / 100.f;
        float dist = fanPos.getDistanceFrom(playerPos) / (RANGE * 2.f) + 0.01f; // shouldn't be 0
        core::vector2df accel = speed * m_velocity / dist;
        core::vector2df playerMovement = player->getMovement();
        playerMovement += (m_reverse) ? -accel : accel;

        player->setMovementX(playerMovement.X);
        player->setMovementY(playerMovement.Y);
    }
}
