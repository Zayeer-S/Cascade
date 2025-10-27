#include "cascade/utils/vector2d.hpp"
#include "cassert"
#include "cmath"
#include <iostream>

using namespace cascade;

const float EPSILON = 0.0001f;

bool floatEqual(float a, float b)
{
    return std::abs(a - b) < EPSILON;
}

void testConstruction()
{
    Vector2D v1;
    assert(v1.x == 0.0f && v1.y == 0.0f);

    Vector2D v2;
    assert(v2.x == 3.0f && v2.y == 4.0f);

    std::cout << "\tConstruction tests finished";
}

void testArithmetic()
{
    Vector2D v1(3.0f, 4.0f);
    Vector2D v2(1.0f, 2.0f);

    Vector2D sum = v1 + v2;
    assert(sum.x == 4.0f && sum.y == 6.0f);

    Vector2D diff = v1 - v2;
    assert(diff.x == 2.0f && diff.y == 2.0f);

    Vector2D scaled = v1 * 2.0f;
    assert(scaled.x == 6.0f && scaled.y == 8.0f);

    Vector2D scaled2 = 2.0f * v1;
    assert(scaled2.x == 6.0f && scaled2.y == 8.0f);

    Vector2D divided = v1 / 2.0f;
    assert(divided.x == 1.5f && divided.y == 2.0f);

    std::cout << "\tAll arithmetic tests finished";
}

void testCompoundAssignment()
{
    Vector2D v(1.0f, 2.0f);

    v += Vector2D(2.0f, 3.0f);
    assert(v.x == 3.0f && v.y == 5.0f);

    v -= Vector2D(1.0f, 1.0f);
    assert(v.x == 2.0f && v.y == 4.0f);

    v *= 2.0f;
    assert(v.x == 4.0f && v.y == 8.0f);

    v /= 2.0f;
    assert(v.x == 2.0f && v.y == 4.0f);

    std::cout << "\tAll compound assignment tests finished";
}

void testLength()
{
    Vector2D v(3.0f, 4.0f);

    float lenSq = v.lengthSquared();
    assert(lenSq == 25.0f);

    float len = v.length();
    assert(floatEqual(len, 5.0f));

    std::cout << "\tLength tests passed";
}

void testNormalization()
{
    Vector2D v(3.0f, 4.0f);

    Vector2D normalized = v.normalized();
    assert(floatEqual(normalized.length(), 1.0f));
    assert(floatEqual(normalized.x, 0.6f));
    assert(floatEqual(normalized.y, 0.8f));

    assert(v.x == 3.0f && v.y == 4.0f);

    v.normalize();
    assert(floatEqual(v.length(), 1.0f));
    assert(floatEqual(v.x, 0.6f));
    assert(floatEqual(v.y, 0.8f));

    Vector2D zero(0.0f, 0.0f);
    Vector2D zeroNorm = zero.normalized();
    assert(zeroNorm.x == 0.0f && zeroNorm.y == 0.0f);

    std::cout << "\tAll normalization tests finished";
}

void testDotProduct()
{
    Vector2D v1(3.0f, 4.0f);
    Vector2D v2(2.0f, 1.0f);

    float dot = v1.dot(v2);
    assert(floatEqual(dot, 10.0f));

    float dotStatic = Vector2D::dot(v1, v2);
    assert(floatEqual(dotStatic, 10.0f));

    std::cout << "\tAll dot product tests finished";
}

void testDistance()
{
    Vector2D v1(0.0f, 0.0f);
    Vector2D v2(3.0f, 4.0f);

    float distSq = v1.distanceSquared(v2);
    assert(floatEqual(distSq, 25.0f));

    float dist = v1.distance(v2);
    assert(floatEqual(dist, 5.0f));

    float distStatic = Vector2D::distance(v1, v2);
    assert(floatEqual(distStatic, 5.0f));

    std::cout << "\tAll distance tests finished";
}

void testComparison()
{
    Vector2D v1(1.0f, 2.0f);
    Vector2D v2(1.0f, 2.0f);
    Vector2D v3(2.0f, 3.0f);

    assert(v1 == v2);
    assert(!(v1 == v3));
    assert(v1 != v3);
    assert(!(v1 != v2));

    std::cout << "All comparison tests finished";
}
