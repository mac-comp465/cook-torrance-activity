#version 330

// Fragment shader


// These get passed in from the vertex shader and are interpolated (varying) properties,
// so they change for each pixel across the triangle:
in vec4 interpSurfPosition; // position on the surface in world coordinates
in vec3 interpSurfNormal; // normal in world coordinates

//TODO: add a uniform variable for the light position
uniform vec4 lightPosition; // in world coordinates

//Textures
uniform samplerCube environmentMap; // The cube map containing the environment to reflect


//Material Properties
uniform float r0; // The Fresnel reflectivity when the incident angle is 0
uniform float m; // The roughness of the material

// TODO: add other material properties variables





// TODO: Light Properties





// The camera location in world coordinates
uniform vec3 eye_world;

// This is an out variable for the final color we want to render this fragment.
out vec4 fragColor;

void main() {

    // Start with black and then add lighting to the final color as we calculate it
	vec3 materialColor = vec3(0.0, 0.0, 0.0);

	// TODO: Fill this in using to cook-torrance model
    
    
    
    
    

	// TODO: Tell OpenGL to use the r,g,b compenents of materialColor mixed with the reflected color for the color of this fragment (pixel).
    fragColor.rgb = materialColor; // Change me

	// And, set the alpha component to 1.0 (completely opaque, no transparency).
	fragColor.a = 1.0;
}
