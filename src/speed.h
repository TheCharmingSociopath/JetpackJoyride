#include "main.h"

#ifndef SPEED_H
#define SPEED_H
#define GLM_ENABLE_EXPERIMENTAL


class Speed {
public:
    Speed() {}
    Speed(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
private:
    VAO *object;
};

#endif // SPEED_H
