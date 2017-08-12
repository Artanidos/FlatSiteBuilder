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

#ifndef HYPERLINK_H
#define HYPERLINK_H

#include <QEvent>
#include <QLabel>

class Hyperlink : public QLabel
{
    Q_OBJECT
public:
    Hyperlink(QString text);

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent (QEvent *event);
    void leaveEvent (QEvent *event);

    void setColor(QString color);
    void setHovered(bool value);
    void setAutohover(bool value);

signals:
    void clicked();

private slots:
    void linkActivated();

private:
    QString m_text;
    QString m_color;
    QString m_hover;
    bool m_autohover;
};

#endif // HYPERLINK_H
