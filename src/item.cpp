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

#include "item.h"
#include <QDebug>

#include "datatransfer.h"

void File::setName(QString name)
{
	this->name = name;

	detectFileType();
}

QString File::baseName()
{
	if(!m_baseName.isEmpty())
		return m_baseName;
	return name;
}

void File::detectFileType()
{
	type = File::UNDEFINED;

	for(int i = 0; i < TYPES_COUNT; i++)
	{
		QRegExp rx(getRxForFileType((File::FileTypes)i));

		if(rx.exactMatch(name))
		{
			type = (File::FileTypes)i;

			if(rx.captureCount() == 3)
			{
				m_baseName = rx.cap(2);
				version = rx.cap(3).toInt();
			}

			break;
		}
	}
}

QPixmap File::icon()
{
	if( type == File::UNDEFINED )
		detectFileType();

	if(!m_icon.isNull())
		return m_icon;

	QString s = QString(":/gfx/icons/%1.png").arg(getInternalNameForFileType(type));

	if(QFile::exists(s))
		m_icon = QPixmap(s);

	return m_icon;
}

QString File::getInternalNameForFileType(File::FileTypes type)
{
	switch(type)
	{
	case File::PRT_PROE:
		return "prt_proe";
	case File::ASM:
		return "asm";
	case File::DRW:
		return "drw";
	case File::FRM:
		return "frm";
	case File::NEU_PROE:
		return "neu_proe";
	case File::CATPART:
		return "catpart";
	case File::CATPRODUCT:
		return "catproduct";
	case File::CATDRAWING:
		return "catdrawing";
	case File::PRT_NX:
		return "prt_nx";
	case File::SLDPRT:
		return "sldprt";
	case File::SLDASM:
		return "sldasm";
	case File::SLDDRW:
		return "slddrw";
	case File::PAR:
		return "par";
	case File::PSM:
		return "psm";
	case File::DFT:
		return "dft";
	case File::IPT:
		return "ipt";
	case File::IAM:
		return "iam";
	case File::IDW:
		return "idw";
	case File::DWG:
		return "dwg";
	case File::STEP:
		return "step";
	case File::IGES:
		return "iges";
	case File::DXF:
		return "dxf";
	case File::STL:
		return "stl";
	case File::BLEND:
		return "blend";
	case File::PDF:
		return "pdf";
	case File::OFFICE_WRITER:
		return "office-document";
	case File::OFFICE_CALC:
		return "office-spreadsheet";
	case File::OFFICE_IMPRESS:
		return "office-presentation";
	case File::OFFICE_DRAW:
		return "office-drawing";
	case File::OFFICE_PROJECT:
		return "office-project";
	case File::OFFICE_BASE:
		return "office-database";
	default:
		return "undefined";
	}
}

QString File::getLabelForFileType(File::FileTypes type)
{
	switch(type)
	{
	case File::PRT_PROE:
		return "*.prt";
	case File::ASM:
		return "*.asm";
	case File::DRW:
		return "*.drw";
	case File::FRM:
		return "*.frm";
	case File::NEU_PROE:
		return "*.neu";
	case File::CATPART:
		return "*.catpart";
	case File::CATPRODUCT:
		return "*.catproduct";
	case File::CATDRAWING:
		return "*.catdrawing";
	case File::PRT_NX:
		return "*.prt";
	case File::SLDPRT:
		return "*.sldprt";
	case File::SLDASM:
		return "*.sldasm";
	case File::SLDDRW:
		return "*.slddrw";
	case File::PAR:
		return "*.par";
	case File::PSM:
		return "*.psm";
	case File::DFT:
		return "*.dft";
	case File::IPT:
		return "*.ipt";
	case File::IAM:
		return "*.iam";
	case File::IDW:
		return "*.idw";
	case File::DWG:
		return "*.dwg";
	case File::STEP:
		return "*.step";
	case File::IGES:
		return "*.iges";
	case File::DXF:
		return "*.dxf";
	case File::STL:
		return "*.stl";
	case File::BLEND:
		return "*.blend";
	case File::PDF:
		return "*.pdf";
	case File::OFFICE_WRITER:
		return QObject::tr("Office document");
	case File::OFFICE_CALC:
		return QObject::tr("Office spreadsheet");
	case File::OFFICE_IMPRESS:
		return QObject::tr("Office presentation");
	case File::OFFICE_DRAW:
		return QObject::tr("Office drawing");
	case File::OFFICE_PROJECT:
		return QObject::tr("Office project");
	case File::OFFICE_BASE:
		return QObject::tr("Office database");
	default:
		return "undefined";
	}
}

