/*
*MEIZU Technology. ZHUHAI 2013
*
*Cui.YY
*Description:
*gl2_basic_render class was used for rendering real OpenGL frame
*with standard EGL context.
*This is very common class, and easy to extend to gl1, gl3.
*In order to use Android Native surface  should be combined with
*NativeSurfaceFlingerEGLService class.
*
*Suggestion: This is the best candiadate to study shader programming.
*
*/
#ifndef  GL2_BASIC_RENDER_H
#define GL2_BASIC_RENDER_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sched.h>
#include <pthread.h>

//TODO: Support GLES1
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
//TODO: Support GLES3

#include <utils/RefBase.h>
#include <utils/Looper.h>
#include <utils/String8.h>

#include <ui/FramebufferNativeWindow.h>
#include <gui/DisplayEventReceiver.h>

#include "MatrixTransform.h"
#include "VertexGenerator.h"

namespace android
{
class gl2_basic_render:public virtual RefBase
{
public:
    EGLNativeWindowType windowSurface;

    gl2_basic_render(unsigned int index, unsigned int step);
    void startRender(EGLNativeWindowType windowSurface);
    static void* mainRender(void* windowSurface);   //static for function callback

private:

    /* ---------- Data Declare Area Start, Note it's a global member ---------- */

    //Each frame render color
    static GLclampf gColorMatrix[6][4];
    /* polygon info*/
    static GLfloat gSimpleTriangleVertices[6];
    GLfloat * mCubeVertices;
    GLfloat * mCubeColor;
    GLuint * mCubeIndices;
    GLuint mCubeNumOfIndex;
    GLfloat * mBlenderVertices;

    //Shader for vertex
    static const char * gVS_Header_Attribute_vertexPosition;  //Header
    static const char * gVS_Header_Uniform_rotationMatrix;
    static const char * gVS_Header_Uniform_scaleMatrix;
    static const char * gVS_Header_Uniform_translationMatrix;
    static const char * gVS_Header_Attribute_passColor;
    static const char * gVS_Header_Varying_colorToFrag;

    static const char * gVS_Main_Start_Function;  //Body
    static const char * gVS_Function_Direct_Pass_Position;
    static const char * gVS_Function_Pass_RO_Multi_Position;
    static const char * gVS_Function_Pass_SC_Multi_Position;
    static const char * gVS_Function_Pass_TR_Multi_Position;
    static const char * gVS_Function_Pass_Color_To_Frag;
    static const char * gVS_Main_End_Function;

    //Shader for fragment
    static const char * gFS_Header_Precision_Mediump_Float;  //Header
    static const char * gFS_Header_Varying_colorToFrag;
    
    static const char * gFS_Main_Start_Function;  //Body
    static const char * gFS_Function_Pass_Constant_Color;
    static const char * gFS_Function_Direct_Pass_Color;
    static const char * gFS_Main_End_Function;

    /* ========= Data Declare Area End, Note it's a global member ========= */


    /* --- Attribute, Uniform handler Start, Note it's a class object property --- */

    //Shader for vertex
    GLuint mAttrVSPosition;
    GLuint mUniVSrotateMat;
    GLuint mUniVSscaleMat;
    GLuint mUniVStranslateMat;
    GLuint mAttrVSColorPass;


    //Shader for fragment

    /* === Attribute, Uniform handler End, Note it's a class object property ===  */



    /* ------- Rendering options Start, Not it's a class object property------- */
    /* Note:If add an new item, should also keep with polygonShaderSetup function */
    bool hasColorConstantPass;
    bool hasColorDirectPass;
    bool hasPreciMidium;
    bool hasRotation;
    bool hasScale;
    bool hasTranslation;
    bool hasLighting;
    bool hasTextureMap;
    bool hasFBO;
    bool hasVBO;

    bool hasSimTriangle;
    bool hasCube;
    bool hasBlender;

    bool hasVAO;//OpenGL3.0 only

    /*====== Rendering options End, Not it's a class object property====== */



    /* ------ Various buffer object Start------ */
    GLuint mVBOForVI[3];
    /* ===== Varous transform matrix   End ======*/



    /* ------ Various transform matrix  Start------ */
    Matrix44 mRotateMatrix;
    Matrix44 mScaleMatrix;
    Matrix44 mTranslateMatrix;
    /* ===== Varous transform matrix   End ======*/




    //global shader source, class object property
    String8 mVertexShader;
    String8 mFramgmentShader;

    //Polygon render function
    void polygonShaderSetup();
    bool polygonBuildnLink(int w, int h, const char vertexShader[], const char fragmentShader[]);
    void polygonDraw();


    /*Cui.YY
    static constexpr GLclampf gColorMatrix[6][4];  in class static member initialization,
    just only available in C++11 standard, must combine used with constexpr keyword*/
    unsigned int mIndex;
    unsigned int mStep;
    unsigned int mCounter;
    GLuint mOGLProgram;
    nsecs_t mOldTimeStamp;

    //Each transformation specific
    unsigned int mRotationAngle;
    GLfloat mScaleMagnitude;
    bool mScaleIncreasing;
    GLfloat mTranslationMagnitude;

    DisplayEventReceiver mDisplayEventReceiver;
    sp<Looper> mLoop;
    EGLSurface mEGLSurface;
    EGLDisplay mEGLDisplay;

    //Used to setup basic rendering environment
    GLuint loadShader(GLenum shaderType, const char* pSource);
    GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);
    static void frameControl(int fd, int events, void* data);  //static for function callback

};

}

#endif
