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

class Slide : public QObject
{
    Q_OBJECT

public:
    Slide() {}

    QString title() {return m_adminlabel.isEmpty() ? "New Slide" : m_adminlabel;}
    QString adminLabel() {return m_adminlabel;}
    QString source() {return m_source;}

    void setSource(QString source) {m_source = source;}
    void setAdminLable(QString label) {m_adminlabel = label;}

private:
    QString m_source;
    QString m_adminlabel;
};

class SliderEditor : public EditorInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.github.Artanidos.FlatSiteBuilder.EditorInterface" FILE "slidereditor.json")
    Q_INTERFACES(EditorInterface)

public:
    SliderEditor();
    QString className() {return "SliderEditor";}
    QString displayName() {return "Slider";}
    QString tagName() {return "Slider";}
    QImage icon() {return QImage(":/slider.png");}
    QString getHtml(QDomElement ele);
    void setContent(QDomElement ele);

private slots:
    void closeEditor();
    void addSlide();
    void checkStateChanged(bool);
    void deleteButtonClicked();
    void tableDoubleClicked(int r, int);
    void animationFineshedZoomIn();
    void animationFineshedZoomOut();
    void editorClosed();

private:
    QPushButton *m_deleteButton;
    QLineEdit *m_adminlabel;
    QTableWidget *m_list;
    QList<Slide*> m_slides;
    AbstractEditor *m_editor;
    QParallelAnimationGroup *m_animationgroup;
    QPropertyAnimation *m_animx;
    QPropertyAnimation *m_animy;
    QPropertyAnimation *m_animw;
    QPropertyAnimation *m_animh;
    QWidget *m_sourcewidget;
    int m_row;

    void addListItem(Slide *slide);
    void animate(QTableWidgetItem *item);
};

#endif // SLIDEREDITOR_H



/*
<div class="fullwidthbanner-container roundedcorners">
    <div class="fullwidthbanner">
        <ul>
            <!-- SLIDE -->
            <li data-transition="incube-horizontal" data-slotamount="5" data-masterspeed="700" >
                <img src="assets/images/happypeople.png" alt="" data-bgfit="cover" data-bgposition="center top" data-bgrepeat="no-repeat">
            </li>
            <!-- SLIDE -->
            <li data-transition="incube-horizontal" data-slotamount="5" data-masterspeed="700" >
                <img src="assets/images/tagcloud.png" alt="" data-bgfit="cover" data-bgposition="center top" data-bgrepeat="no-repeat">
            </li>
        </ul>
        <div class="tp-bannertimer"></div>
    </div>
</div>

 */
