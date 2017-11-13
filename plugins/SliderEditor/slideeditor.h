/****************************************************************************
** Copyright (C) 2017 Olaf Japp
**
** This file is part of FlatSiteBuilder.
**
**  FlatSiteBuilder is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  FlatSiteBuilder is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with FlatSiteBuilder.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef SLIDEEDITOR_H
#define SLIDEEDITOR_H

#include "interfaces.h"
#include "animateableeditor.h"

class Slide;
class ImageSelector;
class QLineEdit;
class QTextEdit;
class SlideEditor : public AnimateableEditor
{
    Q_OBJECT

public:
    SlideEditor();

    void setContent(QString) {/* unused */}
    void setSlide(Slide *slide);
    Slide *slide() {return m_slide;}

public slots:
    void closeEditor() override;

signals:
    void close();

private slots:
    void seek();

private:
    QLineEdit *m_adminlabel;
    QLineEdit *m_source;
    ImageSelector *m_image;
    Slide *m_slide;
    QTextEdit *m_innerHtml;
};

#endif // SLIDEEDITOR_H
