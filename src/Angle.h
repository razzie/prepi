/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef ANGLE_H_INCLUDED
#define ANGLE_H_INCLUDED

class DegAngle
{
public:
    DegAngle(float angle = 0.f);
    ~DegAngle();
    DegAngle& operator= (float angle);
    DegAngle  operator+ (float angle) const;
    DegAngle& operator+=(float angle);
    DegAngle  operator- (float angle) const;
    DegAngle& operator-=(float angle);
    operator float() const;
    DegAngle& rotateTowards(float angle, float rate = 360.f);
    float distanceFrom(float angle) const;
    bool isBetween(float a, float b) const;

private:
    float m_angle;
};

#endif // ANGLE_H_INCLUDED
