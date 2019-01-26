#include "main.h"

#ifndef RING_H
#define RING_H
#define GLM_ENABLE_EXPERIMENTAL

class Ring {
public:
    Ring() {}
    Ring(float x, float y);
    color_t color;
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
private:
    VAO *object[2];
};

#endif // RING_H
