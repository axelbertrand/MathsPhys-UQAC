#pragma once

const char* particleVertexShaderSource = "#version 330 core\n"
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

const char* particleFragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 circleCenter;\n"
"uniform float circleRadius;\n"
"void main()\n"
"{\n"
"   vec2 fragCoord = gl_FragCoord.xy;"
"	float squareDistFromCenter = pow(fragCoord.x - circleCenter.x, 2) + pow(fragCoord.y - circleCenter.y, 2);\n"
"   if (squareDistFromCenter > pow(circleRadius,2))\n"
"   {\n"
"       discard;\n"
"   }\n"
"   else\n"
"   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"

"}\n\0";

const char* backgroundVertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"   float x = 2.0*(aPos.x/800.0)-1;\n"
"   float y = 2.0*(aPos.y/600.0)-1;\n"
"   float z = aPos.z;\n"
"   gl_Position = vec4(x, y, z, 1.0);\n"
"   color = aColor;\n"
"}\0";

const char* backgroundFragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 color;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(color, 1.0f);\n"
"}\n\0";