#include <QDir>
#include <QFileDialog>
#include <QDebug>

#include "addeditdatasource.h"
#include "ui_addeditdatasource.h"

AddEditDataSource::AddEditDataSource(BaseDataSource *dataSource, Actions action, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AddEditDataSource)
{
	ui->setupUi(this);

	dataSources << dataSource;
	lastDataSource = dataSource;

	ui->dataSourceComboBox->setCurrentIndex( dataSource->dataSource );
	ui->stackedWidget->setCurrentIndex( dataSource->dataSource );

	refill();

	connect(ui->fileDialogButton, SIGNAL(clicked()), this, SLOT(openFileDialog()));
	connect(ui->dataSourceComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(dataSourceTypeChanged(int)));

	connect(ui->labelLineEdit, SIGNAL(textEdited(QString)), this, SLOT(labelChangedByUser()));

	if( action == EDIT )
		setWindowTitle(tr("Edit data source"));
}

AddEditDataSource::~AddEditDataSource()
{
	delete ui;

	foreach(BaseDataSource *bs, dataSources)
		if( bs != lastDataSource )
			delete bs;
}

BaseDataSource* AddEditDataSource::dataSource()
{
	lastDataSource->label = ui->labelLineEdit->text();

	switch( lastDataSource->dataSource )
	{
	case LOCAL: {
		LocalDataSource *s = static_cast<LocalDataSource*>(lastDataSource);

		s->localPath = ui->pathLineEdit->text();

		break;
	}
	case FTP: {
		FtpDataSource *s = static_cast<FtpDataSource*>(lastDataSource);

		s->remoteHost = ui->txtHost->text();
		s->remotePort = ui->txtPort->text().toInt();
		s->remoteBaseDir = ui->txtBaseDir->text();
		s->remoteLogin = ui->txtLogin->text();
		s->remotePassword = ui->txtPass->text();

		break;
	}
	default:
		break;
	}

	return lastDataSource;
}

void AddEditDataSource::openFileDialog()
{
	ui->pathLineEdit->setText( QFileDialog::getExistingDirectory(this, tr("Select directory"), QDir::homePath()) );
}

void AddEditDataSource::dataSourceTypeChanged(int index)
{
	foreach(BaseDataSource *bs, dataSources)
	{
		if( bs->dataSource == index )
		{
			lastDataSource = bs;
			//refill();
			qDebug() << "Found already existing instance";
			return;
		}
	}

	qDebug() << "creating new instance";

	switch( index )
	{
	case LOCAL: {
		LocalDataSource *s = new LocalDataSource();
		s->lwItem = lastDataSource->lwItem;

		dataSources << s;
		lastDataSource = s;
		break;
	}
	case FTP: {
		FtpDataSource *s = new FtpDataSource();
		s->lwItem = lastDataSource->lwItem;

		dataSources << s;
		lastDataSource = s;
		break;
	}
	default:
		break;
	}
}

void AddEditDataSource::refill()
{
	ui->labelLineEdit->setText(lastDataSource->label);

	switch( lastDataSource->dataSource )
	{
	case LOCAL: {
		LocalDataSource *s = static_cast<LocalDataSource*>(lastDataSource);

		if( s->localPath != s->label )
			labelChangedByUser();

		ui->pathLineEdit->setText( s->localPath );

		break;
	}
	case FTP: {
		FtpDataSource *s = static_cast<FtpDataSource*>(lastDataSource);

		if( s->remoteHost != s->label )
			labelChangedByUser();

		ui->txtHost->setText(s->remoteHost);
		ui->txtPort->setText(QString::number(s->remotePort));
		ui->txtBaseDir->setText(s->remoteBaseDir);
		ui->txtLogin->setText(s->remoteLogin);
		ui->txtPass->setText(s->remotePassword);
		ui->checkPassive->setChecked(s->ftpPassiveMode);
	}
	default:
		break;
	}
}

void AddEditDataSource::labelChangedByUser()
{
	disconnect(ui->pathLineEdit, SIGNAL(textChanged(QString)), ui->labelLineEdit, SLOT(setText(QString)));
	disconnect(ui->txtHost, SIGNAL(textChanged(QString)), ui->labelLineEdit, SLOT(setText(QString)));
}
