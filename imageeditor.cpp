#include "imageeditor.h"
#include "imageselector.h"
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QComboBox>
#include <QTest>

ImageEditor::ImageEditor()
    : AbstractEditor()
{
    m_temp = "";
    QGridLayout *grid = new QGridLayout();
    grid->setMargin(0);
    QPushButton *save = new QPushButton("Save and Exit");
    QPushButton *cancel = new QPushButton("Cancel");
    QHBoxLayout *hbox = new QHBoxLayout();

    QLabel *titleLabel = new QLabel("Image Module");
    QFont fnt = titleLabel->font();
    fnt.setPointSize(16);
    fnt.setBold(true);
    titleLabel->setFont(fnt);
    m_image = new ImageSelector();
    m_image->setPixmap(QPixmap::fromImage(QImage(":/images/image_placeholder.png")));
    QVBoxLayout *imageVBox = new QVBoxLayout();
    QHBoxLayout *imageHBox = new QHBoxLayout();
    imageHBox->addStretch();
    imageHBox->addWidget(m_image);
    imageHBox->addStretch();
    imageVBox->addStretch();
    imageVBox->addLayout(imageHBox);
    imageVBox->addStretch();
    hbox->addStretch();
    hbox->addWidget(save);
    hbox->addWidget(cancel);
    m_animationCombo = new QComboBox();
    m_animationCombo->addItem("None", "none");
    m_animationCombo->addItem("Fade In", "fadeIn");
    m_animationCombo->addItem("Fade In Up", "fadeInUp");
    m_animationCombo->addItem("Fade In Down","fadeInDown");
    m_animationCombo->addItem("Fade In Left","fadeInLeft");
    m_animationCombo->addItem("Fade In Right","fadeInRight");
    m_animationCombo->addItem("Fade In Up Big","fadeInUpBig");
    m_animationCombo->addItem("Fade In Down Big","fadeInDownBig");
    m_animationCombo->addItem("Fade In Left Big","fadeInLeftBig");
    m_animationCombo->addItem("Fade In Right Big","fadeInRightBig");
    m_animationCombo->addItem("Bounce In","bounceIn");
    m_animationCombo->addItem("Bounce In Up","bounceInUp");
    m_animationCombo->addItem("Bounce In Down","bounceInDown");
    m_animationCombo->addItem("Bounce In Left","bounceInLeft");
    m_animationCombo->addItem("Bounce In Right","bounceInRight");
    m_animationCombo->addItem("Rotate In","rotateIn");
    m_animationCombo->addItem("Rotate In Up Left","rotateInUpLeft");
    m_animationCombo->addItem("Rotate In Down Left","rotateInDownLeft");
    m_animationCombo->addItem("Rotate In Up Right","rotateInUpRight");
    m_animationCombo->addItem("Rotate In Down Right","rotateInDownRight");
    m_animationCombo->addItem("Flash","flash");
    m_animationCombo->addItem("Shake","shake");
    m_animationCombo->addItem("Bounce","bounce");
    m_animationCombo->addItem("Tada","tada");
    m_animationCombo->addItem("Swing","swing");
    m_animationCombo->addItem("Wobble","wobble");
    m_animationCombo->addItem("Wiggle","wiggle");
    grid->addWidget(titleLabel, 0, 0);
    grid->addWidget(new QLabel("Animation"), 1, 0);
    grid->addWidget(m_animationCombo, 2, 0);
    grid->addLayout(imageVBox, 3, 0, 1, 3);
    grid->addLayout(hbox, 4, 0, 1, 3);
    setLayout(grid);

    connect(m_animationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(contentChanged()));
    connect(m_image, SIGNAL(clicked()), this, SLOT(seek()));
    connect(save, SIGNAL(clicked(bool)), this, SLOT(save()));
    connect(cancel, SIGNAL(clicked(bool)), this, SLOT(cancel()));
}

void ImageEditor::setContent(QDomElement element)
{
    m_element = element;
    QString src = m_element.attribute("src", "");
    m_temp = src;
    if(!src.isEmpty())
        m_image->setPixmap(QPixmap::fromImage(QImage(src)));
    QString anim = m_element.attribute("animation", "none");
    int index = m_animationCombo->findData(anim, Qt::UserRole);
    m_animationCombo->setCurrentIndex(index);
}

void ImageEditor::seek()
{
    QString fileName;
    QFileDialog *dialog = new QFileDialog();
    dialog->setFileMode(QFileDialog::AnyFile);
    dialog->setNameFilter(tr("Images (*.png)(*.gif)(*.jpg);;All (*)"));
    dialog->setWindowTitle(tr("Load Image"));
    dialog->setOption(QFileDialog::DontUseNativeDialog, true);
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    //dialog->setDirectory(m_defaultPath);
    if(dialog->exec())
        fileName = dialog->selectedFiles().first();
    delete dialog;
    if (fileName.isEmpty())
        return;

    QFileInfo info(fileName);
    m_temp = m_site->path() + "/assets/images/" + info.fileName();
    QFile::copy(fileName, m_temp);
    m_image->setPixmap(QPixmap::fromImage(QImage(m_temp)));
    contentChanged();
}

void ImageEditor::save()
{
    if(m_changed)
    {
        if(m_element.isNull())
        {
            QDomDocument doc;
            m_element = doc.createElement("Image");
        }
        m_element.setAttribute("src", m_temp);
        m_element.setAttribute("animation", m_animationCombo->currentData(Qt::UserRole).toString());
    }
    emit close(this);
}

void ImageEditor::cancel()
{
    m_changed = false;
    emit close(this);
}
