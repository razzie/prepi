#ifndef SHAPE_H_INCLUDED
#define SHAPE_H_INCLUDED

#include <iostream>
#include <vector>
#include "irrlicht.h"

typedef std::vector<irr::core::vector2df> PointArray;

class b2Body;
class Level;

class Shape
{
public:
    enum class Type : unsigned
    {
        BOX = 0,
        SPHERE = 1,
        POLYGON = 2
    };

    struct SphereData
    {
        irr::core::vector2df m_center;
        irr::f32 m_radius;
    };

    Shape();
    Shape(std::istream&);
    Shape(const irr::core::rectf&);
    Shape(const SphereData&);
    Shape(const PointArray&);
    Shape(const Shape&);
    Shape(Shape&&);
    ~Shape();
    Shape& operator= (const irr::core::rectf&);
    Shape& operator= (const SphereData&);
    Shape& operator= (const PointArray&);
    Shape& operator= (const Shape&);
    Shape& operator= (Shape&&);
    Shape  operator+ (irr::core::vector2df) const;
    Shape& operator+=(irr::core::vector2df);
    Shape  operator* (float scale) const;
    Shape& operator*=(float scape);

    Type getType() const;
    irr::core::rectf getBoxData() const; // bounding box is generated for all types
    SphereData getSphereData() const;
    const PointArray& getPolygonData() const;
    float getArea() const;
    bool isPointInside(irr::core::vector2df) const;
    void addToBody(b2Body* body, float scale = 1.0f) const;
    void draw(Level*, irr::core::vector2df) const;

private:
    Type m_type;
    irr::core::rectf m_boxData;
    SphereData m_sphereData;
    PointArray m_polygonData;
};

std::istream& operator>> (std::istream&, Shape&);
std::istream& operator>> (std::istream&, PointArray&);

#endif // SHAPE_H_INCLUDED
