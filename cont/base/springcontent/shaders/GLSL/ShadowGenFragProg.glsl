#extension GL_ARB_conservative_depth : enable
#if (GL_FRAGMENT_PRECISION_HIGH == 1)
	// ancient GL3 ATI drivers confuse GLSL for GLSL-ES and require this
	precision highp float;
#else
	precision mediump float;
#endif

uniform sampler2D alphaMaskTex;

uniform vec4 shadowParams;
uniform vec2 alphaParams;

in vec4 vertColor;
in vec4 texCoord;

#if (GL_ARB_conservative_depth == 1)
	layout(depth_unchanged) out float gl_FragDepth;
#endif

out float shadowIntensity;

void main() {
#if defined(SHADOWGEN_PROGRAM_PARTICLE)
	float alpha = texture(alphaMaskTex, texCoord.xy).a * vertColor.a;
	if (alpha < 0.01)
		discard;

	shadowIntensity = 2.0 * alpha * alpha;
#else
	shadowIntensity = 1.0;
#endif
}
