/****************************************************************************
** Copyright (C) 2017 Olaf Japp
**
** This file is part of FlatSiteBuilder.
**
**  AnimationMaker is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  AnimationMaker is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with AnimationMaker.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "generator.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

Generator::Generator()
{
}

void Generator::addVariable(QString name, QString value)
{
    variables.insert(name, value);
}

/*
 * Syntax comes from Django
 *
 * Variable
 * {{ varname }}
 *
 * Includes
 * {% include filename %}
 *
 * Loop
 * {% for page in pages %}
 *     {{ page.title }}
 * {% endfor %}
 *
 */

QString Generator::translate(QString source)
{
    QChar ch;
    QString code = "";
    QString varname = "";
    QString output = "";
    bool inCode = false;
    bool inVar = false;

    for(int pos = 0; pos < source.length(); pos++)
    {
        ch = source.at(pos);
        if (ch == '{')
        {
            if(pos + 1 < source.length())
            {
                QChar next = source.at(pos + 1);
                if(next == "{")
                {
                    inVar = true;
                    pos++;
                }
                else if(next == "%")
                {
                    inCode = true;
                    pos++;
                }
            }
        }
        else if (ch == '}')
        {
            if(pos + 1 < source.length())
            {
                QChar next = source.at(pos + 1);
                if(next == "}")
                {
                    inVar = false;
                    pos++;
                    output += variables.value(varname.trimmed());
                    varname = "";
                }
            }
        }
        else if(ch == "%")
        {
            if(pos + 1 < source.length())
            {
                QChar next = source.at(pos + 1);
                if(next == "}")
                {
                    inCode = false;
                    pos++;
                    output += "todo";
                    code = "";
                }
            }
        }
        else
        {
            if(inVar)
            {
                varname += ch;
            }
            else if(inCode)
            {
                code += ch;
            }
            else
                output += ch;
        }
    }
    return output;
}
