#version 430 core

out vec3 FragColor;
in vec2 TexCoords;

uniform sampler2D  SceneBuffer;

void main(){
    vec3 color = texture(SceneBuffer,TexCoords).rgb;

    //ACES ToneMapping
    const float A = 2.51f;
	const float B = 0.03f;
	const float C = 2.43f;
	const float D = 0.59f;
	const float E = 0.14f;
	//color *= adapted_lum;
	color = (color * (A * color + B)) / (color * (C * color + D) + E);

    // gamma
	color = pow(color, vec3(1.0/2.2));

    FragColor = color;

	//FragColor = vec3(1);
}