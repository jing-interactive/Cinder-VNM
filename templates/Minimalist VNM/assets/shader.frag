uniform sampler2D uTex0;
uniform sampler2D uTex1;
uniform sampler2D uTex2;
uniform sampler2D uTex3;

in vec4     Color;
in vec3     Normal;
in vec2     TexCoord;

out vec4    oColor;

void main( void )
{
    oColor = texture( uTex0, TexCoord.st ) * Color;
}
