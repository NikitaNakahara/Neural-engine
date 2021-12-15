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
uniform vec3 viewPos;

float near = 0.1; 
float far  = 100.0;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // обратно к NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));  
}

void main()
{
   vec3 lightColor = vec3(1.0, 1.0, 1.0);

   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(lightPos - fragPos);

   vec3 viewDir = normalize(viewPos - fragPos);
   vec3 reflectDir = reflect(-lightDir, norm);

   float ambientStrength = 0.1;
   vec3 ambient = ambientStrength * lightColor * texture(texture_diffuse1, TexCoords).rgb;

   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   vec3 specular = spec * lightColor * texture(texture_specular1, TexCoords).rrr; 
   vec3 spec_map = texture(texture_specular1, TexCoords).rrr;

   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = diff * lightColor * texture(texture_diffuse1, TexCoords).rgb;
   vec3 diff_map = texture(texture_diffuse1, TexCoords).rgb;

   vec3 result = vec3(ambient + diffuse + specular);

   float ratio = 1.00 / 1.53;
   vec3 I = normalize(fragPos - viewPos);
   vec3 Reflect = reflect(I, normalize(Normal));
   vec3 Refract = refract(I, normalize(Normal), ratio);

   FragColor = vec4(result, 1.0);
}