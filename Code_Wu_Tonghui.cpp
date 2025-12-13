//=============================================================================
// BASIC INCLUDES AND DEFINITIONS
//=============================================================================
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#define _USE_MATH_DEFINES

//=============================================================================
// BASIC DATA STRUCTURES
//=============================================================================
// Direction enumeration for movement control
enum Direction {
    UP,     // Upward movement
    DOWN,   // Downward movement
    LEFT,   // Leftward movement
    RIGHT   // Rightward movement
};

// Position structure for 2D coordinates
struct Position {
    int x;  // X coordinate
    int y;  // Y coordinate
};

// 3D Vector structure with basic operations
struct Vec3 {
    float x, y, z;  // Vector components

    // Constructors
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    Vec3(float x_, float y_) : x(x_), y(y_), z(0) {}
};

// Constant for an accurate mathematical representation of    (pi).
const float M_PI = 3.14159265358979323846f;

//=============================================================================
// WINDOW AND DISPLAY SETTINGS
//=============================================================================
// Screen dimensions
int screenWidth = 1024;          // Width of the window in pixels
int screenHeight = 768;          // Height of the window in pixels

// Mouse interaction
bool mouseLeftDown = false;      // Left mouse button state
bool mouseRightDown = false;     // Right mouse button state
float mouseX, mouseY;            // Current mouse cursor position

// Lighting and visual effects
bool spotlightsEnabled = false;  // Global spotlight control
float sceneBrightness = 1.0f;    // Overall scene brightness

GLfloat initialAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat initialDiffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat initialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float initialShininess = 32.0f;

//=============================================================================
// CAMERA SYSTEM
//=============================================================================
// Camera position
float cameraPosX = 0.0f;         // Camera X position
float cameraPosY = 0.0f;         // Camera Y position
float cameraPosZ = 0.0f;         // Camera Z position

// Camera orientation
float cameraAngleY = 0.0f;       // Horizontal rotation (yaw)
float cameraAngleX = 20.0f;      // Vertical rotation (pitch)
float cameraDistance = 9.5f;     // Distance from target
float fieldOfView = 70.0f;       // Perspective field of view
float shakeIntensity = 0.0f;     // Camera shake magnitude

//=============================================================================
// TEXTURE MANAGEMENT
//=============================================================================
// Texture-related variables for controlling texture rendering.
bool gUseTexture = true;       // Toggles texture rendering.
unsigned int texVec[40];         // Array of texture identifiers
unsigned int trackTextureId;     // Texture for roller coaster track
unsigned int supportTextureId;   // Texture for support structures

//=============================================================================
// UI AND INTERACTION ELEMENTS
//=============================================================================
// Arrow animation
float arrowPositionY = 0.0f;        // Vertical position of the arrow
float arrowDirection = 1.0f;        // Arrow movement direction (1: up, -1: down)
const float ARROW_SPEED = 0.1f;     // Speed of arrow movement
const float ARROW_MAX_HEIGHT = 1.0f; // Maximum height of arrow movement
const float ARROW_MIN_HEIGHT = 0.0f; // Minimum height of arrow movement

// Door interaction
float doorAngle = 0.0f;             // Current door rotation angle
bool doorOpen = false;              // Door state (true: open, false: closed)
const float DOOR_OPEN_ANGLE = 90.0f; // Maximum door opening angle
const float DOOR_SPEED = 5.0f;      // Speed of door movement

//=============================================================================
// CHARACTER SYSTEM
//=============================================================================
// Position and orientation
float manPositionX = 0.0f;       // Character X position in world space
float manPositionY = 0.0f;       // Character Y position in world space
float manPositionZ = 0.0f;       // Character Z position in world space
float manRotationAngle = 0.0f;   // Character rotation around Y axis

// Character state flags
bool isWalking = false;          // Walking animation state
bool isRunning = false;          // Running animation state
bool isJumping = false;          // Jumping state
bool isFalling = false;          // Falling state

// Character dimensions
const float legHeight = 2.0f;    // Height of character legs
const float bodyHeight = 3.3f;   // Height of character torso
const float armLength = 2.5f;    // Length of character arms
const float headRadius = 0.9f;   // Radius of character head
const float neckHeight = 0.35f;  // Height of character neck
const float totalHeight = legHeight + bodyHeight + neckHeight + headRadius * 2.0f;

//=============================================================================
// CHARACTER ANIMATION
//=============================================================================
// Limb animation
float limbSwing = 0.0f;          // Current limb swing angle
float limbSwingDirection = 1.0f;  // Direction of limb swing
bool isAlternateTexture = false; // Alternate texture toggle

// Joint rotation angles
float shoulderAngle = 0.0f;      // Current shoulder rotation
float elbowAngle = 0.0f;         // Current elbow rotation
float hipAngle = 0.0f;           // Current hip rotation
float kneeAngle = 0.0f;          // Current knee rotation

//=============================================================================
// PHYSICS AND MOVEMENT
//=============================================================================
// Jump mechanics
float jumpStartTime = 0.0f;      // Time when jump began
float jumpDuration = 0.5f;       // Total jump duration
float jumpStartX, jumpStartY, jumpStartZ;  // Jump starting coordinates
float jumpVelocityX = 0.0f;      // Jump velocity in X direction
float jumpVelocityY = 8.0f;      // Jump velocity in Y direction
float jumpVelocityZ = 5.0f;      // Jump velocity in Z direction

// Fall mechanics
float fallStartTime = 0.0f;      // Time when fall began
float fallInitialX = 0.0f;       // Fall starting X position
float fallInitialY = 0.0f;       // Fall starting Y position
float fallInitialZ = 0.0f;       // Fall starting Z position
float fallVelocityX = 0.0f;      // Fall velocity in X direction
float fallVelocityY = 0.0f;      // Fall velocity in Y direction
float fallVelocityZ = 0.0f;      // Fall velocity in Z direction
const float FALL_DURATION = 5.0f; // Maximum fall duration


//=============================================================================
// ENVIRONMENT SETTINGS
//=============================================================================
// Grass area boundaries
const float GRASS_MIN_X = -50.0f; // Minimum X coordinate of grass area
const float GRASS_MAX_X = 50.0f;  // Maximum X coordinate of grass area
const float GRASS_MIN_Z = -50.0f; // Minimum Z coordinate of grass area
const float GRASS_MAX_Z = 50.0f;  // Maximum Z coordinate of grass area

// House boundaries
const float HOUSE_MIN_X = -6.0f;  // Minimum X coordinate of house
const float HOUSE_MAX_X = 6.0f;   // Maximum X coordinate of house
const float HOUSE_MIN_Z = -6.0f;  // Minimum Z coordinate of house
const float HOUSE_MAX_Z = 6.0f;   // Maximum Z coordinate of house

//=============================================================================
// SKY AND WEATHER EFFECTS
//=============================================================================
// Sky properties
float skyRadius = 500.0f;        // Radius of the sky dome
bool isFadingSky = false;        // Sky transition effect flag
float skyFadeProgress = 0.0f;    // Progress of sky transition (0.0 to 1.0)
float skyFadeDuration = 3.0f;    // Duration of sky transition
float skyFadeStartTime = 0.0f;   // Start time of sky transition
bool useCloudySky = false;       // Cloudy sky texture flag

// Lightning effects
struct LightningBolt {
    std::vector<Vec3> points;  // Points representing the lightning path
    float duration;            // Duration of the lightning effect
    float elapsedTime;         // Time elapsed since lightning started
    bool active;               // Activation state of the lightning bolt
};

LightningBolt currentLightning; // Current lightning bolt instance
bool isLightningActive = false;  // Lightning activity flag
float lightningDuration = 0.0f;  // Duration of lightning effect
float lightningTimer = 0.0f;     // Timer for lightning intervals

//=============================================================================
// AMUSEMENT PARK RIDES
//=============================================================================
// Drop Tower parameters
const float TOWER_POSITION_X = -35.0f;  // X position of drop tower
const float TOWER_POSITION_Z = -35.0f;  // Z position of drop tower
const float TOWER_HEIGHT = 80.0f;       // Height of drop tower
const float BASE_SIZE = 18.0f;          // Size of tower base
const float POLE_RADIUS = 1.2f;         // Radius of support poles
const float POLE_SPACING = 8.0f;        // Spacing between poles
const float levelSpacing = 6.0f;        // Vertical spacing between levels

// Drop Tower dynamics
float dropHeight = TOWER_HEIGHT;        // Initial drop height
bool isDropping = false;                // Dropping state flag
float dropSpeed = 0.0f;                 // Current drop speed
float dropAcceleration = 35.0f;         // Drop acceleration
float recoverySpeed = 15.0f;            // Recovery speed
bool isRecovering = false;              // Recovery state flag

//=============================================================================
// FERRIS WHEEL SYSTEM
//=============================================================================
// Ferris Wheel properties
bool ferrisWheelLightsOn = false;       // Light system activation state
float ferrisWheelRotationAngle = 0.0f;  // Current wheel rotation angle
int numSpokes = 24;                     // Number of wheel spokes
float ferrisWheelPosX = 10.0f;          // Wheel X position
float ferrisWheelPosY = 0.0f;           // Wheel Y position
float ferrisWheelPosZ = 0.0f;           // Wheel Z position

// Ferris Wheel lighting
struct FerrisWheelLight {
    GLfloat position[4];              // Light position (x, y, z, w)
    GLfloat diffuse[4];               // Diffuse light color (r, g, b, a)
    GLfloat constantAttenuation;      // Constant light falloff
    GLfloat linearAttenuation;        // Linear light falloff
    GLfloat quadraticAttenuation;     // Quadratic light falloff
};
std::vector<FerrisWheelLight> wheelLights;  // Collection of wheel lights

//=============================================================================
// ROLLER COASTER SYSTEM
//=============================================================================
const float CARRIAGE_WIDTH = 3.0f;
const float CARRIAGE_LENGTH = 4.0f;
const float SEAT_HEIGHT = 1.8f;
const float CARRIAGE_HEIGHT = 2.5f;
// Track positioning
const float COASTER_START_X = -30.0f;   // Track starting X position
const float COASTER_START_Y = 0.3f;     // Track starting Y position
const float COASTER_START_Z = 50.0f;    // Track starting Z position

// Coaster dimensions
const float COASTER_WIDTH = 60.0f;      // Track width
const float COASTER_HEIGHT = 35.0f;     // Maximum track height
const float COASTER_LENGTH = 80.0f;     // Total track length

// Track system
struct TrackPoint {
    Vec3 position;                      // Point position in 3D space
    Vec3 normal;                        // Surface normal at point
    float twist;                        // Track twist angle
};
std::vector<TrackPoint> trackPoints;    // Track point collection

// Coaster dynamics
float coasterSpeed = 18.0f;             // Current coaster speed
float coasterAcceleration = 2.0f;       // Speed change rate
const float MIN_SPEED = 18.0f;          // Minimum allowed speed
const float MAX_SPEED = 30.0f;          // Maximum allowed speed
const float GRAVITY = 3.0f;             // Gravity constant
bool isCoasterRunning = true;           // Operation state
float trainPosition = 0.0f;             // Position along track

//=============================================================================
// ROLLER COASTER INTERACTION AND CAMERA TRANSITION
//=============================================================================

// Enumeration for train movement direction
enum TrainDirection {
    FORWARD,   // Train moving forward
    BACKWARD   // Train moving backward
};

// Coaster interaction variables
bool isRidingCoaster = false;        // Flag indicating if the player is riding the coaster
bool isCoasterActive = false;        // Flag indicating if the coaster is currently active
float coasterInteractDistance = 15.0f; // Distance within which the player can interact with the coaster
Vec3 lastPlayerPosition;             // Player's position before boarding the coaster
float originalTrainPosition;         // Train's position when it starts moving

// Camera transition variables
bool isTransitioningCamera = false;  // Flag indicating if the camera is transitioning
float cameraTransitionProgress = 0.0f; // Progress of the camera transition (0 to 1)
float cameraTransitionDuration = 1.0f; // Duration of the camera transition
Vec3 transitionStartPos;             // Starting position of the camera transition
Vec3 transitionStartLook;            // Starting look-at position of the camera transition

// Structure to represent the state of the train
struct TrainState {
    Vec3 position;                   // Current position of the train
    float orientation;               // Current orientation of the train
    float speed;                     // Current speed of the train
    float rotationMatrix[16];        // Rotation matrix for the train's orientation
};

// Current train state
TrainState currentTrain;             // Instance of the current train state
TrainDirection currentDirection = FORWARD; // Current direction of the train
TrainDirection nextDirection = FORWARD;    // Next direction of the train

//=============================================================================
// FLYING CHAIR RIDE SYSTEM
//=============================================================================
// Chair positioning
float chairCurrentX = 0.0f;             // Current X position
float chairCurrentY = 8.0f;             // Current Y position
float chairCurrentZ = 10.0f;            // Current Z position
const float detectionRadius = 8.5f;     // Interaction detection range

// Rotation parameters
float flyingChairRotationAngle = 0.0f;  // Base rotation angle
float additionalRotationAngle = 0.0f;   // Secondary rotation angle
float oscillateAngle = 0.0f;            // Oscillation angle
float tiltAngle = 20.0f;                // Chair tilt angle
float alpha = 0.0f;                     // Turntable rotation angle

//=============================================================================
// FLYING CHAIR RIDE DYNAMICS
//=============================================================================
// Chair movement dynamics
bool isRidingChair = false;           // Riding state flag
bool isAggregating = true;            // Aggregation state flag
bool oscillateDirection = true;       // Oscillation direction flag
float currentChairRadius = 10.0f;     // Current orbit radius

// Chair rotation speeds
const float ORBIT_SPEED = 0.2f;                       // Orbit rotation speed
const float ROTATION_SPEED = 0.2f;                    // Y-axis rotation speed
const float ADDITIONAL_ROTATION_SPEED = 0.1f;         // Additional rotation speed
const float OSCILLATE_SPEED = 0.2f;                   // Oscillation speed
const float MAX_OSCILLATE_ANGLE = 45.0f;              // Maximum oscillation angle
const float AGGREGATE_SPREAD_SPEED = 0.05f;           // Aggregation and dispersion speed
const float MIN_CHAIR_RADIUS = 8.0f;                  // Minimum orbit radius
const float MAX_CHAIR_RADIUS = 12.0f;                 // Maximum orbit radius

//=============================================================================
// SNAKE GAME SYSTEM
//=============================================================================
// Game grid settings
const int snake_grid_columns = 30;    // Number of grid columns
const int snake_grid_rows = 30;       // Number of grid rows
float snake_cell_size = 2.0f;         // Size of each grid cell

// Snake game state
Direction snake_dir = RIGHT;          // Initial snake direction
std::vector<Position> snake_body;     // Snake body segments
Position snake_food;                  // Food position
bool snake_game_over = false;         // Game over state
bool snake_game_started = false;      // Game started state

// Fireworks effects
struct SnakeParticle {
    float x, y, z;       // Particle position
    float vx, vy, vz;    // Particle velocity
    float r, g, b;       // Particle color (RGB)
    float life;          // Remaining lifetime
};
std::vector<SnakeParticle> snake_fireworks; // Fireworks particles
bool snake_trigger_fireworks = false;       // Fireworks trigger flag
Position snake_fireworks_position;          // Fireworks position

//=============================================================================
// GAME STATE MANAGEMENT
//=============================================================================
// Game state enumeration
enum GameState {
    MAIN_SCENE,   // Main 3D scene
    SNAKE_GAME    // Snake game mode
};

// Current game state
GameState currentGameState = MAIN_SCENE;

//=============================================================================
// UI ELEMENTS
//=============================================================================
// Console and ring positioning
const float consoleX = -4.9f;            // Console X position
const float consoleY = -4.6f;            // Console Y position
const float consoleZ = -1.5f * 3.0f;     // Console Z position
const float circleX = -4.9f;             // Circle X position
const float circleZ = -4.6f;             // Circle Z position
const float circleRadius = 1.0f;         // Circle radius

//=============================================================================
// VECTOR OPERATIONS
//=============================================================================
// Vector cross product
Vec3 crossProduct(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x);
}

// Vector normalization
void normalize(Vec3& v) {
    float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length > 0) {
        v.x /= length;
        v.y /= length;
        v.z /= length;
    }
}

// Vector dot product
float dotProduct(const Vec3& v1, const Vec3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// Vector length
float vectorLength(const Vec3& v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

// Vector addition
Vec3 addVectors(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

// Vector subtraction
Vec3 subtractVectors(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

// Vector scaling
Vec3 scaleVector(const Vec3& v, float scale) {
    return Vec3(v.x * scale, v.y * scale, v.z * scale);
}
// Function declarations for managing the Snake game logic and rendering.
void snake_init();                       // Initializes the Snake game variables.
void snake_display();                    // Renders the Snake game on screen.
void snake_timer(int);                   // Timer function for Snake game logic updates.
void snake_restart_game();               // Resets the Snake game state to initial conditions.
void snake_create_fireworks(float x, float y, float z); // Creates fireworks at a specified position.
void snake_update_fireworks();           // Updates the positions and states of fireworks particles.
void snake_draw_fireworks();             // Draws the fireworks particles on screen.
void snake_draw_cube(float x, float y, float z, float size, float r, float g, float b); // Draws a colored cube in the game.
void snake_draw_grid();                  // Renders the grid for the Snake game.
void snake_draw_snake_lines();           // Draws connecting lines for the snake's body.
void snake_draw_text_3d(const std::string& text, float x, float y, float z, float scale, float depth); // Renders 3D text.

// Saves the current OpenGL state by storing all attributes and the current transformation matrix.
void saveGLState() {
    glPushAttrib(GL_ALL_ATTRIB_BITS); // Saves all OpenGL attributes (e.g., colors, lighting, textures).
    glPushMatrix();                   // Saves the current transformation matrix.
}

// Restores the previously saved OpenGL state.
void restoreGLState() {
    glPopMatrix();                    // Restores the transformation matrix to the last saved state.
    glPopAttrib();                    // Restores all OpenGL attributes to their previous values.
}

// Helper function to render a hemisphere using quadrilateral strips.
// @param radius: Radius of the hemisphere.
// @param startAngle: Starting latitude angle in degrees.
// @param endAngle: Ending latitude angle in degrees.
void drawHemisphere(float radius, float startAngle, float endAngle) {
    int latSegments = 50;           // Number of segments along the latitude (vertical slices).
    int lonSegments = 50;           // Number of segments along the longitude (horizontal slices).

    // Convert angle ranges from degrees to radians.
    float latStart = startAngle * M_PI / 180.0f;
    float latEnd = endAngle * M_PI / 180.0f;

    // Step size for latitude and longitude divisions.
    float latStep = (latEnd - latStart) / latSegments;
    float lonStep = 2.0f * M_PI / lonSegments;

    // Iterate through latitude and longitude segments to render the hemisphere.
    for (int i = 0; i < latSegments; ++i) {
        float lat0 = latStart + i * latStep;         // Starting latitude for the current segment.
        float lat1 = latStart + (i + 1) * latStep;   // Ending latitude for the current segment.

        glBegin(GL_QUAD_STRIP); // Begin drawing a strip of quadrilaterals.
        for (int j = 0; j <= lonSegments; ++j) {
            float lon = j * lonStep; // Current longitude angle.

            // First vertex on the current latitude.
            float x0 = radius * sinf(lat0) * cosf(lon);
            float y0 = radius * cosf(lat0);
            float z0 = radius * sinf(lat0) * sinf(lon);
            float s0 = (float)j / lonSegments;  // Texture coordinate (s).
            float t0 = (float)i / latSegments;  // Texture coordinate (t).

            glTexCoord2f(s0, t0);              // Set texture coordinates for the first vertex.
            glVertex3f(x0, y0, z0);            // Specify the position of the first vertex.

            // Second vertex on the next latitude.
            float x1 = radius * sinf(lat1) * cosf(lon);
            float y1 = radius * cosf(lat1);
            float z1 = radius * sinf(lat1) * sinf(lon);
            float t1 = (float)(i + 1) / latSegments; // Texture coordinate (t) for the second vertex.

            glTexCoord2f(s0, t1);              // Set texture coordinates for the second vertex.
            glVertex3f(x1, y1, z1);            // Specify the position of the second vertex.
        }
        glEnd(); // End the current strip.
    }
}

// Renders a textured sphere representing the sky.
// @param radius: Radius of the sky sphere.
void drawSkySphere(float radius) {
    glEnable(GL_TEXTURE_2D);                             // Enable 2D texture mapping.
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // Use modulate mode for combining texture and color.
    GLUquadric* quad = gluNewQuadric();                  // Create a new quadric object for rendering.
    gluQuadricTexture(quad, GL_TRUE);                    // Enable texture mapping on the quadric.
    gluQuadricNormals(quad, GLU_SMOOTH);                 // Set normals for smooth shading.
    gluSphere(quad, radius, 64, 64);                     // Render a sphere with the specified radius and detail.
    gluDeleteQuadric(quad);                              // Delete the quadric object to free memory.
    glDisable(GL_TEXTURE_2D);                            // Disable 2D texture mapping.
}

// Structure representing a particle used for simulating rain.
struct RainParticle {
    float x, y, z;      // Position of the particle in 3D space.
    float speedY;       // Falling speed of the particle along the Y-axis.
};

// Container for managing a collection of rain particles.
std::vector<RainParticle> rainParticles;

// Flag indicating whether the rain effect is active.
bool isRaining = false;

// Maximum number of rain particles allowed in the simulation.
int maxRainParticles = 4000;

// Initialize rain particles within the scene
void initRainParticles() {
    rainParticles.clear();  // Clear any existing rain particles from the list

    // Define boundaries for the grass area where the rain will fall
    float grassMinX = -50.0f;
    float grassMaxX = 50.0f;
    float grassMinZ = -50.0f;
    float grassMaxZ = 50.0f;
    float rainHeight = 100.0f;  // Starting height of rain droplets (above the scene)

    // Generate rain particles and assign random positions within the specified boundaries
    for (int i = 0; i < maxRainParticles; ++i) {
        RainParticle particle;
        grassMinX + static_cast<float>(rand()) / RAND_MAX * (grassMaxX - grassMinX);
        particle.x = grassMinX + static_cast<float>(rand()) / RAND_MAX * (grassMaxX - grassMinX);  // Random X within grass area
        particle.y = rainHeight + static_cast<float>(rand()) / RAND_MAX * 50.0f;  // Randomize Y (height) for rain
        particle.z = grassMinZ + static_cast<float>(rand()) / RAND_MAX * (grassMaxZ - grassMinZ);  // Random Z within grass area
        particle.speedY = 20.0f + static_cast<float>(rand()) / RAND_MAX * 10.0f;  // Randomize fall speed for each particle
        rainParticles.push_back(particle);  // Add the particle to the rain collection
    }
}

// Update the positions of rain particles based on elapsed time (deltaTime)
void updateRainParticles(float deltaTime) {
    for (auto& particle : rainParticles) {
        particle.y -= particle.speedY * deltaTime;  // Update Y position based on speed and elapsed time

        // Reset the particle if it has fallen below the ground level
        if (particle.y <= -1.5f) {
            // Reinitialize particle to simulate continuous falling
            particle.y = 100.0f + static_cast<float>(rand()) / RAND_MAX * 50.0f;  // Reset to a random height
            // Randomize X and Z positions within the grass area
            particle.x = GRASS_MIN_X + static_cast<float>(rand()) / RAND_MAX * (GRASS_MAX_X - GRASS_MIN_X);
            particle.z = GRASS_MIN_Z + static_cast<float>(rand()) / RAND_MAX * (GRASS_MAX_Z - GRASS_MIN_Z);
        }
    }
}

// Render rain particles to the screen, creating the visual effect of falling rain
void drawRainParticles() {
    if (!isRaining) return;

    glPushMatrix();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_LIGHTING);

    // 设置更细的线宽
    glLineWidth(1.4f); // 减小线宽，使雨滴更细

    // 调整雨滴颜色和透明度，使其更细腻
    glColor4f(0.7f, 0.7f, 1.0f, 0.4f); // 降低不透明度，使雨滴看起来更细

    // 添加风向效果
    float windAngle = 15.0f;
    float windStrength = 0.2f;

    glBegin(GL_LINES);
    for (const auto& particle : rainParticles) {
        // 减小雨滴长度
        float length = (rand() % 3 + 6) / 10.0f; // 减小长度范围，使雨滴更短

        // 计算风的影响
        float xOffset = length * windStrength * cos(windAngle * M_PI / 180.0f);
        float zOffset = length * windStrength * sin(windAngle * M_PI / 180.0f);

        glVertex3f(particle.x, particle.y, particle.z);
        glVertex3f(particle.x + xOffset, particle.y - length, particle.z + zOffset);
    }
    glEnd();

    // 恢复默认线宽
    glLineWidth(1.0f);

    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glPopMatrix();
}



// Generate a lightning bolt with random zigzag points within the scene
void generateLightningBolt() {
    currentLightning.points.clear();  // Clear any previous lightning points
    currentLightning.duration = 0.4f;  // Set the duration of the lightning (in seconds)
    currentLightning.elapsedTime = 0.0f;  // Initialize the elapsed time for lightning
    currentLightning.active = true;  // Set the lightning to be active

    // Randomly generate the starting position of the lightning in the rain area
    float startX = GRASS_MIN_X + static_cast<float>(rand()) / RAND_MAX * (GRASS_MAX_X - GRASS_MIN_X);
    float startY = 100.0f;  // Lightning starts high in the sky
    float startZ = GRASS_MIN_Z + static_cast<float>(rand()) / RAND_MAX * (GRASS_MAX_Z - GRASS_MIN_Z);

    // Randomly generate the end position near the ground, with a slight random offset
    float endX = startX + (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 20.0f;
    float endY = 0.0f;  // Lightning ends at ground level
    float endZ = startZ + (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 20.0f;

    // Generate the lightning path using random segments to create a zigzag pattern
    int numSegments = 10;  // Define the number of zigzag segments
    float segmentLengthY = (startY - endY) / numSegments;  // Length of each segment in the Y direction
    float prevX = startX, prevY = startY, prevZ = startZ;
    currentLightning.points.push_back({ prevX, prevY, prevZ });  // Add the starting point

    // Generate intermediate lightning points between the start and end
    for (int i = 1; i <= numSegments; ++i) {
        float nextX = prevX + (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 10.0f;
        float nextY = prevY - segmentLengthY;
        float nextZ = prevZ + (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 10.0f;
        currentLightning.points.push_back({ nextX, nextY, nextZ });
        prevX = nextX;
        prevY = nextY;
        prevZ = nextZ;
    }
}

// Render a full-screen lightning flash effect
void drawLightningFlash() {
    if (!isRaining || !isLightningActive) return;  // Exit if it's not raining or lightning is inactive

    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);  // Save current OpenGL attributes
    glDisable(GL_DEPTH_TEST);  // Disable depth testing to ensure the flash renders over everything
    glDisable(GL_LIGHTING);    // Disable lighting to avoid interaction with the flash
    glEnable(GL_BLEND);        // Enable alpha blending for the flash effect
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Set standard alpha blending

    // Calculate flash intensity based on the lightning duration, normalized to the [0, 1] range
    float flashIntensity = lightningDuration / 0.2f;
    glColor4f(1.0f, 1.0f, 1.0f, flashIntensity);  // Set color to white with intensity based on flash duration

    // Draw a full-screen quad to simulate the flash
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, screenWidth, 0, screenHeight);  // Set orthogonal projection for screen space
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(screenWidth, 0);
    glVertex2f(screenWidth, screenHeight);
    glVertex2f(0, screenHeight);
    glEnd();

    // Restore the matrix state after rendering the flash
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glPopAttrib();  // Restore OpenGL attributes
}

// Render the lightning bolt as a line
void drawLightningBolt() {
    if (!isRaining || !isLightningActive) return;  // Exit if it's not raining or lightning is inactive

    glColor3f(1.0f, 1.0f, 1.0f);  // Set color for lightning (white)

    glPushAttrib(GL_ENABLE_BIT);  // Save current OpenGL attributes
    glDisable(GL_LIGHTING);  // Disable lighting for brightness effect

    // Draw the lightning bolt using lines
    glBegin(GL_LINES);
    // Randomize the starting position for the lightning around the character
    float startX = manPositionX + (rand() % 20 - 10);
    float startY = 100.0f;  // Start in the sky
    float startZ = manPositionZ + (rand() % 20 - 10);

    // Set the ending point on the ground
    float endX = startX + (rand() % 10 - 5);
    float endY = -5.0f;  // Ground level
    float endZ = startZ + (rand() % 10 - 5);

    glVertex3f(startX, startY, startZ);  // Start point of lightning
    glVertex3f(endX, endY, endZ);        // End point of lightning
    glEnd();

    glPopAttrib();  // Restore OpenGL attributes
}

// Update lightning bolt state, managing timing and effects
void updateLightningBolt(float deltaTime) {
    if (!isRaining) return;

    lightningTimer -= deltaTime;
    if (lightningTimer <= 0.0f) {
        isLightningActive = true;
        lightningDuration = 0.2f;
        lightningTimer = rand() % 10 - 3;
        sceneBrightness = 2.5f;
    }

    if (isLightningActive) {
        lightningDuration -= deltaTime;

        if (lightningDuration <= 0.0f) {
            isLightningActive = false;
            sceneBrightness = 1.0f;
        }
    }
}

// Initialize the Snake Game environment
void snake_init() {
    // Set the background color (light blue)
    glClearColor(0.5f, 0.7f, 1.0f, 1.0f);

    // Enable depth testing for 3D rendering
    glEnable(GL_DEPTH_TEST);

    // Enable lighting and set up a light source
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Configure the position of the light source
    GLfloat light_pos[] = { (snake_grid_columns * snake_cell_size) / 2.0f, 50.0f, (snake_grid_rows * snake_cell_size) / 2.0f, 1.0f };
    GLfloat light_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    // Enable color material to apply colors to the objects based on the material
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Initialize the snake game state
    snake_body.clear();
    snake_dir = RIGHT;
    snake_game_over = false;
    snake_game_started = true;

    // Set the initial position of the snake at the center of the grid
    snake_body.push_back({ snake_grid_columns / 2, snake_grid_rows / 2 });

    // Place the first food item at a random position within the grid, avoiding the snake's body
    bool valid_position = false;
    while (!valid_position) {
        snake_food.x = (rand() % snake_grid_columns);
        snake_food.y = (rand() % snake_grid_rows);
        valid_position = true;
        for (const auto& segment : snake_body) {
            if (segment.x == snake_food.x && segment.y == snake_food.y) {
                valid_position = false;
                break;
            }
        }
    }

    // Start the game timer
    glutTimerFunc(150, snake_timer, 0);
}

// Create fireworks effect at a specific position in the Snake Game
void snake_create_fireworks(float x, float y, float z) {
    const int num_particles = 100;  // Number of particles in the fireworks
    snake_fireworks.clear();  // Clear any previous fireworks

    // Generate random particles for the fireworks
    for (int i = 0; i < num_particles; ++i) {
        SnakeParticle p;
        p.x = x;
        p.y = y;
        p.z = z;
        float speed = static_cast<float>(rand()) / RAND_MAX * 5.0f + 5.0f;  // Random speed
        float angle1 = static_cast<float>(rand()) / RAND_MAX * M_PI * 2;  // Random angle for direction
        float angle2 = static_cast<float>(rand()) / RAND_MAX * M_PI;  // Random angle for direction

        // Calculate particle velocity based on angles
        p.vx = speed * sin(angle2) * cos(angle1);
        p.vy = speed * cos(angle2);
        p.vz = speed * sin(angle2) * sin(angle1);

        // Randomize color for the fireworks
        p.r = static_cast<float>(rand()) / RAND_MAX;
        p.g = static_cast<float>(rand()) / RAND_MAX;
        p.b = static_cast<float>(rand()) / RAND_MAX;

        p.life = 1.2f;  // Lifespan of each particle (in seconds)

        snake_fireworks.push_back(p);  // Add particle to the fireworks collection
    }
}

// Update the state of the fireworks particles
void snake_update_fireworks() {
    float delta_time = 0.05f;  // Time step for the update

    // Update each particle's position based on its velocity
    for (auto& p : snake_fireworks) {
        p.x += p.vx * delta_time;
        p.y += p.vy * delta_time;
        p.z += p.vz * delta_time;
        p.vy -= 9.8f * delta_time;  // Apply gravity (negative Y-velocity)
        p.life -= delta_time * 0.5f;  // Gradually reduce lifespan for fading effect
    }

    // Remove any particles that have expired (life <= 0)
    snake_fireworks.erase(
        std::remove_if(snake_fireworks.begin(), snake_fireworks.end(),
            [](const SnakeParticle& p) { return p.life <= 0.0f; }),
        snake_fireworks.end()
    );
}

// Render the fireworks to the screen
void snake_draw_fireworks() {
    glDisable(GL_LIGHTING);  // Disable lighting for particles to ensure full color visibility
    glPointSize(4.0f);  // Set the size of the points (particles)
    glBegin(GL_POINTS);
    for (const auto& p : snake_fireworks) {
        glColor4f(p.r, p.g, p.b, p.life);  // Set color with alpha (transparency based on life)
        glVertex3f(p.x, p.y, p.z);  // Draw each particle as a point in 3D space
    }
    glEnd();
    glEnable(GL_LIGHTING);  // Re-enable lighting after rendering the fireworks
}

// Restart the Snake Game, reinitializing all necessary components
void snake_restart_game() {
    snake_init();  // Call the init function to reset the game state
}

// Timer logic for the Snake Game
void snake_timer(int) {
    if (!snake_game_over && currentGameState == SNAKE_GAME) {
        // Move the snake's head according to the current direction
        Position head = snake_body.front();
        switch (snake_dir) {
        case UP:    head.y += 1; break;
        case DOWN:  head.y -= 1; break;
        case LEFT:  head.x -= 1; break;
        case RIGHT: head.x += 1; break;
        }

        // Check for collision with the boundaries of the game grid
        if (head.x < 0 || head.x >= snake_grid_columns || head.y < 0 || head.y >= snake_grid_rows) {
            snake_game_over = true;
            glutPostRedisplay();
            return;
        }

        // Check for collision with the snake's body (self-collision)
        for (const auto& segment : snake_body) {
            if (segment.x == head.x && segment.y == head.y) {
                snake_game_over = true;
                glutPostRedisplay();
                return;
            }
        }

        // Add the new head position to the front of the snake body
        snake_body.insert(snake_body.begin(), head);

        // Check if the snake has eaten food
        if (head.x == snake_food.x && head.y == snake_food.y) {
            // Trigger the fireworks effect upon food consumption
            snake_create_fireworks((snake_food.x + 0.5f) * snake_cell_size, snake_cell_size,
                (snake_food.y + 0.5f) * snake_cell_size);

            // Generate new food at a valid position
            bool valid_position = false;
            while (!valid_position) {
                snake_food.x = rand() % snake_grid_columns;
                snake_food.y = rand() % snake_grid_rows;
                valid_position = true;
                // Ensure that food does not overlap with the snake's body
                for (const auto& segment : snake_body) {
                    if (segment.x == snake_food.x && segment.y == snake_food.y) {
                        valid_position = false;
                        break;
                    }
                }
            }
        }
        else {
            // Remove the tail to maintain the snake's constant length
            snake_body.pop_back();
        }

        // Update fireworks if active
        if (!snake_fireworks.empty()) {
            snake_update_fireworks();
        }

        glutPostRedisplay();
        glutTimerFunc(150, snake_timer, 0); // Set the game speed with a timer
    }
}

// Function to render a cube in the Snake Game
void snake_draw_cube(float x, float y, float z, float size, float r, float g, float b) {
    glPushMatrix();
    glTranslatef((x + 0.5f) * snake_cell_size, y * snake_cell_size, (z + 0.5f) * snake_cell_size);
    glColor3f(r, g, b); // Set color based on the input values
    glutSolidCube(size * snake_cell_size); // Draw a cube with the given size
    glPopMatrix();
}

// Function to render the game grid for the Snake Game
void snake_draw_grid() {
    // Draw the grid floor
    glColor3f(0.8f, 0.8f, 0.8f); // Light gray color
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0, 0.0f, 0);
    glVertex3f(snake_grid_columns * snake_cell_size, 0.0f, 0);
    glVertex3f(snake_grid_columns * snake_cell_size, 0.0f, snake_grid_rows * snake_cell_size);
    glVertex3f(0, 0.0f, snake_grid_rows * snake_cell_size);
    glEnd();

    // Enable anti-aliasing for smooth lines
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glLineWidth(1.0f); // Set a uniform line width

    // Draw grid lines
    glColor3f(0.2f, 0.6f, 0.2f); // Green grid lines
    glBegin(GL_LINES);
    // Vertical grid lines
    for (int i = 0; i <= snake_grid_columns; ++i) {
        float x = i * snake_cell_size;
        glVertex3f(x, 0.02f, 0);
        glVertex3f(x, 0.02f, snake_grid_rows * snake_cell_size);
    }
    // Horizontal grid lines
    for (int j = 0; j <= snake_grid_rows; ++j) {
        float z = j * snake_cell_size;
        glVertex3f(0, 0.02f, z);
        glVertex3f(snake_grid_columns * snake_cell_size, 0.02f, z);
    }
    glEnd();

    // Disable anti-aliasing
    glDisable(GL_LINE_SMOOTH);

    // Draw decorative walls
    glColor3f(0.7f, 0.4f, 0.1f); // Brown walls
    float wall_height = 5.0f * snake_cell_size;

    // Left wall
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, snake_grid_rows * snake_cell_size);
    glVertex3f(0, wall_height, snake_grid_rows * snake_cell_size);
    glVertex3f(0, wall_height, 0);
    glEnd();

    // Right wall
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(snake_grid_columns * snake_cell_size, 0, 0);
    glVertex3f(snake_grid_columns * snake_cell_size, 0, snake_grid_rows * snake_cell_size);
    glVertex3f(snake_grid_columns * snake_cell_size, wall_height, snake_grid_rows * snake_cell_size);
    glVertex3f(snake_grid_columns * snake_cell_size, wall_height, 0);
    glEnd();

    // Back wall
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0, 0, snake_grid_rows * snake_cell_size);
    glVertex3f(snake_grid_columns * snake_cell_size, 0, snake_grid_rows * snake_cell_size);
    glVertex3f(snake_grid_columns * snake_cell_size, wall_height, snake_grid_rows * snake_cell_size);
    glVertex3f(0, wall_height, snake_grid_rows * snake_cell_size);
    glEnd();

    // Front wall
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0, 0, 0);
    glVertex3f(snake_grid_columns * snake_cell_size, 0, 0);
    glVertex3f(snake_grid_columns * snake_cell_size, wall_height, 0);
    glVertex3f(0, wall_height, 0);
    glEnd();
}

// Function to draw the connecting lines between snake segments
void snake_draw_snake_lines() {
    glLineWidth(5.0f);
    glColor3f(1.0f, 0.5f, 0.0f); // Core color (orange)

    float half_cell_size = snake_cell_size / 2.0f;

    for (size_t i = 0; i < snake_body.size(); ++i) {
        Position current = snake_body[i];
        Position next;
        if (i < snake_body.size() - 1) {
            next = snake_body[i + 1];
        }
        else {
            next = current;
        }

        glPushMatrix();
        glTranslatef((current.x + 0.5f) * snake_cell_size, 1.0f * snake_cell_size, (current.y + 0.5f) * snake_cell_size);

        // Draw lines to represent the snake's body connections
        glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 0.0f); // Edge color (yellow)

        if (current.x == next.x) {
            // Vertical line
            glVertex3f(0, 0, -half_cell_size);
            glVertex3f(0, 0, half_cell_size);
        }
        else if (current.y == next.y) {
            // Horizontal line
            glVertex3f(-half_cell_size, 0, 0);
            glVertex3f(half_cell_size, 0, 0);
        }
        else {
            // Diagonal turn (for corners)
            glVertex3f(0, 0, 0);
            if ((current.x - next.x) == 1 && (current.y - next.y) == 1) {
                glVertex3f(-half_cell_size, 0, 0);
                glVertex3f(0, 0, -half_cell_size);
            }
            else if ((current.x - next.x) == -1 && (current.y - next.y) == -1) {
                glVertex3f(half_cell_size, 0, 0);
                glVertex3f(0, 0, half_cell_size);
            }
            else if ((current.x - next.x) == -1 && (current.y - next.y) == 1) {
                glVertex3f(half_cell_size, 0, 0);
                glVertex3f(0, 0, -half_cell_size);
            }
            else if ((current.x - next.x) == 1 && (current.y - next.y) == -1) {
                glVertex3f(-half_cell_size, 0, 0);
                glVertex3f(0, 0, half_cell_size);
            }
        }
        glEnd();

        glPopMatrix();
    }
    glLineWidth(1.0f);
}

