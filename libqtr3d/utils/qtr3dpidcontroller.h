#ifndef QTR3DPIDCONTROLLER_H
#define QTR3DPIDCONTROLLER_H

template<typename T>
class Qtr3dPidController {
public:
    Qtr3dPidController(T Kp, T Ki, T Kd) :
        Kp(Kp), Ki(Ki), Kd(Kd),
        previousError(), integralError() {}

    T calculate(T setpoint, T measurement, float sampleTime) {
        T error = setpoint - measurement;

        T proportional = Kp * error;

        integralError += error * sampleTime;
        T integral = Ki * integralError;

        T derivative = Kd * (error - previousError) / sampleTime;
        previousError = error;

        return  proportional + integral + derivative;
    }

private:
    T Kp, Ki, Kd;
    T previousError;
    T integralError;
};


#endif // QTR3DPIDCONTROLLER_H
