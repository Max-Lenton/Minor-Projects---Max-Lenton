#include <windows.h>  // Must be first on Windows
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

// Terrain settings
const int TERRAIN_WIDTH = 100;
const int TERRAIN_HEIGHT = 100;
const float TERRAIN_SCALE = 2.0f;
const float HEIGHT_SCALE = 10.0f;

// Camera settings
float cameraX = 50.0f;
float cameraY = 20.0f;
float cameraZ = 50.0f;
float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;

// Mouse control
int mouseX = 0, mouseY = 0;
bool mouseLeftPressed = false;

// Structure for 3D vertex
struct Vertex3D {
    float x, y, z;
    float nx, ny, nz; // Normal vector
    Vertex3D(float x = 0, float y = 0, float z = 0, float nx = 0, float ny = 1, float nz = 0)
        : x(x), y(y), z(z), nx(nx), ny(ny), nz(nz) {}
};

// Terrain data
std::vector<std::vector<float>> heightMap;
std::vector<std::vector<Vertex3D>> terrainVertices;

// Generate random height values for terrain
void generateHeightMap() {
    heightMap.resize(TERRAIN_HEIGHT);
    terrainVertices.resize(TERRAIN_HEIGHT);
    
    for (int i = 0; i < TERRAIN_HEIGHT; i++) {
        heightMap[i].resize(TERRAIN_WIDTH);
        terrainVertices[i].resize(TERRAIN_WIDTH);
        
        for (int j = 0; j < TERRAIN_WIDTH; j++) {
            // Simple noise-based height generation
            float height = 0.0f;
            float amplitude = HEIGHT_SCALE;
            float frequency = 0.05f;
            
            // Multiple octaves of noise for more interesting terrain
            for (int octave = 0; octave < 4; octave++) {
                height += amplitude * sin(i * frequency) * cos(j * frequency);
                amplitude *= 0.5f;
                frequency *= 2.0f;
            }
            
            // Add some randomness
            height += (rand() % 100 - 50) / 100.0f * HEIGHT_SCALE * 0.3f;
            
            heightMap[i][j] = height;
        }
    }
}

// Calculate normals for smooth lighting
void calculateNormals() {
    for (int i = 1; i < TERRAIN_HEIGHT - 1; i++) {
        for (int j = 1; j < TERRAIN_WIDTH - 1; j++) {
            // Get neighboring heights
            float hL = heightMap[i][j-1];     // Left
            float hR = heightMap[i][j+1];     // Right
            float hD = heightMap[i-1][j];     // Down
            float hU = heightMap[i+1][j];     // Up
            
            // Calculate normal using cross product
            float nx = (hL - hR) / (2.0f * TERRAIN_SCALE);
            float ny = 2.0f;
            float nz = (hD - hU) / (2.0f * TERRAIN_SCALE);
            
            // Normalize
            float length = sqrt(nx*nx + ny*ny + nz*nz);
            if (length > 0) {
                nx /= length;
                ny /= length; 
                nz /= length;
            }
            
            terrainVertices[i][j] = Vertex3D(
                j * TERRAIN_SCALE, 
                heightMap[i][j], 
                i * TERRAIN_SCALE, 
                nx, ny, nz
            );
        }
    }
}

// Get color based on height (terrain coloring)
void getTerrainColor(float height, float& r, float& g, float& b) {
    height = height / HEIGHT_SCALE + 0.5f; // Normalize to 0-1
    
    if (height < 0.2f) {
        // Water/deep areas - blue
        r = 0.2f; g = 0.4f; b = 0.8f;
    } else if (height < 0.4f) {
        // Sand/beach - yellow-brown
        r = 0.8f; g = 0.7f; b = 0.4f;
    } else if (height < 0.7f) {
        // Grass/forest - green
        r = 0.2f; g = 0.6f; b = 0.2f;
    } else if (height < 0.9f) {
        // Rock/mountain - gray-brown
        r = 0.5f; g = 0.4f; b = 0.3f;
    } else {
        // Snow-capped peaks - white
        r = 0.9f; g = 0.9f; b = 0.9f;
    }
}

// Initialize OpenGL
void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    // Set light position
    GLfloat lightPosition[] = {50.0f, 100.0f, 50.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    
    // Set clear color to sky blue
    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
    
    // Generate terrain
    srand(time(NULL));
    generateHeightMap();
    calculateNormals();
}

