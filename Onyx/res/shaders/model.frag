

#version 330 core

out vec4 color;

in vec3 frag_pos;  
in vec3 frag_normal;  
  
uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec3 lightColour;
uniform vec3 objectColour;
uniform float fade;
uniform float wireframe;

void main()
{
	// Ambient
	float ambientStrength = 0.2f;

	vec3 ambient = ambientStrength * lightColour;
  	
	// Diffuse 
	vec3 norm = normalize(frag_normal);
	vec3 lightDir = normalize(lightPos - frag_pos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColour;
    
	// Specular
	float specularStrength = 0.9f;
	vec3 viewDir = normalize(viewPos - frag_pos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColour; 

	vec3 result = (ambient + diffuse + specular) * objectColour;

	float dist = distance(viewPos, frag_pos);
 
	float opacity = clamp(dist / 10.0f, 0.0f, 1.0f) * fade;

	color = vec4(result * wireframe, 1.0f - opacity);
} 