// Function to render 3D text with reduced thickness
void snake_draw_text_3d(const std::string& text, float x, float y, float z, float scale, float depth) {
    glPushMatrix();
    glTranslatef(x, y, z);

    // Rotate the text to face the camera
    glRotatef(-40, 0, 0, 0);


    glColor3f(1.0f, 0.0f, 0.0f); // Red front face of the text

    for (float d = -depth / 4.0f; d <= depth / 4.0f; d += scale * 0.2f) { // 修改这里
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, d);
        glScalef(scale, scale, scale);
        for (char c : text) {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
        }
        glPopMatrix();
    }
    glPopMatrix();
}


// Main display function for the Snake Game
void snake_display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // Set up a global ambient light to increase overall scene brightness
    GLfloat ambientLight[] = { 1.9f, 1.9f, 1.9f, 1.9f }; // Higher values will increase brightness
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);

    // Set up the other spotlight or directional lights as usual
    GLfloat diffuseLight[] = { 1.9f, 1.9f, 1.9f, 1.9f };  // White diffuse light
    GLfloat position[] = { 20.0f, 20.0f, 20.0f, 20.0f };      // Light position
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glEnable(GL_LIGHT0);  // Enable light source 0

    // Set the camera view (perspective)
    gluLookAt(
        (snake_grid_columns * snake_cell_size) / 2.0f, 80.0f, (snake_grid_rows * snake_cell_size) + 20.0f,
        (snake_grid_columns * snake_cell_size) / 2.0f, 0.0f, (snake_grid_rows * snake_cell_size) / 2.0f,
        0.0f, 1.0f, 0.0f
    );

    // Draw the game grid
    snake_draw_grid();

    // Draw the snake's body
    for (size_t i = 0; i < snake_body.size(); ++i) {
        if (i == 0)
            snake_draw_cube(snake_body[i].x, 0.5f, snake_body[i].y, 1.0f, 0.0f, 1.0f, 0.0f); // Head (green)
        else
            snake_draw_cube(snake_body[i].x, 0.5f, snake_body[i].y, 0.9f, 0.0f, 0.8f, 0.0f); // Body
    }

    // Draw the connecting lines for the snake
    snake_draw_snake_lines();

    // Draw the food object
    snake_draw_cube(snake_food.x, 0.5f, snake_food.y, 1.0f, 1.0f, 0.0f, 0.0f); // Food (red)

    // Render fireworks if any are present
    if (!snake_fireworks.empty()) {
        snake_draw_fireworks();
    }

    // Display "GAME OVER" text if the game has ended
    if (snake_game_over) {
        glColor3f(1.0f, 1.0f, 1.0f);
        snake_draw_text_3d("GAME OVER",
            (snake_grid_columns / 5) * snake_cell_size,
            7.0f * snake_cell_size,
            ((snake_grid_rows / 2.0f) - 2.0f) * snake_cell_size,
            0.025f * snake_cell_size,
            0.8f * snake_cell_size
        );
        snake_draw_text_3d("Press 'R' to Restart",
            (snake_grid_columns / 10) * snake_cell_size,
            1.0f * snake_cell_size,
            ((snake_grid_rows / 2.0f) - 2.0f) * snake_cell_size,
            0.02f * snake_cell_size,
            0.6f * snake_cell_size
        );
    }



    glutSwapBuffers();
}

// Function to load a texture from a file
unsigned int loadGLTexture(const char* filename) {
    unsigned int texture;

    static GLint   imagewidth;
    static GLint   imageheight;
    static GLint   pixellength;
    static GLubyte* pixeldata;
    FILE* pfile;
    errno_t err = fopen_s(&pfile, filename, "rb");
    if (err != 0 || pfile == nullptr) {
        std::cerr << "Unable to open texture file: " << filename << std::endl;
        exit(1);
    }

    fseek(pfile, 0x0012, SEEK_SET); // Skip BMP file header
    fread(&imagewidth, sizeof(imagewidth), 1, pfile); // Read image width
    fread(&imageheight, sizeof(imageheight), 1, pfile); // Read image height
    pixellength = imagewidth * 3;
    while (pixellength % 4 != 0)
        pixellength++;
    pixellength *= imageheight;
    pixeldata = (GLubyte*)malloc(pixellength);
    if (pixeldata == 0) {
        std::cerr << "Unable to allocate memory for texture." << std::endl;
        exit(1);
    }
    fseek(pfile, 54, SEEK_SET);
    fread(pixeldata, pixellength, 1, pfile);
    fclose(pfile);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#ifdef GL_BGR_EXT
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imagewidth, imageheight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixeldata);
#else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imagewidth, imageheight, 0, GL_BGR, GL_UNSIGNED_BYTE, pixeldata);
#endif

    free(pixeldata); // Release memory after uploading to GPU

    return texture;
}


void mySolidCube(float scale, unsigned int texId) {
    glPushMatrix();
    glScaled(scale, scale, scale); // Scale the cube

    if (gUseTexture) {
        glColor3f(1, 1, 1);  // Set color to white for texturing
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texId);  // Bind the texture
    }
    else {
        glColor3ub(255, 211, 155);  // Set a solid color for the cube
        glDisable(GL_TEXTURE_2D);
    }

    glBegin(GL_QUADS);  // Start drawing the cube as a series of quadrilaterals
    // Front face
    glNormal3f(0, 0, 1);
    glTexCoord2d(0.0, 0.0); glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2d(1.0, 0.0); glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2d(1.0, 1.0); glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2d(0.0, 1.0); glVertex3f(-0.5f, 0.5f, 0.5f);

    // Back face
    glNormal3f(0, 0, -1);
    glTexCoord2d(0.0, 0.0); glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2d(1.0, 0.0); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2d(1.0, 1.0); glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2d(0.0, 1.0); glVertex3f(0.5f, 0.5f, -0.5f);

    // Left face
    glNormal3f(-1, 0, 0);
    glTexCoord2d(0.0, 0.0); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2d(1.0, 0.0); glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2d(1.0, 1.0); glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2d(0.0, 1.0); glVertex3f(-0.5f, 0.5f, -0.5f);

    // Right face
    glNormal3f(1, 0, 0);
    glTexCoord2d(0.0, 0.0); glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2d(1.0, 0.0); glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2d(1.0, 1.0); glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2d(0.0, 1.0); glVertex3f(0.5f, 0.5f, 0.5f);

    // Top face
    glNormal3f(0, 1, 0);
    glTexCoord2d(0.0, 0.0); glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2d(1.0, 0.0); glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2d(1.0, 1.0); glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2d(0.0, 1.0); glVertex3f(-0.5f, 0.5f, -0.5f);

    // Bottom face
    glNormal3f(0, -1, 0);
    glTexCoord2d(0.0, 0.0); glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2d(1.0, 0.0); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2d(1.0, 1.0); glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2d(0.0, 1.0); glVertex3f(0.5f, -0.5f, 0.5f);

    glEnd();  // End of drawing

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();  // Restore previous transformation matrix
}


// Structure to represent a cloud
struct Cloud {
    GLfloat x, y, z; // Position of the cloud in 3D space (X, Y, Z coordinates)
    GLfloat size;    // Size of the cloud
    std::vector<GLfloat> sphereOffsetsX; // Offsets for the X-coordinate of spheres in the cloud
    std::vector<GLfloat> sphereOffsetsY; // Offsets for the Y-coordinate of spheres in the cloud
    std::vector<GLfloat> sphereOffsetsZ; // Offsets for the Z-coordinate of spheres in the cloud
    std::vector<GLfloat> sphereSizes;    // Sizes of the individual spheres in the cloud
};

// Vector to store all clouds
std::vector<Cloud> clouds;

// Function to initialize clouds with random positions, sizes, and shapes
void initClouds() {
    int numClouds = 20; // Number of clouds to generate

    for (int i = 0; i < numClouds; ++i) {
        Cloud cloud;
        // Generate random position for the cloud
        cloud.x = rand() % 2000 - 1000; // Random X position (-1000 to 1000)
        cloud.y = rand() % 200 + 200;   // Random Y position (height) (200 to 400)
        cloud.z = rand() % 2000 - 1000; // Random Z position (-1000 to 1000)
        cloud.size = rand() % 50 + 50;  // Random size of the cloud (50 to 100)

        int numSpheres = 20; // Number of spheres to form the cloud

        for (int j = 0; j < numSpheres; ++j) {
            // Generate random offsets for each sphere within the cloud
            GLfloat offsetX = (rand() % 100 - 50) * cloud.size * 0.01f; // X offset
            GLfloat offsetY = (rand() % 50 - 25) * cloud.size * 0.01f;  // Y offset
            GLfloat offsetZ = (rand() % 100 - 50) * cloud.size * 0.01f; // Z offset
            // Size of the sphere, proportional to the cloud size
            GLfloat sphereSize = cloud.size * (0.3f + (rand() % 70) / 100.0f);

            // Add the generated properties to the cloud
            cloud.sphereOffsetsX.push_back(offsetX);
            cloud.sphereOffsetsY.push_back(offsetY);
            cloud.sphereOffsetsZ.push_back(offsetZ);
            cloud.sphereSizes.push_back(sphereSize);
        }

        // Add the generated cloud to the clouds vector
        clouds.push_back(cloud);
    }
}

// Function to draw a single cloud
// Parameters:
// - cloud: The Cloud object to be drawn
// - color: The RGBA color array for the cloud
void drawCloud(const Cloud& cloud, const GLfloat color[4]) {
    glPushMatrix();
    // Move to the cloud's position
    glTranslatef(cloud.x, cloud.y, cloud.z);

    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set blending mode

    // Set the cloud color with transparency
    glColor4fv(color);
    glDisable(GL_TEXTURE_2D); // Disable texture mapping for drawing the cloud

    // Draw each sphere in the cloud
    int numSpheres = cloud.sphereOffsetsX.size();
    for (int i = 0; i < numSpheres; ++i) {
        glPushMatrix();
        // Move to the sphere's offset position within the cloud
        glTranslatef(cloud.sphereOffsetsX[i], cloud.sphereOffsetsY[i], cloud.sphereOffsetsZ[i]);
        // Draw the sphere (solid)
        glutSolidSphere(cloud.sphereSizes[i], 20, 20);
        glPopMatrix();
    }

    // Restore rendering states
    glEnable(GL_TEXTURE_2D); // Re-enable texture mapping
    glDisable(GL_BLEND);     // Disable blending
    glPopMatrix();           // Restore the previous transformation
}

// Function to draw clouds in the sky
void drawSkyClouds() {
    glPushMatrix(); // Save the current matrix state
    GLfloat cloudColor[4]; // Array to store the RGBA color of the clouds

    if (isFadingSky) {
        // If the sky is fading, transition the cloud color
        float progress = skyFadeProgress; // Progress of the sky fade (0 to 1)
        cloudColor[0] = 1.0f - 0.5f * progress; // Reduces the brightness for R (white to gray)
        cloudColor[1] = 1.0f - 0.5f * progress; // Reduces the brightness for G (white to gray)
        cloudColor[2] = 1.0f - 0.5f * progress; // Reduces the brightness for B (white to gray)
        cloudColor[3] = 0.6f + 0.2f * progress; // Adjusts transparency (slightly more opaque)
    }
    else {
        if (isRaining) {
            // Cloud color for rainy weather (darker gray and less transparency)
            cloudColor[0] = 0.6f; // R component for gray
            cloudColor[1] = 0.6f; // G component for gray
            cloudColor[2] = 0.6f; // B component for gray
            cloudColor[3] = 0.8f; // Higher opacity for rainy clouds
        }
        else {
            // Cloud color for sunny weather (bright white and semi-transparent)
            cloudColor[0] = 1.0f; // R component for white
            cloudColor[1] = 1.0f; // G component for white
            cloudColor[2] = 1.0f; // B component for white
            cloudColor[3] = 0.6f; // Semi-transparent for sunny clouds
        }
    }

    // Draw each cloud using the calculated cloud color
    for (const auto& cloud : clouds) {
        drawCloud(cloud, cloudColor); // Pass the cloud and its color to the drawCloud function
    }

    glPopMatrix(); // Restore the previous matrix state
}

// Function to draw the ground
void drawGround() {
    glPushMatrix(); // Save the current matrix state
    glTranslatef(0.0f, -0.1f, 0.0f);
    glColor3f(0.3f, 0.7f, 0.3f); // Set the ground color to green (representing grass)

    if (gUseTexture) {
        // If textures are enabled, use the grass texture
        glEnable(GL_TEXTURE_2D); // Enable 2D texturing
        glBindTexture(GL_TEXTURE_2D, texVec[9]); // Bind the grass texture
    }
    else {
        // If textures are disabled, ensure texturing is off
        glDisable(GL_TEXTURE_2D);
    }

    // Draw the ground as a quad
    glBegin(GL_QUADS); // Begin drawing a quadrilateral
    glNormal3f(0, 1, 0); // Set normal pointing upwards (0, 1, 0)
    glTexCoord2d(0.0, 0.0); glVertex3f(-50.0f, 0.0f, -50.0f); // Bottom-left corner
    glTexCoord2d(50.0, 0.0); glVertex3f(50.0f, 0.0f, -50.0f); // Bottom-right corner
    glTexCoord2d(50.0, 50.0); glVertex3f(50.0f, 0.0f, 50.0f); // Top-right corner
    glTexCoord2d(0.0, 50.0); glVertex3f(-50.0f, 0.0f, 50.0f); // Top-left corner
    glEnd(); // End drawing the quadrilateral

    if (gUseTexture) {
        glDisable(GL_TEXTURE_2D); // Disable texturing after drawing
    }

    glPopMatrix(); // Restore the previous matrix state
}

// Structure to represent a tree
struct Tree {
    float x, z;           // X and Z position of the tree on the ground
    float height;         // Height of the tree
    float trunkRadius;    // Radius of the tree trunk
};

std::vector<Tree> trees; // Vector to store multiple trees

// Function to initialize tree positions and properties
void initTrees() {
    int numTrees = 5; // Number of trees to generate
    const float FERRIS_WHEEL_CLEARANCE = 25.0f;  // Minimum distance from Ferris wheel
    const float FLYING_CHAIR_CLEARANCE = 25.0f;  // Minimum distance from flying chair
    const float HOUSE_CLEARANCE = 20.0f;         // Minimum distance from house

    for (int i = 0; i < numTrees; ++i) {
        Tree tree;
        bool validPosition;

        do {
            // Generate random positions within the grass area
            tree.x = GRASS_MIN_X + static_cast<float>(rand()) / RAND_MAX * (GRASS_MAX_X - GRASS_MIN_X);
            tree.z = GRASS_MIN_Z + static_cast<float>(rand()) / RAND_MAX * (GRASS_MAX_Z - GRASS_MIN_Z);

            // Check if position is valid (away from all structures)
            validPosition =
                // Check house area
                (abs(tree.x) >= HOUSE_CLEARANCE || abs(tree.z) >= HOUSE_CLEARANCE) &&

                // Check Ferris wheel area
                (sqrt(pow(tree.x - ferrisWheelPosX, 2) + pow(tree.z - ferrisWheelPosZ, 2)) >= FERRIS_WHEEL_CLEARANCE) &&

                // Check flying chair area (centered at -35, 0)
                (sqrt(pow(tree.x - (-35.0f), 2) + pow(tree.z - 0.0f, 2)) >= FLYING_CHAIR_CLEARANCE);

        } while (!validPosition);

        // Set tree properties
        tree.height = 6.0f + static_cast<float>(rand()) / RAND_MAX * 2.0f;  // Random height between 6-8
        tree.trunkRadius = 0.3f;

        trees.push_back(tree);
    }
}

// Structure to represent leaf positions
struct LeafPosition {
    float x, y, z; // X, Y, Z coordinates of a leaf
};

std::vector<LeafPosition> leafPositions; // Vector to store leaf positions

// Function to generate random leaf positions
// Parameters:
// - numLeaves: Number of leaves to generate
// - treeHeight: Height of the tree
// - minHeightRatio: Minimum height of leaves as a ratio of treeHeight
// - spread: Horizontal spread of leaves around the trunk
void generateLeafPositions(int numLeaves, float treeHeight, float minHeightRatio, float spread) {
    leafPositions.clear(); // Clear any existing leaf positions
    for (int i = 0; i < numLeaves; ++i) {
        LeafPosition pos;

        // Generate random X and Z positions within the spread
        pos.x = ((rand() % 100 - 50) / 50.0f) * spread;
        pos.z = ((rand() % 100 - 50) / 50.0f) * spread;

        // Generate random Y position within the specified height range
        float minY = treeHeight * minHeightRatio;
        float maxY = treeHeight;
        pos.y = minY + (rand() % 100 / 100.0f) * (maxY - minY);

        leafPositions.push_back(pos); // Add the leaf position to the vector
    }
}

// Function to draw the foliage (leaves) of a tree
void drawFoliage(float treeHeight) {
    int numLeaves = 200;         // Number of leaves
    float minHeightRatio = 0.5f; // Minimum height of leaves as a ratio of treeHeight
    float spread = 2.0f;         // Horizontal spread of leaves

    if (leafPositions.empty()) {
        generateLeafPositions(numLeaves, treeHeight, minHeightRatio, spread);
    }

    glColor3f(0.0f, 0.8f, 0.0f); // Set foliage color (green)

    glBegin(GL_POINTS); // Draw each leaf as a point
    for (const auto& pos : leafPositions) {
        glVertex3f(pos.x, pos.y, pos.z);
    }
    glEnd();
}

// Function to draw tree branches
// Parameters:
// - treeHeight: Height of the tree
// - trunkRadius: Radius of the trunk
void drawBranches(float treeHeight, float trunkRadius) {
    int numBranches = 5;         // Number of branches
    float branchLength = treeHeight * 0.3f; // Length of each branch

    glColor3f(0.55f, 0.27f, 0.07f); // Set branch color (brown)

    for (int i = 0; i < numBranches; ++i) {
        float angle = (360.0f / numBranches) * i; // Distribute branches evenly in a circle
        glPushMatrix();
        glTranslatef(0.0f, treeHeight * 0.5f, 0.0f); // Move to the midpoint of the trunk
        glRotatef(angle, 0, 1, 0);  // Rotate around the Y-axis
        glRotatef(-45, 1, 0, 0);    // Tilt the branch downwards

        GLUquadric* quad = gluNewQuadric();
        gluQuadricNormals(quad, GLU_SMOOTH); // Enable smooth shading
        gluQuadricTexture(quad, GL_TRUE);    // Enable texture mapping for the branch

        // Draw the branch as a tapered cylinder
        gluCylinder(quad, trunkRadius * 0.1f, 0.0f, branchLength, 12, 4);

        gluDeleteQuadric(quad); // Delete the quadric object
        glPopMatrix();
    }
}

// Function to render all trees
void drawTrees() {
    glPushMatrix();

    for (const auto& tree : trees) {
        glPushMatrix();
        glTranslatef(tree.x, 0.0f, tree.z); // Move to the tree's position

        float treeHeight = tree.height;
        float trunkRadius = tree.trunkRadius;

        // Render the trunk with optional texture mapping
        if (gUseTexture) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texVec[11]); // Bind bark texture
            glColor3f(1.0f, 1.0f, 1.0f); // Set color to white for texture
        }
        else {
            glColor3f(0.55f, 0.27f, 0.07f); // Brown color for the trunk
        }

        // Draw the trunk as a cylinder
        GLUquadric* quad = gluNewQuadric();
        gluQuadricNormals(quad, GLU_SMOOTH); // Smooth shading
        gluQuadricTexture(quad, GL_TRUE);    // Enable texture mapping

        glPushMatrix();
        glRotatef(-90, 1, 0, 0); // Rotate to make the cylinder upright
        gluCylinder(quad, trunkRadius, trunkRadius * 0.7f, treeHeight, 20, 20);

        // Draw the base of the trunk
        gluDisk(quad, 0.0f, trunkRadius, 20, 1);

        // Draw the top of the trunk
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, treeHeight);
        gluDisk(quad, 0.0f, trunkRadius * 0.7f, 20, 1);
        glPopMatrix();

        glPopMatrix();

        // Draw branches
        drawBranches(treeHeight, trunkRadius);

        gluDeleteQuadric(quad); // Delete the quadric object

        if (gUseTexture) {
            glDisable(GL_TEXTURE_2D); // Disable texture mapping
        }

        // Draw foliage as multiple layers of spheres
        int foliageLayers = 4; // Number of foliage layers
        float foliageHeight = 5.0f; // Total foliage height
        float layerHeight = foliageHeight / foliageLayers; // Height of each layer
        float widthFactor = 1.5f; // Width adjustment factor for foliage

        glColor3f(0.0f, 0.8f, 0.0f); // Green color for foliage

        for (int i = 0; i < foliageLayers; ++i) {
            float sphereRadius = (foliageHeight - i * layerHeight) * 0.4f * widthFactor;

            glPushMatrix();
            glTranslatef(0.0f, treeHeight + i * layerHeight, 0.0f); // Position foliage layer
            glutSolidSphere(sphereRadius, 16, 16); // Draw sphere for foliage
            glPopMatrix();
        }

        glPopMatrix();
    }

    glPopMatrix();
}

// Function to render the building's base (floor) with an added rug
void renderBase() {
    glPushMatrix(); // Preserve the current transformation state

    glTranslatef(0.0f, -0.05f, 0.0f); // Slightly lower the base for alignment
    glScalef(12.0f, 0.1f, 12.0f); // Scale to form a flat floor surface

    // Add a rug on the floor
    glPushMatrix();
    glTranslatef(0.0f, 0.03f, 0.0f); // Slightly elevate the rug above the floor


    if (gUseTexture) {
        glEnable(GL_TEXTURE_2D); // Enable texturing for the rug
        glBindTexture(GL_TEXTURE_2D, texVec[0]); // Bind rug texture (ensure texVec[11] is loaded)
        glColor3f(1.0f, 1.0f, 1.0f); // White color for texture fidelity
    }
    else {
        glColor3ub(128, 0, 0); // Dark red color for the rug if no texture
        glDisable(GL_TEXTURE_2D); // Disable texturing
    }

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f); // Upward normal

    // Define rug vertices with texture coordinates
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.0f, 0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.0f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.0f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.0f, -0.5f);
    glEnd();

    glDisable(GL_TEXTURE_2D); // Disable texturing after rendering rug
    glPopMatrix(); // Restore transformation

    glPopMatrix(); // Restore the original transformation state
}



// Function to render the building's roof with an added chimney
void renderRoof() {
    glPushMatrix(); // Save the current transformation matrix

    glTranslatef(0.0f, 6.0f, 0.0f); // Position the roof atop the building

    if (gUseTexture) {
        glEnable(GL_TEXTURE_2D); // Enable texturing for the roof
        glBindTexture(GL_TEXTURE_2D, texVec[10]); // Bind the roof texture
        glColor3f(1.0f, 1.0f, 1.0f); // Use white color to preserve texture colors
    }
    else {
        glColor3ub(150, 75, 0); // Apply a solid brown color when no texture
        glDisable(GL_TEXTURE_2D); // Disable texturing
    }

    // Draw the gabled roof using triangles and quads
    glBegin(GL_TRIANGLES);
    // Front triangle of the roof
    glNormal3f(0.0f, 0.707f, -0.707f); // Normal pointing upwards and forward
    glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, 2.0f, -6.0f); // Apex vertex
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.0f, 0.0f, -6.0f); // Bottom-left vertex
    glTexCoord2f(1.0f, 0.0f); glVertex3f(6.0f, 0.0f, -6.0f);  // Bottom-right vertex

    // Back triangle of the roof
    glNormal3f(0.0f, 0.707f, 0.707f); // Normal pointing upwards and backward
    glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, 2.0f, 6.0f);  // Apex vertex
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.0f, 0.0f, 6.0f);  // Bottom-left vertex
    glTexCoord2f(1.0f, 0.0f); glVertex3f(6.0f, 0.0f, 6.0f);   // Bottom-right vertex
    glEnd();

    // Draw the rectangular sides of the roof
    glBegin(GL_QUADS);
    // Left side of the roof
    glNormal3f(-1.0f, 0.0f, 0.0f); // Normal pointing left
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.0f, 0.0f, -6.0f); // Back-bottom
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-6.0f, 0.0f, 6.0f);  // Front-bottom
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 2.0f, 6.0f);   // Front-top
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 2.0f, -6.0f);  // Back-top

    // Right side of the roof
    glNormal3f(1.0f, 0.0f, 0.0f); // Normal pointing right
    glTexCoord2f(0.0f, 0.0f); glVertex3f(6.0f, 0.0f, -6.0f);  // Back-bottom
    glTexCoord2f(1.0f, 0.0f); glVertex3f(6.0f, 0.0f, 6.0f);   // Front-bottom
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 2.0f, 6.0f);   // Front-top
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 2.0f, -6.0f);  // Back-top
    glEnd();

    if (gUseTexture) {
        glDisable(GL_TEXTURE_2D); // Turn off texturing after rendering the roof
    }

    // Add a chimney on the roof
    glPushMatrix();
    glTranslatef(-4.0f, 1.7f, -4.0f); // Position the chimney
    glScalef(0.5f, 2.0f, 0.5f); // Scale for chimney dimensions

    if (gUseTexture) {
        glEnable(GL_TEXTURE_2D); // Enable texturing for the chimney
        glBindTexture(GL_TEXTURE_2D, texVec[12]); // Bind chimney texture (ensure texVec[12] is loaded)
        glColor3f(1.0f, 1.0f, 1.0f); // White color for texture fidelity
    }
    else {
        glColor3ub(105, 105, 105); // Dark gray color for chimney if no texture
        glDisable(GL_TEXTURE_2D); // Disable texturing
    }

    // Draw the chimney as a scaled cube or cylinder
    glutSolidCube(1.0f); // Using a solid cube for simplicity; can be replaced with cylinders for more detail

    glDisable(GL_TEXTURE_2D); // Disable texturing after chimney
    glPopMatrix(); // Restore transformation

    glPopMatrix(); // Restore the transformation matrix
}



// Function to draw the door of the building
void drawDoor() {
    glPushMatrix(); // Save the current transformation matrix

    // Define the dimensions and position of the door
    float doorWidth = 2.0f;        // Width of the door
    float doorHeight = 4.0f;       // Height of the door
    float doorThickness = 0.1f;    // Thickness of the door
    float doorX = -1.0f;           // X-offset for the door's position
    float doorY = 0.0f;            // Y-position at ground level
    float doorZ = 6.0f;            // Z-offset for the door's position

    // Translate and rotate the door to allow animation (e.g., opening/closing)
    glTranslatef(doorX, doorY, doorZ); // Move to the left edge of the door
    glRotatef(doorAngle, 0.0f, 1.0f, 0.0f); // Rotate the door around the Y-axis
    glTranslatef(0.0f, doorHeight / 2.0f, 0.0f); // Align the door's center for rotation

    if (gUseTexture) {
        glEnable(GL_TEXTURE_2D);  // Enable texture mapping for the door
        glBindTexture(GL_TEXTURE_2D, texVec[22]);  // Bind the door texture
        glColor3f(1.0f, 1.0f, 1.0f);  // Set color to white for texture rendering
    }
    else {
        glColor3ub(139, 69, 19);  // Brown color for the door (if no texture)
        glDisable(GL_TEXTURE_2D);  // Ensure texture mapping is disabled
    }

    // Draw the door as a rectangular prism
    glBegin(GL_QUADS);
    // Front face
    glNormal3f(0, 0, 1);  // Normal vector pointing forward
    glTexCoord2d(0.0, 0.0); glVertex3f(0.0f, -doorHeight / 2.0f, doorThickness / 2.0f);
    glTexCoord2d(1.0, 0.0); glVertex3f(doorWidth, -doorHeight / 2.0f, doorThickness / 2.0f);
    glTexCoord2d(1.0, 1.0); glVertex3f(doorWidth, doorHeight / 2.0f, doorThickness / 2.0f);
    glTexCoord2d(0.0, 1.0); glVertex3f(0.0f, doorHeight / 2.0f, doorThickness / 2.0f);

    // Back face
    glNormal3f(0, 0, -1);  // Normal vector pointing backward
    glTexCoord2d(0.0, 0.0); glVertex3f(doorWidth, -doorHeight / 2.0f, -doorThickness / 2.0f);
    glTexCoord2d(1.0, 0.0); glVertex3f(0.0f, -doorHeight / 2.0f, -doorThickness / 2.0f);
    glTexCoord2d(1.0, 1.0); glVertex3f(0.0f, doorHeight / 2.0f, -doorThickness / 2.0f);
    glTexCoord2d(0.0, 1.0); glVertex3f(doorWidth, doorHeight / 2.0f, -doorThickness / 2.0f);
    glEnd();

    if (gUseTexture) {
        glDisable(GL_TEXTURE_2D);  // Disable texture mapping after drawing
    }

    glPopMatrix(); // Restore the transformation matrix
}

