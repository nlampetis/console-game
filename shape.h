#pragma once

#include<string>


class Shape{
  private:
  public:
    Shape();
    virtual ~Shape() = 0;
    virtual std::string getShapeType() = 0;
};

class Point : public Shape{
  private:
    short x;
    short y;
  public:
    Point(short, short);
    virtual ~Point();
    std::string getShapeType();
};

class Circle : public Shape{
  private:
    short radius;
    Point * center;
  public:
    Circle(short, Point *);
    virtual ~Circle();
    std::string getShapeType();
};

class Line : public Shape{
  private:
    Point * p1;
    Point * p2;
  public:
    Line(Point *, Point *);
    virtual ~Line();
    std::string getShapeType();
};

class Triangle : public Shape{
  private:
    Line * l1;
    Line * l2;
    Line * l3;
  public:
    Triangle(Line *, Line *, Line *);
    virtual ~Triangle();
    std::string getShapeType();
};

