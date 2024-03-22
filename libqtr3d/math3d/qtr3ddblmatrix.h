#ifndef QTR3D_DOUBLE_MATRIX_H
#define QTR3D_DOUBLE_MATRIX_H

#include <QGenericMatrix>
#include <QMatrix4x4>
#include "qtr3ddblvector.h"

//----------------------------------------- Double Matrix 4x4  -------------------------------------------------
class Qtr3dDblMatrix4x4 : public QGenericMatrix<4,4,double>
{
public:
    Qtr3dDblMatrix4x4();

    void scale(double scaleFactor);
    void scale(const Qtr3dDblVector3D &v);
    void translate(const Qtr3dDblVector3D &v);
    void rotate(double angle,double x, double y, double z);
    void rotate(double angle, const Qtr3dDblVector3D &v);
    void perspective(double verticalAngle, double aspectRatio, double nearPlane, double farPlane);
    void lookAt(const Qtr3dDblVector3D &eye, const Qtr3dDblVector3D &center, const Qtr3dDblVector3D &up);

    double       &columnMajor(int row, int column);
    const double &columnMajorC(int row, int column) const;
    // const Qtr3dDblMatrix4x4 &operator*(const Qtr3dDblMatrix4x4& rhs) const;
    const Qtr3dDblMatrix4x4 &operator*(const Qtr3dDblVector3D &v) const;

    QMatrix4x4 toFloat() const;
    void       fromFloat(const QMatrix4x4 &mat);

    static void rotationMatrix(Qtr3dDblMatrix4x4 &matrix, double angle, const Qtr3dDblVector3D &v);
    static void frustumMatrix(Qtr3dDblMatrix4x4 &matrix, double left, double right, double bottom, double top, double nearPlane, double farPlane);
    static void perspectiveMatrix(Qtr3dDblMatrix4x4 &matrix, double angle, double aspect, double nearPlane, double farPlane);

    friend Qtr3dDblVector3D operator*(const Qtr3dDblMatrix4x4& matrix, const Qtr3dDblVector3D& vector);
    friend Qtr3dDblVector3D operator*(const Qtr3dDblVector3D& vector, const Qtr3dDblMatrix4x4& matrix);

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

inline Qtr3dDblVector3D operator*(const Qtr3dDblMatrix4x4& matrix, const Qtr3dDblVector3D& vector)
{
    Q_ASSERT(0);
    return Qtr3dDblVector3D();
}

inline QVector3D operator*(const Qtr3dDblMatrix4x4& m, const QVector3D& vector)
{
    qreal x, y, z, w;

        x = vector.x() *  m.columnMajorC(0,0) +
            vector.y() *  m.columnMajorC(1,0) +
            vector.z() *  m.columnMajorC(2,0) +
            m.columnMajorC(3,0);
        y = vector.x() * m.columnMajorC(0,1) +
            vector.y() * m.columnMajorC(1,1) +
            vector.z() * m.columnMajorC(2,1) +
            m.columnMajorC(3,1);
        z = vector.x() * m.columnMajorC(0,2) +
            vector.y() * m.columnMajorC(1,2) +
            vector.z() * m.columnMajorC(2,2) +
            m.columnMajorC(3,2);
        w = vector.x() * m.columnMajorC(0,3) +
            vector.y() * m.columnMajorC(1,3) +
            vector.z() * m.columnMajorC(2,3) +
            m.columnMajorC(3,3);
        if (w == 1.0f)
            return QVector3D(x, y, z);


    return QVector3D(x / w, y / w, z / w);
}

#endif // QTR3DMATH_H
