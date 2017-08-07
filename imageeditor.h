#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

#include "abstracteditor.h"
#include "imageselector.h"
#include <QComboBox>

class ImageEditor : public AbstractEditor
{
    Q_OBJECT

public:
    ImageEditor();

    void setContent(QDomElement ele);
    QDomElement content() {return m_element;}

private slots:
    void seek();
    void save();
    void cancel();

private:
    QString m_temp;
    QString m_animation;
    ImageSelector *m_image;
    QComboBox *m_animationCombo;
};

#endif // IMAGEEDITOR_H
