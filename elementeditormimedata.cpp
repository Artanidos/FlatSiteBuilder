#include "elementeditormimedata.h"

ElementEditorMimeData::ElementEditorMimeData()
{

}

void ElementEditorMimeData::setData(ElementEditor *ce)
{
    m_data = ce;
}

ElementEditor *ElementEditorMimeData::getData() const
{
    return m_data;
}
