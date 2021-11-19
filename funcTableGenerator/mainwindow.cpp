#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init();
    newControl = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::init()
{
    enableSelection(false);
    fileSaveAvailable = false;
    fileOpened = false;

    settings =  new settingDialog();
    codeGenerator = new codeGenerate();
    funcPtr = new functionPointerDialog();
    structure = new structureDialog();
    element = new elementDialog();
    filePrint = new filePrintForm();

    connect(ui->actionNew,SIGNAL(triggered()),this,SLOT(newOperation()));
    connect(ui->actionNew,SIGNAL(triggered()),settings,SLOT(show()));
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(open()));
    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(save()));
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(exitApp()));
    connect(settings,SIGNAL(setFileInformations(fileParams *)),codeGenerator,SLOT(filesGenerate(fileParams *)));
    connect(settings,SIGNAL(setFileInformations(fileParams *)),this,SLOT(setPrintFileInformation(fileParams *)));
    connect(funcPtr,SIGNAL(setFunctionPointerArguments(QString )),this,SLOT(gotoStructureInformation(QString )));
    connect(structure,SIGNAL(setStructureArguments(QString )),this,SLOT(gotoElementInformation(QString )));
    connect(codeGenerator,SIGNAL(codeGenerated(QString , QString)),this,SLOT(gotoFileInformation(QString , QString)));

    connect(funcPtr,SIGNAL(setFunctionPointerArguments(fucPointerArr *)),codeGenerator,SLOT(generateFunctionPointerPrototype(fucPointerArr *)));
    connect(structure,SIGNAL(setStructureArguments(structureArr *)),codeGenerator,SLOT(generateStructurePrototype(structureArr *)));
    connect(element,SIGNAL(setElements(elementArray *)),codeGenerator,SLOT(generateElementPrototype(elementArray *)));

    connect(settings,SIGNAL(setFileInformations(fileParams *)),this,SLOT(writeFileConfiguration(fileParams *)));
    connect(funcPtr,SIGNAL(setFunctionPointerArguments(fucPointerArr *)),this,SLOT(writeFuncPointerConfiguration(fucPointerArr *)));
    connect(structure,SIGNAL(setStructureArguments(structureArr *)),this,SLOT(writeStructureConfiguration(structureArr *)));
    connect(element,SIGNAL(setElements(elementArray *)),this,SLOT(writeElementConfiguration(elementArray *)));

    ui->selectionComboBox->addItem("Structure array generation with function pointer");
    ui->selectionComboBox->addItem("Structure array generation without function pointer");
}


void MainWindow::exitApp(){
    QCoreApplication::quit();
}

void MainWindow::on_selectionOKButton_clicked()
{
    switch(ui->selectionComboBox->currentIndex())
    {
    case 0: funcPtr->show();
        break;
    case 1: structure->show();
        structure->init("");
        break;
    default:
        break;
    }
    recorder->writeSelectionParam(ui->selectionComboBox->currentIndex());
}

void MainWindow::save()
{
    if(fileSaveAvailable)
    {
        QString fileNewName=QFileDialog::getSaveFileName(this,tr("Directory"),"",tr("configuration(*.cfg)"));
        QFile::copy("configuration.cfg",fileNewName);
    }
    else
    {
        QMessageBox::warning(this, tr("Save Information"),
                             tr("Firstly open or new operation must be realized!"),
                             QMessageBox::Ok);
    }
}

void MainWindow::newOperation()
{
    fileOpened = false;
}

void MainWindow::open()
{
    if(newControl)
    {
        exitApp();
        return;
    }
    bool status=false;
    fileOpened = true;
    fileSaveAvailable = true;
    QString fileNewName=QFileDialog::getOpenFileName(this,tr("Directory"),"configuration.cfg",tr("configuration(*.cfg)"));
    reader = new record();
    reader->init(fileNewName);
    status=reader->readParams();
    if(status)
    {
        if(fileNewName!= "")
        {
            QMessageBox::warning(this, tr("Open Information"),
                                 tr("File format corrupted od file empty!"),
                                 QMessageBox::Ok);
        }
        else
        {
            QMessageBox::warning(this, tr("Open Information"),
                                 tr("File not selected!"),
                                 QMessageBox::Ok);
        }
    }
    else
    {
        newControl = true;
        fileParams params=reader->getFileParamData();
        codeGenerator->filesGenerate(&params);
        writeFileConfiguration(&params);
        recorder->writeSelectionParam(reader->getSelectionData());
        filePrint->setSourceFileName(params.sourceFileName);
        filePrint->setHeaderFileName(params.HeaderFileName);

        switch(reader->getSelectionData())
        {
        case 0:
            funcPtr->show();
            funcPtr->init(reader->getFucPointerData());
            break;
        case 1:
            structure->show();
            structure->init(reader->getStructureData());
            break;
        default:
            break;
        }
    }
}

void MainWindow::enableSelection(bool select)
{
    ui->selectionComboBox->setEnabled(select);
    ui->selectionOKButton->setEnabled(select);
    ui->openGeneratedFileLocationButton->setEnabled(select);
}

void MainWindow::gotoStructureInformation(QString funcPtrName)
{
    if(fileOpened)
    {
        structure->show();
        structure->init(reader->getStructureData());
    }
    else
    {
        structure->show();
        structure->init(funcPtrName);
    }
}

void MainWindow::gotoElementInformation(QString funcPtrName)
{
    if(fileOpened)
    {
        element->show();
        element->init(reader->getElementData());
    }
    else
    {
        element->show();
        element->init(funcPtrName);
    }
}

void MainWindow::setPrintFileInformation(fileParams *data)
{
    fileSaveAvailable = true;
    if(!newControl)
    {
        newControl = true;
        enableSelection(true);
        filePrint->setSourceFileName(data->sourceFileName);
        filePrint->setHeaderFileName(data->HeaderFileName);
    }
    else
    {
        exitApp();
        return;
    }
}

void MainWindow::gotoFileInformation(QString sourceData, QString headerData)
{
    filePrint->show();
    filePrint->printSourceFile(sourceData);
    filePrint->printHeaderFile(headerData);
}

void MainWindow::on_openGeneratedFileLocationButton_clicked()
{
    QFileDialog::getOpenFileName(this,tr("Directory"),codeGenerator->getCodeGeneratedDirectory());
}

void MainWindow::writeFileConfiguration(fileParams *data)
{
    recorder = new record();
    recorder->init("configuration.cfg",true);
    recorder->writeFileParam(data);
}

void MainWindow::writeFuncPointerConfiguration(fucPointerArr *data)
{
    recorder->writeFunctionPointerParam(data);
}

void MainWindow::writeStructureConfiguration(structureArr *data)
{
    recorder->writeStructureParam(data);
}

void MainWindow::writeElementConfiguration(elementArray *data)
{
    recorder->writeElementParam(data);
}

void MainWindow::on_openFileInfoButton_clicked()
{
    if(fileOpened)
    {
         settings->init(reader->getFileParamData(),true);
    }
    else
    {
        settings->init(true);
    }
    settings->show();
}
