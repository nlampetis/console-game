#include "shape.h"

// constructors
Point::Point(short xx, short yy) : x(xx), y(yy) {}

Circle::Circle(short rr, Point *pp) : radius(rr), center(pp) {}

Line::Line(Point *pp1, Point *pp2) : p1{pp1}, p2{pp2} {}

Triangle::Triangle(Line *ll1, Line *ll2, Line *ll3)
    : l1{ll1}, l2{ll2}, l3{ll3} {}

// destructors
Circle::~Circle() { delete center; }

Line::~Line() {
  delete p1;
  delete p2;
}

Triangle::~Triangle() {
  delete l1;
  delete l2;
  delete l3;
}

// overloads
std::string Point::getShapeType() { return "point"; }

std::string Circle::getShapeType() { return "circle"; }

std::string Line::getShapeType() { return "line"; }

std::string Triangle::getShapeType() { return "triangle"; }
