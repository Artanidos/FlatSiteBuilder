#include "elementeditor.h"
#include "droparea.h"
#include "elementeditormimedata.h"
#include "flatbutton.h"
#include "roweditor.h"
#include <QMimeData>
#include <QDrag>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QTest>

ElementEditor::ElementEditor()
{
    setAutoFillBackground(true);
    setMinimumWidth(120);
    setMinimumHeight(50);
    setMaximumHeight(50);
    m_mode = Mode::Empty;
    m_normalColor = QColor(palette().base().color().name()).lighter().name();
    m_enabledColor = palette().base().color().name();
    m_dropColor = QColor(palette().base().color().name()).lighter().lighter().name();
    setColor(m_normalColor);
    m_link = new Hyperlink("(+) Insert Module");

    m_editButton = new FlatButton(":/images/edit_normal.png", ":/images/edit_hover.png");
    m_copyButton = new FlatButton(":/images/copy_normal.png", ":/images/copy_hover.png");
    m_closeButton = new FlatButton(":/images/close_normal.png", ":/images/close_hover.png");
    m_editButton->setVisible(false);
    m_copyButton->setVisible(false);
    m_closeButton->setVisible(false);
    m_text = new QLabel("Text");
    m_text->setVisible(false);
    QHBoxLayout *layout= new QHBoxLayout();
    layout->addWidget(m_link);
    layout->addWidget(m_editButton);
    layout->addWidget(m_copyButton);
    layout->addWidget(m_text, 1);
    layout->addWidget(m_closeButton);
    setLayout(layout);

    connect(m_link, SIGNAL(linkActivated(QString)), this, SLOT(enable()));
    connect(m_editButton, SIGNAL(clicked()), this, SLOT(edit()));
    connect(m_copyButton, SIGNAL(clicked()), this, SLOT(copy()));
    connect(m_closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

void ElementEditor::setColor(QString name)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(name));
    setPalette(pal);
}

void ElementEditor::mousePressEvent(QMouseEvent *event)
{
    if(m_mode != Mode::Enabled)
        return;
    if(parentWidget()->layout()->count() == 1)
    {
        emit elementDragged();
    }
    ElementEditorMimeData *mimeData = new ElementEditorMimeData();
    mimeData->setData(this);

    QPixmap pixmap(this->size());
    this->render(&pixmap);
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(event->pos());
    drag->setPixmap(pixmap);
    this->hide();

    if(drag->exec(Qt::MoveAction) == Qt::IgnoreAction)
        this->show();
}

void ElementEditor::dropped()
{
    //seems to be a bug that after dropping the item the bgcolor changes
    setColor(m_enabledColor);
}

void ElementEditor::setMode(Mode mode)
{
    m_mode = mode;
    if(mode == Mode::Empty)
    {
        m_link->setVisible(true);
        m_editButton->setVisible(false);
        m_copyButton->setVisible(false);
        m_closeButton->setVisible(false);
        m_text->setVisible(false);
        setColor(m_normalColor);
    }
    else if(mode == Mode::Enabled)
    {
        m_link->setVisible(false);
        m_editButton->setVisible(true);
        m_copyButton->setVisible(true);
        m_closeButton->setVisible(true);
        m_text->setVisible(true);
        m_text->setText("Text");
        setColor(m_enabledColor);
    }
    else if(mode == Mode::Dropzone)
    {
        m_link->setVisible(false);
        m_editButton->setVisible(false);
        m_copyButton->setVisible(false);
        m_closeButton->setVisible(false);
        m_text->setVisible(true);
        m_text->setText("Drop Here");
        setColor(m_dropColor);
    }
}

void ElementEditor::enable()
{
    setMode(Mode::Enabled);
    emit elementEnabled();
}

void ElementEditor::close()
{
    parentWidget()->layout()->removeWidget(this);
    delete this;
}

void ElementEditor::edit()
{
    qDebug() << "edit";
}

void ElementEditor::copy()
{
    //ElementEditor *ee = new ElementEditor();
    //m_container->insertWidget(m_container->count() - 1, ee, 0, Qt::AlignTop);
    //ee->setContainer(m_container);
}
