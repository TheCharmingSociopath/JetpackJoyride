#include "main.h"

#ifndef BOOMERANG_H
#define BOOMERANG_H
#define GLM_ENABLE_EXPERIMENTAL


class Boomerang {
public:
    Boomerang() {}
    Boomerang(float x, float y, color_t color);
    glm::vec3 position;
    int time;
    float rotation, arg = 0, a = 5, b = 3, cx, cy;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
private:
    VAO *object;
};

#endif // BOOMERANG_H
