#include "Shape.h"

using namespace irr;

Shape::Shape()
 : m_type(Type::BOX)
 , m_boxData(0.f, 0.f, 1.f, 1.f)
 , m_sphereData {{0.5f, 0.5f}, 0.5f}
{
}

Shape::Shape(const core::rectf& box)
{
    *this = box;
}

Shape::Shape(const SphereData& sphere)
{
    *this = sphere;
}

Shape::Shape(const Shape& shape)
{
    *this = shape;
}

Shape::~Shape()
{
}

Shape& Shape::operator= (const core::rectf& box)
{
    m_type = Type::BOX;
    m_boxData = box;
    m_sphereData.center.X = box.UpperLeftCorner.X + box.getWidth()/2;
    m_sphereData.center.Y = box.UpperLeftCorner.Y + box.getHeight()/2;
    m_sphereData.radius = (box.getWidth() > box.getHeight()) ? (box.getWidth()/2) : (box.getHeight()/2);
    return *this;
}

Shape& Shape::operator= (const SphereData& sphere)
{
    m_type = Type::SPHERE;
    m_sphereData = sphere;
    m_boxData.UpperLeftCorner = sphere.center - core::vector2df(sphere.radius, sphere.radius);
    m_boxData.LowerRightCorner = sphere.center + core::vector2df(sphere.radius, sphere.radius);
    return *this;
}

Shape& Shape::operator= (const Shape& shape)
{
    m_type = shape.m_type;
    m_boxData = shape.m_boxData;
    m_sphereData = shape.m_sphereData;
    return *this;
}

Shape::Type Shape::getType() const
{
    return m_type;
}

core::rectf Shape::getBoxData() const
{
    return m_boxData;
}

Shape::SphereData Shape::getSphereData() const
{
    return m_sphereData;
}
