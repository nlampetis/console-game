#include "basicobjects.h"

Wall::Wall() {}

Wall::~Wall() {}

const short &Wall::getx() const {}

const short &Wall::gety() const {}

bool Wall::isPassable() {}

Sprite *Wall::getSprite() const {}

void Wall::onUpdate() {}

const bool Wall::hasDoor() const {}

Projectile::Projectile() {}

Projectile::~Projectile() {}

const short &Projectile::getx() const {}

const short &Projectile::gety() const {}

bool Projectile::isPassable() {}

Sprite *Projectile::getSprite() const {}

void Projectile::onUpdate() {}

Ground::Ground() {}

Ground::~Ground() {}

const short &Ground::getx() const {}

const short &Ground::gety() const {}

bool Ground::isPassable() {}

Sprite *Ground::getSprite() const {}

void Ground::onUpdate() {}