// Function to render the building's walls
void renderWalls() {
    glPushMatrix(); // Preserve the current transformation matrix

    glColor3ub(255, 193, 193); // Set base color for walls (light red)

    // Render the left wall
    glPushMatrix();
    glTranslatef(-6.0f, 3.0f, 0.0f); // Position to the left side
    glScalef(1.5f, 6.0f, 12.0f); // Scale for thickness and size

    if (gUseTexture) {
        glColor3f(1.0f, 1.0f, 1.0f); // White color for texture
        glEnable(GL_TEXTURE_2D); // Enable texturing
    }
    else {
        glColor3ub(238, 106, 80); // Solid color when no texture
        glDisable(GL_TEXTURE_2D); // Disable texturing
    }

    glBindTexture(GL_TEXTURE_2D, texVec[1]); // Bind wall texture
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f); // Normal pointing right

    // Define vertices with texture coordinates
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.5f, 0.5f);
    glEnd();

    glDisable(GL_TEXTURE_2D); // Disable texturing after wall
    glPopMatrix(); // Restore transformation

    // Render the front wall
    glPushMatrix();
    glTranslatef(0.0f, 3.0f, -6.0f); // Position to the front
    glScalef(12.0f, 6.0f, 1.5f); // Scale for size and thickness

    if (gUseTexture) {
        glColor3f(1.0f, 1.0f, 1.0f); // White for texture
        glEnable(GL_TEXTURE_2D); // Enable texturing
    }
    else {
        glColor3ub(238, 106, 80); // Solid color when no texture
        glDisable(GL_TEXTURE_2D); // Disable texturing
    }

    glBindTexture(GL_TEXTURE_2D, texVec[1]); // Bind wall texture
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f); // Normal pointing forward

    // Define vertices with texture coordinates
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D); // Disable texturing after wall
    glPopMatrix(); // Restore transformation

    glColor3ub(255, 255, 255); // Reset color to white for subsequent objects

    // Render the right wall
    glPushMatrix();
    glTranslatef(6.0f, 3.0f, 0.0f); // Position to the right side
    glScalef(1.5f, 6.0f, 12.0f); // Scale for thickness and size

    if (gUseTexture) {
        glColor3f(1.0f, 1.0f, 1.0f); // White for texture
        glEnable(GL_TEXTURE_2D); // Enable texturing
    }
    else {
        glColor3ub(238, 106, 80); // Solid color when no texture
        glDisable(GL_TEXTURE_2D); // Disable texturing
    }

    glBindTexture(GL_TEXTURE_2D, texVec[1]); // Bind wall texture
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f); // Normal pointing left

    // Define vertices with texture coordinates
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.5f, -0.5f);
    glEnd();

    glDisable(GL_TEXTURE_2D); // Disable texturing after wall
    glPopMatrix(); // Restore transformation

    // Render the left segment of the back wall
    glPushMatrix();
    glTranslatef(-3.5f, 3.0f, 6.0f); // Position the left back wall segment
    glScalef(5.0f, 6.0f, 1.5f); // Scale for width and height

    if (gUseTexture) {
        glColor3f(1.0f, 1.0f, 1.0f); // White for texture
        glEnable(GL_TEXTURE_2D); // Enable texturing
    }
    else {
        glColor3ub(238, 106, 80); // Solid color when no texture
        glDisable(GL_TEXTURE_2D); // Disable texturing
    }

    glBindTexture(GL_TEXTURE_2D, texVec[1]); // Bind wall texture
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f); // Normal facing front

    // Define vertices with texture coordinates
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D); // Disable texturing after wall
    glPopMatrix(); // Restore transformation

    // Render the right segment of the back wall
    glPushMatrix();
    glTranslatef(3.5f, 3.0f, 6.0f); // Position the right back wall segment
    glScalef(5.0f, 6.0f, 1.5f); // Scale for width and height

    if (gUseTexture) {
        glColor3f(1.0f, 1.0f, 1.0f); // White for texture
        glEnable(GL_TEXTURE_2D); // Enable texturing
    }
    else {
        glColor3ub(238, 106, 80); // Solid color when no texture
        glDisable(GL_TEXTURE_2D); // Disable texturing
    }

    glBindTexture(GL_TEXTURE_2D, texVec[1]); // Bind wall texture
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f); // Normal facing front

    // Define vertices with texture coordinates
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D); // Disable texturing after wall
    glPopMatrix(); // Restore transformation

    // Render the upper section of the back wall (ceiling area)
    glPushMatrix();
    glTranslatef(0.0f, 4.0f, 6.0f); // Position the upper back wall
    glScalef(2.0f, 2.0f, 1.5f); // Scale for size
    glTranslatef(0.0f, 0.5f, 0.0f); // Adjust vertical position

    if (gUseTexture) {
        glColor3f(1.0f, 1.0f, 1.0f); // White for texture
        glEnable(GL_TEXTURE_2D); // Enable texturing
    }
    else {
        glColor3ub(238, 106, 80); // Solid color when no texture
        glDisable(GL_TEXTURE_2D); // Disable texturing
    }

    glBindTexture(GL_TEXTURE_2D, texVec[1]); // Bind wall texture
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f); // Normal facing front

    // Define vertices with texture coordinates
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D); // Disable texturing after wall
    glPopMatrix(); // Restore transformation

    // Render the left window on the front wall
    glPushMatrix();
    glTranslatef(-3.0f, 3.0f, -5.99f); // Position the left window
    glScalef(1.5f, 1.5f, 0.1f); // Scale for window size
    glColor3ub(173, 216, 230); // Light blue color for glass
    glutSolidCube(1.0f); // Draw the window as a solid cube
    glPopMatrix(); // Restore transformation

    // Render the right window on the front wall
    glPushMatrix();
    glTranslatef(3.0f, 3.0f, -5.99f); // Position the right window
    glScalef(1.5f, 1.5f, 0.1f); // Scale for window size
    glColor3ub(173, 216, 230); // Light blue color for glass
    glutSolidCube(1.0f); // Draw the window as a solid cube
    glPopMatrix(); // Restore transformation

    // Render the roof by calling the roof rendering function
    renderRoof();

    glPopMatrix(); // Restore the original transformation matrix
}

// Function to render a sofa with enhanced visual details
void renderSofa() {
    glPushMatrix(); // Save the current transformation state

    // Render the left armrest of the sofa
    glPushMatrix();
    glTranslatef(-1.7f, 0.6f, 0.0f); // Position relative to sofa center
    glScalef(0.5f, 1.2f, 1.6f); // Scale for armrest dimensions
    glColor3ub(160, 82, 45); // Brown color for armrest
    glutSolidCube(1.0f); // Draw the armrest as a solid cube
    glPopMatrix(); // Restore transformation

    // Render the right armrest of the sofa
    glPushMatrix();
    glTranslatef(1.7f, 0.6f, 0.0f); // Position relative to sofa center
    glScalef(0.5f, 1.2f, 1.6f); // Scale for armrest dimensions
    glColor3ub(160, 82, 45); // Brown color for armrest
    glutSolidCube(1.0f); // Draw the armrest as a solid cube
    glPopMatrix(); // Restore transformation

    // Render the seat cushion of the sofa
    glPushMatrix();
    glTranslatef(0.0f, 0.15f, 0.0f); // Position relative to sofa center
    glScalef(3.4f, 0.3f, 1.6f); // Scale for cushion dimensions
    glColor3ub(210, 180, 140); // Tan color for cushion
    glutSolidCube(1.0f); // Draw the cushion as a solid cube
    glPopMatrix(); // Restore transformation

    // Render the backrest of the sofa
    glPushMatrix();
    glTranslatef(0.0f, 0.75f, -0.7f); // Position slightly behind the cushion
    glScalef(3.8f, 1.5f, 0.2f); // Scale for backrest dimensions
    glColor3ub(160, 82, 45); // Brown color for backrest
    glutSolidCube(1.0f); // Draw the backrest as a solid cube
    glPopMatrix(); // Restore transformation

    // Render the left side cushion of the sofa
    glPushMatrix();
    glTranslatef(-0.75f, 0.5f, 0.2f); // Position above the seat
    glScalef(1.4f, 0.4f, 1.6f); // Scale for cushion dimensions
    glColor3ub(210, 180, 140); // Tan color for cushion
    glutSolidCube(1.0f); // Draw the cushion as a solid cube
    glPopMatrix(); // Restore transformation

    // Render the right side cushion of the sofa
    glPushMatrix();
    glTranslatef(0.75f, 0.5f, 0.2f); // Position above the seat
    glScalef(1.5f, 0.4f, 1.6f); // Scale for cushion dimensions
    glColor3ub(210, 180, 140); // Tan color for cushion
    glutSolidCube(1.0f); // Draw the cushion as a solid cube
    glPopMatrix(); // Restore transformation

    // Add decorative pillows on the sofa
    // Left pillow
    glPushMatrix();
    glTranslatef(-1.0f, 0.6f, 0.3f); // Position the left pillow
    glScalef(0.3f, 0.2f, 0.3f); // Scale for pillow dimensions
    glColor3ub(255, 192, 203); // Pink color for pillow
    glutSolidCube(1.0f); // Draw the pillow as a solid cube
    glPopMatrix(); // Restore transformation

    // Right pillow
    glPushMatrix();
    glTranslatef(1.0f, 0.6f, 0.3f); // Position the right pillow
    glScalef(0.3f, 0.2f, 0.3f); // Scale for pillow dimensions
    glColor3ub(255, 192, 203); // Pink color for pillow
    glutSolidCube(1.0f); // Draw the pillow as a solid cube
    glPopMatrix(); // Restore transformation

    glPopMatrix(); // Restore the original transformation state
}


// Function to render a cupboard with enhanced details
void renderCupboard() {
    glPushMatrix(); // Save the current transformation state

    glColor3ub(135, 206, 235); // Set base color to sky blue

    // Render the left side panel of the cupboard
    glPushMatrix();
    glTranslatef(-1.0f, 0.4f, 0.0f); // Position relative to cupboard center
    glScalef(0.1f, 0.8f, 0.8f); // Scale for panel dimensions
    if (gUseTexture) {
        glEnable(GL_TEXTURE_2D); // Enable texturing
        glBindTexture(GL_TEXTURE_2D, texVec[4]); // Bind cupboard texture
        glColor3f(1.0f, 1.0f, 1.0f); // White for texture fidelity
    }
    else {
        glColor3ub(70, 130, 180); // Steel blue color if no texture
        glDisable(GL_TEXTURE_2D); // Disable texturing
    }
    glutSolidCube(1.0f); // Draw the panel as a solid cube
    glDisable(GL_TEXTURE_2D); // Disable texturing after panel
    glPopMatrix(); // Restore transformation

    // Render the right side panel of the cupboard
    glPushMatrix();
    glTranslatef(1.0f, 0.4f, 0.0f); // Position relative to cupboard center
    glScalef(0.1f, 0.8f, 0.8f); // Scale for panel dimensions
    if (gUseTexture) {
        glEnable(GL_TEXTURE_2D); // Enable texturing
        glBindTexture(GL_TEXTURE_2D, texVec[4]); // Bind cupboard texture
        glColor3f(1.0f, 1.0f, 1.0f); // White for texture fidelity
    }
    else {
        glColor3ub(70, 130, 180); // Steel blue color if no texture
        glDisable(GL_TEXTURE_2D); // Disable texturing
    }
    glutSolidCube(1.0f); // Draw the panel as a solid cube
    glDisable(GL_TEXTURE_2D); // Disable texturing after panel
    glPopMatrix(); // Restore transformation

    // Render the base of the cupboard
    glPushMatrix();
    glTranslatef(0.0f, 0.2f, 0.0f); // Position relative to cupboard center
    glScalef(2.0f, 0.1f, 0.8f); // Scale for base dimensions
    if (gUseTexture) {
        glEnable(GL_TEXTURE_2D); // Enable texturing
        glBindTexture(GL_TEXTURE_2D, texVec[4]); // Bind cupboard texture
        glColor3f(1.0f, 1.0f, 1.0f); // White for texture fidelity
    }
    else {
        glColor3ub(70, 130, 180); // Steel blue color if no texture
        glDisable(GL_TEXTURE_2D); // Disable texturing
    }
    glutSolidCube(1.0f); // Draw the base as a solid cube
    glDisable(GL_TEXTURE_2D); // Disable texturing after base
    glPopMatrix(); // Restore transformation

    // Render the top shelf of the cupboard
    glPushMatrix();
    glTranslatef(0.0f, 0.8f, 0.0f); // Position relative to cupboard center
    glScalef(2.4f, 0.1f, 0.9f); // Scale for shelf dimensions
    if (gUseTexture) {
        glEnable(GL_TEXTURE_2D); // Enable texturing
        glBindTexture(GL_TEXTURE_2D, texVec[4]); // Bind cupboard texture
        glColor3f(1.0f, 1.0f, 1.0f); // White for texture fidelity
    }
    else {
        glColor3ub(70, 130, 180); // Steel blue color if no texture
        glDisable(GL_TEXTURE_2D); // Disable texturing
    }
    glutSolidCube(1.0f); // Draw the shelf as a solid cube
    glDisable(GL_TEXTURE_2D); // Disable texturing after shelf
    glPopMatrix(); // Restore transformation

    // Add cupboard doors
    // Left door
    glPushMatrix();
    glTranslatef(-0.75f, 0.4f, 0.41f); // Position the left door slightly in front of the panel
    glScalef(0.45f, 0.8f, 0.05f); // Scale for door dimensions
    if (gUseTexture) {
        glEnable(GL_TEXTURE_2D); // Enable texturing
        glBindTexture(GL_TEXTURE_2D, texVec[14]); // Bind door texture (ensure texVec[14] is loaded)
        glColor3f(1.0f, 1.0f, 1.0f); // White for texture fidelity
    }
    else {
        glColor3ub(169, 169, 169); // Dark gray color if no texture
        glDisable(GL_TEXTURE_2D); // Disable texturing
    }
    glutSolidCube(1.0f); // Draw the door as a solid cube
    glDisable(GL_TEXTURE_2D); // Disable texturing after door

    // Add a doorknob to the left door
    glPushMatrix();
    glTranslatef(0.3f, 0.0f, 0.0f); // Position the doorknob on the door
    glColor3ub(255, 215, 0); // Gold color for the doorknob
    glutSolidSphere(0.05f, 16, 16); // Draw the doorknob as a small sphere
    glPopMatrix(); // Restore transformation
    glPopMatrix(); // Restore transformation

    // Right door
    glPushMatrix();
    glTranslatef(0.75f, 0.4f, 0.41f); // Position the right door slightly in front of the panel
    glScalef(0.45f, 0.8f, 0.05f); // Scale for door dimensions
    if (gUseTexture) {
        glEnable(GL_TEXTURE_2D); // Enable texturing
        glBindTexture(GL_TEXTURE_2D, texVec[14]); // Bind door texture (ensure texVec[14] is loaded)
        glColor3f(1.0f, 1.0f, 1.0f); // White for texture fidelity
    }
    else {
        glColor3ub(169, 169, 169); // Dark gray color if no texture
        glDisable(GL_TEXTURE_2D); // Disable texturing
    }
    glutSolidCube(1.0f); // Draw the door as a solid cube
    glDisable(GL_TEXTURE_2D); // Disable texturing after door

    // Add a doorknob to the right door
    glPushMatrix();
    glTranslatef(-0.3f, 0.0f, 0.0f); // Position the doorknob on the door
    glColor3ub(255, 215, 0); // Gold color for the doorknob
    glutSolidSphere(0.05f, 16, 16); // Draw the doorknob as a small sphere
    glPopMatrix(); // Restore transformation
    glPopMatrix(); // Restore transformation

    glPopMatrix(); // Restore the original transformation state
}



/**
 * Renders a floor lamp with dynamic lighting effects and material properties.
 * The lamp consists of three main components:
 * 1. A stable cylindrical base
 * 2. A vertical support pole
 * 3. A conical lampshade with illumination
 */
void renderFloorLamp() {
    glPushMatrix(); // Preserve the current transformation matrix state

    // Store current material properties for later restoration
    GLfloat current_ambient[4], current_diffuse[4], current_specular[4];
    GLfloat current_shininess;
    glGetMaterialfv(GL_FRONT, GL_AMBIENT, current_ambient);
    glGetMaterialfv(GL_FRONT, GL_DIFFUSE, current_diffuse);
    glGetMaterialfv(GL_FRONT, GL_SPECULAR, current_specular);
    glGetMaterialfv(GL_FRONT, GL_SHININESS, &current_shininess);

    // Define material properties for the metallic lamp base
    GLfloat base_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };   // Ambient reflection coefficient
    GLfloat base_diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };   // Diffuse reflection coefficient
    GLfloat base_specular[] = { 0.9f, 0.9f, 0.9f, 1.0f };  // Specular reflection coefficient
    GLfloat base_shininess = 32.0f;                       // Material shininess factor

    // Apply material properties to the lamp base
    glMaterialfv(GL_FRONT, GL_AMBIENT, initialAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, initialDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, initialSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, initialShininess);

    // Render the cylindrical base of the lamp
    glPushMatrix();
    glTranslatef(0.0f, 0.01f, 0.0f);  // Slight elevation to prevent z-fighting
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);  // Rotate to vertical orientation
    glutSolidCylinder(0.5f, 0.01f, 32, 8);  // Parameters: radius, height, slices, stacks
    glPopMatrix();

    // Render the vertical support pole
    glPushMatrix();
    glTranslatef(0.0f, 3.0f, 0.0f);  // Position at appropriate height
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);  // Vertical alignment
    glutSolidCylinder(0.07f, 3.0f, 32, 8);  // Thin, tall cylinder
    glPopMatrix();

    // Define material properties for the illuminated lampshade
    GLfloat shade_ambient[] = { 0.8f, 0.8f, 0.0f, 1.0f };   // Warm ambient glow
    GLfloat shade_diffuse[] = { 1.0f, 1.0f, 0.0f, 1.0f };   // Yellow diffuse color
    GLfloat shade_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // Bright specular highlights
    GLfloat shade_shininess = 128.0f;                      // High shininess for glossy appearance

    // Apply lampshade material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, shade_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, shade_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, shade_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shade_shininess);

    // Create and configure quadric object for smooth shading
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);  // Enable smooth normal vectors

    // Render the conical lampshade
    glPushMatrix();
    glTranslatef(0.0f, 3.0f, 0.0f);  // Position at top of pole
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);  // Vertical orientation
    gluCylinder(quad, 0.2f, 0.5f, 0.7f, 32, 8);  // Tapered cylinder for shade
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);  // Flip orientation for top cap
    gluDisk(quad, 0.0f, 0.2f, 32, 32);  // Top cap of lampshade
    glPopMatrix();

    gluDeleteQuadric(quad);  // Clean up quadric object

    // Configure and enable spotlight if lighting is active
    if (spotlightsEnabled) {
        // Define light source properties
        GLfloat light_position[] = { 0.0f, 3.5f, 0.0f, 1.0f };  // Positioned above lampshade
        GLfloat light_ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f };   // Soft ambient illumination
        GLfloat light_diffuse[] = { 1.0f, 1.0f, 0.8f, 1.0f };   // Warm diffuse light
        GLfloat light_specular[] = { 1.0f, 1.0f, 0.8f, 1.0f };  // Bright specular component
        GLfloat spot_direction[] = { 0.0f, -1.0f, 0.0f };       // Downward beam direction

        // Configure spotlight parameters
        glEnable(GL_LIGHT1);  // Activate light source
        glLightfv(GL_LIGHT1, GL_POSITION, light_position);
        glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

        // Set spotlight characteristics
        glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0f);     // Cone angle of light beam
        glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0f);    // Light concentration factor
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);

        // Configure light attenuation parameters
        glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);    // Constant factor
        glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.05f);     // Distance-based factor
        glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.01f);  // Quadratic falloff
    }

    // Restore original material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, current_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, current_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, current_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, current_shininess);

    glPopMatrix();  // Restore original transformation matrix
}


// Function to render a table lamp
void renderTableLamp() {
    glPushMatrix(); // Save the current transformation state

    glColor3ub(139, 134, 130); // Set lamp base color to gray

    // Render the lamp's base (a thin cylinder for stability)
    glPushMatrix();
    glTranslatef(0.0f, 0.01f, 0.0f); // Slightly elevate the base above ground
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Align cylinder vertically
    glutSolidCylinder(0.5f, 0.01f, 32, 8); // Draw the base cylinder
    glPopMatrix(); // Restore transformation

    // Render the lamp pole (a shorter, thin cylinder)
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.0f); // Position above the base
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Align cylinder vertically
    glutSolidCylinder(0.07f, 1.5f, 32, 8); // Draw the pole cylinder
    glPopMatrix(); // Restore transformation

    glColor3ub(255, 215, 0); // Set lampshade color to yellow

    // Create a quadric object for smooth shading
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH); // Enable smooth normals

    // Render the lampshade (a tapered cylinder with a base disk)
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.0f); // Position at the top of the pole
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Align cylinder vertically
    gluCylinder(quad, 0.2f, 0.5f, 0.7f, 32, 8); // Draw the tapered cylinder
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f); // Flip to align disk with base
    gluDisk(quad, 0.0f, 0.2f, 32, 32); // Draw the base disk
    glPopMatrix(); // Restore transformation

    gluDeleteQuadric(quad); // Clean up the quadric object
    glPopMatrix(); // Restore the original transformation state
}

// Function to render a desk with enhanced visual details and additional objects
void renderDesk() {
    glPushMatrix(); // Save the current transformation state

    // Render the desk's top surface
    glPushMatrix();
    glTranslatef(0.0f, 0.01f, 0.0f); // Slightly elevate above ground
    glScalef(5.0f, 1.0f, 5.0f); // Scale for desk dimensions

    if (gUseTexture) {
        glEnable(GL_TEXTURE_2D); // Enable texturing if active
        glBindTexture(GL_TEXTURE_2D, texVec[5]); // Bind desk texture
        glColor3f(1.0f, 1.0f, 1.0f); // White to maintain texture colors
    }
    else {
        glColor3ub(131, 111, 255); // Apply a blue shade when no texture
        glDisable(GL_TEXTURE_2D); // Disable texturing
    }

    glBegin(GL_QUADS); // Start defining a quadrilateral
    glNormal3f(0.0f, 1.0f, 0.0f); // Normal pointing upward

    // Define vertices with texture coordinates
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.0f, 0.5f);
    glTexCoord2f(5.0f, 0.0f); glVertex3f(0.5f, 0.0f, 0.5f);
    glTexCoord2f(5.0f, 1.0f); glVertex3f(0.5f, 0.0f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.0f, -0.5f);
    glEnd(); // End quadrilateral definition

    glDisable(GL_TEXTURE_2D); // Disable texturing after rendering
    glPopMatrix(); // Restore transformation

    glColor3ub(222, 184, 135); // Set color for desk legs (wood-like)

    // Define positions and rotation angles for desk legs
    float legPositions[4][2] = { {0.7f, 0.5f}, {0.7f, -0.5f}, {-0.7f, 0.5f}, {-0.7f, -0.5f} };
    float legRotations[4] = { 30.0f, 30.0f, -30.0f, -30.0f };

    // Render each desk leg
    for (int i = 0; i < 4; ++i) {
        glPushMatrix();
        glTranslatef(legPositions[i][0], 0.0f, legPositions[i][1]); // Position leg
        glRotatef(legRotations[i], 0.0f, 0.0f, 1.0f); // Rotate leg outward
        glTranslatef(0.0f, 0.8f, 0.0f); // Adjust height
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Align cylinder vertically
        glutSolidCylinder(0.05f, 0.8f, 32, 8); // Draw the leg cylinder
        glPopMatrix(); // Restore transformation
    }

    // Render the middle support bar under the desk
    glPushMatrix();
    glTranslatef(0.0f, 0.74f, 0.0f); // Position near legs
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Align horizontally
    glutSolidCylinder(0.8f, 0.05f, 32, 8); // Draw the support cylinder
    glPopMatrix(); // Restore transformation

    // Add additional objects on the desk
    glColor3ub(255, 140, 0); // Orange color for desk objects

    // Render a laptop on the desk
    glPushMatrix();
    glTranslatef(0.3f, 0.8f, -0.3f); // Position the laptop
    glScalef(0.6f, 0.05f, 0.4f); // Scale for laptop dimensions
    glColor3ub(105, 105, 105); // Dim gray color for laptop base
    glutSolidCube(1.0f); // Draw the laptop base

    // Render the laptop screen
    glPushMatrix();
    glTranslatef(0.0f, 0.5f, 0.0f); // Position the screen on top of the base
    glScalef(1.0f, 0.5f, 0.05f); // Scale for screen dimensions
    glColor3ub(0, 0, 0); // Black color for the screen
    glutSolidCube(1.0f); // Draw the screen as a solid cube
    glPopMatrix(); // Restore transformation
    glPopMatrix(); // Restore transformation

    // Render a desk lamp on the desk
    glPushMatrix();
    glTranslatef(0.0f, 0.8f, 0.6f); // Position the desk lamp
    glScalef(0.4f, 0.4f, 0.4f); // Scale for screen dimensions
    renderTableLamp(); // Call the table lamp rendering function
    glPopMatrix(); // Restore transformation

    glColor3ub(238, 118, 0); // Set color for desk objects (orange)
    // Render a teacup on the desk
    glPushMatrix();
    glTranslatef(-0.3f, 0.86f, -0.3f); // Position the teacup
    glutSolidTeacup(0.3f); // Draw the teacup
    glPopMatrix(); // Restore transformation

    // Render a teapot on the desk
    glPushMatrix();
    glTranslatef(0.3f, 0.86f, 0.0f); // Position the teapot
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f); // Rotate for orientation
    glutSolidTeapot(0.2f); // Draw the teapot
    glPopMatrix(); // Restore transformation


    glPopMatrix(); // Restore the original transformation state
}

// Function to render a partition board with added decorative elements
void renderPartitionBoard() {
    glPushMatrix(); // Save the current transformation state

    // Create a quadric object for smooth cylinder rendering
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH); // Enable smooth shading

    glColor3ub(160, 82, 45); // Set color for cylindrical sections (brown)

    // Render the lower cylindrical section of the partition
    glPushMatrix();
    glScalef(15.0f, 1.0f, 1.0f); // Scale for width
    glRotatef(45.0f, 0.0f, 1.0f, 0.0f); // Rotate for alignment
    glTranslatef(0.0f, 0.15f, 0.0f); // Adjust vertical position
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Align cylinder upright
    gluCylinder(quad, 0.09f, 0.1f, 0.15f, 4, 4); // Draw the lower cylinder
    glPopMatrix(); // Restore transformation

    // Render the rectangular section of the partition
    glPushMatrix();
    glTranslatef(0.0f, 3.0f, 0.0f); // Position the rectangular section
    glScalef(1.9f, 5.9f, 0.1f); // Scale for dimensions
    if (gUseTexture) {
        glEnable(GL_TEXTURE_2D); // Enable texturing
        glBindTexture(GL_TEXTURE_2D, texVec[4]); // Bind partition texture
        glColor3f(1.0f, 1.0f, 1.0f); // White for texture fidelity
    }
    else {
        glColor3ub(160, 82, 45); // Brown color if no texture
        glDisable(GL_TEXTURE_2D); // Disable texturing
    }
    glutSolidCube(1.0f); // Draw the section as a solid cube
    glDisable(GL_TEXTURE_2D); // Disable texturing after section
    glPopMatrix(); // Restore transformation

    glColor3ub(160, 82, 45); // Reset color for upper cylindrical section

    // Render the upper cylindrical section of the partition
    glPushMatrix();
    glTranslatef(0.0f, 5.8f, 0.0f); // Position the upper cylinder
    glScalef(15.0f, 1.0f, 1.0f); // Scale for width
    glRotatef(45.0f, 0.0f, 1.0f, 0.0f); // Rotate for alignment
    glTranslatef(0.0f, 0.1f, 0.0f); // Adjust vertical position
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Align cylinder upright
    gluCylinder(quad, 0.1f, 0.09f, 0.2f, 4, 4); // Draw the upper cylinder
    glPopMatrix(); // Restore transformation

    glPopMatrix(); // Restore the original transformation state
}



void drawTexturedCube(GLfloat size, GLuint textureID) {
    GLfloat halfSize = size / 2.0f;
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);
    // ǰ  
    glNormal3f(0.0f, 0.0f, 1.0f); //         
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfSize, -halfSize, halfSize);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(halfSize, -halfSize, halfSize);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(halfSize, halfSize, halfSize);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfSize, halfSize, halfSize);
    //     
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(halfSize, -halfSize, -halfSize);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfSize, -halfSize, -halfSize);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfSize, halfSize, -halfSize);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(halfSize, halfSize, -halfSize);
    //     
    glNormal3f(-1.0f, 0.0f, 0.0f);
    // ...     Ƶػ        棩
    glEnd();
}


// Draw a photo and decorations
void drawPhoto() {
    glPushMatrix(); // Push the current matrix onto the stack

    // Draw a photo frame
    glColor3ub(139, 69, 0); // Set the color of the photo frame
    glPushMatrix(); // Push the current matrix onto the stack
    glTranslated(0, 3.2, -5.9); // Translate the photo frame
    glScaled(4, 3, 0.1); // Scale the photo frame
    glutSolidCube(1.f); // Draw a solid cube for the photo frame
    glPopMatrix(); // Pop the matrix from the stack

    // Draw a textured photo
    glPushMatrix(); // Push the current matrix onto the stack
    glTranslated(0, 3.2, -5.8); // Translate the photo
    glScaled(3.8, 2.9, 1); // Scale the photo
    if (gUseTexture) {
        glColor3f(1.0, 1.0, 1.0f); // Set the color to white for texture
        glEnable(GL_TEXTURE_2D); // Enable 2D texturing
    }
    else {
        glColor3ub(0, 191, 255); // Set a blue color
        glDisable(GL_TEXTURE_2D); // Disable texturing
    }
    glBindTexture(GL_TEXTURE_2D, texVec[6]); // Bind the texture
    glBegin(GL_QUADS); // Begin drawing a quad
    // Define the vertices and texture coordinates of the quad
    glNormal3f(0, 0, 1);
    glTexCoord2d(0.0, 0.0); glVertex3f(-0.5, -0.5, +0);
    glTexCoord2d(1, 0.0); glVertex3f(0.5, -0.5, -0);
    glTexCoord2d(1, 1); glVertex3f(+0.5, 0.5, -0);
    glTexCoord2d(0.0, 1); glVertex3f(-0.5, 0.5, 0);
    glEnd(); // End drawing the quad
    glDisable(GL_TEXTURE_2D); // Disable 2D texturing
    glPopMatrix(); // Pop the matrix from the stack

    glPopMatrix(); // Pop the matrix from the stack
}

void drawSpotlightLamp() {
    // Define the dimensions of the lamp components
    float lampBaseRadius = 0.3f; // Radius of the lamp base
    float lampBaseHeight = 0.1f; // Height of the lamp base
    float lampBodyRadius = 0.15f; // Radius of the lamp body
    float lampBodyHeight = 1.0f; // Height of the lamp body
    float lampHeadRadius = 0.25f; // Radius of the lamp head
    float lampHeadHeight = 0.5f; // Height of the lamp head

    // Set material properties for a metallic texture
    GLfloat matAmbient[] = { 0.25f, 0.25f, 0.25f, 1.0f }; // Ambient component of material color
    GLfloat matDiffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f }; // Diffuse component of material color
    GLfloat matSpecular[] = { 0.774597f, 0.774597f, 0.774597f, 1.0f }; // Specular component of material color
    GLfloat matShininess[] = { 76.8f }; // Shininess of the material

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient); // Set the ambient material property
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse); // Set the diffuse material property
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular); // Set the specular material property
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess); // Set the shininess material property

    // Draw the lamp base (a disk attached to the wall)
    glPushMatrix(); // Push the current matrix onto the stack
    glRotatef(-90, 1, 0, 0); // Rotate the base to lie flat against the wall
    glutSolidCylinder(lampBaseRadius, lampBaseHeight, 32, 8); // Draw a cylinder for the base
    glPopMatrix(); // Pop the matrix from the stack

    // Draw the lamp body (the cylindrical part connecting the base and the head)
    glPushMatrix(); // Push the current matrix onto the stack
    glTranslatef(0.0f, -lampBodyHeight / 2.0f, 0.0f); // Translate the body
    glRotatef(180, 0, 1, 0); // Rotate the body downwards
    glutSolidCylinder(lampBodyRadius, lampBodyHeight, 16, 8); // Draw a cylinder for the body
    glPopMatrix(); // Pop the matrix from the stack

    // Draw the lamp head (the conical part representing the lampshade)
    glPushMatrix(); // Push the current matrix onto the stack
    glTranslatef(0.0f, -lampBodyHeight, 0.0f); // Translate the head
    glRotatef(180, 0, 1, 0); // Rotate the head downwards
    glutSolidCone(lampHeadRadius, lampHeadHeight, 32, 8); // Draw a cone for the head
    glPopMatrix(); // Pop the matrix from the stack
}

