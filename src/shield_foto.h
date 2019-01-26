#include "main.h"

#ifndef SHIELD_FOTO_H
#define SHIELD_FOTO_H
#define GLM_ENABLE_EXPERIMENTAL


class Shield_foto {
public:
    Shield_foto() {}
    Shield_foto(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    int time;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(float x, float y);
private:
    VAO *object[2];
};

#endif // SHIELD_FOTO_H
