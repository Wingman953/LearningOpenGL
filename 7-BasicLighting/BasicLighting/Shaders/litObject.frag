#version 330

in vec3 normal; // The surface normal.
in vec3 objColour; // The colour of the object.
in vec3 lightCol; // The colour of the light.
in vec3 lightPos; // The position of the light in world space.
in vec3 fragPos; // World space location of the fragment.

// The final colour we will see at this location on screen.
out vec4 fragColour;

void main(void)
{
	// Ambient light.
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightCol;

	// Diffuse light.
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightCol;

    vec3 result = (ambient + diffuse) * objColour;
	
	fragColour = vec4(result, 1.0);
}
