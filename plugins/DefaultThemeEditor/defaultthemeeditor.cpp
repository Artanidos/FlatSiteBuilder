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

#include "defaultthemeeditor.h"
#include "flatbutton.h"
#include "mainwindow.h"
#include <QLineEdit>
#include <QStatusBar>
#include <QGridLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>

DefaultThemeEditor::DefaultThemeEditor()
{
    m_hidePoweredBy = new QCheckBox("Hide powered by FlatSiteBuilder in footer");
    m_titleLabel->setText("Default Theme Settings");
    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addStretch();

    m_layout->addWidget(m_hidePoweredBy, 1, 0, 1, 3);
    m_layout->addWidget(new QLabel(""), 2, 0);
    m_layout->addWidget(new QLabel("Looks a bit empty here, right?"), 3, 0, 1, 3);
    m_layout->addWidget(new QLabel("This is just a sample theme editor."), 4, 0, 1, 3);
    m_layout->addWidget(new QLabel("To give theme creators an overview of what is possible."), 5, 0, 1, 3);
    m_layout->addLayout(vbox, 6, 0); // for stretching only

    connect(m_hidePoweredBy, SIGNAL(stateChanged(int)), this, SLOT(showPoweredChanged()));
}

void DefaultThemeEditor::showPoweredChanged()
{
    if(m_isHidePoweredByEnabled != m_hidePoweredBy->isChecked())
        contentChanged("show powered by changed");
}

void DefaultThemeEditor::load()
{
    // set default values in case load failes
    m_isHidePoweredByEnabled = false;

    QFile theme(m_filename);
    if (!theme.open(QIODevice::ReadOnly))
    {
        m_win->statusBar()->showMessage("Unable to open " + m_filename);
        return;
    }
    QXmlStreamReader xml(&theme);
    if(xml.readNextStartElement())
    {
        if(xml.name() == "Settings")
        {
            m_isHidePoweredByEnabled = xml.attributes().value("hidePoweredBy").toString() == "true";
            m_hidePoweredBy->setChecked(m_isHidePoweredByEnabled);
        }
    }
    theme.close();
}

void DefaultThemeEditor::save()
{
    QFile file(m_filename);
    if(!file.open(QFile::WriteOnly))
    {
        m_win->statusBar()->showMessage("Unable to open file " + m_filename);
        return;
    }
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("Settings");
    xml.writeAttribute("hidePoweredBy", (m_hidePoweredBy->isChecked() ? "true" : "false"));
    xml.writeEndElement();
    xml.writeEndDocument();
    file.close();

    m_win->statusBar()->showMessage("Theme settings have been saved.");
}

QVariantMap DefaultThemeEditor::themeVars()
{
    load();
    QVariantMap vars;
    vars["hidePoweredBy"] = m_isHidePoweredByEnabled;
    return vars;
}
