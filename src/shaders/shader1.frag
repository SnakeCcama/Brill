#version 330 core

struct Material{

    sampler2D diffuse;
    sampler2D specular;
    float shine;

};


struct Light {
    
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};


in vec3 normals;
in vec3 fragPos;
in vec2 texCoord;

uniform Material material;
uniform Light light; 
uniform sampler2D emissionTexture; 

uniform vec3 viewPos; 

out vec4 fragColor;

void main()
{
    
    
    vec3 boxT = vec3(texture(material.diffuse, texCoord));
    vec3 emmisionColor = texture(emissionTexture, texCoord).rgb;
    
    vec3 ambient = light.ambient * (boxT + emmisionColor);
    
    vec3 lightDir = normalize (light.position - fragPos);
    vec3 norm = normalize(normals);
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * (boxT + emmisionColor);

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shine);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord));

    

    
    fragColor = vec4(ambient + diffuse + specular, 1.0);
}