/**
 * @brief Configures and renders spotlights within the scene.
 *
 * This function sets up two spotlights positioned on the right wall of the scene.
 * If spotlights are disabled, it ensures that the corresponding OpenGL light sources are disabled.
 * When enabled, it calculates the positions and directions of the spotlights relative to the game console,
 * configures their lighting parameters, and renders both the spotlight fixtures and their light beams.
 */
void setupSpotlights()
{
    // Check if spotlights are enabled; disable them if not
    if (!spotlightsEnabled) {
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT2);
        return;
    }

    // Calculate the position of the game console
    float scaleFactor = 2.0f;
    float consoleScaleFactor = 3.0f; // Consistent with the scaleFactor used in drawGameConsole
    float baseHeight = 0.15f;

    float consoleX = -5.0f * scaleFactor;
    float consoleY = ((consoleScaleFactor - 1) + baseHeight) * scaleFactor;
    float consoleZ = -1.5f * consoleScaleFactor * scaleFactor;

    float wallHeight = 6.0f * scaleFactor;

    // Define the positions of the spotlights on the right wall (X = -3.0f)
    float spotX = -3.0f; // X-coordinate of the right wall
    float spotY = wallHeight; // Y-coordinate of the spotlight, positioned at the top of the wall
    float spotZ1 = consoleZ - 2.0f * scaleFactor; // Z-coordinate of the first spotlight
    float spotZ2 = consoleZ + 2.0f * scaleFactor; // Z-coordinate of the second spotlight

    // Calculate the direction vectors for the spotlights, pointing towards the game console
    GLfloat spotPosition1[] = { spotX, spotY, spotZ1, 1.0f };
    GLfloat spotPosition2[] = { spotX, spotY, spotZ2, 1.0f };

    // Direction vector from the first spotlight to the game console
    float dirX1 = consoleX - spotX;
    float dirY1 = consoleY - spotY;
    float dirZ1 = consoleZ - spotZ1;
    float length1 = sqrt(dirX1 * dirX1 + dirY1 * dirY1 + dirZ1 * dirZ1);
    GLfloat spotDirection1[] = { -dirX1 / length1, dirY1 / length1, -dirZ1 / length1 };

    // Direction vector from the second spotlight to the game console
    float dirX2 = consoleX - spotX;
    float dirY2 = consoleY - spotY;
    float dirZ2 = consoleZ - spotZ2;
    float length2 = sqrt(dirX2 * dirX2 + dirY2 * dirY2 + dirZ2 * dirZ2);
    GLfloat spotDirection2[] = { -dirX2 / length2, dirY2 / length2, -dirZ2 / length2 };

    // Define the lighting parameters to enhance the brightness of the spotlights
    GLfloat lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f }; // Increase ambient light for overall brightness
    GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Enhance diffuse light intensity
    GLfloat lightSpecular[] = { 1.5f, 1.5f, 1.5f, 1.0f }; // Enhance specular light for stronger highlights

    // Configure the first spotlight (GL_LIGHT1)
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT1, GL_POSITION, spotPosition1);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDirection1);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25.0f); // Set the spotlight cutoff angle
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 15.0f); // Set the spotlight exponent for concentration

    // Configure the second spotlight (GL_LIGHT2)
    glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT2, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT2, GL_POSITION, spotPosition2);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDirection2);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 25.0f); // Set the spotlight cutoff angle
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 15.0f); // Set the spotlight exponent for concentration

    // Render the first spotlight fixture
    glPushMatrix();
    glTranslatef(spotX, spotY, spotZ1);

    // Calculate the rotation axis and angle to align the fixture with the spotlight direction
    GLfloat upVector[] = { 0.0f, -1.0f, 0.0f }; // Default orientation of the fixture pointing along the -Y axis
    GLfloat rotationAxis1[3];
    // Compute the cross product of upVector and spotDirection1 to determine the rotation axis
    rotationAxis1[0] = upVector[1] * spotDirection1[2] - upVector[2] * spotDirection1[1];
    rotationAxis1[1] = upVector[2] * spotDirection1[0] - upVector[0] * spotDirection1[2];
    rotationAxis1[2] = upVector[0] * spotDirection1[1] - upVector[1] * spotDirection1[0];
    // Compute the angle between upVector and spotDirection1
    float dotProduct1 = upVector[0] * spotDirection1[0] + upVector[1] * spotDirection1[1] + upVector[2] * spotDirection1[2];
    float angle1 = acosf(dotProduct1) * 180.0f / M_PI;

    glRotatef(angle1, rotationAxis1[0], rotationAxis1[1], rotationAxis1[2]);
    drawSpotlightLamp(); // Render the spotlight lamp model
    glPopMatrix();

    // Render the second spotlight fixture
    glPushMatrix();
    glTranslatef(spotX, spotY, spotZ2);

    // Calculate the rotation axis and angle to align the fixture with the spotlight direction
    GLfloat rotationAxis2[3];
    // Compute the cross product of upVector and spotDirection2 to determine the rotation axis
    rotationAxis2[0] = upVector[1] * spotDirection2[2] - upVector[2] * spotDirection2[1];
    rotationAxis2[1] = upVector[2] * spotDirection2[0] - upVector[0] * spotDirection2[2];
    rotationAxis2[2] = upVector[0] * spotDirection2[1] - upVector[1] * spotDirection2[0];
    // Compute the angle between upVector and spotDirection2
    float dotProduct2 = upVector[0] * spotDirection2[0] + upVector[1] * spotDirection2[1] + upVector[2] * spotDirection2[2];
    float angle2 = acosf(dotProduct2) * 180.0f / M_PI;

    glRotatef(angle2, rotationAxis2[0], rotationAxis2[1], rotationAxis2[2]);
    drawSpotlightLamp(); // Render the spotlight lamp model
    glPopMatrix();

    // Render the light beams as semi-transparent cones
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_LIGHTING); // Disable lighting to correctly display colors of the beams

    // Enable depth testing and disable depth writing to prevent beams from obscuring other objects
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    glColor4f(1.0f, 1.0f, 0.8f, 0.3f); // Set the color to a semi-transparent light yellow

    // Render the first light beam
    glPushMatrix();
    glTranslatef(spotX, spotY, spotZ1);
    glRotatef(angle1, rotationAxis1[0], rotationAxis1[1], rotationAxis1[2]);

    GLUquadric* quadric1 = gluNewQuadric();
    gluQuadricNormals(quadric1, GLU_SMOOTH);
    float beamLength1 = length1;
    float baseRadius1 = beamLength1 * tanf(25.0f * M_PI / 180.0f); // Calculate the base radius based on the spotlight cutoff angle
    gluCylinder(quadric1, 0.0f, baseRadius1, beamLength1, 32, 1); // Draw the cone representing the light beam
    gluDeleteQuadric(quadric1);
    glPopMatrix();

    // Render the second light beam
    glPushMatrix();
    glTranslatef(spotX, spotY, spotZ2);
    glRotatef(angle2, rotationAxis2[0], rotationAxis2[1], rotationAxis2[2]);

    GLUquadric* quadric2 = gluNewQuadric();
    gluQuadricNormals(quadric2, GLU_SMOOTH);
    float beamLength2 = length2;
    float baseRadius2 = beamLength2 * tanf(25.0f * M_PI / 180.0f); // Calculate the base radius based on the spotlight cutoff angle
    gluCylinder(quadric2, 0.0f, baseRadius2, beamLength2, 32, 1); // Draw the cone representing the light beam
    gluDeleteQuadric(quadric2);
    glPopMatrix();

    // Restore depth writing and re-enable lighting
    glDepthMask(GL_TRUE);
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
}

/**
 * @brief Renders a dynamic, layered circular beam at the specified position.
 *
 * This function creates a visually appealing circular beam composed of multiple layers.
 * Each layer has varying transparency and color intensity to simulate a glowing effect.
 * The beam dynamically changes over time to produce a pulsating glow.
 *
 * @param centerX The X-coordinate of the center position.
 * @param centerY The Y-coordinate of the center position.
 * @param centerZ The Z-coordinate of the center position.
 * @param scaleFactor The scaling factor to adjust the size of the beam.
 */
void drawCoolCircle(float centerX, float centerY, float centerZ, float scaleFactor) {
    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ); // Translate to the specified center position

    float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; // Retrieve the current time in seconds
    int numLayers = 200; // Number of layers in the circular beam
    float layerSpacing = 0.03f * scaleFactor; // Vertical spacing between each layer
    float baseRadius = 2.0f * scaleFactor; // Base radius of the circular beam

    for (int layer = 0; layer < numLayers; ++layer) {
        // Calculate the vertical position of the current layer
        float currentHeight = layerSpacing * layer;

        // Dynamically calculate the transparency and glow intensity for the current layer
        float alpha = pow(1.0f - static_cast<float>(layer) / numLayers, 2.0f); // Smooth transparency decay
        float glowFactor = (sinf(time * 2.0f + layer * 0.3f) + 1.0f) / 2.0f; // Dynamic brightness fluctuation

        // Adjust the color gradient based on the layer index
        float intensity = 1.0f - static_cast<float>(layer) / numLayers; // Intensity decreases from 1.0 to 0.0
        float red = intensity * 1.0f;
        float green = intensity; // Green component decreases but remains present to maintain a yellow hue
        float blue = 0.0f; // Blue component remains zero to preserve the yellow base color

        glColor4f(red, green, blue, alpha * glowFactor); // Set the color and transparency for the current layer

        // Set the line width to create a varying thickness effect for each layer
        glLineWidth(3.0f + layer * 0.2f);

        // Draw the circular ring for the current layer
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 360; ++i) {
            float angleRad = i * M_PI / 180.0f; // Convert angle to radians
            float radius = baseRadius; // Radius remains constant for all layers
            glVertex3f(cos(angleRad) * radius, currentHeight, sin(angleRad) * radius);
        }
        glEnd();
    }

    glPopMatrix();
}


void drawGameConsole()
{
    float scaleFactor = 3.0f; // Uniform scaling factor
    float baseHeight = 0.15f; // Controls the height of the base to ensure it touches the ground

    // Set the material properties for the game console
    GLfloat matAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat matDiffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat matShininess[] = { 50.0f }; // Specular shininess factor

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

    // Render the base of the game console
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(-5.0, 0, -1.5f * scaleFactor);
    glScalef(1.8f * scaleFactor, 0.4f * scaleFactor, 1.2f * scaleFactor);

    if (gUseTexture) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texVec[19]); // Bind the texture for the base
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    else {
        glColor3ub(30, 30, 30); // Set a solid color if textures are not used
    }

    // Draw a textured solid cube for the base
    mySolidCube(1.0f, texVec[19]);

    if (gUseTexture) {
        glDisable(GL_TEXTURE_2D);
    }

    // Draw an additional decorative circle beneath the base
    drawCoolCircle(-0.1, baseHeight - 0.1f, 0.1, 0.5f);

    glPopMatrix();

    // Render the main unit of the game console
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(-5.0, (scaleFactor - 1) + baseHeight, -1.5f * scaleFactor);
    glScalef(1.2f * scaleFactor, 1.2f * scaleFactor, 0.8f * scaleFactor);

    if (gUseTexture) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texVec[19]); // Bind the texture for the main unit
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    else {
        glColor3ub(70, 70, 70); // Set a solid color if textures are not used
    }

    // Draw a textured solid cube for the main unit
    mySolidCube(1.0f, texVec[19]);

    if (gUseTexture) {
        glDisable(GL_TEXTURE_2D);
    }

    glPopMatrix();

    // Render the screen of the game console with a semi-transparent effect
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f); // Rotate 90 degrees to the left
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f); // Rotate 180 degrees backwards
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4ub(0, 191, 255, 150); // Semi-transparent blue color for the screen
    glTranslatef(-5.0, (2 * scaleFactor) + baseHeight, -1.45f * scaleFactor);
    glScalef(0.9f * scaleFactor, 0.6f * scaleFactor, 0.1f * scaleFactor); // Increase thickness to ensure solidity
    glutSolidCube(1.4f); // Draw the screen as a solid cube
    glDisable(GL_BLEND);
    glPopMatrix();

    // Render the neon stripes on the side of the main unit to add a cool effect
    // First neon stripe
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glColor3ub(255, 20, 147); // Pink color for the neon stripe
    glTranslatef(-5.0, (1.7f * scaleFactor) + baseHeight, -1.5f * scaleFactor);
    glScalef(0.1f * scaleFactor, 0.8f * scaleFactor, 0.05f * scaleFactor); // Increase thickness to ensure solidity
    glutSolidCube(1.0f); // Draw the first neon stripe
    glPopMatrix();

    // Second neon stripe
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glColor3ub(255, 20, 147); // Pink color for the neon stripe
    glTranslatef(-5, (1.7f * scaleFactor) + baseHeight, -1.5f * scaleFactor);
    glScalef(0.1f * scaleFactor, 0.8f * scaleFactor, 0.05f * scaleFactor); // Increase thickness to ensure solidity
    glutSolidCube(1.0f); // Draw the second neon stripe
    glPopMatrix();

    // Render the button area of the game console
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glColor3ub(50, 50, 50); // Dark color for the button area
    glTranslatef(-5.0f, (1.3f * scaleFactor) + baseHeight, -1.45f * scaleFactor);
    glScalef(1.0f * scaleFactor, 0.1f * scaleFactor, 0.5f * scaleFactor); // Increase thickness to ensure solidity
    glutSolidCube(1.0f); // Draw the button area as a solid cube
    glPopMatrix();

    // Render the red buttons positioned above the button area
    glColor3ub(255, 0, 0); // Red color for the buttons
    float buttonRadius = 0.1f * scaleFactor;
    for (int i = 0; i < 2; ++i)
    {
        glPushMatrix();
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f); // Rotate 90 degrees to the left
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f); // Rotate 180 degrees backwards
        glTranslatef(-5.4f + (0.15f * i) * scaleFactor, (1.3f * scaleFactor) + baseHeight, -1.35f * scaleFactor);
        glutSolidSphere(buttonRadius, 30, 30); // Draw the red button as a solid sphere
        glPopMatrix();
    }

    // Render the directional keys with a solid design, positioned to the right of the red buttons
    // Directional key base
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f); // Rotate 90 degrees to the left
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f); // Rotate 180 degrees backwards
    glColor3ub(0, 0, 0); // Black color for the directional key base
    glTranslatef(-5.4f + 0.35f * scaleFactor, (1.33f * scaleFactor) + baseHeight, -1.35f * scaleFactor); // Move to the right position
    glScalef(0.35f * scaleFactor, 0.05f * scaleFactor, 0.35f * scaleFactor); // Increase thickness to ensure solidity
    glutSolidCube(1.0f); // Draw the base of the directional key
    glPopMatrix();

    // Directional key horizontal bar
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f); // Rotate 90 degrees to the left
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f); // Rotate 180 degrees backwards
    glColor3ub(0, 0, 255); // Blue color for the horizontal bar
    glTranslatef(-5.4f + 0.4f * scaleFactor, (1.37f * scaleFactor) + baseHeight, -1.3f * scaleFactor); // Move to the right position
    glScalef(0.12f * scaleFactor, 0.05f * scaleFactor, 0.06f * scaleFactor); // Increase thickness to ensure solidity
    glutSolidCube(1.0f); // Draw the horizontal bar of the directional key
    glPopMatrix();

    // Directional key vertical bar
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f); // Rotate 90 degrees to the left
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f); // Rotate 180 degrees backwards
    glColor3ub(0, 0, 255); // Blue color for the vertical bar
    glTranslatef(-5.4f + 0.45f * scaleFactor, (1.37f * scaleFactor) + baseHeight, -1.3f * scaleFactor); // Move to the right position
    glScalef(0.06f * scaleFactor, 0.05f * scaleFactor, 0.12f * scaleFactor); // Increase thickness to ensure solidity
    glutSolidCube(1.0f); // Draw the vertical bar of the directional key
    glPopMatrix();

    // Render the arrow indicator positioned above the game console
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f); // Align with the direction of the game console
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f); // Rotate 180 degrees backwards

    // Set the position of the arrow above the game console
    glTranslatef(-5.0f, (2 * scaleFactor) + baseHeight + arrowPositionY + 2.5f, -1.5f * scaleFactor);

    glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Rotate to point the arrow tip downward

    // Apply a glowing effect using blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set the emission property to create a self-glowing yellow color
    GLfloat emission[] = { 1.0f, 1.0f, 0.0f, 1.0f }; // Yellow emission
    glMaterialfv(GL_FRONT, GL_EMISSION, emission);

    // Draw the arrow as a solid cone
    glutSolidCone(0.45f, 0.9f, 20, 20); // Adjust size to fit the scene

    // Reset the emission property to prevent unintended glowing
    GLfloat no_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);

    glDisable(GL_BLEND); // Disable blending after rendering
    glPopMatrix();
}


/**
 * @brief Renders a support column with a cylindrical body and circular top and bottom.
 *
 * This function creates a support column by drawing a cylinder and capping it with
 * top and bottom disks. The column is oriented along the Y-axis.
 *
 * @param height The height of the support column.
 * @param radius The radius of the cylindrical body of the support column.
 */
void drawSupportColumn(float height, float radius)
{
    // Create a new quadric object for rendering the cylinder and disks
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);      // Enable smooth normals for lighting
    gluQuadricTexture(quad, GL_TRUE);         // Enable texture coordinates

    // Enable and bind texture if texture usage is enabled
    if (gUseTexture)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texVec[30]); // Bind the desired texture
        glColor3f(1.0f, 1.0f, 1.0f);              // Set color to white to display texture colors correctly
    }
    else
    {
        glColor3f(0.7f, 0.7f, 0.7f);              // Set default color if textures are not used
    }

    // Draw the cylindrical body of the support column
    glPushMatrix();
    glTranslatef(10, 0.0f, -1.8f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);      // Rotate to align the cylinder along the Y-axis
    gluCylinder(quad, radius, radius, height, 16, 1); // Draw the cylinder with specified radius and height
    glPopMatrix();

    // Draw the top circular disk of the support column
    glPushMatrix();
    glTranslatef(0.0f, height, 0.0f);          // Move to the top of the cylinder
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);      // Rotate to align the disk with the Y-axis
    gluDisk(quad, 0.0f, radius, 16, 1);        // Draw the top disk
    glPopMatrix();

    // Draw the bottom circular disk of the support column
    glPushMatrix();
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);       // Rotate to align the disk with the Y-axis
    gluDisk(quad, 0.0f, radius, 16, 1);        // Draw the bottom disk
    glPopMatrix();

    // Disable texture mapping if it was previously enabled
    if (gUseTexture)
    {
        glDisable(GL_TEXTURE_2D);
    }

    // Delete the quadric object to free resources
    gluDeleteQuadric(quad);
}

/**
 * @brief Renders the foundation of a structure as a scaled cube with optional texture.
 *
 * This function draws the foundation using a solid cube scaled to the specified dimensions.
 * It applies concrete-like material properties and optionally textures the foundation
 * if texture usage is enabled.
 *
 * @param width The width of the foundation.
 * @param depth The depth of the foundation.
 * @param height The height of the foundation.
 */
void drawFoundation(float width, float depth, float height)
{
    // Define material properties for the concrete foundation
    GLfloat foundationAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat foundationDiffuse[] = { 0.6f, 0.6f, 0.6f, 1.0f };
    GLfloat foundationSpecular[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat foundationShininess = 10.0f;

    // Apply material properties to the front faces
    glMaterialfv(GL_FRONT, GL_AMBIENT, foundationAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, foundationDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, foundationSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, foundationShininess);

    // Enable texture mapping only if needed
    if (gUseTexture)
    {
        glEnable(GL_TEXTURE_2D);             // Enable texture mapping
        glBindTexture(GL_TEXTURE_2D, texVec[30]);  // Bind the texture
        glColor3f(1.0f, 1.0f, 1.0f);        // Set color to white (so the texture colors are visible)
    }
    else
    {
        glColor3f(0.75f, 0.75f, 0.75f);  // Set color to gray if no texture
    }
    glColor3f(0.75f, 0.75f, 0.75f);  // Set color to gray if no texture
    // Draw the foundation as a scaled cube
    glPushMatrix();
    glTranslatef(10.0f, -height / 1.5f, 0.0f);      // Position the foundation appropriately
    glScalef(width, height, depth);               // Scale the cube to desired dimensions
    glutSolidCube(1.0f);                           // Draw a solid cube
    glPopMatrix();

    // Disable texture mapping if it was previously enabled
    if (gUseTexture)
    {
        glDisable(GL_TEXTURE_2D);
    }
}


/**
 * @brief Renders the support structures with enhanced details, including support columns, foundation, and cross braces.
 *
 * This function sets up the material properties for the support structures and optionally applies textures.
 * It then draws the left support column, the concrete foundation, and cross braces connecting various parts
 * of the support system. The function ensures that textures are appropriately enabled or disabled based
 * on the global texture usage flag.
 */
 /**
  * @brief Renders the support structures for the Ferris wheel, including support columns and foundation.
  *
  * This function sets up material properties and textures for the support structures.
  * It draws the support columns and the foundation with the texture at texVec[30] if texture usage is enabled.
  * Transformations are carefully managed to position the supports relative to the Ferris wheel.
  */
void drawSupports()
{
    // Define material properties for the support structures
    GLfloat supportAmbient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat supportDiffuse[] = { 0.6f, 0.6f, 0.6f, 1.0f };
    GLfloat supportSpecular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat supportShininess = 30.0f;

    // Apply material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, supportAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, supportDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, supportSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, supportShininess);

    // Define parameters for the support structures
    float supportHeight = 15.0f;        // Height of the support column
    float supportRadius = 0.5f;         // Radius of the support column
    float baseWidth = 12.0f;            // Width of the foundation base
    float baseDepth = 8.0f;             // Depth of the foundation base
    float baseHeight = 1.0f;            // Height of the foundation base

    // Draw the left support column
    glPushMatrix();
    glTranslatef(-baseWidth / 2.0f + 6.0f, 0.0f, -1.0f); // Adjusted position
    // Enable and bind texture if texture usage is enabled
    if (gUseTexture)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texVec[30]); // Bind the desired texture
        glColor3f(1.0f, 1.0f, 1.0f);              // Set color to white
    }
    else
    {
        glColor3f(0.7f, 0.7f, 0.7f);              // Set default color if textures are not used
    }
    drawSupportColumn(supportHeight, supportRadius);      // Render the support column
    glPopMatrix();

    // Draw the right support column
    glPushMatrix();
    glTranslatef(baseWidth / 2.0f - 6.0f, 0.0f, -1.0f); // Adjusted position
    // Enable and bind texture if texture usage is enabled
    if (gUseTexture)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texVec[30]); // Bind the desired texture
        glColor3f(1.0f, 1.0f, 1.0f);              // Set color to white
    }
    else
    {
        glColor3f(0.7f, 0.7f, 0.7f);              // Set default color if textures are not used
    }
    drawSupportColumn(supportHeight, supportRadius);      // Render the support column
    glPopMatrix();

    // Draw the foundation
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f); // Foundation is centered
    drawFoundation(baseWidth, baseDepth, baseHeight - 1.5f); // Render the foundation
    glPopMatrix();

    // Disable texture mapping if it was previously enabled
    if (gUseTexture)
    {
        glDisable(GL_TEXTURE_2D);
    }
}

/**
 * @brief Initializes the lights for the Ferris wheel, positioning them evenly around the wheel.
 *
 * This function sets up a predefined number of point lights around the Ferris wheel to simulate lighting.
 * Each light is positioned at a specific angle around the wheel's circumference, with warm diffuse colors
 * and attenuation factors to mimic realistic lighting effects. The function ensures that the number of
 * lights does not exceed the maximum supported by OpenGL (GL_LIGHT7).
 */
void initFerrisWheelLights()
{
    int numLights = 6; // Limit to 6 lights to avoid exceeding GL_LIGHT7
    wheelLights.clear(); // Clear any existing lights

    for (int i = 0; i < numLights; ++i)
    {
        FerrisWheelLight light;
        float angle = i * 360.0f / numLights; // Calculate the angle for each light
        float rad = angle * M_PI / 180.0f;    // Convert angle to radians

        // Set the position of the light in a circular arrangement around the wheel
        light.position[0] = 10.0f * cos(rad);            // X-coordinate
        light.position[1] = 10.0f * sin(rad) + 10.0f;    // Y-coordinate with an offset
        light.position[2] = 0.0f;                        // Z-coordinate remains constant
        light.position[3] = 1.0f;                        // Position as a positional light

        // Define warm diffuse light color
        light.diffuse[0] = 1.0f;   // Red component
        light.diffuse[1] = 0.6f;   // Green component
        light.diffuse[2] = 0.3f;   // Blue component
        light.diffuse[3] = 1.0f;   // Alpha component

        // Set attenuation factors to simulate point light behavior
        light.constantAttenuation = 1.0f;  // Constant attenuation factor
        light.linearAttenuation = 0.05f;   // Linear attenuation factor
        light.quadraticAttenuation = 0.01f;// Quadratic attenuation factor

        wheelLights.push_back(light); // Add the configured light to the wheelLights vector
    }
}

/**
 * @brief Renders inner rings as solid toruses with varying radii and thickness.
 *
 * This function draws a specified number of inner rings around a rim. Each ring has a reduced radius
 * compared to the rim radius and an increased thickness to enhance visual details. The rings are
 * optionally textured based on the global texture usage flag.
 *
 * @param rimRadius The radius of the outermost rim around which the inner rings are drawn.
 * @param numInnerRings The number of inner rings to render.
 */
void drawInnerRings(float rimRadius, int numInnerRings)
{
    for (int r = 1; r <= numInnerRings; ++r)
    {
        // Calculate the radius for the current inner ring
        float ringRadius = rimRadius * (1.0f - r * 0.25f); // Inner ring radius
        float ringThickness = 0.15f; // Increased thickness for inner rings

        glPushMatrix();

        // Define material properties and color for the inner rings
        GLfloat ringAmbient[] = { 0.6f, 0.6f, 0.6f, 1.0f };
        GLfloat ringDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
        GLfloat ringSpecular[] = { 0.9f, 0.9f, 0.9f, 1.0f };
        GLfloat ringShininess = 50.0f;

        // Apply material properties to the front faces
        glMaterialfv(GL_FRONT, GL_AMBIENT, ringAmbient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, ringDiffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, ringSpecular);
        glMaterialf(GL_FRONT, GL_SHININESS, ringShininess);

        // Enable and bind texture if texture usage is enabled
        if (gUseTexture)
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texVec[26]); // Bind metal texture
            glColor3f(1.0f, 1.0f, 1.0f);              // Set color to white to display texture colors correctly
        }
        else
        {
            glColor3f(0.8f, 0.8f, 0.8f);              // Set color to light gray if textures are not used
        }

        // Draw the solid torus representing the inner ring
        glutSolidTorus(ringThickness, ringRadius, 32, 128);

        // Disable texture mapping if it was previously enabled
        if (gUseTexture)
        {
            glDisable(GL_TEXTURE_2D);
        }

        glPopMatrix();
    }
}

/**
 * @brief Renders a cabin with swing animation and specified color.
 *
 * This function draws a cabin for the Ferris wheel, applying a swing angle to simulate swinging motion.
 * The cabin consists of a scaled cube for the main body, a cone for the roof, and four windows. Textures
 * are optionally applied based on the global texture usage flag.
 *
 * @param swingAngle The angle by which the cabin is swung to simulate movement.
 * @param r The red component of the cabin's color.
 * @param g The green component of the cabin's color.
 * @param b The blue component of the cabin's color.
 */
void drawCabin(float swingAngle, float r, float g, float b)
{
    glPushMatrix();
    // Apply the swing angle to simulate swinging motion
    glRotatef(swingAngle, 0.0f, 0.0f, 1.0f);

    // Slightly increase the size of the cabin for better visibility
    float cabinScale = 1.4f;

    // Draw the main body of the cabin
    glPushMatrix();
    glScalef(cabinScale, cabinScale, cabinScale);
    if (gUseTexture)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texVec[27]); // Bind cabin texture
        glColor3f(r, g, b); // Set color based on parameters
    }
    else
    {
        glColor3f(r, g, b); // Use specified color if textures are not used
    }
    glutSolidCube(1.0f); // Draw the cabin as a solid cube
    if (gUseTexture)
    {
        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();

    // Draw the roof of the cabin
    glColor3f(0.6f, 0.3f, 0.0f); // Brown color for the roof
    glPushMatrix();
    glTranslatef(0.0f, 0.6f * cabinScale, 0.0f); // Position the roof on top of the cabin
    glutSolidCone(0.6f * cabinScale, 0.4f * cabinScale, 16, 16); // Draw the roof as a solid cone
    glPopMatrix();

    // Draw the windows of the cabin
    glColor3f(0.7f, 0.9f, 1.0f); // Light blue color for the windows

    // Front window
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, (0.51f + 0.1f) * cabinScale); // Position the front window
    glScalef(0.6f * cabinScale, 0.6f * cabinScale, 0.01f); // Scale the window to be flat
    glutSolidCube(1.0f); // Draw the front window as a solid cube
    glPopMatrix();

    // Back window
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, (-0.51f - 0.1f) * cabinScale); // Position the back window
    glScalef(0.6f * cabinScale, 0.6f * cabinScale, 0.01f); // Scale the window to be flat
    glutSolidCube(1.0f); // Draw the back window as a solid cube
    glPopMatrix();

    // Right window
    glPushMatrix();
    glTranslatef((0.51f + 0.1f) * cabinScale, 0.0f, 0.0f); // Position the right window
    glScalef(0.01f, 0.6f * cabinScale, 0.6f * cabinScale); // Scale the window to be thin
    glutSolidCube(1.0f); // Draw the right window as a solid cube
    glPopMatrix();

    // Left window
    glPushMatrix();
    glTranslatef((-0.51f - 0.1f) * cabinScale, 0.0f, 0.0f); // Position the left window
    glScalef(0.01f, 0.6f * cabinScale, 0.6f * cabinScale); // Scale the window to be thin
    glutSolidCube(1.0f); // Draw the left window as a solid cube
    glPopMatrix();

    glPopMatrix(); // End of cabin rendering
}

/**
 * @brief Converts HSV color values to RGB color values.
 *
 * This helper function takes hue, saturation, and value (brightness) components of a color
 * and converts them to their corresponding red, green, and blue components. The conversion
 * is based on the standard HSV to RGB conversion algorithm.
 *
 * @param h Hue component, ranging from 0.0f to 1.0f.
 * @param s Saturation component, ranging from 0.0f to 1.0f.
 * @param v Value (brightness) component, ranging from 0.0f to 1.0f.
 * @param r Reference to the red component to be set.
 * @param g Reference to the green component to be set.
 * @param b Reference to the blue component to be set.
 */
void HSVtoRGB(float h, float s, float v, float& r, float& g, float& b)
{
    float c = v * s; // Chroma
    float hPrime = h * 6.0f; // H' sector
    float x = c * (1.0f - fabs(fmod(hPrime, 2.0f) - 1.0f));
    float m = v - c;

    if (hPrime >= 0.0f && hPrime < 1.0f) {
        r = c; g = x; b = 0.0f;
    }
    else if (hPrime >= 1.0f && hPrime < 2.0f) {
        r = x; g = c; b = 0.0f;
    }
    else if (hPrime >= 2.0f && hPrime < 3.0f) {
        r = 0.0f; g = c; b = x;
    }
    else if (hPrime >= 3.0f && hPrime < 4.0f) {
        r = 0.0f; g = x; b = c;
    }
    else if (hPrime >= 4.0f && hPrime < 5.0f) {
        r = x; g = 0.0f; b = c;
    }
    else if (hPrime >= 5.0f && hPrime < 6.0f) {
        r = c; g = 0.0f; b = x;
    }
    else {
        r = 0.0f; g = 0.0f; b = 0.0f;
    }

    r += m;
    g += m;
    b += m;
}

/**
 * @brief Renders a textured rectangular block positioned in front of the Ferris wheel.
 *
 * This function draws a rectangular block with specified dimensions and applies a texture
 * to the front face if texture usage is enabled. The block is positioned to align with
 * the center of the Ferris wheel. The other five faces of the block are rendered without textures.
 *
 * @note The texture coordinates and normals are properly set to ensure correct lighting and texturing.
 */
