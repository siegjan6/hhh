#ifndef EDITORFORM_H
#define EDITORFORM_H


#include "BaseForm.h"
#include "WebContainer.h"

class CEditorForm : public CBaseForm
{
    Q_OBJECT
public:
    explicit CEditorForm(QString filePath, const QList< QPair<QString, QString> >& objs, QWidget *parent = 0);
    explicit CEditorForm(QString content, bool isContent, const QList<QPair<QString, QString> > &objs, QWidget *parent = 0);

public:
    virtual bool save();

protected:
    virtual void closeEvent(QCloseEvent *);

private:
    CWebContainer* m_webview;
    QString m_filePath;
    QString m_fileName;
signals:

public slots:
    void onSetModify(bool modify);
};

#endif // EDITORFORM_H
