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

#include "slidereditor.h"
#include "slideeditor.h"
#include "tablecellbuttons.h"
#include "flatbutton.h"
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QXmlStreamWriter>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QTextEdit>
#include <QTest>

SliderEditor::SliderEditor()
{
    m_changed = false;
    m_editor = 0;
    setAutoFillBackground(true);

    QGridLayout *grid = new QGridLayout();
    grid->setMargin(0);

    m_adminlabel = new QLineEdit;
    m_adminlabel->setMaximumWidth(200);
    QLabel *titleLabel = new QLabel("Slider Module");
    QFont fnt = titleLabel->font();
    fnt.setPointSize(16);
    fnt.setBold(true);
    titleLabel->setFont(fnt);

    FlatButton *close = new FlatButton(":/images/close_normal.png", ":/images/close_hover.png");
    close->setToolTip("Close Editor");

    QPushButton *addSlide = new QPushButton("Add Slide");
    addSlide->setMaximumWidth(120);

    m_list = new QTableWidget(0, 2, this);
    m_list->verticalHeader()->hide();
    m_list->setSelectionMode(QAbstractItemView::SingleSelection);
    m_list->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_list->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch );
    m_list->setToolTip("Double click to edit item");
    QStringList labels;
    labels << "" << "Name";
    m_list->setHorizontalHeaderLabels(labels);

    grid->addWidget(titleLabel, 0, 0);
    grid->addWidget(close, 0, 2, 1, 1, Qt::AlignRight);
    grid->addWidget(addSlide, 1, 0);
    grid->addWidget(m_list, 2, 0, 1, 3);
    grid->addWidget(new QLabel("Admin Label"), 4, 0);
    grid->addWidget(m_adminlabel, 5, 0);

    setLayout(grid);

    connect(addSlide, SIGNAL(clicked(bool)), this, SLOT(addSlide()));
    connect(m_adminlabel, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
    connect(close, SIGNAL(clicked()), this, SLOT(closeEditor()));
    connect(m_list, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(tableDoubleClicked(int, int)));

    installEventFilter(this);
}

bool SliderEditor::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == this && event->type() == QEvent::Resize && m_editor)
    {
        m_editor->resize(size());
    }
    return false;
}

void SliderEditor::setContent(QString content)
{
    m_content = content;
    m_additionalAttributes.clear();

    QXmlStreamReader stream(m_content);
    stream.readNextStartElement();
    foreach(QXmlStreamAttribute att, stream.attributes())
    {
        QString attName = att.name().toString();
        QString value = att.value().toString();
        if(attName == "adminlabel")
            m_adminlabel->setText(value);
        else
            m_additionalAttributes.insert(attName, value);
    }
    m_list->setRowCount(0);

    while(stream.readNext())
    {
        if(stream.isStartElement() && stream.name() == "Slide")
        {
            Slide *s = new Slide();
            s->setSource(stream.attributes().value("src").toString());
            s->setAdminLabel(stream.attributes().value("adminlabel").toString());
            s->setInnerHtml(stream.readElementText());
            addListItem(s);
        }
        else if(stream.isEndElement() && stream.name() == "Slider")
            break;
    }
    m_changed = false;
}

QString SliderEditor::load(QXmlStreamReader *xml)
{
    QString content;
    QXmlStreamWriter stream(&content);
    if(xml->name() == "Slider")
    {
        stream.writeStartElement("Slider");
        foreach(QXmlStreamAttribute att, xml->attributes())
        {
            stream.writeAttribute(att.name().toString(), att.value().toString());
        }
        while(xml->readNext())
        {
            if(xml->isStartElement() && xml->name() == "Slide")
            {
                stream.writeStartElement("Slide");
                stream.writeAttribute("src", xml->attributes().value("src").toString());
                stream.writeAttribute("adminlabel", xml->attributes().value("adminlabel").toString());
                stream.writeCDATA(xml->readElementText());
                stream.writeEndElement();
            }
            else if(xml->isEndElement() && xml->name() == "Slider")
                break;

        }
        stream.writeEndElement();
    }
    return content;
}

