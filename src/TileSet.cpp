#include <dirent.h>
#include <fstream>
#include "Box2D\Box2D.h"
#include "Globals.h"
#include "Level.h"
#include "TileSet.h"
//#include "Element.h"
//#include "Motion.h"
#include "Parser.h"

using namespace irr;

static b2BodyType motionTypeToBodyType(Motion::Type motionType)
{
    switch (motionType)
    {
        case Motion::Type::DYNAMIC:
        case Motion::Type::AUTO:
            return b2_dynamicBody;

        case Motion::Type::STRAIGHT:
        case Motion::Type::CIRCULAR:
            return b2_kinematicBody;

        case Motion::Type::STATIC:
        case Motion::Type::UNSTABLE:
        default:
            return b2_staticBody;
    }
}

Shape TileData::getBoundingShape(core::vector2di imgPosition) const
{
    unsigned tileId = (tileDimension.X * imgPosition.Y) + imgPosition.X;
    auto it = boundings.find(tileId);
    if (it == boundings.end())
        return {};
    else
        return it->second;
}

b2Body* TileData::createBody(Element* element) const
{
    b2Body* body = nullptr;
    Motion::Type motionType = element->getMotionType();

    if (motionType != Motion::Type::NONE)
    {
        Shape shape = element->getTileData()->getBoundingShape(element->getImagePosition());
        core::vector2df position = element->getPosition();

        b2BodyDef bodyDef;
        bodyDef.type = motionTypeToBodyType(motionType);
        bodyDef.position.Set(position.X, position.Y);
        bodyDef.fixedRotation = true; // do not rotate!
        bodyDef.userData = element;
        body = element->getLevel()->getPhysics()->CreateBody(&bodyDef);

        b2FixtureDef fixtureDef;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.5f;

        switch (shape.getType())
        {
            case Shape::Type::BOX:
                {
                    core::rectf boundingBox = shape.getBoxData();
                    b2PolygonShape boxShape;
                    boxShape.SetAsBox(boundingBox.getWidth()/2 - 0.01f, boundingBox.getHeight()/2 - 0.01f,
                                       { boundingBox.getWidth()/2 - (1.f - boundingBox.UpperLeftCorner.X),
                                         boundingBox.getHeight()/2 - (1.f - boundingBox.UpperLeftCorner.Y) },
                                       0.f);

                    fixtureDef.shape = &boxShape;
                    body->CreateFixture(&fixtureDef);
                    break;
                }

            case Shape::Type::SPHERE:
                {
                    Shape::SphereData boundingSphere = shape.getSphereData();
                    b2CircleShape circleShape;
                    circleShape.m_p.Set(-boundingSphere.center.X, -boundingSphere.center.Y);
                    circleShape.m_radius = boundingSphere.radius - 0.02f;

                    fixtureDef.shape = &circleShape;
                    body->CreateFixture(&fixtureDef);
                }
        }
    }

    return body;
}


TileSet::TileSet(Globals* globals, std::string name)
 : m_globals(globals)
 , m_name(name)
{
    std::string basedir("../media/tilesets/");
    basedir += name;
    basedir += "/";

    findBackgroundData(basedir + "background/", m_backgrounds);
    findTileData(basedir + "ground/", m_grounds);
    findTileData(basedir + "enemy/", m_enemies);
    findTileData(basedir + "reward/", m_rewards);
    findTileData(basedir + "player/", m_players);
    findTileData(basedir + "finish/", m_finishes);
}

TileSet::~TileSet()
{
}

std::string TileSet::getName() const
{
    return m_name;
}

irr::video::ITexture* TileSet::getBackground(unsigned id, SearchType search) const
{
    if (search == SearchType::EXACT)
    {
        auto it = m_backgrounds.find(id);
        if (it != m_backgrounds.end())
        {
            BackgroundData* data = &(it->second);
            return data->texture;
        }
        else
            return nullptr;
    }
    else
    {
        if (search == SearchType::NEXT) ++id;

        for (auto& it : m_backgrounds)
        {
            if (it.first >= id) return it.second.texture;
        }

        return nullptr;
    }
}

const TileData* TileSet::getGroundData(unsigned id, SearchType search) const
{
    if (search == SearchType::EXACT)
    {
        auto it = m_grounds.find(id);
        if (it != m_grounds.end())
        {
            TileData* data = &(it->second);
            return data;
        }
        else
            return nullptr;
    }
    else
    {
        if (search == SearchType::NEXT) ++id;

        for (auto& it : m_grounds)
        {
            if (it.first >= id) return &(it.second);
        }

        return nullptr;
    }
}

const TileData* TileSet::getEnemyData(unsigned id, SearchType search) const
{
    if (search == SearchType::EXACT)
    {
        auto it = m_enemies.find(id);
        if (it != m_enemies.end())
        {
            TileData* data = &(it->second);
            return data;
        }
        else
            return nullptr;
    }
    else
    {
        if (search == SearchType::NEXT) ++id;

        for (auto& it : m_enemies)
        {
            if (it.first >= id) return &(it.second);
        }

        return nullptr;
    }
}

