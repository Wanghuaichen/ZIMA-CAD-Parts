#ifndef ADDEDITDATASOURCE_H
#define ADDEDITDATASOURCE_H

#include <QDialog>
#include <QList>
#include "basedatasource.h"
#include "ftpdatasource.h"
#include "localdatasource.h"

namespace Ui {
class AddEditDataSource;
}

class AddEditDataSource : public QDialog
{
	Q_OBJECT

public:
	enum Actions {
		ADD=0,
		EDIT
	};

	explicit AddEditDataSource(BaseDataSource *dataSource, Actions action, QWidget *parent = 0);
	~AddEditDataSource();

	BaseDataSource *dataSource();

private:
	void refill();

	Ui::AddEditDataSource *ui;
	QList<BaseDataSource*> dataSources;
	BaseDataSource *lastDataSource;
private slots:
	void openFileDialog();
	void dataSourceTypeChanged(int index);
	void labelChangedByUser();
};

#endif // ADDEDITDATASOURCE_H