void SliderEditor::closeEditor()
{
    if(m_changed)
    {
        m_content = "";
        QXmlStreamWriter stream(&m_content);
        stream.writeStartElement("Slider");
        foreach(QString attName, m_additionalAttributes.keys())
        {
            stream.writeAttribute(attName, m_additionalAttributes.value(attName));
        }
        stream.writeAttribute("adminlabel", m_adminlabel->text());
        for(int i = 0; i < m_list->rowCount(); i++)
        {
            QTableWidgetItem *item = m_list->item(i, 1);
            Slide *slide = qvariant_cast<Slide*>(item->data(Qt::UserRole));
            stream.writeStartElement("Slide");
            stream.writeAttribute("src", slide->source());
            stream.writeAttribute("adminlabel", slide->adminLabel());
            stream.writeCDATA(slide->innerHtml());
            stream.writeEndElement();
        }
        stream.writeEndElement();
    }
    emit close();
}

void SliderEditor::addSlide()
{
    Slide *slide = new Slide();
    addListItem(slide);
    contentChanged();
    tableDoubleClicked(m_list->rowCount() - 1, 0);
}

void SliderEditor::addListItem(Slide *slide)
{
    int rows = m_list->rowCount();
    m_list->setRowCount(rows + 1);
    TableCellButtons *tcb = new TableCellButtons;
    tcb->setItem(slide);
    connect(tcb, SIGNAL(deleteItem(QObject*)), this, SLOT(deleteSlide(QObject*)));
    connect(tcb, SIGNAL(editItem(QObject*)), this, SLOT(editSlide(QObject*)));
    m_list->setCellWidget(rows, 0, tcb);
    m_list->setRowHeight(rows, tcb->sizeHint().height());
    QTableWidgetItem *titleItem = new QTableWidgetItem(slide->title());
    titleItem->setFlags(titleItem->flags() ^ Qt::ItemIsEditable);
    titleItem->setData(Qt::UserRole, QVariant::fromValue(slide));
    m_list->setItem(rows, 1, titleItem);
}

void SliderEditor::deleteSlide(QObject *slide)
{
    for(int row = 0; row < m_list->rowCount(); row++)
    {
        QTableWidgetItem *item = m_list->item(row, 1);
        Slide *m = qvariant_cast<Slide*>(item->data(Qt::UserRole));
        if(m == slide)
        {
            m_list->removeRow(row);
            contentChanged();
            break;
        }
    }
}

void SliderEditor::editSlide(QObject *slide)
{
    for(int row = 0; row < m_list->rowCount(); row++)
    {
        QTableWidgetItem *item = m_list->item(row, 1);
        Slide *m = qvariant_cast<Slide*>(item->data(Qt::UserRole));
        if(m == slide)
        {
            m_list->selectRow(row);
            tableDoubleClicked(row, 0);
            break;
        }
    }
}

void SliderEditor::tableDoubleClicked(int r, int)
{
    QTableWidgetItem *item = m_list->item(r, 1);
    Slide *slide = qvariant_cast<Slide*>(item->data(Qt::UserRole));

    m_editor = new SlideEditor();
    m_editor->setSite(m_site);
    m_editor->setSlide(slide);
    connect(m_editor, SIGNAL(close()), this, SLOT(editorClosed()));
    animate(item);
}

void SliderEditor::animate(QTableWidgetItem *item)
{
    m_row = item->row();

    // create a cell widget to get the right position in the table
    m_sourcewidget = new QWidget();
    m_list->setCellWidget(m_row, 1, m_sourcewidget);
    QPoint pos = m_sourcewidget->mapTo(this, QPoint(0,0));

    m_editor->setParent(this);
    m_editor->move(pos);
    m_editor->resize(m_sourcewidget->size());
    m_editor->show();

    m_animationgroup = new QParallelAnimationGroup();
    m_animx = new QPropertyAnimation();
    m_animx->setDuration(300);
    m_animx->setStartValue(pos.x());
    m_animx->setEndValue(0);
    m_animx->setTargetObject(m_editor);
    m_animx->setPropertyName("x");
    m_animationgroup->addAnimation(m_animx);
    m_animy = new QPropertyAnimation();
    m_animy->setDuration(300);
    m_animy->setStartValue(pos.y());
    m_animy->setEndValue(0);
    m_animy->setTargetObject(m_editor);
    m_animy->setPropertyName("y");
    m_animationgroup->addAnimation(m_animy);
    m_animw = new QPropertyAnimation();
    m_animw->setDuration(300);
    m_animw->setStartValue(m_sourcewidget->size().width());
    m_animw->setEndValue(this->size().width());
    m_animw->setTargetObject(m_editor);
    m_animw->setPropertyName("width");
    m_animationgroup->addAnimation(m_animw);
    m_animh = new QPropertyAnimation();
    m_animh->setDuration(300);
    m_animh->setStartValue(m_sourcewidget->size().height());
    m_animh->setEndValue(this->size().height());
    m_animh->setTargetObject(m_editor);
    m_animh->setPropertyName("height");
    m_animationgroup->addAnimation(m_animh);
    connect(m_animationgroup, SIGNAL(finished()), this, SLOT(animationFineshedZoomIn()));
    m_animationgroup->start();
}

