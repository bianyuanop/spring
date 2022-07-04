#if (GL_FRAGMENT_PRECISION_HIGH == 1)
	// ancient GL3 ATI drivers confuse GLSL for GLSL-ES and require this
	precision highp float;
#else
	precision mediump float;
#endif

uniform vec4 shadowParams;

#ifdef SHADOWGEN_PROGRAM_TREE_NEAR
	uniform vec3 cameraDirX;
	uniform vec3 cameraDirY;
	uniform vec3 treeOffset;

	#define MAX_TREE_HEIGHT 60.0
#endif

out vec4 vertColor;
out vec4 texCoord;

void main() {
	#ifdef SHADOWGEN_PROGRAM_TREE_NEAR
		vec3 offsetVec = treeOffset + (cameraDirX * gl_Normal.x) + (cameraDirY * gl_Normal.y);
	#else
		vec3 offsetVec = vec3(0.0, 0.0, 0.0);
	#endif


	vec4 vertexPos = gl_Vertex + vec4(offsetVec, 0.0);
	vec4 vertexShadowPos = gl_ModelViewMatrix * vertexPos;

	vertexShadowPos.xy *= (inversesqrt(abs(vertexShadowPos.xy) + shadowParams.zz) + shadowParams.ww);
	vertexShadowPos.xy += shadowParams.xy;
	//vertexShadowPos.z  += 0.00250;

	gl_Position = gl_ProjectionMatrix * vertexShadowPos;


	#ifdef SHADOWGEN_PROGRAM_MODEL
		gl_ClipVertex  = vertexPos;
		texCoord = gl_MultiTexCoord0;
	#endif

	#ifdef SHADOWGEN_PROGRAM_MAP
	// empty, uses TexGen
	#endif

	#ifdef SHADOWGEN_PROGRAM_TREE_NEAR
		vertColor.xyz = gl_Normal.z * vec3(1.0, 1.0, 1.0);
		vertColor.a = gl_Vertex.y * (0.20 * (1.0 / MAX_TREE_HEIGHT)) + 0.85;
		texCoord = gl_MultiTexCoord0;
	#endif

	#ifdef SHADOWGEN_PROGRAM_TREE_DIST
		vertColor = gl_Color;
		texCoord = gl_MultiTexCoord0;
	#endif

	#ifdef SHADOWGEN_PROGRAM_PROJECTILE
		vertColor = gl_Color;
		texCoord = gl_MultiTexCoord0;
	#endif

	#ifdef SHADOWGEN_PROGRAM_PARTICLE
		vertColor = gl_Color;
		texCoord = gl_MultiTexCoord0;
	#endif
}
