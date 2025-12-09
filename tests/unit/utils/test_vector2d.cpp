#include "cascade/utils/vector2d.hpp"
#include <gtest/gtest.h>
#include <cmath>

using namespace cascade;

const float EPSILON = 0.0001f;

bool floatEqual(float a, float b)
{
    return std::abs(a - b) < EPSILON;
}

TEST(Vector2DTest, Construction)
{
    Vector2D v1;
    EXPECT_EQ(v1.x, 0.0f);
    EXPECT_EQ(v1.y, 0.0f);

    Vector2D v2(3.0f, 4.0f);
    EXPECT_EQ(v2.x, 3.0f);
    EXPECT_EQ(v2.y, 4.0f);
}

TEST(Vector2DTest, Addition)
{
    Vector2D v1(3.0f, 4.0f);
    Vector2D v2(1.0f, 2.0f);

    Vector2D sum = v1 + v2;
    EXPECT_EQ(sum.x, 4.0f);
    EXPECT_EQ(sum.y, 6.0f);
}

TEST(Vector2DTest, Subtraction)
{
    Vector2D v1(3.0f, 4.0f);
    Vector2D v2(1.0f, 2.0f);

    Vector2D diff = v1 - v2;
    EXPECT_EQ(diff.x, 2.0f);
    EXPECT_EQ(diff.y, 2.0f);
}

TEST(Vector2DTest, ScalarMultiplication)
{
    Vector2D v1(3.0f, 4.0f);

    Vector2D scaled = v1 * 2.0f;
    EXPECT_EQ(scaled.x, 6.0f);
    EXPECT_EQ(scaled.y, 8.0f);

    Vector2D scaled2 = 2.0f * v1;
    EXPECT_EQ(scaled2.x, 6.0f);
    EXPECT_EQ(scaled2.y, 8.0f);
}

TEST(Vector2DTest, ScalarDivision)
{
    Vector2D v1(3.0f, 4.0f);

    Vector2D divided = v1 / 2.0f;
    EXPECT_EQ(divided.x, 1.5f);
    EXPECT_EQ(divided.y, 2.0f);
}

TEST(Vector2DTest, CompoundAddition)
{
    Vector2D v(1.0f, 2.0f);
    v += Vector2D(2.0f, 3.0f);

    EXPECT_EQ(v.x, 3.0f);
    EXPECT_EQ(v.y, 5.0f);
}

TEST(Vector2DTest, CompoundSubtraction)
{
    Vector2D v(3.0f, 5.0f);
    v -= Vector2D(1.0f, 1.0f);

    EXPECT_EQ(v.x, 2.0f);
    EXPECT_EQ(v.y, 4.0f);
}

TEST(Vector2DTest, CompoundMultiplication)
{
    Vector2D v(2.0f, 4.0f);
    v *= 2.0f;

    EXPECT_EQ(v.x, 4.0f);
    EXPECT_EQ(v.y, 8.0f);
}

TEST(Vector2DTest, CompoundDivision)
{
    Vector2D v(4.0f, 8.0f);
    v /= 2.0f;

    EXPECT_EQ(v.x, 2.0f);
    EXPECT_EQ(v.y, 4.0f);
}

TEST(Vector2DTest, LengthSquared)
{
    Vector2D v(3.0f, 4.0f);
    float lenSq = v.lengthSquared();

    EXPECT_EQ(lenSq, 25.0f);
}

TEST(Vector2DTest, Length)
{
    Vector2D v(3.0f, 4.0f);
    float len = v.length();

    EXPECT_TRUE(floatEqual(len, 5.0f));
}

TEST(Vector2DTest, Normalized)
{
    Vector2D v(3.0f, 4.0f);
    Vector2D normalized = v.normalized();

    EXPECT_TRUE(floatEqual(normalized.length(), 1.0f));
    EXPECT_TRUE(floatEqual(normalized.x, 0.6f));
    EXPECT_TRUE(floatEqual(normalized.y, 0.8f));

    // Original should be unchanged
    EXPECT_EQ(v.x, 3.0f);
    EXPECT_EQ(v.y, 4.0f);
}

TEST(Vector2DTest, Normalize)
{
    Vector2D v(3.0f, 4.0f);
    v.normalize();

    EXPECT_TRUE(floatEqual(v.length(), 1.0f));
    EXPECT_TRUE(floatEqual(v.x, 0.6f));
    EXPECT_TRUE(floatEqual(v.y, 0.8f));
}

TEST(Vector2DTest, NormalizeZeroVector)
{
    Vector2D zero(0.0f, 0.0f);
    Vector2D zeroNorm = zero.normalized();

    EXPECT_EQ(zeroNorm.x, 0.0f);
    EXPECT_EQ(zeroNorm.y, 0.0f);
}

TEST(Vector2DTest, DotProductMember)
{
    Vector2D v1(3.0f, 4.0f);
    Vector2D v2(2.0f, 1.0f);

    float dot = v1.dot(v2);
    EXPECT_TRUE(floatEqual(dot, 10.0f));
}

TEST(Vector2DTest, DotProductStatic)
{
    Vector2D v1(3.0f, 4.0f);
    Vector2D v2(2.0f, 1.0f);

    float dotStatic = Vector2D::dot(v1, v2);
    EXPECT_TRUE(floatEqual(dotStatic, 10.0f));
}

TEST(Vector2DTest, DistanceSquared)
{
    Vector2D v1(0.0f, 0.0f);
    Vector2D v2(3.0f, 4.0f);

    float distSq = v1.distanceSquared(v2);
    EXPECT_TRUE(floatEqual(distSq, 25.0f));
}

TEST(Vector2DTest, Distance)
{
    Vector2D v1(0.0f, 0.0f);
    Vector2D v2(3.0f, 4.0f);

    float dist = v1.distance(v2);
    EXPECT_TRUE(floatEqual(dist, 5.0f));
}

TEST(Vector2DTest, DistanceStatic)
{
    Vector2D v1(0.0f, 0.0f);
    Vector2D v2(3.0f, 4.0f);

    float distStatic = Vector2D::distance(v1, v2);
    EXPECT_TRUE(floatEqual(distStatic, 5.0f));
}

TEST(Vector2DTest, EqualityOperator)
{
    Vector2D v1(1.0f, 2.0f);
    Vector2D v2(1.0f, 2.0f);
    Vector2D v3(2.0f, 3.0f);

    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 == v3);
}

TEST(Vector2DTest, InequalityOperator)
{
    Vector2D v1(1.0f, 2.0f);
    Vector2D v2(1.0f, 2.0f);
    Vector2D v3(2.0f, 3.0f);

    EXPECT_TRUE(v1 != v3);
    EXPECT_FALSE(v1 != v2);
}