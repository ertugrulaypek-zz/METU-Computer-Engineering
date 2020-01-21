#include "helper.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtx/rotate_vector.hpp"

//#include "glm/gtc/type_ptr.hpp"

    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;

static GLFWwindow * win = NULL;

// Shaders
GLuint idProgramShader;
GLuint idFragmentShader;
GLuint idVertexShader;
GLuint idJpegTexture;
GLuint idHeightMapTexture;
GLuint idMVPMatrix;

int widthTexture, heightTexture;
int lightYIndex = 100;

int textureMappingOffset = 0;

glm::vec3 cameraPosition;
glm::vec3 lightPosition;
glm::vec3 cameraUpVector = glm::vec3(0.0, 1.0, 0.0);
glm::vec3 cameraGazeVector = glm::vec3(0.0, 0.0, 1.0);
glm::vec3 cameraUVector = cross(cameraUpVector, cameraGazeVector);

GLfloat cameraSpeed = 0.0f;

bool isFullscreen = false;

int widthDisplay = 1000;
int heightDisplay = 1000;

int currentWidth, currentHeight;


GLFWmonitor* primary_monitor;
const GLFWvidmode* vidmode;

GLfloat heightFactor = 10.0f;

int vertexCount;
glm::vec3* vertices;

// PART2 variables
int radius = 350;
int horizontalPieces = 250;
int verticalPieces = 125;

float verticalStep = PI / verticalPieces;
float horizontalStep = 2*PI / horizontalPieces;
float horizontalAngle, verticalAngle;
float x, y, z, xy; 


// if setOrUpdate == 1 set
// if setOrUpdate == 0 update
void setUpdateCPU(int setOrUpdate){
    if(!setOrUpdate){
        cameraPosition += cameraGazeVector * cameraSpeed;
    }else{
        cameraPosition = glm::vec3(widthTexture/2, widthTexture/10, -widthTexture/4);
        lightPosition = glm::vec3(widthTexture/2, lightYIndex, heightTexture/2);
    }
    glm::vec3 center = cameraPosition + cameraGazeVector * (GLfloat)0.1;
    glm::mat4 M_view = glm::lookAt(cameraPosition, center, cameraUpVector);
    GLint CPUMV = glGetUniformLocation(idProgramShader, "MV");
    glUniformMatrix4fv(CPUMV, 1, GL_FALSE, &M_view[0][0]);


    glm::mat4 M_projection = glm::perspective((GLfloat)45, (GLfloat)1, (GLfloat)0.1, (GLfloat)1000);    
    glm::mat4 M_model = glm::mat4(1.0f);
    glm::mat4 M_model_view_projection = M_projection * M_view * M_model;
    GLint CPUMVP = glGetUniformLocation(idProgramShader, "MVP");
    glUniformMatrix4fv(CPUMVP, 1, GL_FALSE, &M_model_view_projection[0][0]);

    glm::mat4 M_normal = glm::inverseTranspose(M_view);
    GLint CPUNormal = glGetUniformLocation(idProgramShader, "M_norm");
    glUniformMatrix4fv(CPUNormal, 1, GL_FALSE, &M_normal[0][0]);

    GLint CPUcameraPosition = glGetUniformLocation(idProgramShader, "cameraPosition");
    glUniform3fv(CPUcameraPosition, 1, &cameraPosition[0]);

    GLint CPUlightPosition = glGetUniformLocation(idProgramShader, "lightPosition");
    glUniform3fv(CPUlightPosition, 1, &lightPosition[0]);
}

void initVerticesForOnePx(int i, int j, int& index){
    glm::vec3 vertex0, vertex1, vertex2, vertex3;
    vertex0 = glm::vec3(i, 0, j);
    vertices[index++] = vertex0;

    vertex1 = glm::vec3(i+1, 0, j+1);
    vertices[index++] = vertex1;

    vertex2 = glm::vec3(i+1, 0, j);
    vertices[index++] = vertex2;

    vertex3 = glm::vec3(i, 0, j+1);
    vertices[index++] = vertex3;

    vertices[index++] = vertex0;
    vertices[index++] = vertex1;
}

void createMeshes(){
    int idx=0;
    for(int i=0; i<verticalPieces; i++){
        verticalAngle = PI / 2 - i*verticalStep;
        xy = radius * cosf(verticalAngle);             // r * cos(u)
        z = radius * sinf(verticalAngle);

        for(int j=0; j<horizontalPieces; j++){
            horizontalAngle = j * horizontalStep;
            x = xy * cosf(horizontalAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(horizontalAngle); 
            vertices[idx++] = vec3(x,y,z);

            
        }
    
    }
}


void setCPU(){
    GLint CPUrgbTexture = glGetUniformLocation(idProgramShader, "rgbTexture");
    glUniform1i(CPUrgbTexture, 0);

    GLint CPUHeightMap = glGetUniformLocation(idProgramShader, "heightMapTexture");
    glUniform1i(CPUHeightMap, 1);

    GLint CPUheightFactor = glGetUniformLocation(idProgramShader, "heightFactor");
    glUniform1f(CPUheightFactor, heightFactor);

    GLint CPUwidthTexture = glGetUniformLocation(idProgramShader, "widthTexture");
    glUniform1i(CPUwidthTexture, widthTexture);

    GLint CPUheightTexture = glGetUniformLocation(idProgramShader, "heightTexture");
    glUniform1i(CPUheightTexture, heightTexture);

    GLint CPUtextureMappingOffset = glGetUniformLocation(idProgramShader, "textureMappingOffset");
    glUniform1i(CPUtextureMappingOffset, textureMappingOffset);
}


// Window resize callback
void resize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height );
}


