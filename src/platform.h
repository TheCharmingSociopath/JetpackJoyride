#include "main.h"

#ifndef PLATFORM_H
#define PLATFORM_H
#define GLM_ENABLE_EXPERIMENTAL


class Platform {
public:
    Platform() {}
    Platform(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
private:
    VAO *object;
};

#endif // PLATFORM_H
