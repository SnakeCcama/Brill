#version 330 core


struct Material{

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shine;

};

struct Light{

    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};


in vec3 normals; 
in vec3 fragPos;
out vec4 fragColor;

uniform Material material;
uniform Light light; 

uniform vec3 viewPos;


void main()
{
    
    /* Ambient lighting*/

    vec3 ambient = light.ambient * material.ambient;

    /* Diffuse lighting */

    vec3 norm = normalize(normals);
    vec3 lightDir = normalize (light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse); 

    /* specular lighting */
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shine);
    vec3 specular = light.specular * (spec * material.specular);

    /*Result*/

    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0f);
}


