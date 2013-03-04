#include <stdio.h>

#include "bcm_host.h"

#include "GLES2/gl2.h"
#include "EGL/egl.h"

void initializeGLES();

int main()
{
    initializeGLES();

    getchar();
    printf("Hello World!\n");    
    return 0;
}



void initializeGLES()
{
    //HELLO BROADCOM!
    bcm_host_init();
    uint32_t disp_width, disp_height;
    int32_t ret = graphics_get_display_size(0, &disp_width, &disp_height);
    
    //EGL INITIALISATION
    EGLBoolean res;
    
    //Init display connection
    EGLDisplay disp = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    res = eglInitialize(disp, NULL, NULL);

    //Get a config corresponding to the specified attributes
    EGLint num_configs;
    EGLConfig config;
    EGLint config_attributes[]=
    {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_NONE
    };
    res = eglChooseConfig(disp, config_attributes, &config, 1, &num_configs);

    //Declaring that we'll be using OpenGLES
    res = eglBindAPI(EGL_OPENGL_ES_API);    
    
    //Create rendering context with no context sharing for specific GLES version
    EGLint context_attributes[]=
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };
    EGLContext ctx = eglCreateContext(disp, config, EGL_NO_CONTEXT, context_attributes);
    
    //Create a WindowSurface for the context: Requires a native window
    static EGL_DISPMANX_WINDOW_T nativeWindow;
    DISPMANX_ELEMENT_HANDLE_T dispman_element;
    DISPMANX_DISPLAY_HANDLE_T dispman_display;
    DISPMANX_UPDATE_HANDLE_T dispman_update;
    VC_RECT_T dst_rect;
    VC_RECT_T src_rect;
    
    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.width = disp_width;
    dst_rect.height = disp_height;

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = disp_width << 16;
    src_rect.height = disp_height << 16;

    dispman_display = vc_dispmanx_display_open(0);
    dispman_update = vc_dispmanx_update_start(0);
    dispman_element = vc_dispmanx_element_add(dispman_update, dispman_display, 0, &dst_rect, 0, &src_rect, DISPMANX_PROTECTION_NONE, 0, 0, 0);
    
    nativeWindow.element = dispman_element;
    nativeWindow.width = disp_width;
    nativeWindow.height = disp_height;
    vc_dispmanx_update_submit_sync(dispman_update);

    EGLSurface surf = eglCreateWindowSurface(disp, config, &nativeWindow, NULL);

    //Connect context and surface
    eglMakeCurrent(disp, surf, surf, ctx);

    //DRAWING: GLES2!
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    glFinish();

    //GO GO GO!
    eglSwapBuffers(disp, surf);
}

