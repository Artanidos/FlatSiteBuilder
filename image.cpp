#include "image.h"

Image::Image()
{

}

QString Image::getHtml()
{
    return "<image src=\"" + m_source + "\">";
}
