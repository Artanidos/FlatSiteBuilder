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
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>

DefaultThemeEditor::DefaultThemeEditor() :
    UndoableEditor("Default Theme Editor", "")
{
    m_sampleproperty = new QLineEdit;
    m_showPoweredBy = new QCheckBox("Show powered by FlatSiteBuilder in footer");

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addStretch();

    m_layout->addWidget(new QLabel("Sample Property"), 1, 0);
    m_layout->addWidget(m_sampleproperty, 2, 0, 1, 3);
    m_layout->addWidget(m_showPoweredBy, 3, 0, 1, 3);
    m_layout->addLayout(vbox, 4, 0); // for stretching only

	connect(m_sampleproperty, SIGNAL(textChanged(QString)), this, SLOT(sampleChanged()));
	connect(m_showPoweredBy, SIGNAL(stateChanged(int)), this, SLOT(showPoweredChanged()));
}

void DefaultThemeEditor::sampleChanged()
{
    //if(m_site->title() != m_sampleproperty->text())
        contentChanged("sample property changed");
}

void DefaultThemeEditor::showPoweredChanged()
{
    //if(m_site->title() != m_showPoweredBy->checkState())
        contentChanged("show powered by changed");
}

void DefaultThemeEditor::load()
{
    QFile theme(m_filename);
    if (!theme.open(QIODevice::ReadOnly))
    {
        //m_win->statusBar()->showMessage("Unable to open " + filename);
        return;
    }
    QXmlStreamReader xml(&theme);
    if(xml.readNextStartElement())
    {
        if(xml.name() == "Settings")
        {
            m_sampleproperty->setText(xml.attributes().value("sampleProperty").toString());
            m_showPoweredBy->setChecked(xml.attributes().value("showPoweredBy").toString() == "true");
        }
    }
    theme.close();
}

void DefaultThemeEditor::save()
{
    QFile file(m_filename);
    if(!file.open(QFile::WriteOnly))
    {
        //m_win->statusBar()->showMessage("Unable to open file " + m_sourcePath + "/Site.xml");
        return;
    }
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("Settings");
    xml.writeAttribute("sampleProperty", m_sampleproperty->text());
    xml.writeAttribute("showPoweredBy", (m_showPoweredBy->isChecked() ? "true" : "false"));
    xml.writeEndElement();
    xml.writeEndDocument();
    file.close();
}
