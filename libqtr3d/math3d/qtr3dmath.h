#ifndef QTR3DMATH_H
#define QTR3DMATH_H

#include <QGenericMatrix>
#include <QMatrix4x4>
#include <QVector3D>

//----------------------------------------- Double Vector 3D -------------------------------------------------
class Qtr3dDblVector3D {
public:
    double x;
    double y;
    double z;

    Qtr3dDblVector3D(double ax = 0, double ay = 0, double az = 0) : x(ax), y(ay), z(az) {}
    Qtr3dDblVector3D(const QVector3D &v) : x(v.x()), y(v.y()), z(v.z()) {}

    bool       isNull() const;
    double     distanceToPoint(const Qtr3dDblVector3D &other) const;
    double     length() const;
    Qtr3dDblVector3D normalized() const;
    void       normalize();

    double &operator[](int i);
    double  operator[](int i) const;

    Qtr3dDblVector3D &operator+=(const Qtr3dDblVector3D &vector);
    const Qtr3dDblVector3D operator-(const Qtr3dDblVector3D &vector);
    operator QString() const { return QString("X:%1, Y:%2, Z:%3").arg(x,0,'e',24).arg(y).arg(z); }

    QVector3D  toFloat() const;

    static Qtr3dDblVector3D crossProduct(const Qtr3dDblVector3D &v1, const Qtr3dDblVector3D &v2);
};

// related non-member operators
const Qtr3dDblVector3D operator+(const Qtr3dDblVector3D &v1, const Qtr3dDblVector3D &v2);
const Qtr3dDblVector3D operator-(const Qtr3dDblVector3D &v1, const Qtr3dDblVector3D &v2);
const Qtr3dDblVector3D operator*(double factor, const Qtr3dDblVector3D &vector);
const Qtr3dDblVector3D operator*(const Qtr3dDblVector3D &vector, double factor);
const Qtr3dDblVector3D operator*(const Qtr3dDblVector3D &vector, const Qtr3dDblVector3D &factor);
const Qtr3dDblVector3D operator/(const Qtr3dDblVector3D &vector, double divisor);
bool operator==(const Qtr3dDblVector3D &v1, const Qtr3dDblVector3D &v2);
bool operator!=(const Qtr3dDblVector3D &v1, const Qtr3dDblVector3D &v2);


//----------------------------------------- Double Matrix 4x4  -------------------------------------------------
class Qtr3dDblMatrix4x4 : public QGenericMatrix<4,4,double>
{
public:
    Qtr3dDblMatrix4x4();

    void scale(double scaleFactor);
    void translate(const Qtr3dDblVector3D &v);
    void rotate(double angle, const Qtr3dDblVector3D &v);
    void perspective(double verticalAngle, double aspectRatio, double nearPlane, double farPlane);
    void lookAt(const Qtr3dDblVector3D &eye, const Qtr3dDblVector3D &center, const Qtr3dDblVector3D &up);

    // const Qtr3dDblMatrix4x4 &operator*(const Qtr3dDblMatrix4x4& rhs) const;

    QMatrix4x4 toFloat() const;

    static void rotationMatrix(Qtr3dDblMatrix4x4 &matrix, double angle, const Qtr3dDblVector3D &v);
    static void frustumMatrix(Qtr3dDblMatrix4x4 &matrix, double left, double right, double bottom, double top, double nearPlane, double farPlane);
    static void perspectiveMatrix(Qtr3dDblMatrix4x4 &matrix, double angle, double aspect, double nearPlane, double farPlane);

private:

};


inline Qtr3dDblMatrix4x4 operator*(const Qtr3dDblMatrix4x4& m1, const Qtr3dDblMatrix4x4& m2)
{
    Qtr3dDblMatrix4x4 m;

    m(0,0) = m1(0,0) * m2(0,0)
              + m1(1,0) * m2(0,1)
              + m1(2,0) * m2(0,2)
              + m1(3,0) * m2(0,3);
    m(0,1) = m1(0,1) * m2(0,0)
              + m1(1,1) * m2(0,1)
              + m1(2,1) * m2(0,2)
              + m1(3,1) * m2(0,3);
    m(0,2) = m1(0,2) * m2(0,0)
              + m1(1,2) * m2(0,1)
              + m1(2,2) * m2(0,2)
              + m1(3,2) * m2(0,3);
    m(0,3) = m1(0,3) * m2(0,0)
              + m1(1,3) * m2(0,1)
              + m1(2,3) * m2(0,2)
              + m1(3,3) * m2(0,3);

    m(1,0) = m1(0,0) * m2(1,0)
              + m1(1,0) * m2(1,1)
              + m1(2,0) * m2(1,2)
              + m1(3,0) * m2(1,3);
    m(1,1) = m1(0,1) * m2(1,0)
              + m1(1,1) * m2(1,1)
              + m1(2,1) * m2(1,2)
              + m1(3,1) * m2(1,3);
    m(1,2) = m1(0,2) * m2(1,0)
              + m1(1,2) * m2(1,1)
              + m1(2,2) * m2(1,2)
              + m1(3,2) * m2(1,3);
    m(1,3) = m1(0,3) * m2(1,0)
              + m1(1,3) * m2(1,1)
              + m1(2,3) * m2(1,2)
              + m1(3,3) * m2(1,3);

    m(2,0) = m1(0,0) * m2(2,0)
              + m1(1,0) * m2(2,1)
              + m1(2,0) * m2(2,2)
              + m1(3,0) * m2(2,3);
    m(2,1) = m1(0,1) * m2(2,0)
              + m1(1,1) * m2(2,1)
              + m1(2,1) * m2(2,2)
              + m1(3,1) * m2(2,3);
    m(2,2) = m1(0,2) * m2(2,0)
              + m1(1,2) * m2(2,1)
              + m1(2,2) * m2(2,2)
              + m1(3,2) * m2(2,3);
    m(2,3) = m1(0,3) * m2(2,0)
              + m1(1,3) * m2(2,1)
              + m1(2,3) * m2(2,2)
              + m1(3,3) * m2(2,3);

    m(3,0) = m1(0,0) * m2(3,0)
              + m1(1,0) * m2(3,1)
              + m1(2,0) * m2(3,2)
              + m1(3,0) * m2(3,3);
    m(3,1) = m1(0,1) * m2(3,0)
              + m1(1,1) * m2(3,1)
              + m1(2,1) * m2(3,2)
              + m1(3,1) * m2(3,3);
    m(3,2) = m1(0,2) * m2(3,0)
              + m1(1,2) * m2(3,1)
              + m1(2,2) * m2(3,2)
              + m1(3,2) * m2(3,3);
    m(3,3) = m1(0,3) * m2(3,0)
              + m1(1,3) * m2(3,1)
              + m1(2,3) * m2(3,2)
              + m1(3,3) * m2(3,3);

    return m;
}


#endif // QTR3DMATH_H
