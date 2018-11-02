#version 330

// Fragment shader


// These get passed in from the vertex shader and are interpolated (varying) properties,
// so they change for each pixel across the triangle:
in vec4 interpSurfPosition;
in vec3 interpSurfNormal;

// Light Position in world coordinates
uniform vec4 lightPosition;

//Textures
uniform samplerCube environmentMap;

// Material Properties
uniform float eta; // The eta value to use initially. This reflects all light wavelengths in the same direction
uniform vec3 glassEta; // Contains one eta for each color channel (use eta.r, eta.g, eta.b in your code)
uniform float glassR0; // The Fresnel reflectivity when the incident angle is 0

uniform vec3 eye_world;

// This is an out variable for the final color we want to render this fragment.
out vec4 fragColor;

void main() {

	// TODO: fill this in

    vec3 reflectedColor = vec3(0.0);
    
    
	// Tell OpenGL to use the mix of the refracted and reflected color based on the fresnel term.
    fragColor.rgb = reflectedColor; // change me

	// And, set the alpha component to 1.0 (completely opaque, no transparency).
	fragColor.a = 1.0;
}
