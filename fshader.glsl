#version 330 core

in vec3 normal;
in vec3 position;

layout(location=0) out vec4 fragColor;

uniform vec4 lightPosition;
uniform vec3 lightIntensity;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float shininess;

void main()
{
    vec3 n=normalize(normal);
    vec3 s=normalize(lightPosition.xyz-position);
    vec3 v=normalize(-position.xyz);
    vec3 h=normalize(v+s);
    float sdn=dot(s,n);
    vec3 ambient=Ka;//环境光
    vec3 diffuse=Kd*max(sdn,0.0);//漫反射
    vec3 specular=Ks*mix(0.0,pow(dot(h,n),shininess),step(0.0,sdn));//镜面反射，高光
    //fragColor=vec4(lightIntensity*(ambient+diffuse+specular),1);
    fragColor=vec4(lightIntensity*(0.1*ambient+diffuse+specular),1);
}
