#version 120

attribute vec4 vertPos;
attribute vec4 vertRegPos;
attribute vec3 vertNor;

attribute vec4 weights0;
attribute vec4 weights1;
attribute vec4 weights2;
attribute vec4 weights3;

attribute vec4 bones0;
attribute vec4 bones1;
attribute vec4 bones2;
attribute vec4 bones3;

attribute float boneNum;

uniform int isCPU;

uniform mat4 boneTransforms[18];
uniform mat4 bindPoses[18];
uniform mat4 P;
uniform mat4 MV;

varying vec3 fragPos;
varying vec3 fragNor;
varying vec3 lightPos;

void main()
{
    vec4 posCam;
    vec4 point = vec4(0);
    
    float weights [16] = float[](weights0.x, weights0.y, weights0.z, weights0.w,
                               weights1.x, weights1.y, weights1.z, weights1.w,
                               weights2.x, weights2.y, weights2.z, weights2.w,
                               weights3.x, weights3.y, weights3.z, weights3.w);
    int bones [16] = int[](int(bones0.x), int(bones0.y), int(bones0.z), int(bones0.w),
                           int(bones1.x), int(bones1.y), int(bones1.z), int(bones1.w),
                           int(bones2.x), int(bones2.y), int(bones2.z), int(bones2.w),
                           int(bones3.x), int(bones3.y), int(bones3.z), int(bones3.w));
    
    for (int i = 0; i < int(boneNum); i++)
    {
        vec4 tempPt = bindPoses[bones[i]] * vertRegPos;
        tempPt = boneTransforms[bones[i]] * tempPt;
        point += weights[i] * tempPt;
    }
    
    if (isCPU == 1)
    {
        posCam = MV * vertPos;
    }
    else
    {
        posCam = MV * point;
    }
    
    gl_Position = P * posCam;
    lightPos = (MV * vec4(5, 1, 5, 1)).xyz;
    fragPos = posCam.xyz;
    fragNor = (MV * vec4(vertNor, 0.0)).xyz;
}
