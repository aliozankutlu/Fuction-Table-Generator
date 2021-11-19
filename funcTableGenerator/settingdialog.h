#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include "QFileDialog"
#include "QMessageBox"
#include "QFile"

struct fileParams
{
    QString filePath;
    QString author;
    QString fileName;
    QString sourceFileName;
    QString HeaderFileName;
    QString includeHeaderFile;
    QString version;
};

namespace Ui {
class settingDialog;
}

class settingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit settingDialog(QWidget *parent = 0);
    ~settingDialog();
    bool getFileInfos(fileParams *params);
    void init(bool showControl=false);
    void init(fileParams data,bool showControl=false);
signals:
    void setFileInformations(fileParams *params);

private slots:
    void on_browseButton_clicked();
    void setSourceAndHeaderName(const QString &);
    void on_buttonOK_clicked();

private:
    Ui::settingDialog *ui;
    bool fileInfoStatus;

};

#endif // SETTINGDIALOG_H
