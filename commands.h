#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>

class AddItemCommand : public QUndoCommand
{
public:
    AddItemCommand(qreal x, qreal y, AnimationScene::EditMode mode, QString fileName, AnimationScene *scene, QUndoCommand *parent = 0);
    ~AddItemCommand();
    void undo() override;
    void redo() override;

private:

};

class DeleteItemCommand : public QUndoCommand
{
public:
    DeleteItemCommand(ResizeableItem *item, AnimationScene *scene, QUndoCommand *parent = 0);
    ~DeleteItemCommand();
    void undo() override;
    void redo() override;

private:
    ResizeableItem *m_item;
    AnimationScene *m_scene;
};

#endif // COMMANDS_H
