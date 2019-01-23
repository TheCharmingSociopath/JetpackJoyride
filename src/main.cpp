#include "main.h"
#include "timer.h"
#include "ball.h"
#include "platform.h"
#include "coin.h"
#include "fireline.h"

#define GLM_ENABLE_EXPERIMENTAL

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball ball;
Fireline fireline;
bounding_box_t b, c;
vector <Platform> platform;
vector <Coin> coin;

float screen_zoom = 1, screen_center_x = 0.0f, screen_center_y = 0.0f, camera_center_x = 0.0f, camera_center_y = 0.0f;
float camera_rotation_angle = 0;
int score = 0, lives = 5;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    glm::vec3 eye ( camera_center_x, camera_center_y, 5 );

    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (camera_center_x, camera_center_y, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    ball.draw(VP);
    for (int i=0; i < platform.size(); ++i)
    {
        platform[i].draw(VP);
    }
    for (int i=0; i < coin.size(); ++i)
    {
        coin[i].draw(VP);
    }
    fireline.draw(VP);
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);

    if (left) {
        camera_center_x -= HORIZONTAL_MOVEMENT_VALUE;
        ball.position.x = camera_center_x;
    }
    if (right) {
        camera_center_x += HORIZONTAL_MOVEMENT_VALUE;
        ball.position.x = camera_center_x;
    }
    if (space) {
        ball.speed = 0.1;
    }
    if (up) {
        screen_zoom += 0.01;
    }
    if (down) {
        screen_zoom -= 0.01;
    }
}

void tick_elements() {
    ball.tick();
    camera_rotation_angle += 1;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    ball = Ball(screen_center_x, -6, COLOR_BODY);

    for (int i=0; i < 1000; ++i)
    {
        platform.push_back(Platform(-7 + i, -7, COLOR_PLATFORM));
    }
    for (int i=0; i<10; i += 2)
    {
        coin.push_back(Coin(10 + i, 1, COLOR_COIN_YELLOW));
        coin.push_back(Coin(11 + i, 2, COLOR_COIN_YELLOW));
    }

    fireline = Fireline(camera_center_x, 1, COLOR_RED);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers
        b.x = ball.position.x;
        b.y = ball.position.y;
        b.width = 2;
        b.height = 3;

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands

            // Coin's collission with player
            for(int i=0; i<coin.size(); ++i)
            {
                c.x = coin[i].position.x;
                c.y = coin[i].position.y;
                c.height = 0.5;
                c.width = 0.5;

                if (detect_collision(b, c))
                {
                    if (coin[i].color.g == 0)
                        score += 10;
                    else if(coin[i].color.r == 0)
                        score += 5;
                    else
                        ++score;
                    cout << "Score: " << score << endl;
                    coin.erase(coin.begin() + i);
                }
            }
            //------------------------------------------------------

            // Generate objects
            if ((int)(camera_center_x * 10) % 2500 == 0 and camera_center_x != 0)
            {
                for (int i=0; i<10; i += 2)
                {
                    coin.push_back(Coin(camera_center_x + 10 + i, 1, COLOR_COIN_RED));
                    coin.push_back(Coin(camera_center_x + 11 + i, 2, COLOR_COIN_RED));
                }
            }
            else if ((int)(camera_center_x * 10) % 1500 == 0 and camera_center_x != 0)
            {
                for (int i=0; i<10; i += 2)
                {
                    coin.push_back(Coin(camera_center_x + 10 + i, 1, COLOR_COIN_GREEN));
                    coin.push_back(Coin(camera_center_x + 11 + i, 2, COLOR_COIN_GREEN));
                }
            }
            else if ((int)(camera_center_x * 10) % 500 == 0 and camera_center_x != 0)
            {
                for (int i=0; i<10; i += 2)
                {
                    coin.push_back(Coin(camera_center_x + 10 + i, 1, COLOR_COIN_YELLOW));
                    coin.push_back(Coin(camera_center_x + 11 + i, 2, COLOR_COIN_YELLOW));
                }
            }
            else if ((int)(camera_center_x * 10) % 250 == 0 and camera_center_x != 0)
            {
                //make fire line
                fireline = Fireline(camera_center_x + 10, 1, COLOR_RED);
            }
            // ------------------------------------------------------
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
            reset_screen();
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + 8 / screen_zoom;
    float bottom = screen_center_y - 8 / screen_zoom;
    float left   = screen_center_x - 8 / screen_zoom;
    float right  = screen_center_x + 8 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}

