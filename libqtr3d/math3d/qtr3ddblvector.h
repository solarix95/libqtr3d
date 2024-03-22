#ifndef QTR3D_DOUBLE_VECTOR_H
#define QTR3D_DOUBLE_VECTOR_H


#include <QVector3D>

//----------------------------------------- Double Vector 3D -------------------------------------------------
class Qtr3dDblVector3D {
public:
    double x;
    double y;
    double z;

    Qtr3dDblVector3D(double ax = 0, double ay = 0, double az = 0) : x(ax), y(ay), z(az) {}
    Qtr3dDblVector3D(const Qtr3dDblVector3D &v) : x(v.x), y(v.y), z(v.z) {}
    Qtr3dDblVector3D(const QVector3D &v) : x(v.x()), y(v.y()), z(v.z()) {}

    bool       isNull() const;
    double     distanceToPoint(const Qtr3dDblVector3D &other) const;
    double     length() const;
    Qtr3dDblVector3D normalized() const;
    void       normalize();

    double &operator[](int i);
    double  operator[](int i) const;
    operator QVector3D() const { return QVector3D(float(x),float(y),float(z)); }

    Qtr3dDblVector3D &operator=(const Qtr3dDblVector3D &other);
    Qtr3dDblVector3D &operator+=(const Qtr3dDblVector3D &vector);
    operator QString() const { return QString("X:%1, Y:%2, Z:%3").arg(x,0,'e',24).arg(y).arg(z); }

    QVector3D  toFloat() const;

    static Qtr3dDblVector3D crossProduct(const Qtr3dDblVector3D &v1, const Qtr3dDblVector3D &v2);
};

// related non-member operators
const Qtr3dDblVector3D operator+(const Qtr3dDblVector3D &v1, const Qtr3dDblVector3D &v2);
const Qtr3dDblVector3D operator-(const Qtr3dDblVector3D &v1, const Qtr3dDblVector3D &v2);
const Qtr3dDblVector3D operator-(const Qtr3dDblVector3D &v1);
const Qtr3dDblVector3D operator*(double factor, const Qtr3dDblVector3D &vector);
const Qtr3dDblVector3D operator*(const Qtr3dDblVector3D &vector, double factor);
const Qtr3dDblVector3D operator*(const Qtr3dDblVector3D &vector, const Qtr3dDblVector3D &factor);
const Qtr3dDblVector3D operator/(const Qtr3dDblVector3D &vector, double divisor);
bool operator==(const Qtr3dDblVector3D &v1, const Qtr3dDblVector3D &v2);
bool operator!=(const Qtr3dDblVector3D &v1, const Qtr3dDblVector3D &v2);


#endif
