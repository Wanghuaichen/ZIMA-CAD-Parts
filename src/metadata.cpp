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

#include <QRegExp>
#include <QDir>
#include <QDebug>

#include "metadata.h"
#include "settings.h"
#include "item.h"


MetadataCache * MetadataCache::m_instance = 0;


MetadataCache * MetadataCache::get()
{
    if (!m_instance)
        m_instance = new MetadataCache();
    return m_instance;
}

MetadataCache::MetadataCache()
{
}

MetadataCache::~MetadataCache()
{
    clear();
    delete m_instance;
    m_instance = 0;
}

void MetadataCache::clear()
{
    qDeleteAll(m_map);
    m_map.clear();
}

void MetadataCache::load(const QString &path)
{
    qDebug() << "MCACHE" << path;
    if (m_map.contains(path))
        m_map[path]->deleteLater();
    m_map[path] = new Metadata(path);
}

QStringList MetadataCache::columnLabels(const QString &path)
{
    if (!m_map.contains(path))
        load(path);
    return m_map[path]->columnLabels();
}

QString MetadataCache::partParam(const QString &path, const QString &fname, int column)
{
    if (!m_map.contains(path))
        load(path);
    return m_map[path]->partParam(fname, column);
}

QPixmap* MetadataCache::partThumbnail(const QString &path, const QString fname)
{
    if (!m_map.contains(path))
        load(path);
    qDebug() << 1 << path << m_map[path] << fname;
    QString thumb(m_map[path]->partThumbnailPath(fname));
    qDebug() << thumb;
#warning todo QPixmapCache?
    if (thumb.isEmpty())
        return 0;
    else
        return new QPixmap(thumb);
}


Metadata::Metadata(const QString &path, QObject *parent)
	: QObject(parent),
      m_path(path),
	  m_loadedIncludes(0),
	  m_includedData(false)
{
    qDebug() << "Metadata constructor for" << path;
    m_settings = new QSettings(m_path + "/" + TECHSPEC_DIR + "/" + METADATA_FILE, QSettings::IniFormat);
    m_settings->setIniCodec("utf-8");
    qDebug() << "ini file" << m_settings->fileName();

    m_currentAppLang = Settings::get()->getCurrentLanguageCode().left(2);

    m_settings->beginGroup("params");
    {
        foreach(QString group, m_settings->childGroups())
        {
            if(group == m_currentAppLang)
            {
                lang = group;
                break;
            }
        }

        if(lang.isEmpty())
        {
            QStringList childGroups = m_settings->childGroups();

            if(childGroups.contains("en"))
                lang = "en";
            else if(childGroups.count())
                lang = childGroups.first();
            else {
                m_settings->endGroup();
                return;
            }
        }
    }
    m_settings->endGroup();

    m_settings->beginGroup("include");
    {
        QStringList toInclude;
        QStringList data = buildIncludePaths(m_settings->value("data").toStringList());
        QStringList thumbs = buildIncludePaths(m_settings->value("thumbnails").toStringList());

        toInclude << data << thumbs;

        toInclude.removeDuplicates();

        if(!data.isEmpty())
            m_includedData = true;

        foreach(QString path, toInclude)
            includes << new Metadata(path, this);
    }
    m_settings->endGroup();
}

Metadata::~Metadata()
{
    delete m_settings;

    m_columnLabels.clear();
    label.clear();
    lang.clear();
}

QString Metadata::getLabel()
{
	if(label.count())
		return label;

    return (label = m_settings->value(QString("params/%1/label").arg(lang), QString()).toString());
}

QStringList Metadata::columnLabels()
{
    if(m_columnLabels.count())
        return m_columnLabels;

	QRegExp colRx("^\\d+$");
	int colIndex = 1;

    m_settings->beginGroup("params");
	{
        m_settings->beginGroup(lang);
		{
            foreach(QString col, m_settings->childKeys())
			{
				if(colRx.exactMatch(col))
				{
                    m_columnLabels << m_settings->value( QString("%1").arg(colIndex++) ).toString();
				}
			}
		}
        m_settings->endGroup();
	}
    m_settings->endGroup();

	foreach(Metadata *include, includes)
        m_columnLabels << include->columnLabels();

    return m_columnLabels;
}

QString Metadata::partParam(const QString &partName, int col)
{
    QString partGroup = partName.section('.', 0, 0);
	QString anyVal;
	QString val;

    m_settings->beginGroup(partGroup);

    foreach(QString group, m_settings->childGroups())
	{
        if(!(val = m_settings->value(QString("%1/%2").arg(group).arg(col)).toString()).isEmpty() && group == m_currentAppLang)
			break;

		if(anyVal.isEmpty())
			anyVal = val;
	}

    m_settings->endGroup();

	if(!val.isEmpty())
		return val;
	else {

        QString ret = anyVal.isEmpty() ? m_settings->value(QString("%1/%2").arg(partGroup).arg(col), QString()).toString() : anyVal;

		if(ret.isEmpty())
		{
			foreach(Metadata *include, includes)
			{
                QString tmp = include->partParam(partName, col);

				if(!tmp.isEmpty())
					return tmp;
			}
		}

		return ret;
	}
}

QString Metadata::partThumbnailPath(const QString &partName)
{
    QFileInfo fi(partName);
    QDir d(m_path);
    QString base(fi.baseName());
    QStringList thumbs = d.entryList(QStringList() << base+".png" << base+".jpg",
                                     QDir::Files | QDir::Readable);

    if (thumbs.size())
        return m_path + "/" + thumbs.at(0);

    foreach(Metadata *include, includes)
    {
        QString ret = include->partThumbnailPath(partName);
        if (!ret.isEmpty())
            return ret;
    }

    return QString();
}

void Metadata::deletePart(const QString &part)
{
	QString grp = part.section('.', 0, 0);

	if(grp.isEmpty())
		return;

    m_settings->remove(grp);
}

void Metadata::retranslate(QString lang)
{
	if(lang.isEmpty())
        m_currentAppLang = Settings::get()->getCurrentLanguageCode().left(2);
	else
        m_currentAppLang = lang;

#warning	refresh();
}

QString Metadata::buildIncludePath(const QString &raw)
{
	if(raw.startsWith('/'))
        return QDir::cleanPath(raw);
	else
        Q_ASSERT(0);
}

QStringList Metadata::buildIncludePaths(const QStringList &raw)
{
	QStringList ret;

	foreach(QString s, raw)
	ret << buildIncludePath(s);

	return ret;
}
