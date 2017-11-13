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

#ifndef SLIDEREDITOR_H
#define SLIDEREDITOR_H

#include <QtPlugin>
#include <QObject>
#include <QTableWidget>
#include <QPushButton>
#include "interfaces.h"

class SlideEditor;
class QParallelAnimationGroup;
class QPropertyAnimation;
class Slide : public QObject
{
    Q_OBJECT

public:
    Slide() {}

    QString title() {return m_adminlabel.isEmpty() ? "New Slide" : m_adminlabel;}
    QString adminLabel() {return m_adminlabel;}
    QString source() {return m_source;}
    QString innerHtml() {return m_innerhtml;}

    void setSource(QString source) {m_source = source;}
    void setAdminLabel(QString label) {m_adminlabel = label;}
    void setInnerHtml(QString html) {m_innerhtml = html;}

private:
    QString m_source;
    QString m_adminlabel;
    QString m_innerhtml;
};

class SliderEditor : public ElementEditorInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.github.Artanidos.FlatSiteBuilder.ElementEditorInterface")
    Q_INTERFACES(ElementEditorInterface)

public:
    SliderEditor();

    bool eventFilter(QObject *watched, QEvent *event);
    QString className() {return "SliderEditor";}
    QString displayName() {return "Slider";}
    QString tagName() {return "Slider";}
    QImage icon() {return QImage(":/slider.png");}
    QString version() override {return "1.5.4";}
    QString getHtml(QXmlStreamReader *xml);
    void setContent(QString content);
    QString load(QXmlStreamReader *xml) override;

public slots:
    void closeEditor() override;

private slots:
    void addSlide();
    void tableDoubleClicked(int r, int);
    void animationFineshedZoomIn();
    void animationFineshedZoomOut();
    void editorClosed();
    void deleteSlide(QObject *);
    void editSlide(QObject *);

private:
    QLineEdit *m_adminlabel;
    QLineEdit *m_id;
    QTableWidget *m_list;
    SlideEditor *m_editor;
    QParallelAnimationGroup *m_animationgroup;
    QPropertyAnimation *m_animx;
    QPropertyAnimation *m_animy;
    QPropertyAnimation *m_animw;
    QPropertyAnimation *m_animh;
    QWidget *m_sourcewidget;
    int m_row;
    QHash<QString,QString> m_additionalAttributes;

    void addListItem(Slide *slide);
    void animate(QTableWidgetItem *item);
};

#endif // SLIDEREDITOR_H
