#include <math.h>
#include "qtr3ddblvector.h"

//----------------------------------------- Double Vector 3D -------------------------------------------------
#define EPSILON 1e-8

//-------------------------------------------------------------------------------------------------
bool Qtr3dDblVector3D::isNull() const
{
    return (x*x + y*y + z*z) < EPSILON * EPSILON;
}

//-------------------------------------------------------------------------------------------------
double Qtr3dDblVector3D::distanceToPoint(const Qtr3dDblVector3D &other) const
{
    return std::sqrt((x - other.x) * (x - other.x) +
                     (y - other.y) * (y - other.y) +
                     (z - other.z) * (z - other.z));
}

//-------------------------------------------------------------------------------------------------
double Qtr3dDblVector3D::length() const
{
    return std::sqrt(x*x + y*y + z*z);
}

//-------------------------------------------------------------------------------------------------
Qtr3dDblVector3D Qtr3dDblVector3D::normalized() const
{
    double magnitude = sqrt(x * x + y * y + z * z);
    if (magnitude < EPSILON)
        return Qtr3dDblVector3D(0, 0, 0); // Return a zero vector if the original vector is too small
    return Qtr3dDblVector3D(x / magnitude, y / magnitude, z / magnitude);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dDblVector3D::normalize()
{
    double magnitude = sqrt(x * x + y * y + z * z);
    if (magnitude < EPSILON) {
        x = y = z = 0.0;
        return;
    }

    x = x / magnitude;
    y = y / magnitude;
    z = z / magnitude;
}

//-------------------------------------------------------------------------------------------------
double &Qtr3dDblVector3D::operator[](int i)
{
    Q_ASSERT(i >= 0 && i <= 2);
    if (i==0) return x;
    if (i==1) return y;
    if (i==2) return z;
    return x;
}

//-------------------------------------------------------------------------------------------------
Qtr3dDblVector3D &Qtr3dDblVector3D::operator=(const Qtr3dDblVector3D &other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
}

//-------------------------------------------------------------------------------------------------
double  Qtr3dDblVector3D::operator[](int i) const
{
    Q_ASSERT(i >= 0 && i <= 2);
    if (i==0) return x;
    if (i==1) return y;
    if (i==2) return z;
    return x;
}

//-------------------------------------------------------------------------------------------------
Qtr3dDblVector3D &Qtr3dDblVector3D::operator+=(const Qtr3dDblVector3D &vector)
{
    x += vector.x;
    y += vector.y;
    z += vector.z;
    return *this;
}

//-------------------------------------------------------------------------------------------------
const Qtr3dDblVector3D operator-(const Qtr3dDblVector3D &vector)
{
    return Qtr3dDblVector3D(-vector.x,-vector.y, -vector.z);
}

//-------------------------------------------------------------------------------------------------
QVector3D Qtr3dDblVector3D::toFloat() const
{
    return QVector3D(float(x), float(y), float(z));
}

//-------------------------------------------------------------------------------------------------
Qtr3dDblVector3D Qtr3dDblVector3D::crossProduct(const Qtr3dDblVector3D &v1, const Qtr3dDblVector3D &v2)
{
    return Qtr3dDblVector3D(v1.y * v2.z - v1.z * v2.y,
                            v1.z * v2.x - v1.x * v2.z,
                            v1.x * v2.y - v1.y * v2.x);
}

//-------------------------------------------------------------------------------------------------
const Qtr3dDblVector3D operator+(const Qtr3dDblVector3D &v1, const Qtr3dDblVector3D &v2)
{
    return Qtr3dDblVector3D(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

//-------------------------------------------------------------------------------------------------
const Qtr3dDblVector3D operator-(const Qtr3dDblVector3D &v1, const Qtr3dDblVector3D &v2)
{
    return Qtr3dDblVector3D(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

//-------------------------------------------------------------------------------------------------
const Qtr3dDblVector3D operator*(double factor, const Qtr3dDblVector3D &vector)
{
    return Qtr3dDblVector3D(vector.x*factor, vector.y*factor, vector.z*factor);
}


//-------------------------------------------------------------------------------------------------
const Qtr3dDblVector3D operator*(const Qtr3dDblVector3D &vector, const Qtr3dDblVector3D &factor)
{
    return Qtr3dDblVector3D(vector.x*factor.x, vector.y*factor.y, vector.z*factor.z);
}

//-------------------------------------------------------------------------------------------------
const Qtr3dDblVector3D operator/(const Qtr3dDblVector3D &vector, double divisor)
{
    return Qtr3dDblVector3D(vector.x/divisor, vector.y/divisor, vector.z/divisor);
}

//-------------------------------------------------------------------------------------------------
const Qtr3dDblVector3D operator*(const Qtr3dDblVector3D &vector, double factor)
{
    return Qtr3dDblVector3D(vector.x*factor, vector.y*factor, vector.z*factor);
}

//-------------------------------------------------------------------------------------------------
const Qtr3dDblVector3D operator*(const Qtr3dDblVector3D &vector, float factor)
{
    return Qtr3dDblVector3D(vector.x*factor, vector.y*factor, vector.z*factor);
}

//-------------------------------------------------------------------------------------------------
bool operator==(const Qtr3dDblVector3D &v1, const Qtr3dDblVector3D &v2)
{
    return std::fabs(v1.x - v2.x) < EPSILON &&
            std::fabs(v1.y - v2.y) < EPSILON &&
            std::fabs(v1.z - v2.z) < EPSILON;
}

//-------------------------------------------------------------------------------------------------
bool operator!=(const Qtr3dDblVector3D &v1, const Qtr3dDblVector3D &v2)
{
    return !(v1 == v2);
}

