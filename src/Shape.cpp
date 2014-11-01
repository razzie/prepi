#include <stdexcept>
#include "Box2D\Box2D.h"
#include "Globals.h"
#include "level\Level.h"
#include "Parser.h"
#include "Shape.h"

#define PI 3.14159265358979323846

using namespace irr;

std::istream& operator>> (std::istream&, core::vector2df&);

template<class T>
static void copyContentTo(const T& src, T& dest)
{
    dest.insert(dest.end(), src.begin(), src.end());
}

static core::rectf getPointArrayBoundings(const PointArray& points)
{
    if (points.empty())
    {
        return {0.f, 0.f, 0.f, 0.f};
    }
    else
    {
        core::rectf box{points[0], points[0]};
        for (auto p : points) box.addInternalPoint(p);
        return box;
    }
}

float polygonArea(const PointArray& points)
{
    float area = 0.f; // Accumulates area in the loop
    unsigned j = points.size() - 1; // The last vertex is the 'previous' one to the first

    for (unsigned i = 0, len = points.size(); i < len; ++i)
    {
        area += (points[j].X + points[i].X) * (points[j].Y - points[i].Y);
        j = i; //j is previous vertex to i
    }

    return (area / -2.f);
}

static bool isPointRight(core::line2df l, core::vector2df p)
{
    l.end -= l.start;
    float v = (l.end.Y * (p.X - l.start.X)) + (l.end.X * (l.start.Y - p.Y));
    return (v <= 0);
}

Shape::Shape()
 : m_type(Type::BOX)
 , m_boxData(0.f, 0.f, 1.f, 1.f)
 , m_sphereData {{0.5f, 0.5f}, 0.5f}
 , m_polygonData {{0.f, 0.f}, {1.f, 0.f}, {1.f, 1.f}, {0.f, 1.f}}
{
}

Shape::Shape(std::istream& stream)
{
    Parser p(stream, ',');
    m_type = static_cast<Type>(p.getArg<unsigned>());

    switch (m_type)
    {
        case Type::BOX:
            std::tie(m_boxData.UpperLeftCorner, m_boxData.LowerRightCorner) = p.getArgs<core::vector2df, core::vector2df>();
            break;

        case Type::SPHERE:
            std::tie(m_sphereData.m_center, m_sphereData.m_radius) = p.getArgs<core::vector2df, f32>();
            m_boxData.UpperLeftCorner = m_sphereData.m_center - core::vector2df(m_sphereData.m_radius, m_sphereData.m_radius);
            m_boxData.LowerRightCorner = m_sphereData.m_center + core::vector2df(m_sphereData.m_radius, m_sphereData.m_radius);
            break;

        case Type::POLYGON:
            {
                unsigned n = p.getArg<unsigned>();
                for (unsigned i = 0; i < n; ++i)
                {
                    core::vector2df pos = p.getArg<core::vector2df>();
                    m_polygonData.push_back(pos);
                }
                m_boxData = getPointArrayBoundings(m_polygonData);
                break;
            }

        default:
            throw std::runtime_error("unknown shape type");
    }
}

Shape::Shape(const core::rectf& box)
{
    *this = box;
}

Shape::Shape(const SphereData& sphere)
{
    *this = sphere;
}

Shape::Shape(const PointArray& points)
{
    *this = points;
}

Shape::Shape(const Shape& shape)
{
    *this = shape;
}

Shape::Shape(Shape&& shape)
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
    return *this;
}

Shape& Shape::operator= (const SphereData& sphere)
{
    m_type = Type::SPHERE;
    m_boxData.UpperLeftCorner = sphere.m_center - core::vector2df(sphere.m_radius, sphere.m_radius);
    m_boxData.LowerRightCorner = sphere.m_center + core::vector2df(sphere.m_radius, sphere.m_radius);
    m_sphereData = sphere;
    return *this;
}

Shape& Shape::operator= (const PointArray& points)
{
    m_type = Type::POLYGON;
    m_boxData = getPointArrayBoundings(points);
    m_polygonData.clear();
    copyContentTo(points, m_polygonData);
    return *this;
}

Shape& Shape::operator= (const Shape& shape)
{
    m_type = shape.m_type;
    m_boxData = shape.m_boxData;
    m_sphereData = shape.m_sphereData;
    m_polygonData.clear();
    copyContentTo(shape.m_polygonData, m_polygonData);
    return *this;
}

Shape& Shape::operator= (Shape&& shape)
{
    m_type = shape.m_type;
    m_boxData = std::move(shape.m_boxData);
    m_sphereData = std::move(shape.m_sphereData);
    m_polygonData = std::move(shape.m_polygonData);
    return *this;
}

Shape Shape::operator+ (core::vector2df v) const
{
    Shape shape = *this;
    shape += v;
    return shape;
}

Shape& Shape::operator+= (core::vector2df v)
{
    m_boxData += v;
    m_sphereData.m_center += v;
    for (auto& p : m_polygonData) p += v;
    return *this;
}

Shape Shape::operator* (float scale) const
{
    Shape shape = *this;
    shape *= scale;
    return shape;
}

