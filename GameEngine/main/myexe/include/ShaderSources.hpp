const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   double x = 2.0*(aPos.x/800.0)-1;\n"
"   double y = 2.0*(aPos.y/600.0)-1;\n"
"   double z = aPos.z;\n"
"   gl_Position = vec4(x, y, z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
"}\n\0";