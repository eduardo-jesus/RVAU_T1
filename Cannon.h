#pragma once
class Cannon {
private:
    double angle_;
    bool can_shoot_;
    bool shooting_;
public:
    Cannon(void);
    ~Cannon(void);

    void setAngle(double);
    double getAngle();
    void setCanShoot(bool);
    bool canShoot();
    void setShooting(bool);
    bool isShooting();

    void shoot();
};
