#include <math.h>
#include "qtr3dmath.h"

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


//----------------------------------------- Double Matrix 4x4  -------------------------------------------------
Qtr3dDblMatrix4x4::Qtr3dDblMatrix4x4()
{
}

//-------------------------------------------------------------------------------------------------
void Qtr3dDblMatrix4x4::scale(double scaleFactor)
{
    if (!data()[0])
        data()[0]  = scaleFactor;
    else
        data()[0]  *= scaleFactor;

    if (!data()[5])
        data()[5]  = scaleFactor;
    else
        data()[5]  *= scaleFactor;

    if (!data()[10])
        data()[10]  = scaleFactor;
    else
        data()[10]  *= scaleFactor;

    data()[15] = 1;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dDblMatrix4x4::translate(const Qtr3dDblVector3D &v)
{
    /*
    data()[12] += v.x;
    data()[13] += v.y;
    data()[14] += v.z;
    */




    (*this)(0,3) += (*this)(0,0) * v.x + (*this)(0,1) * v.y + (*this)(0,2) * v.z;
    (*this)(1,3) += (*this)(1,0) * v.x + (*this)(1,1) * v.y + (*this)(1,2) * v.z;
    (*this)(2,3) += (*this)(2,0) * v.x + (*this)(2,1) * v.y + (*this)(2,2) * v.z;
    (*this)(3,3) += (*this)(3,0) * v.x + (*this)(3,1) * v.y + (*this)(3,2) * v.z;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dDblMatrix4x4::rotate(double angle, const Qtr3dDblVector3D &v)
{
    if (angle == 0)
        return;

    double radians = angle * M_PI / 180.0;
    double c = cos(radians);
    double s = sin(radians);
    double t = 1.0 - c;

    double x = v[0], y = v[1], z = v[2];
    double magnitude = sqrt(x*x + y*y + z*z);
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;

    data()[0] = t*x*x + c;
    data()[1] = t*x*y + s*z;
    data()[2] = t*x*z - s*y;
    data()[3] = 0.0;
    data()[4] = t*x*y - s*z;
    data()[5] = t*y*y + c;
    data()[6] = t*y*z + s*x;
    data()[7] = 0.0;
    data()[8] = t*x*z + s*y;
    data()[9] = t*y*z - s*x;
    data()[10] = t*z*z + c;
    data()[11] = 0.0;
    data()[12] = 0.0;
    data()[13] = 0.0;
    data()[14] = 0.0;
    data()[15] = 1.0;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dDblMatrix4x4::perspective(double verticalAngle, double aspectRatio, double nearPlane, double farPlane)
{
    perspectiveMatrix(*this,verticalAngle,aspectRatio,nearPlane,farPlane);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dDblMatrix4x4::lookAt(const Qtr3dDblVector3D &eye, const Qtr3dDblVector3D &center, const Qtr3dDblVector3D &up)
{
    Qtr3dDblVector3D forward  = (center - eye).normalized();
    Qtr3dDblVector3D side     = Qtr3dDblVector3D::crossProduct(forward, up).normalized();
    Qtr3dDblVector3D upVector = Qtr3dDblVector3D::crossProduct(side, forward);

    Qtr3dDblMatrix4x4 m;

    m(0,0) = side.x;
    m(0,1) = side.y;
    m(0,2) = side.z;
    m(0,3) = 0.0f;


    m(1,0) = upVector.x;
    m(1,1) = upVector.y;
    m(1,2) = upVector.z;
    m(1,3) = 0.0f;

    m(2,0) = -forward.x;
    m(2,1) = -forward.y;
    m(2,2) = -forward.z;
    m(2,3) = 0.0f;

    m(3,0) = 0.0f;
    m(3,1) = 0.0f;
    m(3,2) = 0.0f;
    m(3,3) = 1.0f;

    *this = *this* m;
    translate(-eye);
}


//-------------------------------------------------------------------------------------------------
QMatrix4x4 Qtr3dDblMatrix4x4::toFloat() const
{
    QMatrix4x4 ret;

    for (int i = 0; i < 16; ++i)
        ret.data()[i] = float(data()[i]);

    return ret;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dDblMatrix4x4::rotationMatrix(Qtr3dDblMatrix4x4 &matrix, double angle, const Qtr3dDblVector3D &v)
{
    double radians = angle * M_PI / 180.0;
    double c = cos(radians);
    double s = sin(radians);
    double t = 1.0 - c;

    double x = v[0], y = v[1], z = v[2];
    double magnitude = sqrt(x*x + y*y + z*z);
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;

    double *data = matrix.data();
    data[0] = t*x*x + c;
    data[1] = t*x*y + s*z;
    data[2] = t*x*z - s*y;
    data[3] = 0.0;
    data[4] = t*x*y - s*z;
    data[5] = t*y*y + c;
    data[6] = t*y*z + s*x;
    data[7] = 0.0;
    data[8] = t*x*z + s*y;
    data[9] = t*y*z - s*x;
    data[10] = t*z*z + c;
    data[11] = 0.0;
    data[12] = 0.0;
    data[13] = 0.0;
    data[14] = 0.0;
    data[15] = 1.0;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dDblMatrix4x4::frustumMatrix(Qtr3dDblMatrix4x4 &matrix, double left, double right, double bottom, double top, double nearPlane, double farPlane)
{
    if (left == right || bottom == top || nearPlane == farPlane)
        return;

    double width = right - left;
    double invheight = top - bottom;
    double clip = farPlane - nearPlane;

    matrix(0,0) = 2.0f * nearPlane / width;
    matrix(1,0) = 0.0f;
    matrix(2,0) = (left + right) / width;
    matrix(3,0) = 0.0f;
    matrix(0,1) = 0.0f;
    matrix(1,1) = 2.0f * nearPlane / invheight;
    matrix(2,1) = (top + bottom) / invheight;
    matrix(3,1) = 0.0f;
    matrix(0,2) = 0.0f;
    matrix(1,2) = 0.0f;
    matrix(2,2) = -(nearPlane + farPlane) / clip;
    matrix(3,2) = -2.0f * nearPlane * farPlane / clip;
    matrix(0,3) = 0.0f;
    matrix(1,3) = 0.0f;
    matrix(2,3) = -1.0f;
    matrix(3,3) = 0.0f;

}

//-------------------------------------------------------------------------------------------------
void Qtr3dDblMatrix4x4::perspectiveMatrix(Qtr3dDblMatrix4x4 &matrix, double angle, double aspect, double nearPlane, double farPlane)
{
    if (nearPlane == farPlane || aspect == 0.0f)
        return;

    double radians = (angle / 2.0f) * M_PI / 180.0f;
    double sine = sin(radians);
    if (sine == 0.0f)
        return;

    double cotan = cos(radians) / sine;
    double clip = farPlane - nearPlane;

    matrix(0,0) = cotan / aspect;
    matrix(1,0) = 0.0f;
    matrix(2,0) = 0.0f;
    matrix(3,0) = 0.0f;
    matrix(0,1) = 0.0f;
    matrix(1,1) = cotan;
    matrix(2,1) = 0.0f;
    matrix(3,1) = 0.0f;
    matrix(0,2) = 0.0f;
    matrix(1,2) = 0.0f;
    matrix(2,2) = -(nearPlane + farPlane) / clip;
    matrix(3,2) = -1.0f;                                 // achtung column/row major
    matrix(0,3) = 0.0f;
    matrix(1,3) = 0.0f;
    matrix(2,3) = -(2.0f * nearPlane * farPlane) / clip; // achtung column/row major
    matrix(3,3) = 0.0f;
}