void drawFrontBlock()
{
    // Dimensions of the rectangular block
    float blockWidth = 3.6f;   // Length
    float blockHeight = 4.8f;  // Height
    float blockDepth = 1.0f;   // Thickness

    glPushMatrix();
    // Move the block to align with the center position of the Ferris wheel
    glScalef(1.4, 1.4, 1.4);
    glTranslatef(30.0f, 13.0f, 7.8f); // Position at the center of the Ferris wheel (20.0, 13.0, 8.5)

    // 1. Draw the front face with texture if enabled
    if (gUseTexture)
    {
        glEnable(GL_TEXTURE_2D);                     // Enable texturing
        glBindTexture(GL_TEXTURE_2D, texVec[29]);    // Bind texture texVec[29]
    }

    glBegin(GL_QUADS);
    if (gUseTexture)
    {
        glColor3f(1.0f, 1.0f, 1.0f); // Set color to white to display texture colors correctly
        glNormal3f(0.0f, 0.0f, 1.0f); // Normal pointing out of the front face
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-blockWidth / 2, -blockHeight / 2, blockDepth / 2);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(blockWidth / 2, -blockHeight / 2, blockDepth / 2);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(blockWidth / 2, blockHeight / 2, blockDepth / 2);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-blockWidth / 2, blockHeight / 2, blockDepth / 2);
    }
    else
    {
        // If textures are not used, still render the front face
        glColor3f(1.0f, 1.0f, 1.0f); // White color
        glNormal3f(0.0f, 0.0f, 1.0f); // Normal pointing out of the front face
        glVertex3f(-blockWidth / 2, -blockHeight / 2, blockDepth / 2);
        glVertex3f(blockWidth / 2, -blockHeight / 2, blockDepth / 2);
        glVertex3f(blockWidth / 2, blockHeight / 2, blockDepth / 2);
        glVertex3f(-blockWidth / 2, blockHeight / 2, blockDepth / 2);
    }
    glEnd();

    // Disable texturing after rendering the front face
    if (gUseTexture)
    {
        glDisable(GL_TEXTURE_2D); // Disable texturing after completing the front face
    }

    // 2. Draw the other five faces without texture
    glBegin(GL_QUADS);

    // Back face
    glColor3f(0.8f, 0.8f, 0.8f); // Light gray color
    glNormal3f(0.0f, 0.0f, -1.0f); // Normal pointing out of the back face
    glVertex3f(-blockWidth / 2, -blockHeight / 2, -blockDepth / 2);
    glVertex3f(blockWidth / 2, -blockHeight / 2, -blockDepth / 2);
    glVertex3f(blockWidth / 2, blockHeight / 2, -blockDepth / 2);
    glVertex3f(-blockWidth / 2, blockHeight / 2, -blockDepth / 2);

    // Top face
    glNormal3f(0.0f, 1.0f, 0.0f); // Normal pointing out of the top face
    glVertex3f(-blockWidth / 2, blockHeight / 2, -blockDepth / 2);
    glVertex3f(blockWidth / 2, blockHeight / 2, -blockDepth / 2);
    glVertex3f(blockWidth / 2, blockHeight / 2, blockDepth / 2);
    glVertex3f(-blockWidth / 2, blockHeight / 2, blockDepth / 2);

    // Bottom face
    glNormal3f(0.0f, -1.0f, 0.0f); // Normal pointing out of the bottom face
    glVertex3f(-blockWidth / 2, -blockHeight / 2, -blockDepth / 2);
    glVertex3f(blockWidth / 2, -blockHeight / 2, -blockDepth / 2);
    glVertex3f(blockWidth / 2, -blockHeight / 2, blockDepth / 2);
    glVertex3f(-blockWidth / 2, -blockHeight / 2, blockDepth / 2);

    // Right face
    glNormal3f(1.0f, 0.0f, 0.0f); // Normal pointing out of the right face
    glVertex3f(blockWidth / 2, -blockHeight / 2, -blockDepth / 2);
    glVertex3f(blockWidth / 2, blockHeight / 2, -blockDepth / 2);
    glVertex3f(blockWidth / 2, blockHeight / 2, blockDepth / 2);
    glVertex3f(blockWidth / 2, -blockHeight / 2, blockDepth / 2);

    // Left face
    glNormal3f(-1.0f, 0.0f, 0.0f); // Normal pointing out of the left face
    glVertex3f(-blockWidth / 2, -blockHeight / 2, -blockDepth / 2);
    glVertex3f(-blockWidth / 2, blockHeight / 2, -blockDepth / 2);
    glVertex3f(-blockWidth / 2, blockHeight / 2, blockDepth / 2);
    glVertex3f(-blockWidth / 2, -blockHeight / 2, blockDepth / 2);

    glEnd();

    glPopMatrix();
}
// Modified function to draw the Ferris wheel with gradient-colored cabins
/**
 * @brief Draws the Ferris wheel with gradient-colored cabins and matching support frames.
 *
 * This function renders the Ferris wheel, including the rim, inner rings, spokes, support frames,
 * and cabins. Each cabin and its supporting steel frame have the same gradient color to create
 * a cohesive visual effect. The function also handles the rotation and swing animations of the cabins.
 */
void drawFerrisWheel()
{

    // Draw the front block of the scene
    drawFrontBlock();

    // Control the rotation speed of the Ferris wheel
    ferrisWheelRotationAngle += 0.2f;

    glPushMatrix();
    glScalef(1.4, 1.4, 1.4);
    // Move the Ferris wheel to the specified position
    glTranslatef(20.0f, 0.0f, 0.0f);

    // Draw the support structures of the Ferris wheel
    drawSupports();

    // Move the Ferris wheel to the specified position using the global variables
    glTranslatef(ferrisWheelPosX, ferrisWheelPosY, ferrisWheelPosZ);

    // Begin drawing the Ferris wheel
    glPushMatrix();

    // Raise the center of the wheel to Y = 13.0f
    glTranslatef(0.0f, 13.0f, 0.0f);

    // Rotate the wheel around the Z-axis based on the rotation angle
    glRotatef(ferrisWheelRotationAngle, 0.0f, 0.0f, 1.0f);

    // Enable smooth shading for better visual quality
    glShadeModel(GL_SMOOTH);

    // Set warm metallic material properties
    GLfloat matAmbient[] = { 0.4f, 0.2f, 0.1f, 1.0f };
    GLfloat matDiffuse[] = { 0.7f, 0.4f, 0.2f, 1.0f };
    GLfloat matSpecular[] = { 0.9f, 0.6f, 0.3f, 1.0f };
    GLfloat matShininess[] = { 50.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

    float rimRadius = 10.0f;        // Radius of the rim
    float rimThickness = 0.17f;     // Thickness of the rim

    if (gUseTexture)
    {
        glEnable(GL_TEXTURE_2D);               // Enable textures
        glBindTexture(GL_TEXTURE_2D, texVec[26]);  // Bind the specified texture
        glColor3f(1.0f, 1.0f, 1.0f);          // Set color to white (keep original texture color)
        glPushMatrix();
        // Draw the outer rim using a solid torus
        float rimRadius = 10.0f;        // Radius of the rim
        float rimThickness = 0.17f;     // Thickness of the rim
        glutSolidTorus(rimThickness, rimRadius, 32, 128);
        glPopMatrix();
    }
    else
    {
        glColor3f(0.7f, 1.0f, 0.7f);          // Set color to light gray if no texture is used
        // Draw the outer rim using a solid torus
        glPushMatrix();
        glutSolidTorus(rimThickness, rimRadius, 32, 128);
        glPopMatrix();
    }

    if (gUseTexture)
    {
        glDisable(GL_TEXTURE_2D);  // Disable textures when done
    }

    // Draw the inner rings using a separate function
    int numInnerRings = 4; // Number of inner rings
    drawInnerRings(rimRadius, numInnerRings);

    // Draw the spokes with increased thickness
    int numSpokes = 24; // Number of spokes

    for (int i = 0; i < numSpokes; ++i)
    {
        // Calculate the angle for the current spoke
        float spokeAngle = i * 360.0f / numSpokes;

        glPushMatrix();

        // Rotate to the current spoke angle around the Z-axis
        glRotatef(spokeAngle, 0.0f, 0.0f, 1.0f);

        // Create a new quadric object for rendering cylinders
        GLUquadric* quad = gluNewQuadric();
        gluQuadricNormals(quad, GLU_SMOOTH);
        gluQuadricTexture(quad, GL_TRUE);

        float spokeThickness = 0.3f;               // Thickness of the spoke
        float centralCylinderLength = rimRadius * 0.2f; // Length of the central cylinder
        gluCylinder(quad, spokeThickness, spokeThickness, centralCylinderLength, 16, 1);

        // Add a cube at the front end of the central cylinder
        float cubeSize = spokeThickness * 3.5f; // Size of the cube

        glPushMatrix();

        // Move to the end of the central cylinder
        glTranslatef(0.0f, 0.0f, centralCylinderLength + cubeSize / 2.0f);

        // Enable texture for the cube if required
        if (gUseTexture)
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texVec[28]); // Cube texture
            glColor3f(1.0f, 1.0f, 1.0f);
        }
        else
        {
            glColor3f(0.8f, 1.0f, 1.0f); // Light gray color
        }

        // Draw the cube representing the cabin attachment
        glutSolidCube(cubeSize);

        // Disable texture after drawing the cube
        if (gUseTexture)
        {
            glDisable(GL_TEXTURE_2D);
        }

        glPopMatrix();

        // Draw support rods for each inner ring
        for (int r = 1; r <= numInnerRings; ++r)
        {
            float ringRadius = rimRadius * (1.0f - r * 0.25f);

            // Draw the cylindrical support rod
            glPushMatrix();
            gluCylinder(quad, spokeThickness, spokeThickness, ringRadius, 10, 1);
            glPopMatrix();

            // Draw a rectangular prism at the end of the support rod
            float length = ringRadius - 2;          // Length of the prism
            float width = 2.0f * spokeThickness;    // Width of the prism
            float height = spokeThickness;          // Height of the prism

            glPushMatrix();

            // Translate to the end of the support rod
            glTranslatef(0.0f, 0.0f, length);

            // Apply texture if enabled
            if (gUseTexture)
            {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texVec[29]); // Selected texture
                glColor3f(1.0f, 1.0f, 1.0f); // Set color to white to display texture colors
            }
            else
            {
                glColor3f(0.8f, 0.8f, 0.8f); // Light gray color
            }

            // Scale and draw the rectangular prism
            glScalef(width, height, length); // Set the size of the prism
            glutSolidCube(1.0f); // Use a cube to draw the prism

            // Disable texture after drawing the prism
            if (gUseTexture)
            {
                glDisable(GL_TEXTURE_2D);
            }

            glPopMatrix();
        }

        // Delete the quadric object to free resources
        gluDeleteQuadric(quad);
        glPopMatrix();
    }

    // Draw the cabins with gradient colors
    int numCabins = numSpokes; // Assume one cabin per spoke

    for (int i = 0; i < numCabins; ++i)
    {
        glPushMatrix();

        // Calculate the angle for the current cabin
        float spokeAngle = i * 360.0f / numCabins;
        glRotatef(spokeAngle, 0.0f, 0.0f, 1.0f);

        // Translate to the position of the cabin
        glTranslatef(rimRadius, 0.0f, 0.0f);

        // Calculate color to create a gradient effect
        float hue = i / static_cast<float>(numCabins); // Hue ranges from 0 to 1
        float r, g, b;
        HSVtoRGB(hue, 0.8f, 1.0f, r, g, b);

        // Draw the steel frame from the center to the cabin with matching color
        glPushMatrix();
        glTranslatef(-rimRadius, 0.0f, 0.0f); // Move back to the center

        // Set material properties and apply texture for the steel frame
        if (gUseTexture)
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texVec[26]); // Metal texture
            glColor3f(r, g, b); // Set color matching the cabin
        }
        else
        {
            glColor3f(r, g, b); // Set color matching the cabin
        }

        // Create a new quadric object for the frame cylinder
        GLUquadric* quad = gluNewQuadric();
        gluQuadricNormals(quad, GLU_SMOOTH);
        gluQuadricTexture(quad, GL_TRUE);

        float frameThickness = 0.15f; // Thickness of the frame
        float frameLength = rimRadius; // Length of the frame

        // Rotate to align the cylinder along the X-axis
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        gluCylinder(quad, frameThickness, frameThickness, frameLength, 16, 1);

        // Delete the quadric object to free resources
        gluDeleteQuadric(quad);

        // Disable texture after drawing the frame
        if (gUseTexture)
        {
            glDisable(GL_TEXTURE_2D);
        }

        glPopMatrix(); // Restore to the cabin position

        // Calculate the swing angle based on the rotation speed
        float swingAngle = sin((ferrisWheelRotationAngle + spokeAngle) * M_PI / 180.0f) * 10.0f;

        // Call the function to draw the cabin, passing color parameters
        drawCabin(swingAngle, r, g, b);

        glPopMatrix(); // Restore to the center of the wheel
    }

    // Disable textures if they were enabled
    if (gUseTexture)
    {
        glDisable(GL_TEXTURE_2D);
    }

    glPopMatrix(); // Restore the wheel's transformations
    glPopMatrix(); // Restore the Ferris wheel's transformations
}

void drawCylinder(float baseRadius, float topRadius, float height, float r, float g, float b) {
    GLUquadric* quad = gluNewQuadric();
    glColor3f(r, g, b);
    gluCylinder(quad, baseRadius, topRadius, height, 32, 32);
    gluDeleteQuadric(quad);
}
void drawTexturedCylinder(float baseRadius, float topRadius, float height, GLuint textureID) {
    // Check if the texture ID is valid
    if (textureID == 0) {
        std::cerr << "Invalid texture ID!" << std::endl;
        return;
    }

    // Enable textures if required
    if (gUseTexture)
    {
        glEnable(GL_TEXTURE_2D); // Enable 2D texture mapping
        glBindTexture(GL_TEXTURE_2D, textureID); // Bind the metal texture
        glColor3f(1.0f, 1.0f, 1.0f); // Set color to white for proper texture display

        // Create a new quadric object and enable texture mapping
        GLUquadric* quad = gluNewQuadric();
        gluQuadricTexture(quad, GL_TRUE); // Enable texture coordinates

        // Render the textured cylinder
        gluCylinder(quad, baseRadius, topRadius, height, 32, 32);
    }
    else
    {
        glColor3f(0.7f, 0.7f, 0.7f); // Set a light gray color for the cylinder
        // Create a new quadric object and enable texture mapping
        GLUquadric* quad = gluNewQuadric();
        gluQuadricTexture(quad, GL_TRUE); // Enable texture coordinates

        // Render the cylinder without texture
        gluCylinder(quad, baseRadius, topRadius, height, 32, 32);
    }
    glDisable(GL_TEXTURE_2D); // Disable texture mapping after rendering
}

void drawTurntable(GLuint textureID) {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE); // Enable texture mapping

    glPushMatrix();
    glTranslatef(0.0f, 0.1f, 0.0f); // Slightly raise the turntable
    glRotatef(-90.0f, 1, 0, 0); // Rotate to make the disk horizontal

    if (gUseTexture) {
        glEnable(GL_TEXTURE_2D); // Enable 2D texture mapping
        glBindTexture(GL_TEXTURE_2D, textureID); // Bind the provided texture ID
        glColor3f(1.0f, 1.0f, 1.0f); // Maintain the original color of the texture
    }
    else {
        glColor3f(0.7f, 0.7f, 0.7f); // Use a gray color if textures are not used
    }

    // Draw the turntable disk with inner and outer radii of 0.5 and 13.5, 32 segments, and 1 slice
    gluDisk(quad, 0.5f, 13.5f, 32, 1);

    if (gUseTexture) {
        glDisable(GL_TEXTURE_2D); // Disable texture mapping after rendering the disk
    }

    glPopMatrix();

    gluDeleteQuadric(quad);

    // Add decorative spheres around the turntable
    int numDecorations = 16;
    for (int i = 0; i < numDecorations; ++i) {
        glPushMatrix();
        float angle = i * (360.0f / numDecorations); // Compute the angle for each decoration
        float rad = angle * M_PI / 180.0f; // Convert angle to radians
        float x = 8.0f * cos(rad); // X-coordinate of the sphere
        float z = 8.0f * sin(rad); // Z-coordinate of the sphere
        glTranslatef(x, 0.1f, z); // Slightly raise the sphere to avoid overlap with the disk

        if (gUseTexture) {
            glEnable(GL_TEXTURE_2D); // Enable 2D texture mapping
            glBindTexture(GL_TEXTURE_2D, textureID); // Bind the provided texture ID
            glColor3f(1.0f, 1.0f, 1.0f); // Use the original texture color
        }
        else {
            glColor3f(1.0f, 0.0f, 0.0f); // Use red color if textures are not used
        }

        // Render the decorative sphere
        glutSolidSphere(0.2f, 16, 16);

        if (gUseTexture) {
            glDisable(GL_TEXTURE_2D); // Disable texture mapping after rendering the sphere
        }

        glPopMatrix();
    }
}

void drawCable(float startX, float startY, float startZ, float endX, float endY, float endZ) {
    glPushMatrix();
    // Calculate the length of the cable
    float dx = endX - startX;
    float dy = endY - startY;
    float dz = endZ - startZ;
    float length = sqrt(dx * dx + dy * dy + dz * dz) - 2.5f;  // Subtract an offset to ensure the correct length of the cable

    // Set the direction of the cable
    glTranslatef(startX, startY, startZ);
    float angle = atan2(dy, sqrt(dx * dx + dz * dz)) * 180.0 / M_PI;
    glRotatef(angle, -dz, 0.0f, dx);

    // Draw the cable as a thin cylinder
    drawCylinder(0.05f, 0.05f, length, 0.6f, 0.6f, 0.6f); // Gray cable
    glPopMatrix();
}

void chair() {
    glPushMatrix();

    // Scale the overall size of the chair
    glScalef(1.5f, 1.5f, 1.5f);

    // Translate the chair down so that the top of the backrest is at y = 0, making it easier to connect the cable
    glTranslatef(0.0f, -13.5f, 0.0f);

    // Draw the base of the chair
    glPushMatrix();
    glTranslatef(0.0f, -1.0f, 0.0f);
    glScalef(2.0f, 0.5f, 2.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

    if (gUseTexture) {
        glEnable(GL_TEXTURE_2D); // Enable texture mapping
        glBindTexture(GL_TEXTURE_2D, texVec[31]); // Bind the chair texture
        glColor3f(1.0f, 1.0f, 1.0f); // Use original colors from the texture
    }
    else {
        glColor3f(0.7f, 0.7f, 0.7f); // Use a gray color
    }

    // Draw the base
    drawTexturedCube(1.0f, texVec[31]); // Using the chair texture
    glPopMatrix();

    // Draw the backrest
    glPushMatrix();
    glTranslatef(0.0f, 0.5f, -1.0f); // Adjust position to connect with the base
    glScalef(2.0f, 2.5f, 0.5f);

    if (gUseTexture) {
        glEnable(GL_TEXTURE_2D); // Enable texture mapping
        glBindTexture(GL_TEXTURE_2D, texVec[31]); // Bind the chair texture
        glColor3f(1.0f, 1.0f, 1.0f); // Use original colors from the texture
    }
    else {
        glColor3f(0.7f, 0.7f, 0.7f); // Use a gray color
    }

    // Draw the backrest
    drawTexturedCube(1.0f, texVec[31]); // Using the chair texture
    glPopMatrix();

    // Draw the left armrest
    glPushMatrix();
    glTranslatef(-1.1f, 0.0f, 0.0f); // Adjust position to connect with the base
    glScalef(0.2f, 1.5f, 2.0f); // Adjust size to connect with the base

    if (gUseTexture) {
        glEnable(GL_TEXTURE_2D); // Enable texture mapping
        glBindTexture(GL_TEXTURE_2D, texVec[31]); // Bind the chair texture
        glColor3f(1.0f, 1.0f, 1.0f); // Use original colors from the texture
    }
    else {
        glColor3f(0.7f, 0.7f, 0.7f); // Use a gray color
    }

    // Draw the left armrest
    drawTexturedCube(1.0f, texVec[31]);
    glPopMatrix();

    // Draw the right armrest
    glPushMatrix();
    glTranslatef(1.1f, 0.0f, 0.0f);

    if (gUseTexture) {
        glEnable(GL_TEXTURE_2D); // Enable texture mapping
        glBindTexture(GL_TEXTURE_2D, texVec[31]); // Bind the chair texture
        glColor3f(1.0f, 1.0f, 1.0f); // Use original colors from the texture
    }
    else {
        glColor3f(0.7f, 0.7f, 0.7f); // Use a gray color
    }

    // Draw the right armrest
    drawTexturedCube(1.0f, texVec[31]);
    glPopMatrix();

    // Add a connector between the backrest and the armrests (optional)
    glPushMatrix();
    glTranslatef(0.0f, 1.25f, 0.0f); // Position at the top of the armrests
    glScalef(2.2f, 0.2f, 0.5f);

    if (gUseTexture) {
        glEnable(GL_TEXTURE_2D); // Enable texture mapping
        glBindTexture(GL_TEXTURE_2D, texVec[31]); // Bind the chair texture
        glColor3f(1.0f, 1.0f, 1.0f); // Use original colors from the texture
    }
    else {
        glColor3f(0.7f, 0.7f, 0.7f); // Use a gray color
    }

    // Draw the connector
    drawTexturedCube(1.0f, texVec[31]);
    glPopMatrix();

    // Draw the cable
    glPushMatrix();
    float cableEndX = 0.3f;
    float cableEndY = 15.0f;
    float cableEndZ = 0.0f;
    drawCable(-0.1f, -0.05f, -1.0f, cableEndX, cableEndY, cableEndZ); // Connect the top of the backrest to the turntable
    glPopMatrix();

    glPopMatrix();
}

void flyingChair() {
    glPushMatrix();
    static float additionalAngle = 0.0f; // Additional rotation angle
    static float oscillateAngle = 0.0f;  // Oscillation angle
    // Translate the rotating flying chair to the specified position in the scene
    glTranslatef(-35.0f, 8.0f, 0.0f); // Adjust this position based on your scene

    // Draw the fixed central support column
    glPushMatrix();
    glTranslatef(0.0f, 20.0f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Rotate to make the column vertical
    drawTexturedCylinder(0.4f, 1.6f, 28.0f, texVec[32]); // Central column with height of 28.0f
    glPopMatrix();

    // Draw the decorative top
    glPushMatrix();
    glTranslatef(0.0f, 20.0f, 0.0f); // Position at the top of the central axis
    glColor3f(1.0f, 0.84f, 0.0f); // Gold color
    glutSolidSphere(1.0f, 32, 32); // Sphere with radius of 1.0f
    glPopMatrix();

    // Draw the turntable and chairs
    glPushMatrix();
    glTranslatef(0.0f, 20.0f, 0.0f); // Elevate to the top of the central column

    // Update the additional rotation angle
    additionalAngle += ADDITIONAL_ROTATION_SPEED; // Control the speed of additional rotation
    if (additionalAngle > 360.0f) additionalAngle -= 360.0f;

    // Perform multi-axis rotation around the Y and Z axes
    glRotatef(additionalAngle, 0.0f, 1.0f, 0.0f); // Rotate around the Y-axis
    glRotatef(oscillateAngle, 0.0f, 0.0f, 1.0f);  // Oscillate around the Z-axis

    drawTurntable(texVec[33]); // Draw the turntable with texture

    // Update the chair radius to create aggregation and dispersion effects
    if (isAggregating) {
        currentChairRadius -= AGGREGATE_SPREAD_SPEED;
        if (currentChairRadius <= MIN_CHAIR_RADIUS) {
            currentChairRadius = MIN_CHAIR_RADIUS;
            isAggregating = false;
        }
    }
    else {
        currentChairRadius += AGGREGATE_SPREAD_SPEED;
        if (currentChairRadius >= MAX_CHAIR_RADIUS) {
            currentChairRadius = MAX_CHAIR_RADIUS;
            isAggregating = true;
        }
    }

    // Update the oscillation angle
    oscillateAngle = 10.0f * sin((alpha + additionalAngle) * M_PI / 180.0f); // Oscillation amplitude varies between -10 and 10 degrees

    // Parameters for the chairs and cables
    int numChairs = 8;               // Number of chairs
    float cableLength = 12.0f;       // Cable length
    float turntableRadius = 7.0f;    // Turntable radius

    // Draw each chair and cable
    for (int i = 0; i < numChairs; ++i) {
        float baseAngle = i * (360.0f / numChairs);
        float totalAngle = baseAngle + alpha + additionalAngle; // Combined rotation angle
        float angleRad = totalAngle * M_PI / 180.0f;

        // Cable starting point (edge of the turntable)
        float cableStartX = (turntableRadius - 0.5f) * cos(angleRad);  // Adjust the cable connection to the inner side of the turntable
        float cableStartZ = (turntableRadius - 0.5f) * sin(angleRad);
        float cableStartY = 15.0f; // Height of the turntable

        // Chair position (at the end of the cable)
        float chairX = currentChairRadius * cos(angleRad); // Calculate chair position based on the current radius
        float chairZ = currentChairRadius * sin(angleRad);
        float chairY = cableStartY - cableLength; // Height after cable stretch

        // Draw the chair
        glPushMatrix();
        glTranslatef(chairX, chairY, chairZ);

        // Apply the oscillation angle to slightly sway the chair
        glRotatef(-totalAngle + tiltAngle * 0.5f, 0.0f, 1.0f, 0.0f); // Rotate the chair to face outward and slightly tilt it
        glRotatef(oscillateAngle, 0.0f, 0.0f, 1.0f); // Slight oscillation around the Z-axis

        chair(); // Draw the chair
        glPopMatrix();

        // Track the position of the first chair
        if (i == 0) {
            chairCurrentX = chairX;
            chairCurrentZ = chairZ;
            // Since the flying chair's center has already been translated via glTranslatef(-30.0f, 8.0f, 10.0f),
            // we need to convert chairCurrentX and chairCurrentZ to the global coordinate system
            chairCurrentX += -30.0f;
            chairCurrentZ += 10.0f;
            // chairCurrentY remains unchanged since the chair's Y coordinate is fixed
            chairCurrentY = chairY + 8.0f; // Adjust based on actual height
        }
    }

    // Update the rotation angle of the flying chair
    flyingChairRotationAngle = alpha + additionalAngle;

    glPopMatrix(); // End the drawing of the turntable and chairs

    glPopMatrix(); // End the entire flying chair drawing
}


// Function to draw the external scene
void drawExterior()
{
    // Draw the ground
    drawGround();

    // Draw the sky and clouds
    drawSkyClouds();

    // Draw the surrounding trees
    drawTrees();
}

// Draw man body with texture
void drawBody()
{
    glPushMatrix();
    // The position of the body should be at the top of the legs
    glTranslatef(0.0f, legHeight, 0.0f);

    float bodyWidth = 1.6f; // Increase body width
    float bodyDepth = 1.0f; // Increase body depth

    if (gUseTexture)
    {
        glEnable(GL_TEXTURE_2D);
        // Select textures based on texture state
        unsigned int frontTexture = isAlternateTexture ? texVec[20] : texVec[14]; // Front panel texture
        unsigned int backTexture = isAlternateTexture ? texVec[21] : texVec[13];  // Back panel texture
        unsigned int sideTexture = isAlternateTexture ? texVec[23] : texVec[15];  // Side panel texture (added logic)

        // Draw the body using textures
        // Front panel
        glBindTexture(GL_TEXTURE_2D, frontTexture);
        glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);
        glTexCoord2d(0.0, 0.0); glVertex3f(-bodyWidth / 2.0f, 0.0f, bodyDepth / 2.0f);
        glTexCoord2d(1.0, 0.0); glVertex3f(bodyWidth / 2.0f, 0.0f, bodyDepth / 2.0f);
        glTexCoord2d(1.0, 1.0); glVertex3f(bodyWidth / 2.0f, bodyHeight, bodyDepth / 2.0f);
        glTexCoord2d(0.0, 1.0); glVertex3f(-bodyWidth / 2.0f, bodyHeight, bodyDepth / 2.0f);
        glEnd();

        // Back panel
        glBindTexture(GL_TEXTURE_2D, backTexture);
        glBegin(GL_QUADS);
        glNormal3f(0, 0, -1);
        glTexCoord2d(0.0, 0.0); glVertex3f(bodyWidth / 2.0f, 0.0f, -bodyDepth / 2.0f);
        glTexCoord2d(1.0, 0.0); glVertex3f(-bodyWidth / 2.0f, 0.0f, -bodyDepth / 2.0f);
        glTexCoord2d(1.0, 1.0); glVertex3f(-bodyWidth / 2.0f, bodyHeight, -bodyDepth / 2.0f);
        glTexCoord2d(0.0, 1.0); glVertex3f(bodyWidth / 2.0f, bodyHeight, -bodyDepth / 2.0f);
        glEnd();

        // Left side panel
        glBindTexture(GL_TEXTURE_2D, sideTexture); // Body side texture
        glBegin(GL_QUADS);
        glNormal3f(-1, 0, 0);
        glTexCoord2d(0.0, 0.0); glVertex3f(-bodyWidth / 2.0f, 0.0f, -bodyDepth / 2.0f);
        glTexCoord2d(1.0, 0.0); glVertex3f(-bodyWidth / 2.0f, 0.0f, bodyDepth / 2.0f);
        glTexCoord2d(1.0, 1.0); glVertex3f(-bodyWidth / 2.0f, bodyHeight, bodyDepth / 2.0f);
        glTexCoord2d(0.0, 1.0); glVertex3f(-bodyWidth / 2.0f, bodyHeight, -bodyDepth / 2.0f);
        glEnd();

        // Right side panel
        glBegin(GL_QUADS);
        glNormal3f(1, 0, 0);
        glTexCoord2d(0.0, 0.0); glVertex3f(bodyWidth / 2.0f, 0.0f, bodyDepth / 2.0f);
        glTexCoord2d(1.0, 0.0); glVertex3f(bodyWidth / 2.0f, 0.0f, -bodyDepth / 2.0f);
        glTexCoord2d(1.0, 1.0); glVertex3f(bodyWidth / 2.0f, bodyHeight, -bodyDepth / 2.0f);
        glTexCoord2d(0.0, 1.0); glVertex3f(bodyWidth / 2.0f, bodyHeight, bodyDepth / 2.0f);
        glEnd();

        // Top panel
        glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);
        glTexCoord2d(0.0, 0.0); glVertex3f(-bodyWidth / 2.0f, bodyHeight, bodyDepth / 2.0f);
        glTexCoord2d(1.0, 0.0); glVertex3f(bodyWidth / 2.0f, bodyHeight, bodyDepth / 2.0f);
        glTexCoord2d(1.0, 1.0); glVertex3f(bodyWidth / 2.0f, bodyHeight, -bodyDepth / 2.0f);
        glTexCoord2d(0.0, 1.0); glVertex3f(-bodyWidth / 2.0f, bodyHeight, -bodyDepth / 2.0f);
        glEnd();

        glDisable(GL_TEXTURE_2D);
    }
    else
    {
        glColor3f(0.8f, 0.5f, 0.3f); // Body color
        glTranslatef(0.0f, bodyHeight / 2.0f, 0.0f); // Move the body center up
        glScalef(bodyWidth, bodyHeight, bodyDepth);
        glutSolidCube(1.0f);
    }
    glPopMatrix();
}

// Draw man head with texture
void drawHead()
{
    glPushMatrix();
    // The base of the head is at the top of the neck
    glTranslatef(0.0f, legHeight + bodyHeight + neckHeight, 0.0f);
    glTranslatef(0.0f, headRadius, 0.0f); // Move the head up to align its center

    // If needed, rotate the head to face the correct direction
    glRotatef(0, 0, 1, 0);

    if (gUseTexture)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texVec[12]);
        GLUquadric* quad = gluNewQuadric();
        gluQuadricTexture(quad, GL_TRUE);
        gluQuadricNormals(quad, GLU_SMOOTH);
        gluSphere(quad, headRadius, 32, 32);
        gluDeleteQuadric(quad);
        glDisable(GL_TEXTURE_2D);
    }
    else
    {
        glColor3f(1.0f, 0.8f, 0.6f); // Skin color
        glutSolidSphere(headRadius, 32, 32);
    }
    glPopMatrix();
}

// Draw man's neck
void drawNeck()
{
    glPushMatrix();
    // Set the initial position of the neck
    glTranslatef(0.0f, legHeight + bodyHeight, 0.0f);
    glTranslatef(0.0f, neckHeight / 2.0f, 0.0f); // Move the neck center up

    float neckWidth = 0.5f; // Increase neck width

    if (!gUseTexture)
    {
        // Use fixed color
        glColor3f(1.0f, 0.8f, 0.6f); // Skin color
    }
    else
    {
        // Ensure texture is bound
        glEnable(GL_TEXTURE_2D); // Enable texture functionality
        glBindTexture(GL_TEXTURE_2D, texVec[12]);
    }

    // Set the scaling and shape of the neck
    glScalef(neckWidth, neckHeight, neckWidth);
    glutSolidCube(1.0f);

    if (gUseTexture)
    {
        glDisable(GL_TEXTURE_2D);
    }

    glPopMatrix();
}

// Draw the character's leg (add joints)
void drawLeg(bool isLeft)
{
    glPushMatrix();
    float side = isLeft ? -0.5f : 0.5f;
    glTranslatef(side, legHeight + 0.1f, 0.0f);

    // Hip joint rotation
    glRotatef(isLeft ? hipAngle : -hipAngle, 1, 0, 0);

    // Thigh
    if (gUseTexture)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texVec[18]);
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    else
    {
        glColor3f(0.8f, 0.5f, 0.3f);
    }

    glPushMatrix();
    glTranslatef(0.0f, -legHeight / 4.0f, 0.0f);
    glScalef(0.5f, legHeight / 2.0f, 0.5f); // Thicken thigh
    glutSolidCube(1.0f);
    glPopMatrix();

    // Move to knee
    glTranslatef(0.0f, -legHeight / 2.0f, 0.0f);
    // Knee rotation
    glRotatef(isLeft ? kneeAngle : -kneeAngle, 1, 0, 0);

    // Calf
    glPushMatrix();
    glTranslatef(0.0f, -legHeight / 4.0f, 0.0f);
    glScalef(0.45f, legHeight / 2.0f, 0.45f); // Thicken calf
    glutSolidCube(1.0f);
    glPopMatrix();

    // Move to ankle
    glTranslatef(0.0f, -legHeight / 2.0f, 0.0f);


    // Foot
    glPushMatrix();
    glColor3f(0.3f, 0.3f, 0.3f); // Shoe color
    glTranslatef(0.0f, -0.1f, 0.15f);
    glScalef(0.6f, 0.2f, 0.9f);
    glutSolidCube(1.0f); // Simple cube represents the foot
    glPopMatrix();

    if (gUseTexture)
    {
        glDisable(GL_TEXTURE_2D);
    }

    glPopMatrix();
}

