#ifndef _SPIKES_H_
#define _SPIKES_

#include "Object.h"

class Spikes : public Object {
public:
    Spikes(void);
    Spikes(double x, double y, double width, double height);
    ~Spikes(void);

    void draw();
    CollisionBox getCollisionBox();
};

#endif /* _SPIKES_H_ */

