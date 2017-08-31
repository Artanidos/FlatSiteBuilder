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

#include "expander.h"
#include "hyperlink.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QTest>
#include <QPicture>

Expander::Expander(QString header, QString normalIcon, QString hoveredIcon, QString selectedIcon)
{
    m_isExpanded = false;
    m_text = header;
    m_labelNormalColor = palette().link().color().name();
    m_labelHoveredColor = palette().highlight().color().name();
    m_labelSelectedColor = palette().highlightedText().color().name();
    m_normalColor = palette().base().color().name();
    m_selectedColor = palette().highlight().color();
    m_hoveredColor = palette().alternateBase().color();

    setCursor(Qt::PointingHandCursor);

    if(normalIcon != "")
        m_normalIcon = QImage(normalIcon);
    if(hoveredIcon != "")
        m_hoveredIcon = QImage(hoveredIcon);
    if(selectedIcon != "")
        m_selectedIcon = QImage(selectedIcon);

    setAttribute(Qt::WA_Hover, true);

    setColor(m_normalColor);
    setAutoFillBackground(true);

    QVBoxLayout *vbox = new QVBoxLayout();
    QHBoxLayout *hbox = new QHBoxLayout();
    m_icon = new QLabel();
    m_icon->setPixmap(QPixmap::fromImage(m_normalIcon));
    m_hyper = new QLabel();
    m_hyper->setText("<a style=\"color: " + m_labelNormalColor + "; text-decoration: none;\" href=\"#\">" + m_text + "</a>");
    hbox->addWidget(m_icon);
    hbox->addSpacing(5);
    hbox->addWidget(m_hyper);
    hbox->addStretch();
    hbox->setMargin(8);
    vbox->addLayout(hbox);
    m_content = new QWidget();
    m_content->setStyleSheet("background-color: " + palette().base().color().name());
    m_content->setMaximumHeight(0);

    vbox->addWidget(m_content);
    vbox->setMargin(0);
    this->setLayout(vbox);

    connect(m_hyper, SIGNAL(linkActivated(QString)), this, SLOT(buttonClicked()));

    anim = new QParallelAnimationGroup();
    heightAnim = new QPropertyAnimation(m_content, "maximumHeight");
    colorAnim = new QPropertyAnimation(this, "color");
    heightAnim->setDuration(200);
    colorAnim->setDuration(200);
    anim->addAnimation(heightAnim);
    anim->addAnimation(colorAnim);
}

void Expander::setColor(QColor color)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Background, color);
    setPalette(pal);
}

void Expander::mouseReleaseEvent(QMouseEvent*)
{
    setExpanded(!m_isExpanded);
    if(m_isExpanded)
        emit clicked();
}

void Expander::setExpanded(bool value)
{
    if(value == m_isExpanded)
        return;
    if(value)
    {
        m_isExpanded = true;
        QPalette pal = palette();
        pal.setColor(QPalette::Background, m_selectedColor);
        this->setPalette(pal);
        m_icon->setPixmap(QPixmap::fromImage(m_selectedIcon));
        m_hyper->setText("<a style=\"color: " + m_labelSelectedColor + "; text-decoration: none;\" href=\"#\">" + m_text + "</a>");
    }
    else
    {
        m_isExpanded = false;
        QPalette pal = palette();
        pal.setColor(QPalette::Background, m_normalColor);
        this->setPalette(pal);
        m_icon->setPixmap(QPixmap::fromImage(m_normalIcon));
        m_hyper->setText("<a style=\"color: " + m_labelNormalColor + "; text-decoration: none;\" href=\"#\">" + m_text + "</a>");
    }
    if(m_isExpanded)
        expandContent();
    else
        collapseContent();
    emit expanded(m_isExpanded);
}

void Expander::collapseContent()
{
    if(m_content->layout())
        heightAnim->setStartValue(m_content->layout()->sizeHint().height());
    else
        heightAnim->setStartValue(0);
    heightAnim->setEndValue(0);
    colorAnim->setStartValue(m_selectedColor);
    colorAnim->setEndValue(m_normalColor);
    anim->start();
}

void Expander::expandContent()
{
    if(m_content->layout())
        heightAnim->setEndValue(m_content->layout()->sizeHint().height());
    else
        heightAnim->setEndValue(0);
    heightAnim->setStartValue(0);
    colorAnim->setStartValue(m_normalColor);
    colorAnim->setEndValue(m_selectedColor);
    anim->start();
}

void Expander::buttonClicked()
{
   setExpanded(!m_isExpanded);
   if(m_isExpanded)
    emit clicked();
}

void Expander::addLayout(QLayout *l)
{
    m_content->setLayout(l);
}

void Expander::enterEvent(QEvent * event)
{
    if(!m_isExpanded)
    {
        QPalette pal = palette();
        pal.setColor(QPalette::Background, m_hoveredColor);
        this->setPalette(pal);
        m_icon->setPixmap(QPixmap::fromImage(m_hoveredIcon));
        m_hyper->setText("<a style=\"color: " + m_labelHoveredColor + "; text-decoration: none;\" href=\"#\">" + m_text + "</a>");
    }
    QWidget::enterEvent(event);
}

void Expander::leaveEvent(QEvent * event)
{
    if(!m_isExpanded)
    {
        QPalette pal = palette();
        pal.setColor(QPalette::Background, m_normalColor);
        this->setPalette(pal);
        m_icon->setPixmap(QPixmap::fromImage(m_normalIcon));
        m_hyper->setText("<a style=\"color: " + m_labelNormalColor + "; text-decoration: none;\" href=\"#\">" + m_text + "</a>");
    }
    QWidget::leaveEvent(event);
}
