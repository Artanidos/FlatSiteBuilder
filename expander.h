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

#ifndef EXPANDER_H
#define EXPANDER_H

#include <QLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QScrollArea>
#include "hyperlink.h"

class Expander : public QWidget
{
    Q_OBJECT
public:
    Expander(QString header, QString normalIcon = "", QString hoveredIcon = "", QString selectedIcon = "");

    void addLayout(QLayout *l);
    void enterEvent(QEvent * event);
    void leaveEvent(QEvent * event);
    void setExpanded(bool value);

public slots:
    void buttonClicked();

signals:
    void expanded(bool value);

private:
    QWidget *m_content;
    QLabel *m_icon;
    QLabel *m_hyper;
    QString m_labelNormalColor;
    QString m_labelHoveredColor;
    QString m_labelSelectedColor;
    QImage m_normalIcon;
    QImage m_hoveredIcon;
    QImage m_selectedIcon;
    QString m_text;
    bool m_isExpanded;
};

#endif // EXPANDER_H
