#include "texteditor.h"
#include "htmlhighlighter.h"
#include <QGridLayout>
#include <QTabWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>

TextEditor::TextEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(13);

    QGridLayout *grid = new QGridLayout();
    grid->setMargin(0);
    QTabWidget *tab = new QTabWidget();
    m_text = new QTextEdit();
    m_html = new QTextEdit();
    m_html->setFont(font);
    m_html->setAcceptRichText(false);
    m_html->setLineWrapMode(QTextEdit::NoWrap);
    QFontMetrics metrics(font);
    m_html->setTabStopWidth(4 * metrics.width(' '));
    new HtmlHighlighter(m_html->document());

    QPushButton *save = new QPushButton("Save and Exit");
    QPushButton *cancel = new QPushButton("Cancel");
    QHBoxLayout *hbox = new QHBoxLayout();

    QLabel *titleLabel = new QLabel("Edit Text Module");
    QFont fnt = titleLabel->font();
    fnt.setPointSize(16);
    fnt.setBold(true);
    titleLabel->setFont(fnt);

    hbox->addStretch();
    hbox->addWidget(save);
    hbox->addWidget(cancel);
    tab->addTab(m_text, "Visual");
    tab->addTab(m_html, "HTML");
    grid->addWidget(titleLabel, 0, 0);
    grid->addWidget(tab, 1, 0);
    grid->addLayout(hbox, 2, 0);
    setLayout(grid);

    connect(save, SIGNAL(clicked(bool)), this, SLOT(save()));
    connect(cancel, SIGNAL(clicked(bool)), this, SLOT(cancel()));
}

void TextEditor::save()
{
   emit close(this);
}

void TextEditor::cancel()
{
    emit close(this);
}
