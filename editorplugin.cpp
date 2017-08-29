#include "editorplugin.h"

EditorPlugin::EditorPlugin()
{
    m_changed = false;
    setResizeMode(QQuickWidget::SizeRootObjectToView);
    setAttribute(Qt::WA_AlwaysStackOnTop, true);
}
