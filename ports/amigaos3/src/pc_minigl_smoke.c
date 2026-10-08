/*
 * Isolated 68k MiniGL shared-library smoke; not the PhotoCraft editor.
 * Requires the separately installed MiniGL SDK + matching minigl.library.
 * First prove the ABI and texture upload; integrate ReAction later.
 */
#include "pc_tile_convert.h"

#include <proto/minigl.h>
#include <proto/dos.h>

#include <stdio.h>

#define PC_TEXTURE_SIZE 256
#define PC_WINDOW_W 640
#define PC_WINDOW_H 480

/* Not allocated on the small Workbench launch stack. */
static unsigned char s_tile[PC_TEXTURE_SIZE * PC_TEXTURE_SIZE * 4];

static int check_gl(const char *stage)
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("PhotoCraft MiniGL: %s GL error %lu\n",
               stage, (unsigned long)error);
        return 0;
    }
    return 1;
}

int main(void)
{
    GLuint tex = 0;
    int ok = 1;

    if (!pc_fill_rgba8_test_tile(s_tile, PC_TEXTURE_SIZE * 4,
                                 PC_TEXTURE_SIZE, PC_TEXTURE_SIZE)) {
        puts("PhotoCraft MiniGL: staging failed");
        return 20;
    }

    if (!MiniGLOpen()) {
        puts("PhotoCraft MiniGL: minigl.library unavailable");
        return 20;
    }
    mglChoosePixelDepth(32);
    mglChooseWindowMode(GL_TRUE);
    if (!mglCreateContext(0, 0, PC_WINDOW_W, PC_WINDOW_H)) {
        puts("PhotoCraft MiniGL: context creation failed");
        MiniGLClose();
        return 20;
    }

    glViewport(0, 0, PC_WINDOW_W, PC_WINDOW_H);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, PC_WINDOW_W, 0, PC_WINDOW_H, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, PC_TEXTURE_SIZE,
                 PC_TEXTURE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, s_tile);
    ok = check_gl("upload");

    if (ok) {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glColor4f(1, 1, 1, 1);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(192, 112);
        glTexCoord2f(1, 0); glVertex2f(448, 112);
        glTexCoord2f(1, 1); glVertex2f(448, 368);
        glTexCoord2f(0, 1); glVertex2f(192, 368);
        glEnd();
        ok = check_gl("draw");
        glFinish();
        mglSwitchDisplay();
        puts("PhotoCraft MiniGL: frame presented; inspect visual result.");
        Delay(125); /* approximately 2.5 seconds for manual observation */
    }

    glDeleteTextures(1, &tex);
    mglDeleteContext();
    MiniGLClose();
    return ok ? 0 : 5;
}
