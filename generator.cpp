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
