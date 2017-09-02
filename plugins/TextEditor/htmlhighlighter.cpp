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

#include "htmlhighlighter.h"

HtmlHighlighter::HtmlHighlighter(QTextDocument *document)
    : QSyntaxHighlighter(document)
{
    QTextCharFormat entityFormat;
    entityFormat.setForeground(QColor(0, 128, 0));
    entityFormat.setFontWeight(QFont::Normal);
    setFormatFor(Entity, entityFormat);

    QTextCharFormat tagFormat;
    tagFormat.setForeground(QColor("#f0e68c"));
    tagFormat.setFontWeight(QFont::Normal);
    setFormatFor(Tag, tagFormat);

    QTextCharFormat commentFormat;
    commentFormat.setForeground(QColor("#87ceeb"));
    commentFormat.setFontItalic(true);
    setFormatFor(Comment, commentFormat);

    QTextCharFormat codeFormat;
    codeFormat.setForeground(QColor("#ff9e00"));
    setFormatFor(Code, codeFormat);
}

void HtmlHighlighter::setFormatFor(Construct construct, const QTextCharFormat &format)
{
    m_formats[construct] = format;
    rehighlight();
}


void HtmlHighlighter::highlightBlock(const QString &text)
{
    int state = previousBlockState();
    int len = text.length();
    int start = 0;
    int pos = 0;

    while (pos < len)
    {
        switch (state)
        {
            case NormalState:
            default:
                while (pos < len)
                {
                    QChar ch = text.at(pos);
                    if (ch == '<')
                    {
                        if (text.mid(pos, 4) == "<!--")
                        {
                            state = InComment;
                        }
                        else
                        {
                            state = InTag;
                        }
                        break;
                    }
                    else if (ch == '&')
                    {
                        start = pos;
                        while (pos < len && text.at(pos++) != ';')
                            ;
                        setFormat(start, pos - start, m_formats[Entity]);
                    }
                    else if(ch == "{")
                    {
                        if(text.mid(pos, 2) == "{{")
                        {
                            state = InVar;
                            break;
                        }
                        else if(text.mid(pos, 2) == "{%")
                        {
                            state = InLoop;
                            break;
                        }
                        pos++;
                        break;
                    }
                    else
                    {
                        pos++;
                    }
                }
                break;
            case InVar:
                start = pos;
                while (pos < len)
                {
                    if(text.mid(pos, 2) == "}}")
                    {
                        pos += 2;
                        state = NormalState;
                        break;
                    }
                    else
                        pos++;
                }
                setFormat(start, pos - start, m_formats[Code]);
                break;

            case InLoop:
                start = pos;
                while (pos < len)
                {
                    if(text.mid(pos, 2) == "%}")
                    {
                        pos += 2;
                        state = NormalState;
                        break;
                    }
                    else
                        pos++;
                }
                setFormat(start, pos - start, m_formats[Code]);
                break;

            case InComment:
                start = pos;
                while (pos < len)
                {
                    if (text.mid(pos, 3) == "-->")
                    {
                        pos += 3;
                        state = NormalState;
                        break;
                    }
                    else
                    {
                        pos++;
                    }
                }
                setFormat(start, pos - start, m_formats[Comment]);
                break;

            case InTag:
                QChar quote = QChar::Null;
                start = pos;
                while (pos < len)
                {
                    QChar ch = text.at(pos);
                    if (quote.isNull())
                    {
                        if (ch == "\"" || ch == '"')
                        {
                            quote = ch;
                        }
                        else if (ch == '>')
                        {
                            pos++;
                            state = NormalState;
                            break;
                        }
                    }
                    else if (ch == quote)
                    {
                        quote = QChar::Null;
                    }
                    pos++;
                }
                setFormat(start, pos - start, m_formats[Tag]);
                break;
        }
    }

    setCurrentBlockState(state);
}