static void keyPressCallback(GLFWwindow *win, int key, int scode, int action, int mods){
    if(action == GLFW_PRESS && GLFW_KEY_ESCAPE == key){
        glfwSetWindowShouldClose(win, GLFW_TRUE);
    }
    
    if(action == GLFW_PRESS && key == GLFW_KEY_X){
        cameraSpeed = 0.0f;
    }

    else if(action == GLFW_PRESS && key == GLFW_KEY_I){
        setUpdateCPU(1);
        cameraSpeed = 0.0f;
        cameraUpVector = glm::vec3(0.0, 1.0, 0.0);
        cameraGazeVector = glm::vec3(0.0, 0.0, 1.0);
        cameraUVector = cross(cameraUpVector, cameraGazeVector);
    }
    else if (key == GLFW_KEY_T && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        lightYIndex += 5;
        lightPosition = glm::vec3(widthTexture/2, lightYIndex, heightTexture/2);
        GLint CPUlightPosition = glGetUniformLocation(idProgramShader, "lightPosition");
        glUniform3fv(CPUlightPosition, 1, &lightPosition[0]);
    }
    else if (key == GLFW_KEY_G && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        lightYIndex -= 5;
        lightPosition = glm::vec3(widthTexture/2, lightYIndex, heightTexture/2);
        GLint CPUlightPosition = glGetUniformLocation(idProgramShader, "lightPosition");
        glUniform3fv(CPUlightPosition, 1, &lightPosition[0]);
    }

    else if (key == GLFW_KEY_R && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        heightFactor += 0.5;
        GLint CPUheightFactor = glGetUniformLocation(idProgramShader, "heightFactor");
        glUniform1f(CPUheightFactor, heightFactor);
    }
    else if (key == GLFW_KEY_F && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        heightFactor -= 0.5;
        GLint CPUheightFactor = glGetUniformLocation(idProgramShader, "heightFactor");
        glUniform1f(CPUheightFactor, heightFactor);
    }
    else if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        cameraUpVector = rotate(cameraUpVector, -0.05f, cameraUVector);
        cameraGazeVector = rotate(cameraGazeVector, -0.05f, cameraUVector);       
    }
    else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        cameraUpVector = rotate(cameraUpVector, 0.05f, cameraUVector);
        cameraGazeVector = rotate(cameraGazeVector, 0.05f, cameraUVector);
    }
    else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        cameraUVector = rotate(cameraUVector, 0.05f, cameraUpVector);
        cameraGazeVector = rotate(cameraGazeVector, 0.05f, cameraUpVector);
    }
    else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        cameraUVector = rotate(cameraUVector, -0.05f, cameraUpVector);
        cameraGazeVector = rotate(cameraGazeVector, -0.05f, cameraUpVector);   
    }
    else if (key == GLFW_KEY_Y && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        cameraSpeed += 0.01;
    }
    else if (key == GLFW_KEY_H && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        cameraSpeed -= 0.01;
    }
    else if (key == GLFW_KEY_P && action == GLFW_PRESS){
        if (isFullscreen){
            glfwSetWindowMonitor(win, NULL, currentWidth, currentHeight, 1000, 1000, GL_DONT_CARE);
            glViewport(0, 0, 1000, 1000);
            isFullscreen = false;
        }
        else{
            glfwGetWindowPos(win, &currentWidth, &currentHeight);
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor (win, monitor, 0, 0, videoMode->width, videoMode->height, GL_DONT_CARE);
            glViewport(0, 0, videoMode->width, videoMode->height);
            isFullscreen = true;
        }
    }
    else if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        textureMappingOffset -= 1;  
    }
    else if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        textureMappingOffset += 1;
    }
}


static void errorCallback(int error,
  const char * description) {
  fprintf(stderr, "Error: %s\n", description);
}

int main(int argc, char * argv[]) {

    if (argc != 3) {
        printf("Only one texture image expected!\n");
        exit(-1);
    }

    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) {
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    win = glfwCreateWindow(1000, 1000, "CENG477 - HW3", NULL, NULL);

    if (!win) {
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(win);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

        glfwTerminate();
        exit(-1);
    }

    initShaders();
    glUseProgram(idProgramShader);
    initTexture(argv[2], & widthTexture, & heightTexture, 0);
    initTexture(argv[1], & widthTexture, & heightTexture, 1);

    /***************************************************************************/

    // Set the keyPressCallback function to respond user input given by the keyboard
    glfwSetKeyCallback(win, keyPressCallback);
    glfwSetWindowSizeCallback(win, resize);
    // Set camera specifications
    setUpdateCPU(1);
    // Set the viewport
    glViewport(0,0, widthDisplay, heightDisplay);
    
    // Create the meshes
    createMeshes();

    glEnable(GL_DEPTH_TEST);

    setCPU();

    primary_monitor = glfwGetPrimaryMonitor();
    vidmode = glfwGetVideoMode(primary_monitor);
    glfwGetWindowSize(win, &currentWidth, &currentHeight);


    while (!glfwWindowShouldClose(win)) {

        //Clear Color/Depth/Stencil buffers
        glClearStencil(0);
        glClearDepth(1.0f);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


        setUpdateCPU(0);

        //Render
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        glDisableClientState(GL_VERTEX_ARRAY);


        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    /***************************************************************************/
    glfwDestroyWindow(win);
    glfwTerminate();

    return 0;
}
