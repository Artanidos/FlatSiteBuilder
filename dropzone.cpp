#include "dropzone.h"

DropZone::DropZone(int w, int h)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(palette().base().color().name()).lighter().lighter());
    setPalette(pal);
    setAutoFillBackground(true);
    setMinimumWidth(w);
    setMaximumWidth(w);
    setMinimumHeight(h);
    setMaximumHeight(h);
}
