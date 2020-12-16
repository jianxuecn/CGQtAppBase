uniform mediump vec4 lightPosition;
uniform mediump vec4 lightAmbient;
uniform mediump vec4 lightDiffuse;
uniform mediump vec4 lightSpecular;
uniform mediump vec4 materialAmbient;
uniform mediump vec4 materialDiffuse;
uniform mediump vec4 materialEmission;
uniform mediump vec4 materialSpecular;
uniform mediump float materialShininess;

varying mediump vec4 fragVertex;
varying mediump vec3 fragNormal;

void main() {
    //if (!gl_FrontFacing) discard;
    
    mediump vec3 Nn = normalize(fragNormal);
    mediump float darker = 1.0;
    //  if (!gl_FrontFacing) { //does not work in intel hd4000 mac
    if (gl_FrontFacing == false) {
        Nn = -Nn;
        darker = 0.4;
    }

    mediump vec3 Vn = normalize(-fragVertex.xyz);
    mediump vec3 Ln = normalize((lightPosition - fragVertex).xyz);
    mediump vec3 Hn = normalize(Vn+Ln);
    
    mediump float LdotN = dot(Ln, Nn);
    mediump float HdotN = max(dot(Hn, Nn), 0.0) * step(0.0, LdotN);
    LdotN = max(LdotN, 0.0);
    
    mediump vec4 ambientColor = lightAmbient * materialAmbient + materialEmission;
    mediump vec4 diffuseColor = lightDiffuse * materialDiffuse * darker * LdotN;
    mediump vec4 specularColor = lightSpecular * materialSpecular * pow(HdotN, materialShininess);
    
    mediump vec4 color = ambientColor + diffuseColor + specularColor;
    gl_FragColor = vec4(color.rgb, materialDiffuse.a);
    //gl_FragColor = vec4(1.0, 0.0, 0.0, 0.0);
}
