#ifndef COLUMNEDITORMIMEDATA_H
#define COLUMNEDITORMIMEDATA_H

#include <QMimeData>
#include "columneditor.h"

class ColumnEditorMimeData : public QMimeData
{
    Q_OBJECT
public:
    ColumnEditorMimeData();

    void setData(ColumnEditor *);
    ColumnEditor *getData() const;

private:
    ColumnEditor *m_data;
};

#endif // COLUMNEDITORMIMEDATA_H
