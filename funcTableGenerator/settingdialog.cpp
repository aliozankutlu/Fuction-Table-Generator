#include "settingdialog.h"
#include "ui_settingdialog.h"

settingDialog::settingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settingDialog)
{
    ui->setupUi(this);
    init();
}

settingDialog::~settingDialog()
{
    delete ui;
}

void settingDialog::init(bool showControl)
{
    fileInfoStatus = false;
    ui->fileNameLine->setText("");
    ui->authorLine->setText("");
    ui->fileHeaderLine->setText("");
    ui->fileSourceLine->setText("");
    ui->filePathLine->setText("");
    ui->includeHeaderFileLine->setText("");
    ui->versionLine->setText("");

    if(showControl)
    {
        ui->buttonOK->setEnabled(false);
    }
    else
    {
        ui->buttonOK->setEnabled(true);
    }

    connect(ui->fileNameLine,SIGNAL(textChanged(const QString &)),this,SLOT(setSourceAndHeaderName(const QString &)));
}

void settingDialog::init(fileParams data ,bool showControl)
{
    ui->fileNameLine->setText(data.fileName);
    ui->authorLine->setText(data.author);
    ui->fileHeaderLine->setText(data.HeaderFileName);
    ui->fileSourceLine->setText(data.sourceFileName);
    ui->filePathLine->setText(data.filePath);
    ui->includeHeaderFileLine->setText(data.includeHeaderFile);
    ui->versionLine->setText(data.version);

    if(showControl)
    {
        ui->buttonOK->setEnabled(false);
    }
    else
    {
        ui->buttonOK->setEnabled(true);
    }
}

void settingDialog::on_browseButton_clicked()
{
    ui->filePathLine->setText(QFileDialog::getExistingDirectory());
}

void settingDialog::setSourceAndHeaderName(const QString &data)
{
    QString str= data;
    if(ui->fileNameLine->text() == "")
    {
        ui->fileHeaderLine->setText("");
        ui->fileSourceLine->setText("");
    }
    else
    {
        ui->fileHeaderLine->setText(ui->fileNameLine->text()+".h");
        ui->fileSourceLine->setText(ui->fileNameLine->text()+".c");
    }
}

void settingDialog::on_buttonOK_clicked()
{
    fileParams params;
    if(ui->fileNameLine->text() == "")
    {
        QMessageBox::warning(this, tr("Setting Information"),
                             tr("Please enter file name!"),
                             QMessageBox::Ok);
    }
    else if(ui->authorLine->text() == "")
    {
        QMessageBox::warning(this, tr("Setting Information"),
                             tr("Please enter author name!"),
                             QMessageBox::Ok);
    }
    else if(ui->filePathLine->text() == "")
    {
        QMessageBox::warning(this, tr("Setting Information"),
                             tr("Please select file path!"),
                             QMessageBox::Ok);
    }
    else
    {
        fileInfoStatus = true;
        getFileInfos(&params);
        emit setFileInformations(&params);
        close();
    }
}

bool settingDialog::getFileInfos(fileParams *params)
{
    params->filePath = ui->filePathLine->text();
    params->author = ui->authorLine->text();
    params->fileName = ui->fileNameLine->text();
    params->sourceFileName = ui->fileSourceLine->text();
    params->HeaderFileName = ui->fileHeaderLine->text();
    params->includeHeaderFile = ui->includeHeaderFileLine->text();
    params->version = ui->versionLine->text();

    return fileInfoStatus;
}
