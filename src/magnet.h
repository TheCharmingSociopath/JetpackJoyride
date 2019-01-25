#include "main.h"

#ifndef MAGNET_H
#define MAGNET_H
#define GLM_ENABLE_EXPERIMENTAL


class Magnet {
public:
    Magnet() {}
    Magnet(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    int time;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(float x, float y);
private:
    VAO *object;
};

#endif // MAGNET_H
