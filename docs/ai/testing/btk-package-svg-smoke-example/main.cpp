#include <QtCore/QByteArray>
#include <QtSvg/QSvgRenderer>

int main()
{
   const QByteArray svg("<svg xmlns='http://www.w3.org/2000/svg' width='12' height='9'>"
      "<rect width='12' height='9' fill='#336699'/></svg>");

   QSvgRenderer renderer(svg);
   const QSize size = renderer.defaultSize();

   return ! renderer.isValid() || size.width() != 12 || size.height() != 9 ? 1 : 0;
}
