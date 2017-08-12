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

#include "hyperlink.h"
#include <QMouseEvent>

Hyperlink::Hyperlink(QString text)
{
    m_autohover = true;
    m_text = text;
    m_color = palette().link().color().name();
    m_hover = palette().highlight().color().name();
    setText("<a style=\"color: " + m_color + "; text-decoration: none; cursor: pointer;\" href=\"#/\">"+ m_text + "</a>");
    setTextFormat(Qt::RichText);
    setTextInteractionFlags(Qt::TextBrowserInteraction);
    setCursor(Qt::PointingHandCursor);
    connect(this, SIGNAL(linkActivated(QString)), this, SLOT(linkActivated()));
}

void Hyperlink::linkActivated()
{
    emit clicked();
}

void Hyperlink::mousePressEvent(QMouseEvent *event)
{
    event->accept();
}

void Hyperlink::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
    emit clicked();
}

void Hyperlink::enterEvent (QEvent *)
{
    if(m_autohover)
        setText("<a style=\"color: " + m_hover + "; text-decoration: none; cursor: pointer;\" href=\"#/\">"+ m_text + "</a>");
}

void Hyperlink::leaveEvent (QEvent *)
{
    if(m_autohover)
        setText("<a style=\"color: " + m_color + "; text-decoration: none; cursor: pointer;\" href=\"#/\">"+ m_text + "</a>");
}

void Hyperlink::setColor(QString color)
{
    m_color = color;
    setText("<a style=\"color: " + m_color + "; text-decoration: none; cursor: pointer;\" href=\"#/\">"+ m_text + "</a>");
}

void Hyperlink::setHovered(bool hovered)
{
    setText("<a style=\"color: " + (hovered ? m_hover : m_color) + "; text-decoration: none; cursor: pointer;\" href=\"#/\">"+ m_text + "</a>");
}

void Hyperlink::setAutohover(bool value)
{
    m_autohover = value;
}
