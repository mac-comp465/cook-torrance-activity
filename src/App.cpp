#include "App.h"

#include <config/VRDataIndex.h>

using namespace basicgraphics;
using namespace std;
using namespace glm;

App::App(int argc, char** argv) : VRApp(argc, argv)
{
    turntable.reset(new TurntableManipulator());
    turntable->setCenterPosition(vec3(0,0,0));
}

App::~App()
{
	shutdown();
}

void App::onButtonDown(const VRButtonEvent &event) {
    turntable->onButtonDown(event);
}

void App::onButtonUp(const VRButtonEvent &event) {
    turntable->onButtonUp(event);
}

void App::onCursorMove(const VRCursorEvent &event) {
    turntable->onCursorMove(event);
}

void App::onRenderGraphicsContext(const VRGraphicsState &renderState) {
    // This routine is called once per graphics context at the start of the
    // rendering process.  So, this is the place to initialize textures,
    // load models, or do other operations that you only want to do once per
    // frame.
    
	// Is this the first frame that we are rendering after starting the app?
    if (renderState.isInitialRenderCall()) {

		//For windows, we need to initialize a few more things for it to recognize all of the
		// opengl calls.
		#ifndef __APPLE__
			glewExperimental = GL_TRUE;
			GLenum err = glewInit();
			if (GLEW_OK != err)
			{
				std::cout << "Error initializing GLEW." << std::endl;
			}
		#endif     


        glEnable(GL_DEPTH_TEST);
        glClearDepth(1.0f);
        glDepthFunc(GL_LEQUAL);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glEnable(GL_MULTISAMPLE);

		// This sets the background color that is used to clear the canvas
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		// This load shaders from disk, we do it once when the program starts up.
		reloadShaders();

        // This loads the dragon model from a file and initializes an instance of the model class to store it
        modelMesh.reset(new Model("dragon.obj", 1.0, vec4(1.0)));
        
        // Initialize the texture environment map
        // Order should be:
        // +X (right)
        // -X (left)
        // +Y (top)
        // -Y (bottom)
        // +Z (front)
        // -Z (back)
        string textureFiles[] = {"desert_evening_east.jpg", "desert_evening_west.jpg", "desert_evening_up.jpg", "desert_evening_down.jpg", "desert_evening_north.jpg", "desert_evening_south.jpg"};
        environmentMap = Texture::createCubeMapFromFiles(textureFiles, true, 4);
        environmentMap->setTexParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        environmentMap->setTexParameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        environmentMap->setTexParameteri(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        environmentMap->setTexParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        environmentMap->setTexParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        skyBox.reset(new Skybox(environmentMap));
        
        // set the light position to roughly match up with the sun position in the skybox texture
        float radius = 10.0;
        lightPosition = vec4(-1.7*radius, 0.3*radius, -1.0*radius, 1.0);
        drawLightVector = false;
    }
}

void App::onRenderGraphicsScene(const VRGraphicsState &renderState) {
    // This routine is called once per eye/camera.  This is the place to actually
    // draw the scene.
    
	// clear the canvas and other buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    // Setup the camera with a good initial position and view direction to see the table
    glm::mat4 view = turntable->frame();
    glm::mat4 model(1.0);
    
    // Setup the projection matrix so that things are rendered in perspective
    GLfloat windowHeight = renderState.index().getValue("FramebufferHeight");
    GLfloat windowWidth = renderState.index().getValue("FramebufferWidth");
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), windowWidth / windowHeight, 0.01f, 100.0f);
    
    
    // Tell opengl that we want to use the shader
    _shader.use();
    _shader.setUniform("view_mat", view);
    _shader.setUniform("projection_mat", projection);
    _shader.setUniform("model_mat", model);
    
    // Properties of the material the model is made out of (the "K" terms in the equations discussed in class)
    // These values should make the model look like it is made out of a metal, like brass
    vec3 ambientReflectionCoeff(0.7, 0.7, 0.7);
    vec3 diffuseReflectionCoeff(0.7, 0.7, 0.7);
    vec3 specularReflectionCoeff(0.9, 0.9, 0.9);
    float m = 0.55;
    float r0 = 0.7;
    
    // Properties of the light source (the "I" terms in the equations discussed in class)
    // These values are for a white light so the r,g,b intensities are all the same
    vec3 ambientLightIntensity(0.3, 0.3, 0.3);
    vec3 diffuseLightIntensity(0.6, 0.6, 0.6);
    vec3 specularLightIntensity(1.0, 1.0, 1.0);
    
    
    float glassR0 = 0.4;
    float eta = 0.67;
    vec3 glassEta(0.65, 0.67, 0.68);
    
    
    // Pass these parameters into your shader programs... in shader programs these are called "uniform variables"
    
    // TODO: Set shader material properties (k coeffs)
    
    
    
    
    
    
    
    
    // Cook-Torrance specific properties
    _shader.setUniform("r0", r0);
    _shader.setUniform("m", m);
    
    _shader.setUniform("glassR0", glassR0);
    _shader.setUniform("eta", eta);
    _shader.setUniform("glassEta", glassEta);
    
    // TODO: Set shader light properties (intensity and position)
    _shader.setUniform("lightPosition", lightPosition);
    
    
    
    
    
    
    
    
    
    
    //This sets the texture to associate with the cubeMap sampler in the shader which is bound to texture unit 1;
    environmentMap->bind(1);
    _shader.setUniform("environmentMap", 1);
    
    vec3 eyePosition = turntable->getPos();
    _shader.setUniform("eye_world", eyePosition);
    
    // Draw the model
    modelMesh->draw(_shader);
    
    // Draw the skybox. Should be the last thing to draw
    skyBox->draw(view, projection);
    

}

void App::reloadShaders()
{
    _shader.compileShader("vertex.vert", GLSLShader::VERTEX);
    _shader.compileShader("metal.frag", GLSLShader::FRAGMENT);
    _shader.link();
    _shader.use();
}