// Draw the character's arm (add joints)
void drawArm(bool isLeft)
{
    glPushMatrix();
    // Initial position of the arm
    float side = isLeft ? -1.0f : 1.0f;
    glTranslatef(side * (0.7f + 0.05f), legHeight + bodyHeight - 0.5f, 0.0f);

    // Add shoulder abduction rotation to slightly spread the arms
    float armAbductionAngle = -12.0f; // Abduction angle, adjustable as needed
    glRotatef(isLeft ? armAbductionAngle : -armAbductionAngle, 0, 0, 1);

    // Shoulder forward/backward swing rotation
    glRotatef(isLeft ? shoulderAngle : -shoulderAngle, 1, 0, 0);

    // Upper arm
    if (gUseTexture)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texVec[17]);
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    else
    {
        glColor3f(0.8f, 0.5f, 0.3f);
    }

    glPushMatrix();
    glTranslatef(0.0f, -armLength / 4.0f, 0.0f);
    glScalef(0.4f, armLength / 2.0f, 0.4f); // Thicken upper arm
    glutSolidCube(1.0f);
    glPopMatrix();

    // Move to elbow
    glTranslatef(0.0f, -armLength / 2.0f, 0.0f);
    // Elbow rotation
    glRotatef(isLeft ? elbowAngle : -elbowAngle, 1, 0, 0);

    // Forearm
    glPushMatrix();
    glTranslatef(0.0f, -armLength / 4.0f, 0.0f);
    glScalef(0.35f, armLength / 2.0f, 0.35f); // Thicken forearm
    glutSolidCube(1.0f);
    glPopMatrix();

    // Add wrist and hand
    glTranslatef(0.0f, -armLength / 2.0f, 0.0f); // Move to wrist position

    // Hand
    glPushMatrix();
    glColor3f(1.0f, 0.8f, 0.6f); // Skin color
    glTranslatef(0.0f, -0.1f, 0.0f);
    glutSolidSphere(0.2f, 16, 16); // Simple sphere represents the hand
    glPopMatrix();

    if (gUseTexture)
    {
        glDisable(GL_TEXTURE_2D);
    }

    glPopMatrix();
}

// Draw the man
void drawMan()
{
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    {
        glTranslatef(manPositionX, manPositionY, manPositionZ);
        glScalef(0.5f, 0.5f, 0.5f);
        glRotatef(manRotationAngle, 0, 1, 0);

        // Draw legs
        drawLeg(true);  // Left leg
        drawLeg(false); // Right leg

        // Draw body
        drawBody();

        // Draw neck
        drawNeck();

        // Draw arms
        drawArm(true);  // Left arm
        drawArm(false); // Right arm

        // Draw head
        drawHead();
    }
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
}

// Update limb swing during a jump based on elapsed time
void updateJumpLimbSwing(float elapsedTime)
{
    const float gravity = 9.8f;
    float t_up = jumpVelocityY / gravity;
    float jumpDuration = 2.0f * t_up;

    if (elapsedTime < jumpDuration / 3.0f)
    {
        // First third of the jump: limbs move upward
        float factor = elapsedTime / (jumpDuration / 3.0f);
        shoulderAngle = -30.0f * factor;
        elbowAngle = -30.0f * factor;
        hipAngle = -30.0f * factor;
        kneeAngle = -30.0f * factor;
    }
    else if (elapsedTime < 2.0f * jumpDuration / 3.0f)
    {
        // Middle third of the jump: limbs swing downward
        float factor = (elapsedTime - jumpDuration / 3.0f) / (jumpDuration / 3.0f);
        shoulderAngle = -30.0f + 60.0f * factor;
        elbowAngle = -30.0f + 60.0f * factor;
        hipAngle = -30.0f + 60.0f * factor;
        kneeAngle = -30.0f + 60.0f * factor;
    }
    else
    {
        // Last third of the jump: limbs return to neutral position
        float factor = (elapsedTime - 2.0f * jumpDuration / 3.0f) / (jumpDuration / 3.0f);
        shoulderAngle = 30.0f - 30.0f * factor;
        elbowAngle = 30.0f - 30.0f * factor;
        hipAngle = 30.0f - 30.0f * factor;
        kneeAngle = 30.0f - 30.0f * factor;
    }
}

// Update limb swinging based on the character's state
void updateLimbSwing()
{
    if (isFalling)
    {
        // Rapidly swing limbs while falling
        float swingSpeed = 10.0f;
        limbSwing += limbSwingDirection * swingSpeed;
        if (limbSwing > 80.0f || limbSwing < -80.0f)
        {
            limbSwingDirection *= -1.0f;
        }

        // Update joint angles
        shoulderAngle = limbSwing;
        elbowAngle = limbSwing / 2.0f;
        hipAngle = limbSwing;
        kneeAngle = limbSwing / 2.0f;
    }
    else if (isWalking)
    {
        float swingSpeed = isRunning ? 2.5f : 1.5f; // Swing faster when running
        float maxSwingAngle = isRunning ? 50.0f : 35.0f; // Greater swing amplitude when running

        limbSwing += 1.3f * limbSwingDirection * swingSpeed; // Adjust swing speed
        if (limbSwing > maxSwingAngle || limbSwing < -maxSwingAngle)
        {
            limbSwingDirection *= -1.0f;
        }

        // Update joint angles
        shoulderAngle = limbSwing;
        elbowAngle = limbSwing / 2.0f;
        hipAngle = limbSwing;
        kneeAngle = limbSwing / 2.0f;
    }
    else
    {
        // Return limbs to the neutral position
        if (limbSwing > 0)
        {
            limbSwing -= 2.0f;
            if (limbSwing < 0)
                limbSwing = 0;
        }
        else if (limbSwing < 0)
        {
            limbSwing += 2.0f;
            if (limbSwing > 0)
                limbSwing = 0;
        }
        shoulderAngle = limbSwing;
        elbowAngle = limbSwing / 2.0f;
        hipAngle = limbSwing;
        kneeAngle = limbSwing / 2.0f;
    }
}

// Structure to represent the properties of a tower light
struct TowerLight {
    float x, y, z;      // Position of the light in 3D space (x, y, z coordinates)
    float r, g, b;      // Color of the light (red, green, blue components)
    bool isOn;          // State of the light (true if the light is on, false if off)
};

// Vector to store the collection of tower lights
std::vector<TowerLight> towerLights;

// Function to initialize the decorative tower lights
void initTowerLights() {
    // Add decorative lights around the tower at different heights
    for (int height = 0; height < TOWER_HEIGHT; height += 8) {
        // Loop through four cardinal directions (0, 90, 180, and 270 degrees)
        for (int i = 0; i < 4; i++) {
            // Calculate the angle for each direction
            float angle = i * 90.0f * M_PI / 180.0f;
            // Set the radius at which the lights will be placed around the tower
            float radius = POLE_SPACING * 1.2f; // Increase the radius to spread out the lights

            // Create multi-colored lights based on height
            float hue = height / TOWER_HEIGHT; // Color hue changes with height
            float r, g, b;
            // Convert HSV to RGB color model
            HSVtoRGB(hue, 1.0f, 1.0f, r, g, b);

            // Define a new light object with position and color
            TowerLight light = {
                radius * cos(angle),  // x coordinate of the light
                (float)height,        // y coordinate (height of the light)
                radius * sin(angle),  // z coordinate of the light
                r, g, b,              // RGB color values for the light
                true                  // The light is turned on initially
            };

            // Add the newly created light to the towerLights vector
            towerLights.push_back(light);
        }
    }
}

// Structure to define the properties of a particle in the tower's particle system
struct TowerParticle {
    float x, y, z;        // Position of the particle in 3D space
    float vx, vy, vz;     // Velocity components of the particle along the x, y, and z axes
    float life;           // Lifetime of the particle, representing how long it will exist before disappearing
    float r, g, b, a;     // Color and transparency of the particle (RGBA format)
    float size;           // Size of the particle, which can affect its visual appearance
};

// Container to store all particles in the tower's particle system
std::vector<TowerParticle> towerParticles;

// Function to update the dynamic lighting effects on the tower
void updateTowerLights(float deltaTime) {
    static float time = 0.0f;  // Static variable to keep track of time between updates
    time += deltaTime;  // Accumulate time to create continuous animation

    // Iterate over each light in the towerLights vector
    for (size_t i = 0; i < towerLights.size(); i++) {
        // Create a wave-like flicker effect for the lights based on the time and index
        float phase = time * 3.0f + i * 0.2f;  // Calculate the phase of the sine wave for flickering
        float brightness = 0.5f + 0.5f * sin(phase);  // Set brightness based on sine wave

        // Modify the light color based on its height along the tower
        float heightRatio = towerLights[i].y / TOWER_HEIGHT;  // Calculate the height ratio relative to the tower height
        if (isDropping && fabs(heightRatio - dropHeight / TOWER_HEIGHT) < 0.1f) {
            // Lights near the seat (during a drop event) turn red and flicker
            towerLights[i].r = 1.0f;  // Set red component to maximum
            towerLights[i].g = 0.0f;  // Set green component to zero
            towerLights[i].b = 0.0f;  // Set blue component to zero
            towerLights[i].isOn = (sin(time * 10.0f) > 0.0f);  // Flicker effect based on time
        }
        else {
            // Restore the original color based on the height ratio
            float hue = heightRatio;  // Use the height ratio to vary the color hue
            HSVtoRGB(hue, 1.0f, brightness, towerLights[i].r, towerLights[i].g, towerLights[i].b);  // Convert from HSV to RGB
            towerLights[i].isOn = true;  // Ensure the light is on
        }
    }
}

// Function to update the tower particle system
void updateTowerParticles(float deltaTime) {
    // Generate particles when the seat is rapidly dropping
    if (isDropping && dropSpeed > 20.0f) {
        // Add new particles around the seat during the drop
        for (int i = 0; i < 30; i++) {
            TowerParticle particle;

            // Set the initial position of the particle (random position around the seat)
            float angle = (rand() % 360) * M_PI / 180.0f;  // Random angle
            float radius = (rand() % 100) / 100.0f * POLE_SPACING;  // Random radius within a certain range
            particle.x = TOWER_POSITION_X + radius * cos(angle);  // X position of the particle
            particle.y = dropHeight;  // Y position of the particle (at the height of the drop)
            particle.z = TOWER_POSITION_Z + radius * sin(angle);  // Z position of the particle

            // Set the initial velocity of the particle
            particle.vx = (rand() % 200 - 100) / 100.0f * 2.0f;  // Random X velocity
            particle.vy = (rand() % 100) / 100.0f * 5.0f;  // Random Y velocity
            particle.vz = (rand() % 200 - 100) / 100.0f * 2.0f;  // Random Z velocity

            // Set the particle's lifetime and appearance
            particle.life = 1.0f + (rand() % 100) / 100.0f;  // Random lifetime between 1 and 2 seconds
            particle.r = 1.0f;  // Red component of the color
            particle.g = 0.5f + (rand() % 50) / 100.0f;  // Green component of the color, with slight random variation
            particle.b = 0.0f;  // Blue component of the color
            particle.a = 1.0f;  // Full opacity
            particle.size = 0.2f + (rand() % 30) / 100.0f;  // Random particle size between 0.2 and 0.5

            // Add the new particle to the list of tower particles
            towerParticles.push_back(particle);
        }
    }

    // Update existing particles
    for (auto it = towerParticles.begin(); it != towerParticles.end();) {
        // Update the position based on velocity
        it->x += it->vx * deltaTime;  // Update X position
        it->y += it->vy * deltaTime;  // Update Y position
        it->z += it->vz * deltaTime;  // Update Z position

        // Apply gravity effect on the particle
        it->vy -= 9.8f * deltaTime;  // Gravity affects the Y velocity

        // Update the particle's life and adjust transparency
        it->life -= deltaTime;  // Decrease the particle's life over time
        it->a = it->life;  // Transparency decreases as the particle's life diminishes

        // Remove dead particles from the system
        if (it->life <= 0) {
            it = towerParticles.erase(it);  // Remove the particle and move to the next
        }
        else {
            ++it;  // Proceed to the next particle if the current one is still alive
        }
    }
}

/**
 * Renders particle effects for the tower attraction.
 * This function handles the visualization of particles with:
 * - Billboard technique (particles always face camera)
 * - Alpha blending for transparency
 * - Depth testing disabled for proper blending
 */
void drawTowerParticles() {
    glPushMatrix();

    // Disable lighting for particle effects
    glDisable(GL_LIGHTING);

    // Enable alpha blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);  // Additive blending for glow effect

    // Disable depth testing to prevent particle sorting issues
    glDisable(GL_DEPTH_TEST);

    // Render each particle in the system
    for (const auto& particle : towerParticles) {
        glPushMatrix();
        glTranslatef(particle.x, particle.y, particle.z);

        // Implement billboard technique - make particles face camera
        glRotatef(-cameraAngleY, 0.0f, 1.0f, 0.0f);  // Horizontal rotation
        glRotatef(-cameraAngleX, 1.0f, 0.0f, 0.0f);  // Vertical rotation

        // Set particle color and alpha
        glColor4f(particle.r, particle.g, particle.b, particle.a);

        // Draw particle as a textured quad
        glBegin(GL_QUADS);
        float size = particle.size / 2.0f;
        glVertex3f(-size, -size, 0.0f);  // Bottom-left
        glVertex3f(size, -size, 0.0f);   // Bottom-right
        glVertex3f(size, size, 0.0f);    // Top-right
        glVertex3f(-size, size, 0.0f);   // Top-left
        glEnd();

        glPopMatrix();
    }

    // Restore OpenGL states
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

/**
 * Renders the first drop tower with detailed components and effects.
 * This function includes the base, main pillar, support pillars, seating structure,
 * decorative lights, and particle effects.
 */
void drawFirstDropTower() {
    glPushMatrix();

    // Save current material state
    GLfloat currentAmbient[4], currentDiffuse[4], currentSpecular[4];
    GLfloat currentShininess;
    glGetMaterialfv(GL_FRONT, GL_AMBIENT, currentAmbient);
    glGetMaterialfv(GL_FRONT, GL_DIFFUSE, currentDiffuse);
    glGetMaterialfv(GL_FRONT, GL_SPECULAR, currentSpecular);
    glGetMaterialfv(GL_FRONT, GL_SHININESS, &currentShininess);

    glTranslatef(TOWER_POSITION_X, 0.4f, TOWER_POSITION_Z);

    // 1. Draw the base - using deep blue metallic texture
    glPushMatrix();
    if (gUseTexture) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texVec[34]);
    }
    glColor3f(0.2f, 0.3f, 0.4f); // Deep blue tone
    glScalef(BASE_SIZE, 2.0f, BASE_SIZE);
    glutSolidCube(1.0f);

    // Add decorative edges to the base
    glColor3f(0.4f, 0.6f, 0.8f); // Bright blue decoration
    glTranslatef(0.0f, 0.5f, 0.0f);
    glScalef(1.05f, 0.1f, 1.05f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Main central pillar - using gradient colors
    glPushMatrix();
    glTranslatef(0.0f, TOWER_HEIGHT / 2, 0.0f);

    // Gradient effect - draw all faces
    glBegin(GL_QUADS);
    for (float h = 0; h < TOWER_HEIGHT; h += 2.0f) {
        float ratio = h / TOWER_HEIGHT;
        // Use a richer gradient color
        glColor3f(0.3f + ratio * 0.3f,
            0.2f + ratio * 0.4f,
            0.4f + ratio * 0.4f);

        float y = h - TOWER_HEIGHT / 2;
        float size = POLE_RADIUS * 2;

        // Front face
        glVertex3f(-size, y, size);
        glVertex3f(size, y, size);
        glVertex3f(size, y + 2.0f, size);
        glVertex3f(-size, y + 2.0f, size);

        // Back face
        glVertex3f(-size, y, -size);
        glVertex3f(size, y, -size);
        glVertex3f(size, y + 2.0f, -size);
        glVertex3f(-size, y + 2.0f, -size);

        // Left face
        glVertex3f(-size, y, -size);
        glVertex3f(-size, y, size);
        glVertex3f(-size, y + 2.0f, size);
        glVertex3f(-size, y + 2.0f, -size);

        // Right face
        glVertex3f(size, y, -size);
        glVertex3f(size, y, size);
        glVertex3f(size, y + 2.0f, size);
        glVertex3f(size, y + 2.0f, -size);
    }
    glEnd();
    glPopMatrix();

    // Surrounding support pillars - using metallic color
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        float angle = i * 90.0f;
        float radius = POLE_SPACING;
        float x = radius * cos(angle * M_PI / 180.0f);
        float z = radius * sin(angle * M_PI / 180.0f);

        glTranslatef(x, TOWER_HEIGHT / 2, z);

        // Gradient metallic color - all faces
        glBegin(GL_QUADS);
        for (float h = 0; h < TOWER_HEIGHT; h += 2.0f) {
            float ratio = h / TOWER_HEIGHT;
            // Use a cooler metallic gradient color
            glColor3f(0.6f + ratio * 0.2f,
                0.6f + ratio * 0.2f,
                0.7f + ratio * 0.2f);

            float y = h - TOWER_HEIGHT / 2;
            float size = POLE_RADIUS;

            // Front face
            glVertex3f(-size, y, size);
            glVertex3f(size, y, size);
            glVertex3f(size, y + 2.0f, size);
            glVertex3f(-size, y + 2.0f, size);

            // Back face
            glVertex3f(-size, y, -size);
            glVertex3f(size, y, -size);
            glVertex3f(size, y + 2.0f, -size);
            glVertex3f(-size, y + 2.0f, -size);

            // Left face
            glVertex3f(-size, y, -size);
            glVertex3f(-size, y, size);
            glVertex3f(-size, y + 2.0f, size);
            glVertex3f(-size, y + 2.0f, -size);

            // Right face
            glVertex3f(size, y, -size);
            glVertex3f(size, y, size);
            glVertex3f(size, y + 2.0f, size);
            glVertex3f(size, y + 2.0f, -size);
        }
        glEnd();
        glPopMatrix();
    }

    // Seat support structure - using modern color scheme
    glPushMatrix();
    glTranslatef(0.0f, dropHeight, 0.0f);

    // Main crossbeam
    glColor3f(0.3f, 0.5f, 0.7f); // Deep blue tone
    glPushMatrix();
    glScalef(POLE_SPACING * 2, POLE_RADIUS * 2, POLE_RADIUS * 2);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Seating levels
    for (int level = 0; level < 2; level++) {
        float currentHeight = level * levelSpacing;
        glPushMatrix();
        glTranslatef(0.0f, currentHeight, 0.0f);

        // Seat support crossbeam
        glColor3f(0.4f, 0.6f, 0.8f); // Bright blue
        glPushMatrix();
        glScalef(POLE_SPACING * 2, POLE_RADIUS, POLE_RADIUS);
        glutSolidCube(1.0f);
        glPopMatrix();

        // Seats
        float seatRadius = POLE_SPACING - 1.0f;
        int seatsPerLevel = 8;
        for (int i = 0; i < seatsPerLevel; i++) {
            float angle = i * 360.0f / seatsPerLevel;
            glPushMatrix();
            glRotatef(angle, 0.0f, 1.0f, 0.0f);
            glTranslatef(seatRadius, 0.0f, 0.0f);

            // Seat support arm - using gradient color
            glPushMatrix();
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

            // Gradient from dark to light
            glBegin(GL_QUADS);
            float armLength = seatRadius;
            float armWidth = 0.3f;
            float armHeight = 0.3f;
            float gradientSteps = 10.0f;

            for (float g = 0; g < gradientSteps; g++) {
                float ratio = g / gradientSteps;
                float x = ratio * armLength;

                glColor3f(0.3f + ratio * 0.3f,
                    0.5f + ratio * 0.3f,
                    0.7f + ratio * 0.2f);

                // Top face
                glVertex3f(x, armHeight / 2, -armWidth / 2);
                glVertex3f(x + armLength / gradientSteps, armHeight / 2, -armWidth / 2);
                glVertex3f(x + armLength / gradientSteps, armHeight / 2, armWidth / 2);
                glVertex3f(x, armHeight / 2, armWidth / 2);

                // Bottom face
                glVertex3f(x, -armHeight / 2, -armWidth / 2);
                glVertex3f(x + armLength / gradientSteps, -armHeight / 2, -armWidth / 2);
                glVertex3f(x + armLength / gradientSteps, -armHeight / 2, armWidth / 2);
                glVertex3f(x, -armHeight / 2, armWidth / 2);

                // Front face
                glVertex3f(x, -armHeight / 2, armWidth / 2);
                glVertex3f(x + armLength / gradientSteps, -armHeight / 2, armWidth / 2);
                glVertex3f(x + armLength / gradientSteps, armHeight / 2, armWidth / 2);
                glVertex3f(x, armHeight / 2, armWidth / 2);

                // Back face
                glVertex3f(x, -armHeight / 2, -armWidth / 2);
                glVertex3f(x + armLength / gradientSteps, -armHeight / 2, -armWidth / 2);
                glVertex3f(x + armLength / gradientSteps, armHeight / 2, -armWidth / 2);
                glVertex3f(x, armHeight / 2, -armWidth / 2);
            }
            glEnd();
            glPopMatrix();

            // Seat itself - using vibrant colors
            glColor3f(0.8f, 0.3f, 0.3f); // Seat body in red
            glScalef(0.6f, 0.6f, 0.6f);
            chair();
            glPopMatrix();
        }
        glPopMatrix();
    }
    glPopMatrix();

    // Draw decorative lights
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    for (const auto& light : towerLights) {
        if (light.isOn) {
            glPushMatrix();
            glTranslatef(light.x, light.y, light.z);

            // Draw light sphere
            glColor3f(light.r, light.g, light.b);
            glutSolidSphere(0.3f, 12, 12);

            // Draw glow effect
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glColor4f(light.r, light.g, light.b, 0.8f);
            glutSolidSphere(0.5f, 12, 12);
            glDisable(GL_BLEND);

            glPopMatrix();
        }
    }

    // Draw particle effects
    drawTowerParticles();

    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    // Restore original material state
    glMaterialfv(GL_FRONT, GL_AMBIENT, currentAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, currentDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, currentSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, currentShininess);

    glPopMatrix();
}

// Define a vector to store the lights associated with the second drop tower
std::vector<TowerLight> secondTowerLights;

// Define a vector to store the particles associated with the second drop tower
std::vector<TowerParticle> secondTowerParticles;


// Initialize the lights for the second drop tower
void initSecondTowerLights() {
    // Add decorative lights around the tower at various heights
    for (int height = 0; height < TOWER_HEIGHT; height += 8) {
        for (int i = 0; i < 4; i++) {
            float angle = i * 90.0f * M_PI / 180.0f;  // Calculate the angle for each light
            float radius = POLE_SPACING * 1.2f;  // Increase the radius for the light positions

            // Create multicolored lights based on tower height
            float hue = height / TOWER_HEIGHT;  // Vary the color based on the height
            float r, g, b;
            HSVtoRGB(hue, 1.0f, 1.0f, r, g, b);  // Convert HSV to RGB color values

            // Define a new light source with position and color
            TowerLight light = {
                radius * cos(angle),  // x position of the light
                (float)height,        // y position of the light (height)
                radius * sin(angle),  // z position of the light
                r, g, b,              // RGB color components
                true                  // Light is initially turned on
            };

            // Add the created light to the vector
            secondTowerLights.push_back(light);
        }
    }
}

// Particle rendering function for the second drop tower
void drawSecondTowerParticles() {
    glPushMatrix();  // Save the current matrix state

    // Disable lighting to ensure that particles are not affected by lighting
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);  // Enable blending for transparency effects
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);  // Set the blend function to allow transparency
    glDisable(GL_DEPTH_TEST);  // Disable depth testing to ensure particles are rendered on top

    // Iterate over each particle in the second tower's particle system
    for (const auto& particle : secondTowerParticles) {
        glPushMatrix();  // Save the current matrix state for each particle
        glTranslatef(particle.x, particle.y, particle.z);  // Position the particle in 3D space

        // Make the particle always face the camera (billboarding)
        glRotatef(-cameraAngleY, 0.0f, 1.0f, 0.0f);  // Rotate around the Y-axis (horizontal)
        glRotatef(-cameraAngleX, 1.0f, 0.0f, 0.0f);  // Rotate around the X-axis (vertical)

        // Set the particle color and alpha (transparency)
        glColor4f(particle.r, particle.g, particle.b, particle.a);

        // Draw the particle as a square (quad)
        glBegin(GL_QUADS);
        float size = particle.size / 2.0f;  // Half the size of the particle
        glVertex3f(-size, -size, 0.0f);  // Bottom-left corner
        glVertex3f(size, -size, 0.0f);   // Bottom-right corner
        glVertex3f(size, size, 0.0f);    // Top-right corner
        glVertex3f(-size, size, 0.0f);   // Top-left corner
        glEnd();

        glPopMatrix();  // Restore the matrix state after drawing the particle
    }

    // Re-enable depth testing and lighting after rendering the particles
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);

    glPopMatrix();  // Restore the original matrix state
}


// Light update function for the second drop tower
void updateSecondTowerLights(float deltaTime) {
    static float time = 0.0f;  // Static variable to accumulate time across function calls
    time += deltaTime;  // Increment the time by the elapsed time (deltaTime)

    // Iterate over each light in the second tower's light system
    for (size_t i = 0; i < secondTowerLights.size(); i++) {
        // Calculate the phase of the light's sine wave based on time and light index
        float phase = time * 3.0f + i * 0.2f;  // Modulate phase for each light to create variation

        // Compute the brightness of the light based on the sine wave
        float brightness = 0.5f + 0.5f * sin(phase);  // Sine wave oscillation between 0.0 and 1.0

        // Calculate the height ratio of the light's position relative to the tower's height
        float heightRatio = secondTowerLights[i].y / TOWER_HEIGHT;

        // Check if the tower is dropping and the light is near the drop height
        if (isDropping && fabs(heightRatio - dropHeight / TOWER_HEIGHT) < 0.1f) {
            // If the drop height is close, set the light color to red
            secondTowerLights[i].r = 1.0f;  // Set red component to maximum
            secondTowerLights[i].g = 0.0f;  // Set green component to 0
            secondTowerLights[i].b = 0.0f;  // Set blue component to 0
            // Toggle the light's state based on a fast sine wave oscillation
            secondTowerLights[i].isOn = (sin(time * 10.0f) > 0.0f);
        }
        else {
            // If not at the drop height, set the light color based on its height
            float hue = heightRatio;  // Map height to hue for smooth color transition
            // Convert from HSV to RGB using the calculated hue and brightness
            HSVtoRGB(hue, 1.0f, brightness,
                secondTowerLights[i].r,
                secondTowerLights[i].g,
                secondTowerLights[i].b);
            // Ensure the light is always on (not flickering)
            secondTowerLights[i].isOn = true;
        }
    }
}


// Particle update function for the second drop tower
void updateSecondTowerParticles(float deltaTime) {
    // Check if the tower is dropping and the drop speed exceeds a threshold
    if (isDropping && dropSpeed > 20.0f) {
        // Generate new particles every frame when the drop conditions are met
        for (int i = 0; i < 30; i++) {
            TowerParticle particle;

            // Randomly generate the particle's direction (angle) and distance (radius) from the center
            float angle = (rand() % 360) * M_PI / 180.0f;  // Convert random degree to radians
            float radius = (rand() % 100) / 100.0f * POLE_SPACING;  // Random radius within a certain distance

            // Calculate the particle's 3D position based on the angle and radius
            particle.x = -TOWER_POSITION_X + radius * cos(angle);  // x-coordinate
            particle.y = dropHeight;  // y-coordinate (initial drop height)
            particle.z = TOWER_POSITION_Z + radius * sin(angle);  // z-coordinate

            // Assign random velocities for the particle in 3D space
            particle.vx = (rand() % 200 - 100) / 100.0f * 2.0f;  // Random horizontal velocity
            particle.vy = (rand() % 100) / 100.0f * 5.0f;  // Random vertical velocity
            particle.vz = (rand() % 200 - 100) / 100.0f * 2.0f;  // Random depth velocity

            // Set the particle's initial lifetime, color, transparency, and size
            particle.life = 1.0f + (rand() % 100) / 100.0f;  // Random life duration between 1 and 2 seconds
            particle.r = 1.0f;  // Red component of color (fixed to red)
            particle.g = 0.5f + (rand() % 50) / 100.0f;  // Random green component (between 0.5 and 1.0)
            particle.b = 0.0f;  // Blue component of color (fixed to 0)
            particle.a = 1.0f;  // Initial opacity (fully opaque)
            particle.size = 0.2f + (rand() % 30) / 100.0f;  // Random size between 0.2 and 0.5 units

            // Add the newly created particle to the particle system
            secondTowerParticles.push_back(particle);
        }
    }

    // Update the state of each particle in the particle system
    for (auto it = secondTowerParticles.begin(); it != secondTowerParticles.end();) {
        // Update the particle's position based on its velocity and the elapsed time
        it->x += it->vx * deltaTime;  // Update x-position
        it->y += it->vy * deltaTime;  // Update y-position
        it->z += it->vz * deltaTime;  // Update z-position

        // Apply gravity to the particle's vertical velocity (simple gravity model)
        it->vy -= 9.8f * deltaTime;  // Gravity effect (acceleration due to gravity)

        // Decrease the particle's lifetime over time
        it->life -= deltaTime;  // Decrease the remaining life

        // Update the particle's alpha (transparency) based on its remaining life
        it->a = it->life;

        // If the particle's life has ended, remove it from the system
        if (it->life <= 0) {
            it = secondTowerParticles.erase(it);  // Erase the particle and move to the next one
        }
        else {
            ++it;  // Move to the next particle
        }
    }
}


/**
 * Draws the second drop tower with detailed components and effects.
 * The function includes the base, main pillar, support pillars, seating structure,
 * decorative lights, and particle effects.
 */
void drawSecondDropTower() {
    glPushMatrix();

    // Save current material state
    GLfloat currentAmbient[4], currentDiffuse[4], currentSpecular[4];
    GLfloat currentShininess;
    glGetMaterialfv(GL_FRONT, GL_AMBIENT, currentAmbient);
    glGetMaterialfv(GL_FRONT, GL_DIFFUSE, currentDiffuse);
    glGetMaterialfv(GL_FRONT, GL_SPECULAR, currentSpecular);
    glGetMaterialfv(GL_FRONT, GL_SHININESS, &currentShininess);

    glTranslatef(-TOWER_POSITION_X, 0.0f, TOWER_POSITION_Z);

    // 1. Draw the base - using deep blue metallic texture
    glPushMatrix();
    if (gUseTexture) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texVec[34]);
    }
    glColor3f(0.2f, 0.3f, 0.4f); // Deep blue tone
    glScalef(BASE_SIZE, 2.0f, BASE_SIZE);
    glutSolidCube(1.0f);

    // Add decorative edges to the base
    glColor3f(0.4f, 0.6f, 0.8f); // Bright blue decoration
    glTranslatef(0.0f, 0.5f, 0.0f);
    glScalef(1.05f, 0.1f, 1.05f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Main central pillar - using gradient colors
    glPushMatrix();
    glTranslatef(0.0f, TOWER_HEIGHT / 2, 0.0f);

    // Gradient effect - draw all faces
    glBegin(GL_QUADS);
    for (float h = 0; h < TOWER_HEIGHT; h += 2.0f) {
        float ratio = h / TOWER_HEIGHT;
        // Use a richer gradient color
        glColor3f(0.3f + ratio * 0.3f,
            0.2f + ratio * 0.4f,
            0.4f + ratio * 0.4f);

        float y = h - TOWER_HEIGHT / 2;
        float size = POLE_RADIUS * 2;

        // Front face
        glVertex3f(-size, y, size);
        glVertex3f(size, y, size);
        glVertex3f(size, y + 2.0f, size);
        glVertex3f(-size, y + 2.0f, size);

        // Back face
        glVertex3f(-size, y, -size);
        glVertex3f(size, y, -size);
        glVertex3f(size, y + 2.0f, -size);
        glVertex3f(-size, y + 2.0f, -size);

        // Left face
        glVertex3f(-size, y, -size);
        glVertex3f(-size, y, size);
        glVertex3f(-size, y + 2.0f, size);
        glVertex3f(-size, y + 2.0f, -size);

        // Right face
        glVertex3f(size, y, -size);
        glVertex3f(size, y, size);
        glVertex3f(size, y + 2.0f, size);
        glVertex3f(size, y + 2.0f, -size);
    }
    glEnd();
    glPopMatrix();

    // Surrounding support pillars - using metallic color
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        float angle = i * 90.0f;
        float radius = POLE_SPACING;
        float x = radius * cos(angle * M_PI / 180.0f);
        float z = radius * sin(angle * M_PI / 180.0f);

        glTranslatef(x, TOWER_HEIGHT / 2, z);

        // Gradient metallic color - all faces
        glBegin(GL_QUADS);
        for (float h = 0; h < TOWER_HEIGHT; h += 2.0f) {
            float ratio = h / TOWER_HEIGHT;
            // Use a cooler metallic gradient color
            glColor3f(0.6f + ratio * 0.2f,
                0.6f + ratio * 0.2f,
                0.7f + ratio * 0.2f);

            float y = h - TOWER_HEIGHT / 2;
            float size = POLE_RADIUS;

            // Front face
            glVertex3f(-size, y, size);
            glVertex3f(size, y, size);
            glVertex3f(size, y + 2.0f, size);
            glVertex3f(-size, y + 2.0f, size);

            // Back face
            glVertex3f(-size, y, -size);
            glVertex3f(size, y, -size);
            glVertex3f(size, y + 2.0f, -size);
            glVertex3f(-size, y + 2.0f, -size);

            // Left face
            glVertex3f(-size, y, -size);
            glVertex3f(-size, y, size);
            glVertex3f(-size, y + 2.0f, size);
            glVertex3f(-size, y + 2.0f, -size);

            // Right face
            glVertex3f(size, y, -size);
            glVertex3f(size, y, size);
            glVertex3f(size, y + 2.0f, size);
            glVertex3f(size, y + 2.0f, -size);
        }
        glEnd();
        glPopMatrix();
    }

    // Seat support structure - using modern color scheme
    glPushMatrix();
    glTranslatef(0.0f, dropHeight, 0.0f);

    // Main crossbeam
    glColor3f(0.3f, 0.5f, 0.7f); // Deep blue tone
    glPushMatrix();
    glScalef(POLE_SPACING * 2, POLE_RADIUS * 2, POLE_RADIUS * 2);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Seating levels
    for (int level = 0; level < 2; level++) {
        float currentHeight = level * levelSpacing;
        glPushMatrix();
        glTranslatef(0.0f, currentHeight, 0.0f);

        // Seat support crossbeam
        glColor3f(0.4f, 0.6f, 0.8f); // Bright blue
        glPushMatrix();
        glScalef(POLE_SPACING * 2, POLE_RADIUS, POLE_RADIUS);
        glutSolidCube(1.0f);
        glPopMatrix();

        // Seats
        float seatRadius = POLE_SPACING - 1.0f;
        int seatsPerLevel = 8;
        for (int i = 0; i < seatsPerLevel; i++) {
            float angle = i * 360.0f / seatsPerLevel;
            glPushMatrix();
            glRotatef(angle, 0.0f, 1.0f, 0.0f);
            glTranslatef(seatRadius, 0.0f, 0.0f);

            // Seat support arm - using gradient color
            glPushMatrix();
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

            // Gradient from dark to light
            glBegin(GL_QUADS);
            float armLength = seatRadius;
            float armWidth = 0.3f;
            float armHeight = 0.3f;
            float gradientSteps = 10.0f;

            for (float g = 0; g < gradientSteps; g++) {
                float ratio = g / gradientSteps;
                float x = ratio * armLength;

                glColor3f(0.3f + ratio * 0.3f,
                    0.5f + ratio * 0.3f,
                    0.7f + ratio * 0.2f);

                // Top face
                glVertex3f(x, armHeight / 2, -armWidth / 2);
                glVertex3f(x + armLength / gradientSteps, armHeight / 2, -armWidth / 2);
                glVertex3f(x + armLength / gradientSteps, armHeight / 2, armWidth / 2);
                glVertex3f(x, armHeight / 2, armWidth / 2);

                // Bottom face
                glVertex3f(x, -armHeight / 2, -armWidth / 2);
                glVertex3f(x + armLength / gradientSteps, -armHeight / 2, -armWidth / 2);
                glVertex3f(x + armLength / gradientSteps, -armHeight / 2, armWidth / 2);
                glVertex3f(x, -armHeight / 2, armWidth / 2);

                // Front face
                glVertex3f(x, -armHeight / 2, armWidth / 2);
                glVertex3f(x + armLength / gradientSteps, -armHeight / 2, armWidth / 2);
                glVertex3f(x + armLength / gradientSteps, armHeight / 2, armWidth / 2);
                glVertex3f(x, armHeight / 2, armWidth / 2);

                // Back face
                glVertex3f(x, -armHeight / 2, -armWidth / 2);
                glVertex3f(x + armLength / gradientSteps, -armHeight / 2, -armWidth / 2);
                glVertex3f(x + armLength / gradientSteps, armHeight / 2, -armWidth / 2);
                glVertex3f(x, armHeight / 2, -armWidth / 2);
            }
            glEnd();
            glPopMatrix();

            // Seat itself - using vibrant colors
            glColor3f(0.8f, 0.3f, 0.3f); // Seat body in red
            glScalef(0.6f, 0.6f, 0.6f);
            chair();
            glPopMatrix();
        }
        glPopMatrix();
    }
    glPopMatrix();

    // Draw decorative lights
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    for (const auto& light : secondTowerLights) {
        if (light.isOn) {
            glPushMatrix();
            glTranslatef(light.x, light.y, light.z);

            // Draw light sphere
            glColor3f(light.r, light.g, light.b);
            glutSolidSphere(0.3f, 12, 12);

            // Draw glow effect
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glColor4f(light.r, light.g, light.b, 0.8f);
            glutSolidSphere(0.5f, 12, 12);
            glDisable(GL_BLEND);

            glPopMatrix();
        }
    }

    // Draw particle effects
    drawSecondTowerParticles();

    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    // Restore original material state
    glMaterialfv(GL_FRONT, GL_AMBIENT, currentAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, currentDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, currentSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, currentShininess);

    glPopMatrix();
}

