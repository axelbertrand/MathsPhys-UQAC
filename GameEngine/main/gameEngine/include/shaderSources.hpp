#pragma once

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aCenter;\n"
"out vec3 circleCenter;\n"
"void main()\n"
"{\n"
"   float x = 2.0*(aPos.x/800.0)-1;\n"
"   float y = 2.0*(aPos.y/600.0)-1;\n"
"   float z = aPos.z;\n"
"   gl_Position = vec4(x, y, z, 1.0);\n"
"   circleCenter = aCenter;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 circleCenter;\n"
"uniform float circleRadius;\n"
"void main()\n"
"{\n"
"   vec2 fragCoord = gl_FragCoord.xy;"
"	float squareDistFromCenter = pow(fragCoord.x - circleCenter.x, 2) + pow(fragCoord.y - circleCenter.y, 2);\n"
"   if (squareDistFromCenter > circleRadius)\n"
"   {\n"
"       discard;\n"
"   }\n"
"   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
"}\n\0";