void SliderEditor::animationFineshedZoomIn()
{

}

void SliderEditor::editorClosed()
{
    QPoint pos = m_sourcewidget->mapTo(this, QPoint(0,0));
    // correct end values in case of resizing the window
    m_animx->setStartValue(pos.x());
    m_animy->setStartValue(pos.y());
    m_animw->setStartValue(m_sourcewidget->size().width());
    m_animh->setStartValue(m_sourcewidget->size().height());
    m_animationgroup->setDirection(QAbstractAnimation::Backward);
    disconnect(m_animationgroup, SIGNAL(finished()), this, SLOT(animationFineshedZoomIn()));
    connect(m_animationgroup, SIGNAL(finished()), this, SLOT(animationFineshedZoomOut()));
    m_animationgroup->start();

    QTableWidgetItem *item = m_list->item(m_row, 1);
    item->setData(Qt::UserRole, QVariant::fromValue(m_editor->slide()));
    item->setText(m_editor->slide()->title());
    if(m_editor->changed())
        contentChanged();
}

void SliderEditor::animationFineshedZoomOut()
{
    delete m_animationgroup;
    delete m_editor;
    m_editor = NULL;
}

QString SliderEditor::getHtml(QXmlStreamReader *xml)
{
    QHash<QString,QString> attributes;
    QStringList urls;

    QString html = "<div id=\"myCarousel\" class=\"carousel slide\" data-ride=\"carousel\">\n";
    html += "<ol class=\"carousel-indicators\">\n";

    foreach(QXmlStreamAttribute att, xml->attributes())
    {
        QString attName = att.name().toString();
        QString value = att.value().toString();
        if(attName == "adminlabel")
            ; // ignore
        else
            attributes.insert(attName, value);
    }

    while(xml->readNext())
    {
        if(xml->isStartElement() && xml->name() == "Slide")
        {
            QString source = xml->attributes().value("src").toString();
            QString url = source.mid(source.indexOf("assets/images/"));
            urls.append(url);
        }
        else if(xml->isEndElement() && xml->name() == "Slider")
            break;
    }

    int pos = 0;
    foreach(QString url, urls)
    {
        html += "<li data-target=\"#myCarousel\" data-slide-to=\"" + QString::number(pos) + "\"" + (pos == 0 ? " class=\"active\"" : "") + "></li>\n";
        pos++;
    }
    html += "</ol>\n";
    html += "<div class=\"carousel-inner\">\n";

    pos = 0;
    foreach(QString url, urls)
    {
        html += "<div class=\"carousel-item";
        if(pos == 0)
            html += " active";
        html += "\">\n";
        html += "<img class=\"d-block w-100\" src=\"" + url + "\" alt=\"\"";
        foreach(QString attName, attributes.keys())
        {
            html += " " + attName + "=\"" + attributes.value(attName) + "\"";
        }
        html += ">\n";
        html += xml->readElementText() + "\n";
        html += "</div>\n";
        pos++;
    }

    html += "</div>\n";
    html += "<a class=\"carousel-control-prev\" href=\"#myCarousel\" role=\"button\" data-slide=\"prev\">\n";
    html += "<span class=\"carousel-control-prev-icon\" aria-hidden=\"true\"></span>\n";
    html += "<span class=\"sr-only\">Previous</span>\n";
    html += "</a>\n";
    html += "<a class=\"carousel-control-next\" href=\"#myCarousel\" role=\"button\" data-slide=\"next\">\n";
    html += "<span class=\"carousel-control-next-icon\" aria-hidden=\"true\"></span>\n";
    html += "<span class=\"sr-only\">Next</span>\n";
    html += "</a>\n";
    html += "</div>\n";
    return html;
}
