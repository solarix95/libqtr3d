#include <math.h>
#include "qtr3ddblmatrix.h"

#define EPSILON 1e-8

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
void Qtr3dDblMatrix4x4::scale(const Qtr3dDblVector3D &v)
{
    columnMajor(0,0) *= v.x;
    columnMajor(0,1) *= v.x;
    columnMajor(0,2) *= v.x;
    columnMajor(0,3) *= v.x;
    columnMajor(1,0) *= v.y;
    columnMajor(1,1) *= v.y;
    columnMajor(1,2) *= v.y;
    columnMajor(1,3) *= v.y;
    columnMajor(2,0) *= v.z;
    columnMajor(2,1) *= v.z;
    columnMajor(2,2) *= v.z;
    columnMajor(2,3) *= v.z;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dDblMatrix4x4::translate(const Qtr3dDblVector3D &v)
{
    (*this)(0,3) += (*this)(0,0) * v.x + (*this)(0,1) * v.y + (*this)(0,2) * v.z;
    (*this)(1,3) += (*this)(1,0) * v.x + (*this)(1,1) * v.y + (*this)(1,2) * v.z;
    (*this)(2,3) += (*this)(2,0) * v.x + (*this)(2,1) * v.y + (*this)(2,2) * v.z;
    (*this)(3,3) += (*this)(3,0) * v.x + (*this)(3,1) * v.y + (*this)(3,2) * v.z;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dDblMatrix4x4::rotate(double angle, double x, double y, double z)
{
    rotate(angle,Qtr3dDblVector3D(x,y,z));
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
double &Qtr3dDblMatrix4x4::columnMajor(int row, int column)
{
    return (*this)(column,row);
}

//-------------------------------------------------------------------------------------------------
const double &Qtr3dDblMatrix4x4::columnMajorC(int row, int column) const
{
    return (*this)(column,row);
}

//-------------------------------------------------------------------------------------------------
const Qtr3dDblMatrix4x4 &Qtr3dDblMatrix4x4::operator*(const Qtr3dDblVector3D &v) const
{
    Q_ASSERT(0); // TBD and Test
    return *this;
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
void Qtr3dDblMatrix4x4::fromFloat(const QMatrix4x4 &mat)
{
    for (int i = 0; i < 16; ++i)
        data()[i] = mat.data()[i];
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