void updateDropTower(float deltaTime) {
    static float waitTimer = 0.0f;  // Timer to manage the waiting period before the drop starts
    static float bounceHeight = 0.0f;  // Height during the bouncing phase

    // If the tower is not currently dropping or recovering, wait for a period before starting the fall
    if (!isDropping && !isRecovering) {
        waitTimer += deltaTime;
        if (waitTimer >= 1.5f) {  // Reduce waiting time for a more thrilling experience
            isDropping = true;  // Begin the dropping phase
            dropSpeed = 0.0f;  // Reset the drop speed at the start
            waitTimer = 0.0f;  // Reset the timer for the next round
        }
    }

    // During the dropping phase
    if (isDropping) {
        dropSpeed += dropAcceleration * deltaTime;  // Increase the speed due to acceleration
        dropHeight -= dropSpeed * deltaTime;  // Decrease the height to simulate falling

        // Check if the tower has reached the bottom
        if (dropHeight <= 15.0f) {  // Increase the minimum height for a more dramatic stop
            dropHeight = 15.0f;  // Ensure the drop height does not go below this point
            isDropping = false;  // Stop the dropping phase
            isRecovering = true;  // Begin the recovering phase

            // Add bouncing effect
            dropSpeed = recoverySpeed * 0.9f;  // Set initial speed for recovery (bounce)
            bounceHeight = dropHeight;  // Store the bounce height
        }
    }
    // During the recovering (bouncing) phase
    else if (isRecovering) {
        if (bounceHeight > 0) {
            // Simulate the bounce effect by moving upwards
            dropHeight += dropSpeed * deltaTime;
            dropSpeed -= dropAcceleration * 0.8f * deltaTime;  // Reduce the bounce speed due to gravity

            if (dropSpeed < 0) {
                // When the speed becomes negative, it indicates the bounce has been exhausted
                bounceHeight = 0;  // Reset the bounce height
                dropSpeed = recoverySpeed;  // Set the recovery speed for normal upward movement
            }
        }
        else {
            // Normal upward movement
            dropHeight += recoverySpeed * deltaTime;

            // Check if the tower has reached the top
            if (dropHeight >= TOWER_HEIGHT) {
                dropHeight = TOWER_HEIGHT;  // Ensure the tower reaches the top exactly
                isRecovering = false;  // End the recovery phase
                dropSpeed = -recoverySpeed * 0.1f;  // Add a small deceleration effect when reaching the top
            }
        }
    }
}



//=============================================================================
// ROLLER COASTER TRACK GENERATION AND CALCULATION
//=============================================================================

/**
 * Calculates normal vectors for each track point to ensure proper orientation
 * Algorithm:
 * 1. Calculate tangent vector at each point
 * 2. Use up vector and twist angle to determine orientation
 * 3. Generate normal vector using cross product
 * 4. Normalize all vectors to ensure unit length
 */
void calculateTrackNormals() {
    for (size_t i = 0; i < trackPoints.size(); i++) {
        // Calculate tangent vector
        Vec3 tangent;
        if (i == trackPoints.size() - 1) {
            tangent = subtractVectors(trackPoints[i].position, trackPoints[i - 1].position);
        }
        else {
            tangent = subtractVectors(trackPoints[i + 1].position, trackPoints[i].position);
        }
        normalize(tangent);

        // Calculate twisted up vector
        Vec3 up = { 0.0f, 1.0f, 0.0f };
        float twist = trackPoints[i].twist * M_PI / 180.0f;
        up.x = sin(twist);
        up.y = cos(twist);

        // Generate normal vector
        Vec3 normal = crossProduct(tangent, up);
        normalize(normal);

        // Recalculate up vector to ensure orthogonality
        up = crossProduct(normal, tangent);
        normalize(up);

        trackPoints[i].normal = normal;
    }
}

// Initialize the track points for the roller coaster path
void initializeTrackPoints() {
    trackPoints.clear();  // Clear any existing track points

    const float trackLength = 120.0f;  // Total track length in meters
    const float maxHeight = 60.0f;  // Maximum height for the track segments
    const int numPoints = 500;  // Total number of track points

    // First section: Ascending segment
    for (int i = 0; i < numPoints / 8; i++) {
        // Compute the interpolation parameter t
        float t = (float)i / (numPoints / 8);
        TrackPoint point;

        // Calculate position based on the current segment parameters
        point.position = {
            COASTER_START_X + t * 20.0f,
            COASTER_START_Y + t * maxHeight,
            COASTER_START_Z
        };

        point.twist = 0.0f;  // No twist in this section
        trackPoints.push_back(point);  // Add the point to the track
    }

    // Second section: Spiral descent
    float spiralRadius = 25.0f;  // Radius of the spiral curve
    float spiralHeight = maxHeight;  // Starting height for the spiral
    int spiralPoints = numPoints / 4;  // Number of points for the spiral segment

    for (int i = 0; i < spiralPoints; i++) {
        // Compute interpolation parameter t
        float t = (float)i / spiralPoints;
        float angle = t * 4 * M_PI;  // Angle for the spiral curve

        TrackPoint point;

        // Calculate position for the spiral descent based on polar coordinates
        point.position = {
            COASTER_START_X + 20.0f + spiralRadius * cos(angle),
            spiralHeight * (1.0f - t),
            COASTER_START_Z + spiralRadius * sin(angle)
        };

        point.twist = angle * 30.0f;  // Apply twist based on the angle
        trackPoints.push_back(point);  // Add the point to the track
    }

    // Third section: High-speed straight segment with undulation
    float waveAmplitude = 5.0f;  // Amplitude of the wave
    float waveFrequency = 0.5f;  // Frequency of the wave

    for (int i = 0; i < numPoints / 8; i++) {
        // Compute interpolation parameter t
        float t = (float)i / (numPoints / 8);
        TrackPoint point;

        // Position with undulation along the Y-axis
        point.position = {
            COASTER_START_X + 20.0f + spiralRadius + t * 40.0f,
            15.0f + waveAmplitude * sin(t * 2 * M_PI * waveFrequency),
            COASTER_START_Z
        };

        point.twist = 0.0f;  // No twist in this section
        trackPoints.push_back(point);  // Add the point to the track
    }

    // Fourth section: Spiral ascent
    float loopRadius = 15.0f;  // Radius of the loop
    int loopPoints = numPoints / 4;  // Number of points for the loop ascent

    for (int i = 0; i < loopPoints; i++) {
        // Compute interpolation parameter t
        float t = (float)i / loopPoints;
        float angle = t * 2 * M_PI;  // Angle for the loop ascent

        TrackPoint point;

        // Position based on circular motion for the loop ascent
        point.position = {
            COASTER_START_X + 60.0f,
            15.0f + loopRadius * (1 - cos(angle)),
            COASTER_START_Z + loopRadius * sin(angle)
        };

        point.twist = angle * 180.0f / M_PI;  // Apply twist based on the angle
        trackPoints.push_back(point);  // Add the point to the track
    }

    // Fifth section: Final return segment (S-curve)
    for (int i = 0; i < numPoints / 4; i++) {
        // Compute interpolation parameter t
        float t = (float)i / (numPoints / 4);
        TrackPoint point;

        // S-curve positioning for the return segment
        point.position = {
            COASTER_START_X + 60.0f - t * 60.0f,
            15.0f * (1.0f - t),
            COASTER_START_Z + 20.0f * sin(t * 2 * M_PI)
        };

        point.twist = 45.0f * sin(t * 2 * M_PI);  // Apply twist along the S-curve
        trackPoints.push_back(point);  // Add the point to the track
    }

    // Compute track normals (used for calculating orientations and smoothness of the track)
    calculateTrackNormals();
}




//=============================================================================
// TRACK POINT GENERATION AND SPLINE CALCULATIONS
//=============================================================================

/**
 * Implements Catmull-Rom spline interpolation for smooth track generation
 * @param p0,p1,p2,p3: Four control points for spline calculation
 * @param t: Interpolation parameter (0 to 1)
 * @return: Interpolated point on the spline
 */
Vec3 catmullRomSpline(const Vec3& p0, const Vec3& p1, const Vec3& p2, const Vec3& p3, float t) {
    // Calculate spline coefficients
    float t2 = t * t;
    float t3 = t2 * t;

    // Catmull-Rom matrix coefficients
    float b0 = -0.5f * t3 + t2 - 0.5f * t;
    float b1 = 1.5f * t3 - 2.5f * t2 + 1.0f;
    float b2 = -1.5f * t3 + 2.0f * t2 + 0.5f * t;
    float b3 = 0.5f * t3 - 0.5f * t2;

    // Interpolate each component
    return Vec3(
        b0 * p0.x + b1 * p1.x + b2 * p2.x + b3 * p3.x,
        b0 * p0.y + b1 * p1.y + b2 * p2.y + b3 * p3.y,
        b0 * p0.z + b1 * p1.z + b2 * p2.z + b3 * p3.z
    );
}



// Macros to access train position and orientation
#define currentTrainPosition currentTrain.position
#define currentTrainOrientation currentTrain.orientation

// Function to compute the position of the train along the track based on the parameter t
// and the direction of movement (FORWARD or BACKWARD).
Vec3 getTrainPosition(float t, TrainDirection direction) {
    // Total number of points in the track, excluding the first point
    float totalLength = trackPoints.size() - 1;
    float pos;  // Position parameter (normalized)

    // Determine the position along the track based on the direction
    if (direction == FORWARD) {
        // Moving forward along the track
        pos = t * totalLength;
    }
    else {
        // Moving backward along the track
        pos = totalLength - (t * totalLength);
    }

    // Index of the current track point segment
    int index = static_cast<int>(pos);
    // Fractional part representing the position within the segment
    float segmentT = pos - index;

    // Handle edge cases where the position exceeds track bounds
    if (index >= trackPoints.size() - 1) {
        // If the index exceeds the last point, return the position of the last point
        return trackPoints.back().position;
    }
    if (index < 0) {
        // If the index is negative, return the position of the first point
        return trackPoints.front().position;
    }

    // Get the four track points surrounding the current position (for interpolation)
    Vec3 p0 = (index == 0) ? trackPoints[0].position : trackPoints[index - 1].position;  // Previous point or first point
    Vec3 p1 = trackPoints[index].position;  // Current point
    Vec3 p2 = trackPoints[index + 1].position;  // Next point
    Vec3 p3 = (index >= trackPoints.size() - 2) ? trackPoints[index + 1].position : trackPoints[index + 2].position;  // Next-to-next point or last point

    // Perform Catmull-Rom spline interpolation to compute the exact train position
    Vec3 trainPos = catmullRomSpline(p0, p1, p2, p3, segmentT);

    // Compute the direction of movement by checking the train's position at the next step
    float nextT = segmentT + 0.01f;
    Vec3 nextPos = catmullRomSpline(p0, p1, p2, p3, nextT);

    // Calculate the movement direction by subtracting the current position from the next position
    Vec3 movementDirection = subtractVectors(nextPos, trainPos);
    // Normalize the movement direction vector to obtain a unit vector
    normalize(movementDirection);

    // Return the computed train position
    return trainPos;
}

// Function to draw a cube using OpenGL
void drawBox() {
    glBegin(GL_QUADS);  // Begin drawing quadrilateral faces

    // Front face of the cube
    // The normal vector is perpendicular to the face, pointing outwards
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);  // Bottom-left corner
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);   // Bottom-right corner
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);    // Top-right corner
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);   // Top-left corner

    // Back face of the cube
    // The normal vector points in the opposite direction (into the screen)
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f); // Bottom-left corner
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);  // Top-left corner
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);   // Top-right corner
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);  // Bottom-right corner

    // Top face of the cube
    // Normal vector points upwards
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);  // Bottom-left corner
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.5f);   // Bottom-right corner
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, 0.5f);    // Top-right corner
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);   // Top-left corner

    // Bottom face of the cube
    // Normal vector points downwards
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f); // Top-left corner
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);  // Top-right corner
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);   // Bottom-right corner
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);  // Bottom-left corner

    // Right face of the cube
    // Normal vector points to the right
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);  // Bottom-left corner
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);   // Top-left corner
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);    // Top-right corner
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);   // Bottom-right corner

    // Left face of the cube
    // Normal vector points to the left
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f); // Bottom-left corner
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);  // Bottom-right corner
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);   // Top-right corner
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);  // Top-left corner

    glEnd();  // End drawing quadrilateral faces
}


//=============================================================================
// TRACK VISUALIZATION AND RENDERING
//=============================================================================

/**
 * Draws the main track structure with textures and lighting
 * Rendering process:
 * 1. Apply track texture
 * 2. Generate quad strips for track surface
 * 3. Calculate proper normals for lighting
 * 4. Apply texture coordinates for mapping
 */
void drawCoasterTrack() {
    // DEBUG: Track rendering
#ifdef DEBUG_TRACK_RENDERING
    static int frameCount = 0;
    std::cout << "Drawing track frame: " << frameCount++ << std::endl;
#endif

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texVec[43]);

    const float trackWidth = 2.0f;
    const int segmentsPerSection = 40;

    // Iterate through track points to create track surface
    for (size_t i = 0; i < trackPoints.size() - 1; i++) {
        // Get control points for spline segment
        Vec3 p0 = (i == 0) ? trackPoints[0].position : trackPoints[i - 1].position;
        Vec3 p1 = trackPoints[i].position;
        Vec3 p2 = trackPoints[i + 1].position;
        Vec3 p3 = (i == trackPoints.size() - 2) ? trackPoints[i + 1].position : trackPoints[i + 2].position;

        // Create quad strip for track segment
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= segmentsPerSection; j++) {
            float t = (float)j / segmentsPerSection;

            // Calculate current point and next point for direction
            Vec3 currentPoint = catmullRomSpline(p0, p1, p2, p3, t);
            Vec3 nextPoint = (j < segmentsPerSection) ?
                catmullRomSpline(p0, p1, p2, p3, (float)(j + 1) / segmentsPerSection) :
                p2;

            // Calculate track orientation vectors
            Vec3 dir = subtractVectors(nextPoint, currentPoint);
            Vec3 n1 = trackPoints[i].normal;
            Vec3 n2 = trackPoints[i + 1].normal;

            // Interpolate normal vector
            float blend = t;
            Vec3 currentNormal = {
                n1.x * (1 - blend) + n2.x * blend,
                n1.y * (1 - blend) + n2.y * blend,
                n1.z * (1 - blend) + n2.z * blend
            };
            normalize(currentNormal);

            // Calculate right vector for track width
            Vec3 right = crossProduct(dir, currentNormal);
            normalize(right);

            // Generate track vertices with texture coordinates
            glTexCoord2f(0.0f, t);
            glVertex3f(currentPoint.x - right.x * trackWidth,
                currentPoint.y - right.y * trackWidth,
                currentPoint.z - right.z * trackWidth);

            glTexCoord2f(1.0f, t);
            glVertex3f(currentPoint.x + right.x * trackWidth,
                currentPoint.y + right.y * trackWidth,
                currentPoint.z + right.z * trackWidth);
        }
        glEnd();
    }

    glDisable(GL_TEXTURE_2D);
}

//=============================================================================
// SUPPORT STRUCTURE GENERATION AND RENDERING
//=============================================================================

/**
 * Draws a single support pole for the track
 * @param baseX,baseY,baseZ: Base position of the support pole
 * @param height: Height of the support pole
 */
void drawSupportPole(float baseX, float baseY, float baseZ, float height) {
    const float poleRadius = 0.3f;  // Support pole radius

    // Save current matrix state
    glPushMatrix();
    glTranslatef(baseX, baseY, baseZ);
    glRotatef(-90, 1, 0, 0);  // Rotate to align with Y axis

    // Create and render support pole
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    glColor4f(0.8f, 0.8f, 0.8f, 0.2f);  // Set pole color
    gluCylinder(quad, poleRadius, poleRadius, height, 16, 16);
    gluDeleteQuadric(quad);

    glPopMatrix();
}

/**
 * Checks if a track point is within the loop section
 * @param point: Track point to check
 * @return: true if point is in loop section
 */
bool isInLoop(const TrackPoint& point) {
    // Define loop center and radius
    float loopCenterX = COASTER_START_X + 60.0f;
    float loopCenterZ = COASTER_START_Z;
    float loopRadius = 15.0f;

    // Calculate distance from point to loop center
    float dx = point.position.x - loopCenterX;
    float dz = point.position.z - loopCenterZ;
    float distanceFromCenter = sqrt(dx * dx + dz * dz);

    // Check if point is within loop area (with 20% margin)
    return distanceFromCenter <= loopRadius * 1.2f;
}

/**
 * Checks if a track point is on a steep slope section
 * @param point: Track point to check
 * @return: true if point is on steep slope
 */
bool isOnSteepSlope(const TrackPoint& point) {
    float slopeStartX = COASTER_START_X;
    float slopeEndX = COASTER_START_X + 20.0f;

    // Check if point is within slope boundaries
    return (point.position.x >= slopeStartX - 2.0f &&
        point.position.x <= slopeEndX);
}

/**
 * Draws support structures for the entire coaster track
 * Support placement logic:
 * 1. Skip supports in loop sections
 * 2. Skip supports on steep slopes
 * 3. Place supports at regular intervals
 */
void drawCoasterSupports() {
    // DEBUG: Support structure rendering
#ifdef DEBUG_SUPPORT_RENDERING
    int supportCount = 0;
#endif

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texVec[44]);  // Support texture

    // Iterate through track points
    for (size_t i = 0; i < trackPoints.size(); i++) {
        Vec3 p = trackPoints[i].position;

        // Skip supports in special sections
        if (!isInLoop(trackPoints[i]) && !isOnSteepSlope(trackPoints[i])) {
            drawSupportPole(p.x, 0.0f, p.z, p.y);

#ifdef DEBUG_SUPPORT_RENDERING
            supportCount++;
#endif
        }
    }

#ifdef DEBUG_SUPPORT_RENDERING
    std::cout << "Total supports placed: " << supportCount << std::endl;
#endif

    glDisable(GL_TEXTURE_2D);
}

/**
 * Draws the track rails using metal texture
 * Rail rendering process:
 * 1. Apply metal texture
 * 2. Generate parallel rails
 * 3. Add proper lighting and materials
 */
void drawRail(const Vec3& start, const Vec3& end, float railRadius) {
    Vec3 direction = { end.x - start.x, end.y - start.y, end.z - start.z };
    float length = sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);

    glPushMatrix();
    glTranslatef(start.x, start.y, start.z);


    float angle = acos(direction.y / length) * 180.0f / M_PI;
    float axisX = -direction.z;
    float axisZ = direction.x;

    glRotatef(angle, axisX, 0, axisZ);

    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluCylinder(quad, railRadius, railRadius, length, 16, 16);
    gluDeleteQuadric(quad);

    glPopMatrix();
}
void drawTrackRails() {
    const float railRadius = 1.0f;  // Rail thickness

    // Enable texturing for rails
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texVec[42]);  // Metal texture

    // Iterate through track points
    for (size_t i = 0; i < trackPoints.size() - 1; i++) {
        Vec3 p1 = trackPoints[i].position;
        Vec3 p2 = trackPoints[i + 1].position;

        // Draw parallel rails
        drawRail({ p1.x - 0.5f, p1.y, p1.z },
            { p2.x - 0.5f, p2.y, p2.z }, railRadius);
        drawRail({ p1.x + 0.5f, p1.y, p1.z },
            { p2.x + 0.5f, p2.y, p2.z }, railRadius);
    }

    glDisable(GL_TEXTURE_2D);
}

// Function to draw track supports with specified characteristics
void drawTrackSupports() {
    // Enable 2D texture mapping
    glEnable(GL_TEXTURE_2D);
    // Bind the texture identified by texVec[44]
    glBindTexture(GL_TEXTURE_2D, texVec[44]);

    // Define the spacing between the main support pillars
    const int supportInterval = 40;  // Distance between each support pillar
    // Define the minimum height for the support pillars
    const float minHeight = 55.0f;   // Minimum height of each support pillar
    // Define the angle at which supports are oriented relative to the track
    const float supportAngle = 30.0f * M_PI / 180.0f; // Angle of the supports in radians

    // Disable 2D texture mapping after use
    glDisable(GL_TEXTURE_2D);
}


// Function to draw safety bars for the ride's seating area
void drawSafetyBars(float seatWidth, float seatDepth) {
    // Bind the texture for the metal appearance of the bars
    glBindTexture(GL_TEXTURE_2D, texVec[42]); // Metal texture

    // Define the color of the safety bars (gray)
    GLfloat bar_color[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    // Set the material properties for the safety bars (ambient and diffuse)
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, bar_color);

    // Define the radius of the safety bars
    const float barRadius = 0.1f;
    // Create a new quadric object for drawing cylinders (the bars)
    GLUquadric* quad = gluNewQuadric();
    // Enable texture mapping for the quadric object
    gluQuadricTexture(quad, GL_TRUE);

    // Loop to draw the left and right safety bars
    for (float x : {-seatWidth * 0.4f, seatWidth * 0.4f}) {
        // Move the drawing context to the correct position for each safety bar
        glPushMatrix();
        glTranslatef(x, 0, 0);

        // Draw the vertical support bars
        glPushMatrix();
        // Rotate the coordinate system to align the bar vertically
        glRotatef(-90, 1, 0, 0);
        // Draw a vertical cylinder representing the support bar
        gluCylinder(quad, barRadius, barRadius, CARRIAGE_HEIGHT * 0.6f, 12, 1);
        glPopMatrix();

        // Draw the horizontal safety bars
        glPushMatrix();
        // Translate the position to the correct height for the horizontal bar
        glTranslatef(0, CARRIAGE_HEIGHT * 0.5f, 0);
        // Rotate the coordinate system to align the bar horizontally
        glRotatef(90, 0, 1, 0);
        // Draw a horizontal cylinder representing the safety bar
        gluCylinder(quad, barRadius, barRadius, seatWidth * 0.4f, 12, 1);
        glPopMatrix();
    }

    // Clean up by deleting the quadric object after use
    gluDeleteQuadric(quad);
}

// Function to draw the seats in the carriage
void drawSeats() {
    // Enable 2D texture mapping for the seat textures
    glEnable(GL_TEXTURE_2D);
    // Bind the texture for the seat surface
    glBindTexture(GL_TEXTURE_2D, texVec[41]); // Seat texture

    // Define the dimensions of each seat
    const float seatWidth = CARRIAGE_WIDTH * 0.4f; // Seat width is 40% of carriage width
    const float seatDepth = CARRIAGE_LENGTH * 0.2f; // Seat depth is 20% of carriage length
    const int numSeats = 1; // Number of seats (currently set to 1)

    // Define the material properties for the seat
    GLfloat seat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f }; // Ambient color of the seat (dark gray)
    GLfloat seat_diffuse[] = { 0.8f, 0.0f, 0.0f, 1.0f }; // Diffuse color of the seat (red)
    // Set the material properties for the seat's ambient color
    glMaterialfv(GL_FRONT, GL_AMBIENT, seat_ambient);
    // Set the material properties for the seat's diffuse color
    glMaterialfv(GL_FRONT, GL_DIFFUSE, seat_diffuse);

    // Loop through the number of seats to draw each seat
    for (int i = 0; i < numSeats; i++) {
        // Calculate the offset for the seat's position along the depth axis
        float offset = (i - (numSeats - 1) / 2.0f) * seatDepth * 1.5f;

        // Move the seat to the appropriate position in the carriage
        glPushMatrix();
        glTranslatef(0, SEAT_HEIGHT, offset);

        // Draw the seat base (bottom part of the seat)
        glPushMatrix();
        // Scale the base to the appropriate seat dimensions
        glScalef(seatWidth, CARRIAGE_HEIGHT * 0.1f, seatDepth);
        // Call the function to draw the seat base as a box
        drawBox();
        glPopMatrix();

        // Draw the seat backrest (the part that supports the back of the seat)
        glPushMatrix();
        // Translate the backrest to the correct position behind the seat base
        glTranslatef(0, CARRIAGE_HEIGHT * 0.2f, -seatDepth * 0.4f);
        // Rotate the backrest slightly to tilt it (15 degrees)
        glRotatef(15, 1, 0, 0); // Slightly tilt the backrest
        // Scale the backrest to the appropriate dimensions
        glScalef(seatWidth, CARRIAGE_HEIGHT * 0.4f, seatDepth * 0.1f);
        // Call the function to draw the backrest as a box
        drawBox();
        glPopMatrix();

        // Draw the safety bars for the seat
        drawSafetyBars(seatWidth, seatDepth);

        glPopMatrix();
    }

    // Disable 2D texture mapping after drawing the seats
    glDisable(GL_TEXTURE_2D);
}


// Function to draw the wheels of a vehicle
void drawWheels(float length, float width, float radius) {
    // Save the current transformation matrix
    glPushMatrix();

    // Create a new quadric object for drawing cylindrical shapes (used for the wheels)
    GLUquadric* quad = gluNewQuadric();
    // Enable texture mapping for the quadric object
    gluQuadricTexture(quad, GL_TRUE);
    // Bind the texture for the wheels (metal texture)
    glBindTexture(GL_TEXTURE_2D, texVec[42]); // Use metal texture

    // Calculate the offset for the wheel positions along the length of the vehicle
    const float wheelOffset = length * 0.4f;
    // Define the positions of the four wheels in 3D space
    const Vec3 wheelPositions[] = {
        {-width / 2, radius, -wheelOffset}, // Front-left wheel
        {width / 2, radius, -wheelOffset},  // Front-right wheel
        {-width / 2, radius, wheelOffset},  // Rear-left wheel
        {width / 2, radius, wheelOffset}    // Rear-right wheel
    };

    // Loop through each wheel position and draw the corresponding wheel
    for (const auto& pos : wheelPositions) {
        // Save the current transformation matrix for each wheel
        glPushMatrix();
        // Translate the model to the wheel's position
        glTranslatef(pos.x, pos.y, pos.z);
        // Rotate the wheel to make it vertical (aligned with the z-axis)
        glRotatef(90, 0, 1, 0); // Rotate to make the wheel vertical
        // Draw the wheel as a cylinder with the given radius and height (0.5f)
        gluCylinder(quad, radius, radius, 0.5f, 16, 1); // Wheel's cylindrical shape
        // Restore the previous transformation matrix
        glPopMatrix();
    }

    // Delete the quadric object after use
    gluDeleteQuadric(quad);
    // Restore the previous transformation matrix
    glPopMatrix();
}

// Function to draw the main body of the carriage
void drawCarriageBody() {
    // Enable 2D texture mapping for the carriage body
    glEnable(GL_TEXTURE_2D);
    // Bind the texture for the carriage body (main texture for the body)
    glBindTexture(GL_TEXTURE_2D, texVec[40]); // Texture for the carriage body

    // Set material properties for the metal surface of the carriage body
    GLfloat metal_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };   // Ambient reflection color
    GLfloat metal_diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };   // Diffuse reflection color
    GLfloat metal_specular[] = { 0.9f, 0.9f, 0.9f, 1.0f };  // Specular reflection color
    GLfloat metal_shininess[] = { 50.0f };                   // Shininess factor for specular highlights

    // Apply the material properties to the front face (carriage body)
    glMaterialfv(GL_FRONT, GL_AMBIENT, metal_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, metal_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, metal_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, metal_shininess);

    // Draw the main body of the carriage (frame)
    glPushMatrix();
    // Scale the model to match the carriage's dimensions: width, height, and length
    glScalef(CARRIAGE_WIDTH, CARRIAGE_HEIGHT, CARRIAGE_LENGTH);
    // Draw the box representing the body of the carriage
    drawBox();
    glPopMatrix();

    // Draw the roof decoration (a slightly larger box on top of the carriage)
    glPushMatrix();
    // Translate the model to position the roof above the main body of the carriage
    glTranslatef(0, CARRIAGE_HEIGHT * 0.5f, 0);
    // Scale the roof to be slightly larger than the main body of the carriage
    glScalef(CARRIAGE_WIDTH * 1.1f, CARRIAGE_HEIGHT * 0.2f, CARRIAGE_LENGTH * 1.1f);
    // Draw the box representing the roof of the carriage
    drawBox();
    glPopMatrix();

    // Disable 2D texture mapping after drawing the carriage body
    glDisable(GL_TEXTURE_2D);
}

// Function to draw the coaster train, including the train body and seats
void drawCoasterTrain() {
    // Save the current transformation matrix
    glPushMatrix();

    // Translate the model to the current train's position (adding an offset to the y-coordinate)
    glTranslatef(currentTrain.position.x,
        currentTrain.position.y + 4.0f,
        currentTrain.position.z);

    // Apply the current train's orientation to the model (rotation around the y-axis)
    glRotatef(currentTrain.orientation, 0, 1, 0);

    // Draw the train carriage with texture mapping enabled
    glEnable(GL_TEXTURE_2D);
    // Bind the texture for the train body (main texture for the body)
    glBindTexture(GL_TEXTURE_2D, texVec[40]); // Texture for the carriage body

    // Draw the main body of the train (carriage frame)
    glPushMatrix();
    // Scale the model to match the carriage's dimensions (width, height, and length)
    glScalef(6.0f, 4.5f, 6.0f);
    // Draw the box representing the main body of the carriage
    drawBox();
    glPopMatrix();

    // Draw the seats inside the carriage
    glBindTexture(GL_TEXTURE_2D, texVec[41]); // Texture for the seats
    for (int i = 0; i < 4; i++) {
        // Draw each seat at different positions along the z-axis
        glPushMatrix();
        // Translate the seat to the appropriate position within the carriage
        glTranslatef(0.0f, 1.0f, -1.5f + i);
        // Scale the seat dimensions
        glScalef(1.8f, 0.5f, 0.8f);
        // Draw the box representing the seat
        drawBox();
        glPopMatrix();
    }

    // Disable 2D texture mapping after drawing the train
    glDisable(GL_TEXTURE_2D);

    // Restore the previous transformation matrix
    glPopMatrix();
}

// Function to update the position and orientation of the carriage
void updateCarriagePosition(float t) {
    // Get the current position and the next position along the track
    Vec3 currentPos = getTrainPosition(t, currentDirection);  // Current position of the train at time t
    Vec3 nextPos = getTrainPosition(t + 0.01f, nextDirection); // Next position of the train at time t + 0.01

    // Calculate the forward direction vector (the direction from currentPos to nextPos)
    Vec3 forward = {
        nextPos.x - currentPos.x,
        nextPos.y - currentPos.y,
        nextPos.z - currentPos.z
    };
    normalize(forward); // Normalize the forward vector to unit length

    // Calculate the up direction vector (using the normal vector of the track at the current position)
    int currentIndex = static_cast<int>(t * (trackPoints.size() - 1)); // Calculate the current index based on time t
    Vec3 up = trackPoints[currentIndex].normal;  // Track normal vector at the current index
    normalize(up); // Normalize the up vector

    // Calculate the right direction vector as the cross product of forward and up
    Vec3 right = crossProduct(forward, up); // Right vector is perpendicular to both forward and up
    normalize(right); // Normalize the right vector to unit length

    // Update the current position of the train
    currentTrain.position = currentPos; // Set the train's current position

    // Construct the rotation matrix based on the forward, up, and right vectors
    float rotationMatrix[16] = {
        right.x, right.y, right.z, 0,       // Right vector as the first column
        up.x, up.y, up.z, 0,                 // Up vector as the second column
        forward.x, forward.y, forward.z, 0, // Forward vector as the third column
        currentPos.x, currentPos.y, currentPos.z, 1 // Position of the train as the translation part of the matrix
    };

    // Save the constructed rotation matrix for later use during rendering
    memcpy(currentTrain.rotationMatrix, rotationMatrix, sizeof(float) * 16); // Store the matrix in currentTrain's rotationMatrix
}

