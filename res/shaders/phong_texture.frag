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
uniform sampler2D materialDiffuseMap;

in highp vec4 fragVertex;
in highp vec3 fragNormal;
in highp vec2 fragTexCoord;

out highp vec4 fragColor;

void main() {
    //if (!gl_FrontFacing) discard;
    
    vec3 Nn = normalize(fragNormal);
    //  if (!gl_FrontFacing) { //does not work in intel hd4000 mac
    if (gl_FrontFacing == false) {
        Nn = -Nn;
    }
    
    vec3 Vn = normalize(-fragVertex.xyz);
    vec3 Ln = normalize((lightPosition - fragVertex).xyz);
    vec3 Hn = normalize(Vn+Ln);
    
    float LdotN = dot(Ln, Nn);
    float HdotN = max(dot(Hn, Nn), 0.0) * step(0.0, LdotN);
    LdotN = max(LdotN, 0.0);
    
    vec4 ambientColor = lightAmbient * materialAmbient + materialEmission;
    vec4 diffuseColor = lightDiffuse * materialDiffuse * LdotN;
    vec4 specularColor = lightSpecular * materialSpecular * pow(HdotN, materialShininess);
    
    vec4 diffuseMapColor = texture(materialDiffuseMap, fragTexCoord);
    
    vec3 color = (ambientColor.rgb + diffuseColor.rgb) * diffuseMapColor.rgb + specularColor.rgb;
    fragColor = vec4(color, diffuseMapColor.a);
}
