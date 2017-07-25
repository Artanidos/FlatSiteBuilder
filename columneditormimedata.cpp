#include "columneditormimedata.h"

ColumnEditorMimeData::ColumnEditorMimeData()
{

}

void ColumnEditorMimeData::setData(ColumnEditor *ce)
{
    m_data = ce;
}

ColumnEditor *ColumnEditorMimeData::getData() const
{
    return m_data;
}