// Render the terrain
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    
    // Set camera
    glRotatef(cameraAngleX, 1.0f, 0.0f, 0.0f);
    glRotatef(cameraAngleY, 0.0f, 1.0f, 0.0f);
    glTranslatef(-cameraX, -cameraY, -cameraZ);
    
    // Render terrain as triangles
    glBegin(GL_TRIANGLES);
    
    for (int i = 0; i < TERRAIN_HEIGHT - 1; i++) {
        for (int j = 0; j < TERRAIN_WIDTH - 1; j++) {
            // Get the four corner points of the quad
            float x1 = j * TERRAIN_SCALE;
            float z1 = i * TERRAIN_SCALE;
            float y1 = heightMap[i][j];
            
            float x2 = (j + 1) * TERRAIN_SCALE;
            float z2 = i * TERRAIN_SCALE;
            float y2 = heightMap[i][j + 1];
            
            float x3 = j * TERRAIN_SCALE;
            float z3 = (i + 1) * TERRAIN_SCALE;
            float y3 = heightMap[i + 1][j];
            
            float x4 = (j + 1) * TERRAIN_SCALE;
            float z4 = (i + 1) * TERRAIN_SCALE;
            float y4 = heightMap[i + 1][j + 1];
            
            // Set colors based on height
            float r, g, b;
            
            // First triangle
            getTerrainColor(y1, r, g, b);
            glColor3f(r, g, b);
            glVertex3f(x1, y1, z1);
            
            getTerrainColor(y2, r, g, b);
            glColor3f(r, g, b);
            glVertex3f(x2, y2, z2);
            
            getTerrainColor(y3, r, g, b);
            glColor3f(r, g, b);
            glVertex3f(x3, y3, z3);
            
            // Second triangle
            getTerrainColor(y2, r, g, b);
            glColor3f(r, g, b);
            glVertex3f(x2, y2, z2);
            
            getTerrainColor(y4, r, g, b);
            glColor3f(r, g, b);
            glVertex3f(x4, y4, z4);
            
            getTerrainColor(y3, r, g, b);
            glColor3f(r, g, b);
            glVertex3f(x3, y3, z3);
        }
    }
    
    glEnd();
    
    glutSwapBuffers();
}

// Handle window reshape
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

// Handle keyboard input
void keyboard(unsigned char key, int x, int y) {
    const float moveSpeed = 2.0f;
    
    switch (key) {
        case 'w': case 'W':
            cameraZ -= moveSpeed * cos(cameraAngleY * M_PI / 180.0f);
            cameraX -= moveSpeed * sin(cameraAngleY * M_PI / 180.0f);
            break;
        case 's': case 'S':
            cameraZ += moveSpeed * cos(cameraAngleY * M_PI / 180.0f);
            cameraX += moveSpeed * sin(cameraAngleY * M_PI / 180.0f);
            break;
        case 'a': case 'A':
            cameraX -= moveSpeed * cos(cameraAngleY * M_PI / 180.0f);
            cameraZ += moveSpeed * sin(cameraAngleY * M_PI / 180.0f);
            break;
        case 'd': case 'D':
            cameraX += moveSpeed * cos(cameraAngleY * M_PI / 180.0f);
            cameraZ -= moveSpeed * sin(cameraAngleY * M_PI / 180.0f);
            break;
        case 'q': case 'Q':
            cameraY += moveSpeed;
            break;
        case 'e': case 'E':
            cameraY -= moveSpeed;
            break;
        case 27: // ESC
            exit(0);
            break;
    }
    
    glutPostRedisplay();
}

// Handle mouse clicks
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        mouseLeftPressed = (state == GLUT_DOWN);
        mouseX = x;
        mouseY = y;
    }
}

// Handle mouse motion
void mouseMotion(int x, int y) {
    if (mouseLeftPressed) {
        // Calculate mouse movement
        int deltaX = x - mouseX;
        int deltaY = y - mouseY;
        
        // Update camera angles
        cameraAngleY += deltaX * 0.5f;
        cameraAngleX += deltaY * 0.5f;
        
        // Limit vertical rotation
        if (cameraAngleX > 90.0f) cameraAngleX = 90.0f;
        if (cameraAngleX < -90.0f) cameraAngleX = -90.0f;
        
        // Update mouse position
        mouseX = x;
        mouseY = y;
        
        glutPostRedisplay();
    }
}

// Idle function for continuous updates
void idle() {
    // You can add animations here if needed
    // For now, just request a redisplay
    glutPostRedisplay();
}

// Main function
int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Polygon Terrain Viewer");
    
    // Initialize OpenGL settings
    init();
    
    // Set callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutIdleFunc(idle);
    
    // Start the main loop
    glutMainLoop();
    return 0;
}