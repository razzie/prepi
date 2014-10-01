#ifndef SHAPE_H_INCLUDED
#define SHAPE_H_INCLUDED

#include "irrlicht.h"

class b2Body;

class Shape
{
public:
    enum Type
    {
        BOX = 0,
        SPHERE = 1
    };

    struct SphereData
    {
        irr::core::vector2df center;
        irr::f32 radius;
    };

    Shape();
    Shape(const irr::core::rectf&);
    Shape(const SphereData&);
    Shape(const Shape&);
    ~Shape();
    Shape& operator= (const irr::core::rectf&);
    Shape& operator= (const SphereData&);
    Shape& operator= (const Shape&);

    Type getType() const;
    irr::core::rectf getBoxData() const;
    SphereData getSphereData() const;
    void addToBody(b2Body* body, float scale = 1.0f) const;

private:
    Type m_type;
    irr::core::rectf m_boxData;
    SphereData m_sphereData;
};

#endif // SHAPE_H_INCLUDED
