#ifndef _ROTATE_BUTTON_H_
#define _ROTATE_BUTTON_

#include "Object.h"

class RotateButton: public Object  {
private:
    double trans_matrix_[16];
public:
    RotateButton(void);
    ~RotateButton(void);

    double (& getTransMatrix()) [16];
    void draw();
};

#endif 