const TileData* TileSet::getRewardData(unsigned id, SearchType search) const
{
    if (search == SearchType::EXACT)
    {
        auto it = m_rewards.find(id);
        if (it != m_rewards.end())
        {
            TileData* data = &(it->second);
            return data;
        }
        else
            return nullptr;
    }
    else
    {
        if (search == SearchType::NEXT) ++id;

        for (auto& it : m_rewards)
        {
            if (it.first >= id) return &(it.second);
        }

        return nullptr;
    }
}

const TileData* TileSet::getPlayerData(unsigned id, SearchType search) const
{
    if (search == SearchType::EXACT)
    {
        auto it = m_players.find(id);
        if (it != m_players.end())
        {
            TileData* data = &(it->second);
            return data;
        }
        else
            return nullptr;
    }
    else
    {
        if (search == SearchType::NEXT) ++id;

        for (auto& it : m_players)
        {
            if (it.first >= id) return &(it.second);
        }

        return nullptr;
    }
}

const TileData* TileSet::getFinishData(unsigned id, SearchType search) const
{
    if (search == SearchType::EXACT)
    {
        auto it = m_finishes.find(id);
        if (it != m_finishes.end())
        {
            TileData* data = &(it->second);
            return data;
        }
        else
            return nullptr;
    }
    else
    {
        if (search == SearchType::NEXT) ++id;

        for (auto& it : m_finishes)
        {
            if (it.first >= id) return &(it.second);
        }

        return nullptr;
    }
}

const TileData* TileSet::getData(Element::Type type, unsigned id, SearchType search) const
{
    switch (type)
    {
        case Element::Type::GROUND:
            return getGroundData(id, search);

        case Element::Type::ENEMY:
            return getEnemyData(id, search);

        case Element::Type::REWARD:
            return getRewardData(id, search);

        case Element::Type::PLAYER:
            return getPlayerData(id, search);

        case Element::Type::FINISH:
            return getFinishData(id, search);

        default:
            return nullptr;
    }
}

bool TileSet::fillTileData(std::string dirName, std::string fileName, unsigned& id, TileData& data) const
{
    if (fileName.rfind(".png") == std::string::npos) return false; // not png

    try
    {
        data.fileName = dirName + fileName;
        data.texture = m_globals->driver->getTexture(data.fileName.c_str());

        Parser tileParser(fileName, '_');
        std::tie(id, data.tileSize, data.tileDimension.X, data.tileDimension.Y, data.tileCount) =
            tileParser.getArgs<unsigned, unsigned, int, int, unsigned>();

        std::string txtname = dirName + fileName;
        txtname.replace(txtname.end()-3, txtname.end(), "txt");

        std::fstream f(txtname);
        Parser bbParser(f, ',');

        try
        {
            // set up special bounding boxes
            do {
                unsigned id;
                unsigned shape;
                std::tie(id, shape) = bbParser.getArgs<unsigned, unsigned>();

                switch (shape)
                {
                    case Shape::Type::BOX:
                        {
                            core::rectf boundingBox;
                            std::tie(boundingBox.UpperLeftCorner, boundingBox.LowerRightCorner) = bbParser.getArgs<core::vector2df, core::vector2df>();
                            data.boundings[id] = boundingBox;
                            break;
                        }

                    case Shape::Type::SPHERE:
                        {
                            Shape::SphereData boundingSphere;
                            std::tie(boundingSphere.center, boundingSphere.radius) = bbParser.getArgs<core::vector2df, f32>();
                            data.boundings[id] = boundingSphere;
                            break;
                        }

                    default:
                        break;
                }
            }
            while (bbParser.nextLine());
        }
        catch (...) {}

        return true;
    }
    catch (const std::exception& e)
    {
        std::cout << "TileData error: " << fileName << " (" << e.what() << ")" << std::endl;
        return false;
    }
}

void TileSet::findTileData(std::string dirName, std::map<unsigned, TileData>& data) const
{
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir( dirName.c_str() )) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (ent->d_name[0] == '.')
                continue;

            unsigned id;
            TileData td;
            if (fillTileData(dirName, ent->d_name, id, td))
            {
                TileData& savedtd = data[id];
                savedtd = td;
                for (auto& it : td.boundings) savedtd.boundings.insert(it);
            }
        }
        closedir(dir);
    }
}

bool TileSet::fillBackgroundData(std::string dirName, std::string fileName, unsigned& id, BackgroundData& data) const
{
    try
    {
        Parser p(fileName, '_');

        data.fileName = dirName + fileName;
        data.texture = m_globals->driver->getTexture(data.fileName.c_str());
        id = p.getArg<unsigned>();

        return true;
    }
    catch (const std::exception& e)
    {
        std::cout << "BackgroundData error: " << fileName << " (" << e.what() << ")" << std::endl;
        return false;
    }
}

void TileSet::findBackgroundData(std::string dirName, std::map<unsigned, BackgroundData>& data) const
{
    DIR *dir;
    struct dirent *ent;
    unsigned id;
    BackgroundData bd;

    if ((dir = opendir( dirName.c_str() )) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (ent->d_name[0] == '.')
                continue;

            if (fillBackgroundData(dirName, ent->d_name, id, bd))
            {
                data[id] = bd;
            }
        }
        closedir(dir);
    }
}
