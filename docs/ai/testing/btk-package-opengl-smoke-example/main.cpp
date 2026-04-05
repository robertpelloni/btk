#include <QtOpenGL/qgl.h>

int main()
{
   QGLFormat format;
   format.setDepthBufferSize(24);
   format.setVersion(2, 0);

   return ! QGLFormat::hasOpenGL()
      || format.depthBufferSize() != 24
      || format.majorVersion() != 2
      || format.minorVersion() != 0 ? 1 : 0;
}
