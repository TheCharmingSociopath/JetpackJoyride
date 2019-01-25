#include "main.h"

#ifndef LIFE_H
#define LIFE_H
#define GLM_ENABLE_EXPERIMENTAL


class Life {
public:
    Life() {}
    Life(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    bool direction = false;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
private:
    VAO *object;
};

#endif // LIFE_H
