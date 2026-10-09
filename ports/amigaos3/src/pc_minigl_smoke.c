/*
 * PF-SP-001: MiniGL shared-library smoke (not the PhotoCraft editor).
 * Cross-compile for m68k and run manually in AmigaOS/WinUAE.
 */
#include "pc_tile_convert.h"
#include <proto/minigl.h>
#include <proto/dos.h>
#include <stdio.h>

#define TS 256
#define PS 16
#define X0 192
#define Y0 112
#define WW 640
#define WH 480

/* Large memory objects MUST NOT reside on the AmigaOS launch stack. */
static unsigned char tile[TS * TS * 4];
static unsigned char patch[PS * PS * 4];

static int gl_ok(const char *stage)
{
    GLenum e = glGetError();
    if (e != GL_NO_ERROR) {
        printf("PF-SP-001 FAIL %s: GL error %lu\n",
               stage, (unsigned long)e);
        return 0;
    }
    return 1;
}

/* RGB only: framebuffer alpha/depth differ between available RTG modes. */
static int pixel_check(const char *name, GLint x, GLint y,
                       int r, int g, int b)
{
    GLubyte p[4] = {0, 0, 0, 0};
    int dr, dg, db, ok;
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, p);
    if (!gl_ok("glReadPixels")) return 0;
    dr = (int)p[0] - r;
    dg = (int)p[1] - g;
    db = (int)p[2] - b;
    ok = dr >= -8 && dr <= 8 && dg >= -8 && dg <= 8 &&
         db >= -8 && db <= 8;
    printf("PF-SP-001 %s %s actual=%u,%u,%u expected=%d,%d,%d\n",
           ok ? "PASS" : "FAIL", name,
           (unsigned)p[0], (unsigned)p[1], (unsigned)p[2],
           r, g, b);
    return ok;
}

static void draw_tile(GLuint tex)
{
    glClearColor(0.2f, 0.2f, 0.2f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glColor4f(1, 1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(X0, Y0);
    glTexCoord2f(1, 0); glVertex2f(X0 + TS, Y0);
    glTexCoord2f(1, 1); glVertex2f(X0 + TS, Y0 + TS);
    glTexCoord2f(0, 1); glVertex2f(X0, Y0 + TS);
    glEnd();
}

int main(void)
{
    int ok = 1;
    GLuint tex = 0;
    unsigned long i;

    if (!pc_fill_rgba8_test_tile(tile, TS * 4, TS, TS)) {
        puts("PF-SP-001 FAIL: staging");
        return 20;
    }
    if (!MiniGLOpen()) {
        puts("PF-SP-001 BLOCKED: minigl.library unavailable");
        return 20;
    }
    mglChoosePixelDepth(32);
    mglChooseWindowMode(GL_TRUE);
    if (!mglCreateContext(0, 0, WW, WH)) {
        puts("PF-SP-001 FAIL: context creation");
        MiniGLClose();
        return 20;
    }
    puts("PF-SP-001 PASS GL-01: minigl.library + context");
    glViewport(0, 0, WW, WH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WW, 0, WH, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TS, TS, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, tile);
    ok = gl_ok("glTexImage2D") && ok;
    if (ok) {
        draw_tile(tex);
        ok = gl_ok("initial draw") && ok;
        ok = pixel_check("GL-02 dark", X0 + 8, Y0 + 8, 24, 180, 220) && ok;
        ok = pixel_check("GL-02 bright", X0 + 24, Y0 + 8, 220, 70, 30) && ok;
        ok = pixel_check("GL-03 orientation", X0 + 8, Y0 + 24, 220, 70, 30) && ok;
    }
    if (ok) {
        for (i = 0; i < sizeof(patch); i += 4) {
            patch[i] = 255; patch[i + 1] = 0;
            patch[i + 2] = 255; patch[i + 3] = 255;
        }
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, PS, PS,
                        GL_RGBA, GL_UNSIGNED_BYTE, patch);
        ok = gl_ok("glTexSubImage2D") && ok;
        draw_tile(tex);
        ok = gl_ok("updated draw") && ok;
        ok = pixel_check("GL-04 updated", X0 + 8, Y0 + 8, 255, 0, 255) && ok;
        ok = pixel_check("GL-04 unchanged", X0 + 24, Y0 + 8, 220, 70, 30) && ok;
    }
    if (ok) {
        glFinish();
        ok = gl_ok("finish") && ok;
        mglSwitchDisplay();
        ok = gl_ok("present") && ok;
        if (ok) {
            puts("PF-SP-001 PASS: readback + subupdate; inspect picture");
            Delay(150); /* about three seconds for manual observation */
        }
    }
    if (tex) glDeleteTextures(1, &tex);
    mglDeleteContext();
    MiniGLClose();
    puts(ok ? "PF-SP-001 RESULT: PASS (API only)" :
              "PF-SP-001 RESULT: FAIL");
    return ok ? 0 : 5;
}
