#ifndef ELEMENTEDITORMIMEDATA_H
#define ELEMENTEDITORMIMEDATA_H

#include <QMimeData>
#include "elementeditor.h"

class ElementEditorMimeData : public QMimeData
{
    Q_OBJECT
public:
    ElementEditorMimeData();

    void setData(ElementEditor *);
    ElementEditor *getData() const;

private:
    ElementEditor *m_data;
};

#endif // ELEMENTEDITORMIMEDATA_H
