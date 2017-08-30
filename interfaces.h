#ifndef INTERFACES_H
#define INTERFACES_H

class EditorInterface
{
public:
    virtual ~EditorInterface() {}

    virtual QString name() const = 0;

};

#define EditorInterface_iid "org.flatsitebuilder.EditorInterface"

Q_DECLARE_INTERFACE(EditorInterface, EditorInterface_iid)

#endif // INTERFACES_H
