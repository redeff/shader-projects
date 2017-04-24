R"DELIMITER(
#version 130

out vec2 uv;

void main(){
  if(gl_VertexID % 4 == 0){
    gl_Position = vec4(1.0f, 1.0f, 0.0f, 1.0f);
  }else if(gl_VertexID % 4 == 1){
    gl_Position = vec4(1.0f, -1.0f, 0.0f, 1.0f);
  }else if(gl_VertexID % 4 == 2){
    gl_Position = vec4(-1.0f, 1.0f, 0.0f, 1.0f);
  }else{
    gl_Position = vec4(-1.0f, -1.0f, 0.0f, 1.0f);
  }
  uv = gl_Position.xy;
}
)DELIMITER"
