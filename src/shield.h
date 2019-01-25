#include "main.h"

#ifndef SHIELD_H
#define SHIELD_H
#define GLM_ENABLE_EXPERIMENTAL


class Shield {
public:
    Shield() {}
    Shield(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    bool direction = true;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
private:
    VAO *object;
};

#endif // SHIELD_H
