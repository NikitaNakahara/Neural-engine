#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 fragPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform samplerCube skybox;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;


void main()
{
   float specularStr = 0.5;
   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(lightPos - fragPos);
   vec3 viewDir = normalize(viewPos - fragPos);
   vec3 reflectDir = reflect(-lightDir, norm);

   vec3 objectColor = vec3(texture(texture_diffuse1, TexCoords));

   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = diff * lightDiffuse * vec3(texture(texture_diffuse1, TexCoords));

   vec3 ambient = lightAmbient * vec3(texture(texture_diffuse1, TexCoords));
   
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   vec3 specular = lightSpecular * spec * vec3(texture(texture_specular1, TexCoords));

   vec3 result = ambient + diffuse + specular;
   FragColor = vec4(result, 1.0);
}