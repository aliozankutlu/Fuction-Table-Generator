#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settingdialog.h"
#include "filecontrol.h"
#include "codegenerate.h"
#include "functionpointerdialog.h"
#include "structuredialog.h"
#include "elementdialog.h"
#include "fileprintform.h"
#include "record.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void init();
    settingDialog *settings;
    codeGenerate *codeGenerator;
    functionPointerDialog *funcPtr;
    structureDialog *structure;
    elementDialog *element;
    filePrintForm *filePrint;
    record *recorder;
    record *reader;
    bool fileSaveAvailable;
    bool fileOpened;
    bool newControl;
private slots:
     void exitApp();
     void on_selectionOKButton_clicked();
     void enableSelection(bool select);
     void gotoStructureInformation(QString funcPtrName);
     void gotoElementInformation(QString funcPtrName);
     void setPrintFileInformation(fileParams *data);
     void gotoFileInformation(QString sourceData, QString headerData);
     void on_openGeneratedFileLocationButton_clicked();
     void save();
     void newOperation();
     void open();
     void writeFileConfiguration(fileParams *data);
     void writeFuncPointerConfiguration(fucPointerArr *data);
     void writeStructureConfiguration(structureArr *data);
     void writeElementConfiguration(elementArray *data);
     void on_openFileInfoButton_clicked();
};

#endif // MAINWINDOW_H
