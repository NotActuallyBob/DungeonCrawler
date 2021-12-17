#ifndef _ROOM_TILE_
#define _ROOM_TILE_

#include "PCH.hpp"

class RoomTile {
public:
    /**
     * @brief Construct a new Room Tile object
     *
     * @param    texture              texture of the tile
     * @param    x                    location in x-axis
     * @param    y                    location in y-axis
     * @param    walkable             is it a walkable tile or not
     * @param    penetratable         can projectiles penetrate the tile
     */
    RoomTile(std::string texture, float x, float y, bool walkable, bool penetratable);

    const sf::Vector2f getSize() const;
    const sf::Vector2f& getPosition() const;
    const sf::FloatRect getBoundingBox() const;
    const sf::Sprite& getSprite() const;
    bool isWalkable() const;
    bool isPenetratable() const;

private:
    sf::Vector2f position;
    sf::Texture tileTexture;
    sf::Sprite tileSprite;
    bool walkable_;
    bool penetratable_;

    bool setTileTexture(std::string);
};

class WallTile : public RoomTile {
public:
    WallTile(std::string texture, float x, float y)
        : RoomTile(texture, x, y, false, false)
    {
    }
};

/**
 * @brief A wall that is not walkable but penetrable
 *
 */
class FrontWallTile : public RoomTile {
public:
    FrontWallTile(std::string texture, float x, float y)
        : RoomTile(texture, x, y, false, true)
    {
    }
};

class FloorTile : public RoomTile {
public:
    FloorTile(std::string texture, float x, float y)
        : RoomTile(texture, x, y, true, true)
    {
    }
};

#endif