QString File::getRxForFileType(File::FileTypes type)
{
	switch(type)
	{
	case File::PRT_PROE:
		return "((^.+\\.prt)\\.(\\d+)$)";
	case File::ASM:
		return "((^.+\\.asm)\\.(\\d+)$)";
	case File::DRW:
		return "((^.+\\.drw)\\.(\\d+)$)";
	case File::FRM:
		return "((^.+\\.frm)\\.(\\d+)$)";
	case File::NEU_PROE:
		return "((^.+\\.neu)\\.(\\d+)$)";
	case File::CATPART:
		return "(^.+\\.catpart$)";
	case File::CATPRODUCT:
		return "(^.+\\.catproduct$)";
	case File::CATDRAWING:
		return "(^.+\\.catdrawing$)";
	case File::PRT_NX:
		return "(^.+\\.prt$)";
	case File::SLDPRT:
		return "(^.+\\.sldprt$)";
	case File::SLDASM:
		return "(^.+\\.sldasm$)";
	case File::SLDDRW:
		return "(^.+\\.slddrw$)";
	case File::PAR:
		return "(^.+\\.par$)";
	case File::PSM:
		return "(^.+\\.psm$)";
	case File::DFT:
		return "(^.+\\.dft$)";
	case File::IPT:
		return "(^.+\\.ipt$)";
	case File::IAM:
		return "(^.+\\.iam$)";
	case File::IDW:
		return "(^.+\\.idw$)";
	case File::DWG:
		return "(^.+\\.dwg$)";
	case File::STEP:
		return "(^.+\\.step|.+\\.stp$)";
	case File::IGES:
		return "(^.+\\.iges|.+\\.igs$)";
	case File::DXF:
		return "(^.+\\.dxf$)";
	case File::STL:
		return "(^.+\\.stl$)";
	case File::BLEND:
		return "(^.+\\.blend$)";
	case File::PDF:
		return "(^.+\\.pdf$)";
	case File::OFFICE_WRITER:
		return getRxFromStringList(QStringList() << "odt" << "ott" << "odm" << "doc" << "dot" << "docx" << "docm" << "dotx" << "dotm");
	case File::OFFICE_CALC:
		return getRxFromStringList(QStringList() << "ods" << "ots" << "xls" << "xlt" << "xlm" << "xlsx" << "xlsm" << "xltx" << "xltm" << "csv");
	case File::OFFICE_IMPRESS:
		return getRxFromStringList(QStringList() << "odp" << "otp" << "ppt" << "pot" << "pps" << "pptx" << "pptm" << "potx" << "potm" << "ppam" << "ppsx" << "ppsm" << "sldx" << "sldm");
	case File::OFFICE_DRAW:
		return getRxFromStringList(QStringList() << "odg" << "otg");
	case File::OFFICE_PROJECT:
		return getRxFromStringList(QStringList() << "mpd" << "mpp");
	case File::OFFICE_BASE:
		return getRxFromStringList(QStringList() << "odb" << "mdb" << "accdb" << "accde" << "accdt" << "accdr");
	default:
		return "";
	}
}

QString File::getRxFromStringList(const QStringList &extensions)
{
	QString ret;
	foreach(QString i, extensions)
	{
		ret += QString("(^.+\\.%1)|").arg(i);
	}
	ret.chop(1);
	return ret;
}

Item::Item()
{
	isChecked = false;
	openFtpFile = 0;
	parent = 0;
	isDir = false;
	isServer = false;
	isEmpty = true;
	server = 0;
	metadata = 0;
	hasTechSpecs = false;
	hasLoadedChildren = false;
	showText = true;
}

Item::~Item()
{
	delete openFtpFile;
	delete metadata;
	metadata = 0;
	qDeleteAll(files);
	qDeleteAll(m_thumbnails);
	qDeleteAll(children);
	files.clear();
	children.clear();
}

int Item::row() const
{
	if (parent)
		return parent->children.indexOf(const_cast<Item*>(this));

	return 0;
}

Item *Item::child(int r)
{
	return children.value(r);
}


void Item::setNotEmpty()
{
	isEmpty = false;
	if (parent)
		parent->setNotEmpty();
}

QString Item::getLabel()
{
	if(metadata)
		return metadata->getLabel();
	return name;
}

QString Item::pathRelativeToDataSource()
{
	return server->getRelativePathForItem(this);
}

void Item::addThumbnail(Thumbnail *thumb)
{
	m_thumbnails << thumb;
}

QList<Thumbnail*> Item::thumbnails(bool include)
{
	if(!include || !metadata)
		return m_thumbnails;

	QList<Thumbnail*> ret;
	ret << m_thumbnails;

	foreach(Item *it, metadata->includedThumbnailItems())
	ret << it->thumbnails();

	return ret;
}
