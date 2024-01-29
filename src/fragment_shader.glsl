#version 330 core
out vec4 FragColor;

#define ITERATIONS 100

uniform vec2 resolution;
uniform vec2 camera_position;
uniform float time;

void main() {


	vec2 c = 2.0 * gl_FragCoord.xy / resolution - 1.0 + camera_position;
	vec2 z = vec2(c.x, c.y);
	float k = 0.0;

	for(int i=0;i<ITERATIONS;i++) {
		float a = z.x * z.x - z.y * z.y;
		float b = 2.0 * z.x * z.y;
		z = vec2(a, b) + c;
		k = float(i);
		if(length(z) > 4) {
			break;
		}
	}	


	vec3 col = vec3(k * 0.01);

	FragColor = vec4(col, 1.0);
}
