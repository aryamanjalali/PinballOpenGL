#include <GL/glut.h>

#include <iostream>

#include <vector>

#include <cmath>



// Window dimensions

const int WIDTH = 800;

const int HEIGHT = 600;



// Paddle properties

const float PADDLE_WIDTH = 100.0f;

const float PADDLE_HEIGHT = 20.0f;

const float PADDLE_SPEED = 5.0f;

float paddleX = WIDTH / 2.0f - PADDLE_WIDTH / 2.0f;

float paddleY = 20.0f;

int submenu;

// Ball properties

const float BALL_RADIUS = 25.0f;

float ballX = WIDTH / 2.0f;

float ballY = HEIGHT - 50.0f;

float ballSpeedX = 2.0f;

float ballSpeedY = -2.0f;



// Obstacle properties

const float OBSTACLE_WIDTH = 80.0f;

const float OBSTACLE_HEIGHT = 20.0f;

std::vector<std::pair<float, float>> obstacles;



// Game state

bool isGameOver = false;

int score = 0;



// Function to initialize the game

void init()

{

    GLfloat mat_specular[]={1.0,1.0,1.0,1.0};

    GLfloat mat_shininess[] = {50.0};

    GLfloat light_position[]={1.0,1.0,1.0,0.0};

    glMaterialfv(GL_FRONT,GL_SPECULAR, mat_specular);

    glMaterialfv(GL_FRONT,GL_SHININESS, mat_shininess);

    glLightfv(GL_LIGHT0,GL_POSITION,light_position);

    glEnable(GL_LIGHTING);

    glEnable(GL_LIGHT0);

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.13f,0.37f,0.31f, 1.0f);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    gluOrtho2D(0, WIDTH, 0, HEIGHT);

    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_COLOR_MATERIAL);



    // Add obstacles

    obstacles.push_back(std::make_pair(WIDTH / 2.0f - OBSTACLE_WIDTH / 2.0f, HEIGHT / 2.0f -50.0f));

    obstacles.push_back(std::make_pair(WIDTH / 2.0f - OBSTACLE_WIDTH / 2.0f -200.0f, HEIGHT / 2.0f ));

    obstacles.push_back(std::make_pair(WIDTH / 2.0f - OBSTACLE_WIDTH / 2.0f +200.0f, HEIGHT / 2.0f ));

}



void changeBackgroundColor(int option) //menu function

{

    switch (option)

    {

        case 0: // Red

            glClearColor(0.184314f,0.309804f,0.309804f, 1.0f);

            break;

        case 1: // Green

            glClearColor(0.244f, 0.3744f, 0.15f, 1.0f);

            break;

        case 2: // Blue

            glClearColor(0.74902f,0.847059f,0.847059f, 1.0f);

            break;

    }

}

// Function to handle keyboard input

void handleKeypress(unsigned char key, int x, int y)

{

    switch (key)

    {

    case 'a':

        paddleX -= PADDLE_SPEED;

        break;

    case 'd':

        paddleX += PADDLE_SPEED;

        break;

    case 27: // Escape key

        exit(0);

    }

}



// Function to check collision between the ball and the paddle

bool checkCollisionPaddle()

{

    return ballX >= paddleX && ballX <= paddleX + PADDLE_WIDTH && ballY - BALL_RADIUS <= paddleY + PADDLE_HEIGHT;

}



// Function to check collision between the ball and the obstacles

bool checkCollisionObstacles()

{

    for (int i = 0; i < obstacles.size(); i++) {

        float obstacleX = obstacles[i].first;

        float obstacleY = obstacles[i].second;



        if (ballX >= obstacleX && ballX <= obstacleX + OBSTACLE_WIDTH && ballY + BALL_RADIUS >= obstacleY && ballY - BALL_RADIUS <= obstacleY + OBSTACLE_HEIGHT) {

            return true;

        }

    }

    return false;

}



// Function to update the game state

void update()

{

    // Check for collision with walls

    if (ballX - BALL_RADIUS <= 0 || ballX + BALL_RADIUS >= WIDTH) {

        ballSpeedX = -ballSpeedX;

    }

    if (ballY + BALL_RADIUS >= HEIGHT) {

        ballSpeedY = -ballSpeedY;

    }



    // Check for collision with paddle

    if (checkCollisionPaddle()) {

        ballSpeedY = -ballSpeedY;



        // Increment score

        score++;

    }



    // Check for collision with obstacles

    if (checkCollisionObstacles()) {

        ballSpeedY = -ballSpeedY;



        // Increment score

        score++;

    }



    // Check for game over condition

    if (ballY - BALL_RADIUS <= 0) {

        isGameOver = true;

    }



    // Update ball position

    ballX += ballSpeedX;

    ballY += ballSpeedY;

}

// Function to render the scene

void render()

{

    glClear(GL_COLOR_BUFFER_BIT);



    // Draw paddle

    glColor3f(1.0f, 0.0f, 0.0f);

    glRectf(paddleX, paddleY, paddleX + PADDLE_WIDTH, paddleY + PADDLE_HEIGHT);



    // Draw ball

    glColor3f(1.0f, 0.0f, 0.0f);  // Red color

    glPushMatrix();

    glTranslatef(ballX, ballY, 0);

    glBegin(GL_TRIANGLE_FAN);

    for (float angle = 0.0f; angle <= 2.0f * 3.14159f; angle += 0.2f)

    {

        float x = BALL_RADIUS * cos(angle);

        float y = BALL_RADIUS * sin(angle);

        glVertex2f(x, y);

    }

    glEnd();

    glPopMatrix();



    // Draw obstacles

    glColor3f(0.81f, 0.71f, 0.23f);

    for (int i = 0; i < obstacles.size(); i++)

    {

        glRectf(obstacles[i].first, obstacles[i].second, obstacles[i].first + OBSTACLE_WIDTH, obstacles[i].second + OBSTACLE_HEIGHT);

    }



    // Draw score

    glColor3f(0.0f, 0.0f, 0.0f);

    glRasterPos2f(10.0f, 10.0f);

    std::string scoreText = "Score: " + std::to_string(score);

    for (const char& c : scoreText)

    {

        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);

    }



    // Draw game over text if game is over

    if (isGameOver)

    {

        glColor3f(0.0f, 0.0f, 0.0f);

        glRasterPos2f(WIDTH / 2.0f - 50.0f, HEIGHT / 2.0f);

        std::string gameOverText = "Game Over";

        for (const char& c : gameOverText)

        {

            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);

        }

    }



    glutSwapBuffers();

}



// Function to update the game state and render the scene

void updateAndRender(int value)

{

    update();

    render();

    glutTimerFunc(16, updateAndRender, 0); // 60 FPS

}



int main(int argc, char** argv)

{

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(WIDTH, HEIGHT);

    glutCreateWindow("Arcade Game");

    glutDisplayFunc(render);

    submenu = glutCreateMenu(changeBackgroundColor);   //menu function

    glutAddMenuEntry("Grey", 0);

    glutAddMenuEntry("ForestGreen", 1);

    glutAddMenuEntry("SkyBlue", 2);

    glutAttachMenu(GLUT_RIGHT_BUTTON);

    init();



    glutKeyboardFunc(handleKeypress);

    glutTimerFunc(0, updateAndRender, 0);



    glutMainLoop();



    return 0;

}

