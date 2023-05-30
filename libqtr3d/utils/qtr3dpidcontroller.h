#ifndef QTR3DPIDCONTROLLER_H
#define QTR3DPIDCONTROLLER_H

#include <limits>

template<typename T>
class Qtr3dPidController {
public:
    Qtr3dPidController(T Kp, T Ki, T Kd)
        : Kp(Kp), Ki(Ki), Kd(Kd)
        , mPreviousError(), mIntegralError()
        , mFirstCall(true){}

    T calculate(T setpoint, T measurement, float sampleTime) {
        T error = setpoint - measurement;

        T proportional = Kp * error;

        mIntegralError += error * sampleTime;
        T integral = Ki * mIntegralError;

        T derivative = T();
        if (!mFirstCall) {
            derivative = Kd * (error - mPreviousError) / sampleTime;
        } else
            mFirstCall = false;

        mPreviousError = error;

        return  proportional + integral + derivative;
    }

private:
    T    Kp, Ki, Kd;
    T    mPreviousError;
    T    mIntegralError;

    bool mFirstCall;
};


#endif // QTR3DPIDCONTROLLER_H
