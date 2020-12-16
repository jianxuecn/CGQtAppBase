#version 330

uniform vec4 lightPosition;
uniform vec4 lightAmbient;
uniform vec4 lightDiffuse;
uniform vec4 lightSpecular;
uniform vec4 materialAmbient;
uniform vec4 materialDiffuse;
uniform vec4 materialEmission;
uniform vec4 materialSpecular;
uniform float materialShininess;

in highp vec4 fragVertex;
in highp vec3 fragNormal;

out highp vec4 fragColor;

void main() {
    //if (!gl_FrontFacing) discard;
    
    vec3 Nn = normalize(fragNormal);
    float darker = 1.0;
    //  if (!gl_FrontFacing) { //does not work in intel hd4000 mac
    if (gl_FrontFacing == false) {
        Nn = -Nn;
        darker = 0.4;
    }

    vec3 Vn = normalize(-fragVertex.xyz);
    vec3 Ln = normalize((lightPosition - fragVertex).xyz);
    vec3 Hn = normalize(Vn+Ln);
    
    float LdotN = dot(Ln, Nn);
    float HdotN = max(dot(Hn, Nn), 0.0) * step(0.0, LdotN);
    LdotN = max(LdotN, 0.0);
    
    vec4 ambientColor = lightAmbient * materialAmbient + materialEmission;
    vec4 diffuseColor = lightDiffuse * materialDiffuse * darker * LdotN;
    vec4 specularColor = lightSpecular * materialSpecular * pow(HdotN, materialShininess);
    
    vec4 color = ambientColor + diffuseColor + specularColor;
    fragColor = vec4(color.rgb, materialDiffuse.a);
    //fragColor = vec4((Ln+1.0)*0.5, 0.0);
}
