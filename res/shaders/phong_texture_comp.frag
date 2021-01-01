uniform mediump vec4 lightPosition;
uniform mediump vec4 lightAmbient;
uniform mediump vec4 lightDiffuse;
uniform mediump vec4 lightSpecular;
uniform mediump vec4 materialAmbient;
uniform mediump vec4 materialDiffuse;
uniform mediump vec4 materialEmission;
uniform mediump vec4 materialSpecular;
uniform mediump float materialShininess;
uniform sampler2D materialDiffuseMap;

varying mediump vec4 fragVertex;
varying mediump vec3 fragNormal;
varying mediump vec2 fragTexCoord;

void main() {
    //if (!gl_FrontFacing) discard;
    
    mediump vec3 Nn = normalize(fragNormal);
    //  if (!gl_FrontFacing) { //does not work in intel hd4000 mac
    if (gl_FrontFacing == false) {
        Nn = -Nn;
    }

    mediump vec3 Vn = normalize(-fragVertex.xyz);
    mediump vec3 Ln = normalize((lightPosition - fragVertex).xyz);
    mediump vec3 Hn = normalize(Vn+Ln);
    
    mediump float LdotN = dot(Ln, Nn);
    mediump float HdotN = max(dot(Hn, Nn), 0.0) * step(0.0, LdotN);
    LdotN = max(LdotN, 0.0);
    
    mediump vec4 ambientColor = lightAmbient * materialAmbient + materialEmission;
    mediump vec4 diffuseColor = lightDiffuse * materialDiffuse * LdotN;
    mediump vec4 specularColor = lightSpecular * materialSpecular * pow(HdotN, materialShininess);
    
    mediump vec4 diffuseMapColor = texture2D(materialDiffuseMap, fragTexCoord);
    
    mediump vec3 color = (ambientColor.rgb + diffuseColor.rgb) * diffuseMapColor.rgb + specularColor.rgb;
    gl_FragColor = vec4(color, diffuseMapColor.a);
}
