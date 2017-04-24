#include <SDL2/SDL.h>
#include <cstring>
#include <GL/glew.h>
#include <string>
#include "reader.h"
#include "lodepng.h"
#include <iostream>
#include <vector>

int SIZE;

GLuint shader(char* source, GLenum stage){
  GLuint id = glCreateShader(stage);
  if(id == 0)std::cout << "Error creating shader\n";
  glShaderSource(id, 1, &source, nullptr);
  glCompileShader(id);

	GLint success = 0;

	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);

		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(id); // Don't leak the shader.
		printf("%s\n", errorLog.data());
    std::cout << errorLog.data();
	}


  return id;
}

void drawState(char* vert, char* frag){

  //Creating and linking dummy vao and vbo
    GLuint vbo(0);
    GLuint vao(0);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

  //Shader initialization
    GLuint program = glCreateProgram();
    GLuint vertID = shader(vert, GL_VERTEX_SHADER);
    GLuint fragID = shader(frag, GL_FRAGMENT_SHADER);

    glAttachShader(program, vertID);
    glAttachShader(program, fragID);

    glLinkProgram(program);

    glDetachShader(program, vertID);
    glDetachShader(program, fragID);

    glDeleteShader(vertID);
    glDeleteShader(fragID);

    glUseProgram(program);
}

int main(int argc, char* argv[]){
  if(argc < 2 || strcmp(argv[1], "--help") == 0){
    std::cout << "The first argument must be the filepath to the fragment shader you wish to render.\n";
    std::cout << "The second argument defines both screen vertical and horizontal resolution, defaulting to 512 if ommited\n"; 
    return 0;
  }
  SIZE = 512;
  if(argc > 2)SIZE = std::atoi(argv[2]);
  //Initialization

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    glewInit();

  //Window Creation

    SDL_Window* window;

    window = SDL_CreateWindow("viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SIZE, SIZE, SDL_WINDOW_OPENGL);

    SDL_GLContext  a = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, a);

    glewInit();

  //Something useless
    glClearColor(0.6f, 0.9f, 1.0f, 1.0f);

    glClearDepth(1.0f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SDL_GL_SwapWindow(window);

  //Sets up all the static state needed to draw (dummy vao & vbo and shaders)
    if(argc < 2){
      std::cout << "You should input the path to a fragment shader" << std::endl;
      return 1;
    }
    std::string frag = argv[1];
    drawState((char*)
      #include "vert.vs"
      , (char*) read(frag).c_str());


    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    SDL_GL_SwapWindow(window);

    std::cout << "DRAWING FINISHED." << std::endl;

    std::cout << "OPENGL ERRORS: ";
    for(GLenum err; (err = glGetError()) != GL_NO_ERROR;)
    {
      std::cout << err << " ";
    }
    std::cout << std::endl;

  while(true){
    //Event handling
    SDL_Event evnt;
    while(SDL_PollEvent(&evnt)){
      switch(evnt.type){
        case SDL_QUIT:
          return(0);
          break;
      }
    }

    //Drawing

  }

}