// Function to update the position and orientation of the coaster train
void updateCoasterTrain(float deltaTime) {
    // If the coaster is not running, exit the function
    if (!isCoasterRunning) return;

    // Update the train's position based on its current speed and the time delta
    trainPosition += currentTrain.speed * deltaTime;

    // Calculate the current track segment index based on the train's position
    int currentIndex = (int)trainPosition;
    float t = trainPosition - currentIndex;  // Calculate the interpolation factor (fractional part)

    // Ensure the train loops within the bounds of the track by wrapping around
    while (currentIndex >= trackPoints.size() - 1) {
        currentIndex -= (trackPoints.size() - 1);  // Wrap around if the index exceeds the last point
        trainPosition -= (trackPoints.size() - 1); // Adjust the position to fit within the range
    }

    // Get the current track point and the next track point for interpolation
    const TrackPoint& p1 = trackPoints[currentIndex];       // Current track point
    const TrackPoint& p2 = trackPoints[currentIndex + 1];   // Next track point

    // Interpolate the position and direction of the train based on the current and next track points
    currentTrain.position = {
        p1.position.x + (p2.position.x - p1.position.x) * t,   // Interpolated X position
        p1.position.y + (p2.position.y - p1.position.y) * t,   // Interpolated Y position
        p1.position.z + (p2.position.z - p1.position.z) * t    // Interpolated Z position
    };

    // Compute the train's speed based on the height difference (gravitational acceleration)
    float heightDiff = p2.position.y - p1.position.y; // Calculate the height difference between consecutive points
    currentTrain.speed = std::max(MIN_SPEED,   // Ensure the speed is within the minimum speed limit
        std::min(MAX_SPEED,                  // Ensure the speed is within the maximum speed limit
            currentTrain.speed - heightDiff * GRAVITY * deltaTime));  // Adjust speed based on gravity

    // Update the train's orientation (twist) based on the interpolation between the current and next track points
    float twist = p1.twist + (p2.twist - p1.twist) * t;  // Interpolate the twist (orientation) between p1 and p2
    currentTrain.orientation = twist;  // Set the new orientation for the train
}

// Overall rotation angle (in degrees), can be adjusted as needed
float coasterRotationAngle = 0.0f;  // Rotation angle of the coaster, adjustable for the desired effect

// Texture IDs for different materials
GLuint woodTextureId;      // Texture ID for wood material
GLuint metalTextureId;     // Texture ID for metal components


// Function to draw the roller coaster with its components
void drawRollerCoaster() {
    // Apply transformations to the roller coaster model
    glPushMatrix();
    glScalef(0.85, 0.65, 0.65);  // Scale the model to adjust size

    // First apply the overall rotation
    glTranslatef(COASTER_START_X, 0, COASTER_START_Z);  // Translate the model to the starting position of the track
    glRotatef(coasterRotationAngle, 0, 1, 0);            // Apply rotation around the Y-axis for the coaster's orientation
    glTranslatef(-COASTER_START_X, 0, -COASTER_START_Z); // Translate the model back to its original position after rotation

    // Set fixed material properties for the coaster
    GLfloat material_diffuse[] = { 0.8f, 0.2f, 0.2f, 1.0f }; // Diffuse color (red)
    GLfloat material_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f }; // Specular reflection (gray)
    GLfloat material_shininess[] = { 30.0f };                  // Shininess coefficient

    // Apply material properties to the front face
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

    glColor3f(1, 1, 1);  // Set the color to white

    // Draw the supporting structure of the roller coaster
    drawCoasterSupports();

    // Draw the roller coaster track
    drawCoasterTrack();

    // Draw the roller coaster train (after transformations)
    drawCoasterTrain();

    // Pop the transformation matrix to revert to the previous state
    glPopMatrix();
}


void displayCB()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (isLightningActive) {

        GLfloat lightningAmbient[] = { sceneBrightness, sceneBrightness, sceneBrightness, 1.0f };
        GLfloat lightningDiffuse[] = { sceneBrightness, sceneBrightness, sceneBrightness, 1.0f };

        glMaterialfv(GL_FRONT, GL_AMBIENT, lightningAmbient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, lightningDiffuse);
    }
    else {

        glMaterialfv(GL_FRONT, GL_AMBIENT, initialAmbient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, initialDiffuse);
    }

    if (currentGameState == MAIN_SCENE) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        if (!isRidingChair) {
            // **Normal Perspective**

            // Set camera position
            float cameraPosX = manPositionX - cameraDistance * -sin(cameraAngleY * M_PI / 180.0f) * cos(cameraAngleX * M_PI / 180.0f);
            float cameraPosY = manPositionY + cameraDistance * sin(cameraAngleX * M_PI / 180.0f);
            float cameraPosZ = manPositionZ - cameraDistance * -cos(cameraAngleY * M_PI / 180.0f) * cos(cameraAngleX * M_PI / 180.0f);

            // If shaking, add random offsets
            if (shakeIntensity > 0.0f) {
                float maxOffset = shakeIntensity; // Maximum offset
                float shakeX = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f; // -1 to 1
                float shakeY = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
                float shakeZ = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
                cameraPosX += shakeX * maxOffset;
                cameraPosY += shakeY * maxOffset;
                cameraPosZ += shakeZ * maxOffset;
            }

            gluLookAt(
                cameraPosX, cameraPosY, cameraPosZ,                 // Camera position
                manPositionX, manPositionY + totalHeight / 2.0f, manPositionZ, // Look at point (upper body)
                0.0f, 1.0f, 0.0f                                  // Up direction
            );
        }
        else {
            // **Riding Chair Perspective**

            // Calculate the chair's current rotation angle
            float totalRotationAngle = alpha + additionalRotationAngle; // Total rotation angle
            float angleRad = totalRotationAngle * M_PI / 180.0f;

            // Define camera parameters relative to the chair
            float cameraOffsetDistance = 0.0f; // Camera is centered on the chair
            float cameraHeight = 1.0f;          // Height of the camera relative to the chair

            // Compute the camera position, following the chair's rotation
            float camX = chairCurrentX;
            float camY = chairCurrentY + cameraHeight;
            float camZ = chairCurrentZ;

            // Define the camera's look-at direction, which changes with the chair's rotation
            float lookX = camX + sin(angleRad);
            float lookY = camY;
            float lookZ = camZ + cos(angleRad);

            gluLookAt(
                camX, camY, camZ,    // Camera position
                lookX, lookY, lookZ, // Look-at point
                0.0f, 1.0f, 0.0f     // Up direction
            );
        }

        drawRollerCoaster();

        // Set up spotlights
        setupSpotlights();


        // Use a scaling factor of 2.0 for uniform scaling
        float scaleFactor = 2.0f;
        // Scale and translate to appropriately enlarge the scene and maintain reasonable position
        glPushMatrix();
        glScalef(scaleFactor, scaleFactor, scaleFactor);
        glTranslatef(-1.5f, 0.0f, -1.5f); // Adjust translation as needed to ensure proper positioning

        // Draw house and its internal elements
        renderBase();
        renderWalls();
        drawDoor();

        glPushMatrix();
        glTranslated(-5.2, 0, -2.4);
        glRotatef(90, 0, 1, 0);
        glScaled(1.3, 1, 1);
        renderSofa();
        glPopMatrix();

        glPushMatrix();
        glTranslated(-5.4, 0, 1.2);
        glRotatef(90, 0, 1, 0);
        glScaled(1, 1.0f, 1.4f);
        renderCupboard();
        glPopMatrix();

        glPushMatrix();
        glTranslated(-5.4, 0, -5.5);
        renderFloorLamp();
        glPopMatrix();

        glPushMatrix();
        glTranslated(-5.4, 0.8, 1.8);
        renderTableLamp();
        glPopMatrix();

        glPushMatrix();
        glTranslated(-4.8, 0, 3.5);
        renderPartitionBoard();
        glPopMatrix();

        glPushMatrix();
        glTranslated(-1, 0, -2.5);
        renderDesk();
        glPopMatrix();

        drawPhoto();
        glPopMatrix(); // Restore to initial state

        drawGameConsole(); // Call function to draw game console

        // Draw the man
        drawMan();

        // Enable or disable Ferris wheel lights based on user input
        if (ferrisWheelLightsOn) {
            // Enable Ferris wheel lights
            for (int i = 0; i < wheelLights.size(); ++i)
            {
                int lightID = GL_LIGHT1 + i; // Starting from GL_LIGHT1
                glEnable(lightID);
                glLightfv(lightID, GL_POSITION, wheelLights[i].position);
                glLightfv(lightID, GL_DIFFUSE, wheelLights[i].diffuse);
                glLightf(lightID, GL_CONSTANT_ATTENUATION, wheelLights[i].constantAttenuation);
                glLightf(lightID, GL_LINEAR_ATTENUATION, wheelLights[i].linearAttenuation);
                glLightf(lightID, GL_QUADRATIC_ATTENUATION, wheelLights[i].quadraticAttenuation);
            }
        }
        else {
            // Disable Ferris wheel lights
            for (int i = 0; i < wheelLights.size(); ++i)
            {
                int lightID = GL_LIGHT1 + i; // Starting from GL_LIGHT1
                glDisable(lightID);
            }
        }
        drawFerrisWheel();


        // Draw the sky
        glPushMatrix();
        {
            // Remove translation to center the sky sphere on the camera
            GLdouble modelview[16];
            glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
            modelview[12] = 0.0;
            modelview[13] = 0.0;
            modelview[14] = 0.0;
            glLoadMatrixd(modelview);

            // Enable depth testing but disable depth writing to ensure the sky is always in the background
            glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);
            glDepthMask(GL_FALSE);
            glDisable(GL_LIGHTING);

            if (isFadingSky) {
                // Enable blending
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                // Draw clear sky with gradually decreasing opacity
                glColor4f(1.0f, 1.0f, 1.0f, 1.0f - skyFadeProgress);
                glBindTexture(GL_TEXTURE_2D, texVec[24]); // Clear sky texture
                drawSkySphere(500.0f);

                // Draw cloudy sky with gradually increasing opacity
                glColor4f(1.0f, 1.0f, 1.0f, skyFadeProgress);
                glBindTexture(GL_TEXTURE_2D, texVec[25]); // Cloudy sky texture
                drawSkySphere(500.0f);

                // Disable blending
                glDisable(GL_BLEND);
            }
            else {
                // Draw the currently selected sky texture
                if (useCloudySky) {
                    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
                    glBindTexture(GL_TEXTURE_2D, texVec[25]); // Cloudy sky texture
                }
                else {
                    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
                    glBindTexture(GL_TEXTURE_2D, texVec[24]); // Clear sky texture
                }
                drawSkySphere(500.0f);
            }

            // Restore depth writing and lighting
            glEnable(GL_LIGHTING);
            glDepthMask(GL_TRUE);
            glPopAttrib();
        }
        glPopMatrix();

        glPopMatrix(); // Restore to initial state
        // Draw exterior scene
        drawExterior();

        if (isRaining) {
            drawRainParticles();
            if (isLightningActive) {

                GLfloat lightningAmbient[] = { sceneBrightness, sceneBrightness, sceneBrightness, 1.0f };
                GLfloat lightningDiffuse[] = { sceneBrightness, sceneBrightness, sceneBrightness, 1.0f };

                glMaterialfv(GL_FRONT, GL_AMBIENT, lightningAmbient);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, lightningDiffuse);


                drawLightningBolt();
                drawLightningFlash();
            }
            else {

                glMaterialfv(GL_FRONT, GL_AMBIENT, initialAmbient);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, initialDiffuse);
            }
        }


        drawFirstDropTower();
        drawSecondDropTower();
        flyingChair();
        drawTowerParticles();

        glutSwapBuffers();



    }
    else if (currentGameState == SNAKE_GAME) {
        // Render the full-screen Snake game interface
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 500.0f); // Set up perspective projection
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Call the display function for the Snake game
        snake_display();
    }
    drawLightningBolt();


}

// Handle window resizing
void reshapeCB(int w, int h) {
    screenWidth = w;
    screenHeight = h;
    glViewport(0, 0, (GLsizei)screenWidth, (GLsizei)screenHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (currentGameState == MAIN_SCENE) {
        gluPerspective(45.0f, (float)(screenWidth) / screenHeight, 0.1f, 1000.0f);
    }
    else if (currentGameState == SNAKE_GAME) {
        gluPerspective(45.0f, (float)(screenWidth) / screenHeight, 0.1f, 500.0f);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Timer callback function for updating the scene
void timerCB(int millisec)
{

    // Register the timer callback again
    glutTimerFunc(millisec, timerCB, millisec);

    // Calculate delta time in seconds

    float deltaTime = millisec / 1000.0f;
    updateRainParticles(deltaTime);

    // Update lightning bolt state
    updateLightningBolt(deltaTime);

    if (isFalling) {
        // Calculate the elapsed time since the fall started
        float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
        float elapsedTime = currentTime - fallStartTime;

        // Update shake intensity (e.g., linearly decreasing)
        const float MAX_SHAKE_INTENSITY = 0.15f; // Maximum shake intensity
        shakeIntensity = MAX_SHAKE_INTENSITY * (1.0f - (elapsedTime / FALL_DURATION));
        if (shakeIntensity < 0.0f) shakeIntensity = 0.0f;

        // Update position using parabolic trajectory
        manPositionX = fallInitialX + fallVelocityX * elapsedTime;
        manPositionZ = fallInitialZ + fallVelocityZ * elapsedTime;
        const float gravity = 9.8f; // Acceleration due to gravity
        manPositionY = fallInitialY + fallVelocityY * elapsedTime - 0.5f * gravity * elapsedTime * elapsedTime;

        // Update limb swinging for the falling animation
        updateLimbSwing();

        // Check if the fall has ended
        if (elapsedTime >= FALL_DURATION) {
            // Reset the man's position
            manPositionX = 0.0f;
            manPositionY = 0.0f;
            manPositionZ = 0.0f;
            manRotationAngle = 0.0f;

            // Reset falling variables
            isFalling = false;
            fallStartTime = 0.0f;

            // Reset limb angles
            limbSwing = 0.0f;
            limbSwingDirection = 1.0f;
            shoulderAngle = 0.0f;
            elbowAngle = 0.0f;
            hipAngle = 0.0f;
            kneeAngle = 0.0f;

            // Reset shake intensity
            shakeIntensity = 0.0f;
        }
    }
    else {
        // Update limb swinging normally when not falling
        updateLimbSwing();
    }

    if (isJumping) {
        // Calculate the elapsed time since the jump started
        float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
        float elapsedTime = currentTime - jumpStartTime;

        const float gravity = 9.8f;
        manPositionX = jumpStartX + jumpVelocityX * elapsedTime;
        manPositionZ = jumpStartZ + jumpVelocityZ * elapsedTime;
        manPositionY = jumpStartY + jumpVelocityY * elapsedTime - 0.5f * gravity * elapsedTime * elapsedTime;

        // Update limb movements during the jump
        updateJumpLimbSwing(elapsedTime);

        // Calculate total jump duration
        float t_up = jumpVelocityY / gravity;
        float jumpDuration = 2.0f * t_up;

        // Check if the jump has ended
        if (elapsedTime >= jumpDuration || manPositionY <= 0.0f)
        {
            manPositionY = 0.0f;
            isJumping = false;

            // Reset limb angles
            limbSwing = 0.0f;
            limbSwingDirection = 1.0f;
            shoulderAngle = 0.0f;
            elbowAngle = 0.0f;
            hipAngle = 0.0f;
            kneeAngle = 0.0f;
        }
    }

    // Smoothly animate door opening and closing
    if (doorOpen && doorAngle > -90.0f) {
        doorAngle -= 1.0f; // Adjust speed; smaller values result in slower rotation
        if (doorAngle < -90.0f) doorAngle = -90.0f;
    }
    else if (!doorOpen && doorAngle < 0.0f) {
        doorAngle += 1.0f;
        if (doorAngle > 0.0f) doorAngle = 0.0f;
    }

    if (isFadingSky)
    {
        // Calculate the elapsed time since the sky fade started
        float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
        float elapsedTime = currentTime - skyFadeStartTime;

        // Update sky fade progress
        skyFadeProgress = elapsedTime / skyFadeDuration;
        if (skyFadeProgress >= 1.0f)
        {
            skyFadeProgress = 1.0f;
            isFadingSky = false;
            useCloudySky = !useCloudySky;
            isRaining = useCloudySky; // Set raining state based on the new sky condition

            if (isRaining)
            {
                initRainParticles(); // Initialize rain particles

            }
            else
            {

                rainParticles.clear(); // Stop the rain
            }
        }
        glutPostRedisplay(); // Trigger a redraw
    }

    // Update the rotation angle
    alpha += 1.0f;  // Control the rotation speed
    if (alpha > 360.0f) {
        alpha -= 360.0f;  // Ensure the angle stays within a valid range (0   to 360  )
    }

    // Update the tilt angle based on the rotation angle
    tiltAngle = 15.0f + 5.0f * sin(alpha * M_PI / 180.0f);  // The tilt angle oscillates between [10  , 20  ]

    // Calculate deltaTime for tower updates
    float deltaTowerTime = millisec / 1000.0f;  // Convert milliseconds to seconds
    updateDropTower(deltaTowerTime);  // Update the drop tower state
    updateTowerLights(deltaTowerTime);  // Update the tower lights
    updateTowerParticles(deltaTowerTime);  // Update tower particles

    // Update the second drop tower
    updateSecondTowerLights(deltaTowerTime);  // Update the lights of the second tower
    updateSecondTowerParticles(deltaTowerTime);  // Update the particles of the second tower

    // Update the roller coaster train
    float deltaCoasterTime = millisec / 1000.0f;  // Convert milliseconds to seconds
    updateCoasterTrain(deltaCoasterTime);  // Update the roller coaster train state

    // Update arrow position for animation
    arrowPositionY += arrowDirection * 0.06f; // Adjust movement speed
    if (arrowPositionY > 1.2f) {
        arrowPositionY = 1.2f;
        arrowDirection = -0.9f;
    }
    else if (arrowPositionY < -0.5f) {
        arrowPositionY = -0.5f;
        arrowDirection = 1.2f;
    }

    glutPostRedisplay(); // Request a redraw of the scene
}

void keyboardCB(unsigned char key, int x, int y) {
    int mod = glutGetModifiers(); // Get current modifier key state

    if (key == 'H' || key == 'h') {
        spotlightsEnabled = !spotlightsEnabled; // Toggle spotlight status
    }
    if (key == 'O' || key == 'o') {
        cameraDistance -= 1.0f; // Zoom in
        if (cameraDistance < 5.0f) cameraDistance = 5.0f; // Minimum distance limit
        glutPostRedisplay(); // Trigger redraw
    }
    else if (key == 'P' || key == 'p') {
        cameraDistance += 1.0f; // Zoom out
        if (cameraDistance > 180.0f) cameraDistance = 180.0f; // Maximum distance limit
        glutPostRedisplay(); // Trigger redraw
    }

    if (key == ' ') {
        if (!isJumping && !isFalling) {
            isJumping = true;
            jumpStartTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

            // Save starting position
            jumpStartX = manPositionX;
            jumpStartY = manPositionY;
            jumpStartZ = manPositionZ;

            const float jumpSpeed = 5.0f; // Adjust jump speed
            jumpVelocityY = jumpSpeed;

            // Calculate horizontal velocities based on character's facing direction
            float angleRad = manRotationAngle * M_PI / 180.0f;
            jumpVelocityX = -jumpSpeed * sin(angleRad);
            jumpVelocityZ = -jumpSpeed * cos(angleRad);
        }
    }

    if (key == 'T' || key == 't') {
        if (currentGameState == MAIN_SCENE) {
            // Calculate the distance between the character and the circle center
            float dx = manPositionX + circleX;
            float dz = manPositionZ - circleZ;
            float distance = sqrt(dx * dx + dz * dz);

            printf("Man Position: (%f, %f)\n", manPositionX, manPositionZ);
            printf("Circle Position: (%f, %f)\n", circleX, circleZ);
            printf("Distance: %f\n", distance);

            if (distance <= 4.8f) {
                saveGLState();
                currentGameState = SNAKE_GAME;
                snake_init();
            }
            else {
                printf("Not within range.\n");
            }
        }
        else if (currentGameState == SNAKE_GAME) {
            restoreGLState();
            currentGameState = MAIN_SCENE;
            snake_game_over = false;
            snake_game_started = false;
        }
    }

    // Toggle Ferris wheel lights when 'y' is pressed
    if (key == 'Y' || key == 'y') {
        ferrisWheelLightsOn = !ferrisWheelLightsOn; // Toggle the Ferris wheel lights
        glutPostRedisplay(); // Request a redraw
    }

    // Add logic to restart the Snake game when 'R' is pressed and the game is over
    if (key == 'R' || key == 'r') {
        if (currentGameState == SNAKE_GAME && snake_game_over) {
            snake_restart_game();
            snake_game_over = false;
            snake_game_started = true;
            glutPostRedisplay(); // Request a redraw
        }
    }



    if (currentGameState == MAIN_SCENE) {
        float moveSpeed = 0.3f; // Default movement speed
        float rotationSpeed = 5.0f; // Rotation speed

        if (isFalling) {
            // Disable user control during falling
            return;
        }

        if (mod & GLUT_ACTIVE_SHIFT) {
            isRunning = true;
            moveSpeed *= 2.0f; // Running speed is twice the normal speed
        }
        else {
            isRunning = false;
        }

        switch (key)
        {
        case 27: // ESC key
            exit(0);
            break;
        case 'w': // Move forward
        case 'W':
            isWalking = true;
            manPositionX -= moveSpeed * sin(manRotationAngle * M_PI / 180.0f);
            manPositionZ -= moveSpeed * cos(manRotationAngle * M_PI / 180.0f);
            break;
        case 's': // Move backward
        case 'S':
            isWalking = true;
            manPositionX += moveSpeed * sin(manRotationAngle * M_PI / 180.0f);
            manPositionZ += moveSpeed * cos(manRotationAngle * M_PI / 180.0f);
            break;
        case 'a': // Turn left
        case 'A':
            manRotationAngle += rotationSpeed;
            cameraAngleY += rotationSpeed; // Synchronize camera update
            break;
        case 'd': // Turn right
        case 'D':
            manRotationAngle -= rotationSpeed;
            cameraAngleY -= rotationSpeed; // Synchronize camera update
            break;
        case 'f': // Toggle texture
        case 'F':
            isAlternateTexture = !isAlternateTexture; // Toggle texture state
            break;
        case 'B':
        case 'b':
            if (!isFadingSky) {
                isFadingSky = true;
                skyFadeProgress = 0.0f;
                skyFadeStartTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; // Record transition start time
                printf("B key pressed: Initiating sky fade.\n");
                glutPostRedisplay();
            }
            break;
        case 'm':
        case 'M':
            doorOpen = !doorOpen;
            // Let door angle change gradually in timerCB
            break;
        case 'x':
        case 'X': {
            // Calculate the horizontal distance between the character and the chair
            float dx = manPositionX - chairCurrentX;
            float dz = manPositionZ - chairCurrentZ;
            float distance = sqrt(dx * dx + dz * dz) - 13.0f;

            // Check if the character is within detection range
            if (distance <= detectionRadius) {
                isRidingChair = !isRidingChair; // Toggle perspective
                if (isRidingChair) {
                    printf("Entering riding chair perspective\n");
                    // May need to lock the character's position or perform other actions
                }
                else {
                    printf("Exiting riding chair perspective\n");
                    // Restore character's position or perform other actions
                }
                glutPostRedisplay(); // Trigger redraw
            }
            else {
                printf("Character is not within the chair's range and cannot ride.\n");
            }
            break;
        }
        }

        // Check if the character has walked out of the grass area
        if (manPositionX < GRASS_MIN_X || manPositionX > GRASS_MAX_X ||
            manPositionZ < GRASS_MIN_Z || manPositionZ > GRASS_MAX_Z) {
            // Initiate falling sequence
            isFalling = true;
            fallStartTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

            // Record initial position
            fallInitialX = manPositionX;
            fallInitialY = manPositionY;
            fallInitialZ = manPositionZ;

            // Set initial velocity
            fallVelocityX = 0.0f;
            fallVelocityZ = 0.0f;
            fallVelocityY = 5.0f; // Initial upward speed

            isWalking = false; // Stop walking animation

            // Set initial shake intensity
            shakeIntensity = 0.15f; // Adjust this value as needed to control maximum shake effect
        }

        glutPostRedisplay(); // Trigger redraw
    }
}

void special_callback(int key, int x, int y) {
    if (currentGameState == SNAKE_GAME) {
        if (snake_game_over) return;
        switch (key) {
        case GLUT_KEY_UP:
            if (snake_dir != UP) snake_dir = DOWN;
            break;
        case GLUT_KEY_DOWN:
            if (snake_dir != DOWN) snake_dir = UP;
            break;
        case GLUT_KEY_LEFT:
            if (snake_dir != RIGHT) snake_dir = LEFT;
            break;
        case GLUT_KEY_RIGHT:
            if (snake_dir != LEFT) snake_dir = RIGHT;
            break;
        default:
            break;
        }
    }
}

void keyboardUpCB(unsigned char key, int x, int y)
{
    // Callback function for key release events
    switch (key)
    {
    case 'w':
    case 'W':
    case 's':
    case 'S':
        isWalking = false; // Stop walking when 'W' or 'S' keys are released
        break;
    default:
        break;
    }
}

void mouseCB(int button, int state, int x, int y)
{
    // Callback function for mouse button events
    mouseX = x; // Update mouse X position
    mouseY = y; // Update mouse Y position

    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            mouseLeftDown = true; // Left mouse button pressed
        }
        else if (state == GLUT_UP)
        {
            mouseLeftDown = false; // Left mouse button released
        }
    }
    else if (button == GLUT_RIGHT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            mouseRightDown = true; // Right mouse button pressed
        }
        else if (state == GLUT_UP)
        {
            mouseRightDown = false; // Right mouse button released
        }
    }
}

void mouseMotionCB(int x, int y)
{
    // Callback function for mouse motion events
    if (mouseLeftDown)
    {
        // Adjust camera angles based on mouse movement while left button is held down
        cameraAngleY += (x - mouseX) * 0.2f; // Adjust horizontal camera angle
        cameraAngleX += (y - mouseY) * 0.2f; // Adjust vertical camera angle

        // Limit vertical angle to prevent flipping
        if (cameraAngleX > 89.0f) cameraAngleX = 89.0f;
        if (cameraAngleX < -89.0f) cameraAngleX = -89.0f;

        mouseX = x; // Update mouse X position
        mouseY = y; // Update mouse Y position
    }
    if (mouseRightDown)
    {
        // Adjust camera distance based on vertical mouse movement while right button is held down
        cameraDistance -= (y - mouseY) * 0.2f;
        if (cameraDistance < 5.0f)
            cameraDistance = 5.0f; // Prevent over zoom in
        mouseY = y; // Update mouse Y position
    }
}

void initGL()
{
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // Set texture environment mode to modulate

    glEnable(GL_DEPTH_TEST);    // Enable depth testing
    glEnable(GL_LIGHTING);      // Enable lighting
    glEnable(GL_NORMALIZE);     // Enable automatic normalization of normals
    glEnable(GL_TEXTURE_2D);    // Enable 2D texturing

    glClearColor(173 / 255.f, 216 / 255.f, 230 / 255.f, 1.f); // Set background color (sky blue)
    glClearStencil(0);                          // Clear stencil buffer

    GLfloat lightKa[] = { .2f, .2f, .2f, 1.0f };  // Ambient light component
    GLfloat lightKd[] = { 0.7, 0.7, 0.7, 1.0f };  // Diffuse light component
    GLfloat lightKs[] = { 0.8, 0.8, 0.8, 1 };     // Specular light component
    GLfloat lightPos[4] = { 4, 1, 0, 1 };         // Light position
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);    // Set ambient light for light 0
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);    // Set diffuse light for light 0
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);   // Set specular light for light 0
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);  // Set position for light 0

    glEnable(GL_LIGHT0);                        // Enable light source 0
    glEnable(GL_LIGHTING);                      // Enable lighting

    // Set material properties
    float ambient[] = { 0.2, 0.2, 0.2, 1 };
    float diffuse[] = { 0.7, 0.7, 0.7, 1 };
    float specular[] = { 1.0f, 1.0f, 1.0f, 1 };
    float shininess = 32;
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);      // Set ambient material property
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);      // Set diffuse material property
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);    // Set specular material property
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);   // Set shininess
    glEnable(GL_COLOR_MATERIAL);                      // Enable color tracking
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); // Specify which material parameters track the current color

    // Initialize random seed
    srand(static_cast<unsigned int>(time(0)));
    initTrees();                 // Initialize trees in the scene
    initFerrisWheelLights();     // Initialize Ferris wheel lights

    // Load textures
    texVec[0] = loadGLTexture("Texture\\newfloor.bmp");          // Load new floor texture
    texVec[1] = loadGLTexture("texture/wallpaper.bmp");         // Load wallpaper texture
    texVec[3] = loadGLTexture("texture/sofa.bmp");              // Load sofa texture
    texVec[4] = loadGLTexture("texture/wood.bmp");              // Load wood texture
    texVec[5] = loadGLTexture("texture/ground.bmp");            // Load ground texture
    texVec[6] = loadGLTexture("texture/StarryNight.bmp");       // Load picture texture
    texVec[9] = loadGLTexture("texture/newgrass.bmp");          // Load new grass texture
    texVec[10] = loadGLTexture("texture/roof.bmp");             // Load roof texture
    texVec[11] = loadGLTexture("texture/bark.bmp");             // Load bark texture
    texVec[12] = loadGLTexture("texture/head.bmp");             // Load head texture
    texVec[13] = loadGLTexture("texture/Jersey.bmp");           // Load body front texture
    texVec[14] = loadGLTexture("texture/james.bmp");            // Load body back texture
    texVec[15] = loadGLTexture("texture/sideclothe.bmp");       // Load body side texture

    texVec[17] = loadGLTexture("texture/arm.bmp");              // Load arm texture
    texVec[18] = loadGLTexture("texture/leg.bmp");              // Load leg texture
    texVec[19] = loadGLTexture("texture/gameconsole.bmp");      // Load game console texture
    texVec[20] = loadGLTexture("texture/yaomingbei.bmp");       // Load Yaomingbei texture
    texVec[21] = loadGLTexture("texture/yaomingzheng.bmp");     // Load Yaomingzheng texture
    texVec[22] = loadGLTexture("texture/door.bmp");             // Load door texture
    texVec[23] = loadGLTexture("texture/yaomingside.bmp");      // Load Yaomingside texture
    texVec[24] = loadGLTexture("texture/skyy.bmp");             // Load sky texture
    texVec[25] = loadGLTexture("texture/rainysky.bmp");         // Load rainy sky texture
    texVec[26] = loadGLTexture("texture/metal.bmp");            // Load metal texture
    texVec[27] = loadGLTexture("texture/skyy.bmp");             // Load cockpit texture

    texVec[29] = loadGLTexture("texture/xjtlu.bmp");            // Load XJTLU texture
    texVec[30] = loadGLTexture("texture/marble.bmp");
    texVec[31] = loadGLTexture("texture/fabric.bmp");
    texVec[32] = loadGLTexture("texture/column.bmp");
    texVec[33] = loadGLTexture("texture/disk.bmp");

    //texVec[34] = loadGLTexture("texture/tower_base.bmp");
    //texVec[35] = loadGLTexture("texture/tower_frame.bmp");
    //texVec[36] = loadGLTexture("texture/tower_cabin.bmp");
    //texVec[37] = loadGLTexture("texture/tower_rail.bmp");

    texVec[40] = loadGLTexture("texture/train.bmp");
    //texVec[41] = loadGLTexture("texture/coaster/seat.bmp");  
    //texVec[42] = loadGLTexture("texture/coaster/metal.bmp"); 
    texVec[43] = loadGLTexture("texture/track.bmp");
    //texVec[44] = loadGLTexture("texture/coaster/support.bmp");
    //texVec[45] = loadGLTexture("texture/coaster/safety.bmp");  
    currentTrain.orientation = 0.0f;
    currentTrain.speed = MIN_SPEED;

    initializeTrackPoints();


    initTowerLights();
    initSecondTowerLights();
    srand(time(NULL));
}

int main(int argc, char** argv) {
    glutInit(&argc, argv); // Initialize GLUT

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH); // Set display mode with RGBA, double buffering, and depth buffer
    glutInitWindowSize(screenWidth, screenHeight); // Set initial window size
    glutInitWindowPosition(100, 100);              // Set initial window position

    glutCreateWindow("OpenGL House Scene");        // Create window with title

    initGL();             // Initialize OpenGL settings
    initClouds();        // Initialize clouds in the scene

    // Register callback functions
    glutDisplayFunc(displayCB);                // Register display callback
    glutReshapeFunc(reshapeCB);                // Register reshape callback
    glutKeyboardFunc(keyboardCB);              // Register keyboard event callback
    glutSpecialFunc(special_callback);         // Register special key event callback
    glutKeyboardUpFunc(keyboardUpCB);          // Register keyboard release event callback
    glutTimerFunc(33, timerCB, 33);            // Register timer callback with 33ms interval
    glutMouseFunc(mouseCB);                    // Register mouse button event callback
    glutMotionFunc(mouseMotionCB);             // Register mouse motion event callback

    glutMainLoop(); // Enter GLUT event processing loop

    return 0;
}