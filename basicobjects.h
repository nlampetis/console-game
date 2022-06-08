#pragma once

#include "gameobject.h"

struct coord{
  float x;
  float y;
};

class Wall : public GameObject{
  private:
    coord position;
    Sprite * sprite;
  public:

    Wall();
    virtual ~Wall();
    const short& getx() const;
    const short& gety() const;
    bool isPassable();

    Sprite * getSprite() const;
    const bool hasDoor() const;

    void onUpdate();

};

class Projectile : public GameObject{
  private:
    coord position;
    coord velocity;
    Sprite * sprite;
  public:

    Projectile();
    virtual ~Projectile();

    const short& getx() const;
    const short& gety() const;
    bool isPassable();
    Sprite * getSprite() const;
    void onUpdate();
};

class Ground : public GameObject{
  private:
    coord position;
    Sprite * sprite;
  public:
    Ground();
    virtual ~Ground();

    const short& getx() const;
    const short& gety() const;
    bool isPassable();
    Sprite * getSprite() const;
    void onUpdate();

};


