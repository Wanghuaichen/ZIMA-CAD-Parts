/*
  ZIMA-CAD-Parts
  http://www.zima-construction.cz/software/ZIMA-Parts

  Copyright (C) 2011-2012 Jakub Skokan <aither@havefun.cz>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LOCALDATASOURCE_H
#define LOCALDATASOURCE_H

#include <QThread>
#include "basedatasource.h"

class LocalCopier : public QThread
{
Q_OBJECT
public:
	LocalCopier(QList<File*> files = QList<File*>());
	LocalCopier(File* file);
	void run();
	void addFiles(QList<File*> files);
	void addFile(File* file);

private:
	QList<File*> files;
	QString target;

signals:
	void aboutToCopy(File *file);
	void fileCopied(File *file);
	void done();
	void progress(File*);
};

class LocalDataSource : public BaseDataSource
{
	Q_OBJECT
public:
	explicit LocalDataSource(QObject *parent = 0);
	QString internalName();
	QString pathToDataRoot();
	QString getTechSpecPathForItem(Item *item);
	QString getPathForItem(Item *item);
	QString getRelativePathForItem(Item *item);
	QString name();

	QString localPath;

public slots:
	void loadRootItem(Item *item);
	void loadDirectory(Item* item);
	void deleteFiles(QList<File*> files);
	void addFileToDownload(File *f);
	void downloadFiles(QList<File*> files, QString dir);
	void downloadFile(File* file);
	void resumeDownload();
	void abort();
	void loadSettings(QSettings& settings);
	void saveSettings(QSettings& settings);
	void assignTechSpecUrlToItem(QString url, Item *item, QString lang, bool overwrite = false);
	void assignPartsIndexUrlToItem(QString url, Item *item, QString lang, bool overwrite = false);

protected:
	void loadItemLogo(Item *item);

	LocalCopier *copier;

protected slots:
	void aboutToCopy(File *file);

};

#endif // LOCALDATASOURCE_H