Shape& Shape::operator*= (float scale)
{
    m_boxData.UpperLeftCorner *= scale;
    m_boxData.LowerRightCorner *= scale;
    m_sphereData.m_center *= scale;
    m_sphereData.m_radius *= scale;
    for (auto& p : m_polygonData) p *= scale;
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

const PointArray& Shape::getPolygonData() const
{
    return m_polygonData;
}

float Shape::getArea() const
{
    switch (m_type)
    {
        case Type::BOX:
            return m_boxData.getArea();

        case Type::SPHERE:
            return (4 * m_sphereData.m_radius * m_sphereData.m_radius * PI);

        case Type::POLYGON:
            return polygonArea(m_polygonData);
    }

    return 0.f;
}

bool Shape::isPointInside(core::vector2df p) const
{
    switch (m_type)
    {
        case Type::BOX:
            return m_boxData.isPointInside(p);

        case Type::SPHERE:
            return (p.getDistanceFrom(m_sphereData.m_center) <= m_sphereData.m_radius);

        case Type::POLYGON:
            for (unsigned i = 0, len = m_polygonData.size(); i != len; ++i)
            {
                if (!isPointRight({m_polygonData[i], m_polygonData[(i+1)%len]}, p)) return false;
            }
            return true;
    }

    return false;
}

void Shape::addToBody(b2Body* body, float scale) const
{
    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.5f;

    switch (m_type)
    {
        case Type::BOX:
            {
                core::rectf boundingBox = m_boxData;
                boundingBox.UpperLeftCorner *= scale;
                boundingBox.LowerRightCorner *= scale;

                b2PolygonShape boxShape;
                boxShape.SetAsBox(boundingBox.getWidth()/2 - 0.01f, boundingBox.getHeight()/2 - 0.01f,
                                   { boundingBox.getWidth()/2 - (1.f - boundingBox.UpperLeftCorner.X),
                                     boundingBox.getHeight()/2 - (1.f - boundingBox.UpperLeftCorner.Y) },
                                   0.f);

                fixtureDef.shape = &boxShape;
                body->CreateFixture(&fixtureDef);
                break;
            }

        case Type::SPHERE:
            {
                Shape::SphereData boundingSphere = m_sphereData;
                boundingSphere.m_radius *= scale;
                boundingSphere.m_center *= scale;

                b2CircleShape circleShape;
                circleShape.m_p.Set(boundingSphere.m_center.X - 1.f, boundingSphere.m_center.Y - 1.f);
                circleShape.m_radius = boundingSphere.m_radius - 0.02f;

                fixtureDef.shape = &circleShape;
                body->CreateFixture(&fixtureDef);
                break;
            }

        case Type::POLYGON:
            {
                b2Vec2 vertices[b2_maxPolygonVertices];
                int i = 0;
                for (auto it = m_polygonData.begin(), end = m_polygonData.end(); it != end; ++it)
                {
                    core::vector2df p = *it;
                    p *= scale;
                    p -= core::vector2df(1.f, 1.f);
                    vertices[i++].Set(p.X, p.Y);
                }
                b2PolygonShape polygonShape;
                polygonShape.Set(vertices, m_polygonData.size());

                fixtureDef.shape = &polygonShape;
                body->CreateFixture(&fixtureDef);
                break;
            }
    }

    return;
}

void Shape::draw(Level* level, core::vector2df pos) const
{
    unsigned unit = level->getUnitSize();

    switch (m_type)
    {
        case Type::BOX:
            {
                core::rectf box = m_boxData;
                box += pos;

                core::recti pixelBox( (s32)(box.UpperLeftCorner.X * unit),  (s32)(box.UpperLeftCorner.Y * unit),
                                      (s32)(box.LowerRightCorner.X * unit), (s32)(box.LowerRightCorner.Y * unit) );
                pixelBox -= level->getViewOffset();

                level->getGlobals()->driver->draw2DRectangleOutline(pixelBox);
            }
            break;

        case Type::SPHERE:
            {
                core::vector2di center = level->getScreenPosition(m_sphereData.m_center + pos);
                float radius = m_sphereData.m_radius * level->getUnitSize();
                level->getGlobals()->driver->draw2DPolygon(center, radius, {255, 255, 255, 255});
            }
            break;

        case Type::POLYGON:
            for (unsigned i = 0, len = m_polygonData.size(); i < len; ++i)
            {
                core::vector2di p1 = level->getScreenPosition(m_polygonData[i % len] + pos);
                core::vector2di p2 = level->getScreenPosition(m_polygonData[(i+1) % len] + pos);
                level->getGlobals()->driver->draw2DLine(p1, p2);
            }
            break;
    }
}


std::istream& operator>> (std::istream& stream, Shape& shape)
{
    shape = Shape(stream);
    return stream;
}

std::istream& operator>> (std::istream& stream, PointArray& pointArray)
{
    Parser p(stream, ',');

    //while (p.hasNextArg())
    for (unsigned i = 0, num = p.getArg<unsigned>(); i < num; ++i)
    {
        core::vector2df point = p.getArg<core::vector2df>();
        pointArray.push_back(point);
    }

    return stream;
}
