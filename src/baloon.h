#include "main.h"

#ifndef BALOON_H
#define BALOON_H
#define GLM_ENABLE_EXPERIMENTAL


class Baloon {
public:
    Baloon() {}
    Baloon(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    float speed_y;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
private:
    VAO *object;
};

#endif // BALOON_H
