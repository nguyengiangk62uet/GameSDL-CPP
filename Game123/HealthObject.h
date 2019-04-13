// Mạng của nhân vật

#ifndef HEALTH_OBJECT_H_
#define HEALTH_OBJECT_H_

#include "BaseObject.h"
#include <vector>

class HealthObject : public BaseObject
{
public:
    HealthObject();
    ~HealthObject();

    void SetNumber(const int& num) {number_ = num;}
    void AddPosition(const int& xpos);
    void Render(SDL_Surface* des);
    void Init();
    void Decrease();

private:
    int number_;
    std::vector<int> pos_list_;
};



#endif // HEALTH_OBJECT_H_
