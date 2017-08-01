#include "elementeditor.h"
#include "widgetmimedata.h"
#include "flatbutton.h"
#include "moduldialog.h"
#include "roweditor.h"
#include "textdialog.h"
#include "sectioneditor.h"
#include "pageeditor.h"
#include "contenteditor.h"
#include <QMimeData>
#include <QDrag>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QTest>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>

ElementEditor::ElementEditor()
{
    setAutoFillBackground(true);
    setMinimumWidth(120);
    setMinimumHeight(50);
    setMaximumHeight(50);
    m_zoom = false;

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
    layout->addWidget(m_link, 0, Qt::AlignCenter);
    layout->addWidget(m_editButton);
    layout->addWidget(m_copyButton);
    layout->addWidget(m_text, 1, Qt::AlignCenter);
    layout->addWidget(m_closeButton);
    setLayout(layout);

    connect(m_link, SIGNAL(linkActivated(QString)), this, SLOT(enable()));
    connect(m_editButton, SIGNAL(clicked()), this, SLOT(edit()));
    connect(m_copyButton, SIGNAL(clicked()), this, SLOT(copy()));
    connect(m_closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

ElementEditor *ElementEditor::clone()
{
    ElementEditor *nee = new ElementEditor();
    nee->setMode(m_mode);
    nee->setText(m_text->text());
    return nee;
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
    WidgetMimeData *mimeData = new WidgetMimeData();
    mimeData->setData(this);

    QPixmap pixmap(this->size());
    render(&pixmap);

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
    ModulDialog *dlg = new ModulDialog();
    dlg->exec();

    switch(dlg->result())
    {
        case 1:
            m_text->setText("Text");
            break;
        case 2:
            m_text->setText("Image");
            break;
        case 3:
            m_text->setText("Slider");
            break;
        default:
            return;
    }

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

    ColumnEditor *ce = dynamic_cast<ColumnEditor*>(parentWidget());
    if(ce)
    {
        RowEditor *re = dynamic_cast<RowEditor*>(ce->parentWidget());
        if(re)
        {
            SectionEditor *se = dynamic_cast<SectionEditor*>(re->parentWidget());
            if(se)
            {
                PageEditor *pe = dynamic_cast<PageEditor*>(se->parentWidget());
                if(pe)
                {
                    QWidget *sa = dynamic_cast<QWidget*>(pe->parentWidget());
                    if(sa)
                    {
                        QWidget *vp = dynamic_cast<QWidget*>(sa->parentWidget());
                        if(vp)
                        {
                            ContentEditor *cee = dynamic_cast<ContentEditor*>(vp->parentWidget());
                            if(cee)
                                cee->elementEdit(this);
                        }
                    }
                }
            }
        }
    }

    //TextDialog *dlg = new TextDialog();
    //dlg->exec();
    /*
    if(m_zoom)
    {
        QParallelAnimationGroup *ag = new QParallelAnimationGroup();
        QPropertyAnimation *width = new QPropertyAnimation();
        width->setTargetObject(this);
        width->setDuration(300);
        width->setEndValue(120);
        width->setStartValue(900);
        width->setPropertyName("minimumWidth");
        ag->addAnimation(width);
        QPropertyAnimation *height = new QPropertyAnimation();
        height->setTargetObject(this);
        height->setDuration(300);
        height->setEndValue(50);
        height->setStartValue(500);
        height->setPropertyName("minimumHeight");
        ag->addAnimation(height);
        ag->start();
        m_zoom = false;
    }
    else
    {
        QParallelAnimationGroup *ag = new QParallelAnimationGroup();
        QPropertyAnimation *width = new QPropertyAnimation();
        width->setTargetObject(this);
        width->setDuration(300);
        width->setEndValue(900);
        width->setStartValue(120);
        width->setPropertyName("minimumWidth");
        ag->addAnimation(width);
        QPropertyAnimation *height = new QPropertyAnimation();
        height->setTargetObject(this);
        height->setDuration(300);
        height->setEndValue(500);
        height->setStartValue(50);
        height->setPropertyName("minimumHeight");
        ag->addAnimation(height);
        ag->start();
        m_zoom = true;
    }
    */
}

void ElementEditor::copy()
{
    emit elementCopied(this);
}
