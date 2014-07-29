#ifndef SETTINGS_H
#define SETTINGS_H

#include <QHash>
#include "basedatasource.h"
#include "filefilters/filtergroup.h"


/**
 * \brief The Settings singleton.
 * All settings I/O is handled in this class. The singleton is created
 * in the main() function. Also the final Settings::save() is called
 * in main() function.
 *
 * The access to settings is done with Settings::get()
 * Example:
 *  Settings::get()->WorkingDir
 *
 * Most settings are raw attributes without getter/setter.
 * Some settings where it makes sense are wrapped with getter/setter.
 */
class Settings
{
public:
	enum Languages {
	    DETECT=0,
	    ENGLISH,
	    CZECH
	};

	//! The main access method to settings
	static Settings *get();

	//! Write settings to disk
	void save();

	/** \brief Get chosen language code or a default locale.
	 *
	 * The default means that QLocale is used when there is no user preference.
	 * Returns language code in QLocale::name() form. Eg. en_EN
	 */
	QString getCurrentLanguageCode();
	/** \brief Set language code.
	 *
	 * \param lang a QLocale::name() form of string or "default"
	 */
	void setCurrentLanguageCode(const QString &lang);

	// real settings

	QHash<QString,QString> ExternalPrograms;
	//! Current working directory
	QString WorkingDir;
	//! State of the MainWindow
	QByteArray MainWindowState;
	//! Geometry of the MainWindow
	QByteArray MainWindowGeometry;
	//! Splitter position in the ServersWidget. Hack: It's a workaround for broken QSplitter::geometry
	QList<int> ServersSplitterSizes;

	//! Size of the thumbnails in FileModel
	int GUIThumbWidth;
	//! Size of the preview in FileModel
	int GUIPreviewWidth;
	//! Flag if the splash creen should be shown
	bool GUISplashEnabled;
	//! How long it should stop on splash screen
	int GUISplashDuration;
	//! Flag: run in developer mode
	bool DeveloperEnabled;
	//! Flag: show developer tool bar
	bool DeveloperTechSpecToolBar;

	//! ProEProductView path for external java applet
	QString ExtensionsProductViewPath;
	//! ProductView dialog geometry
	QByteArray ExtensionsProductViewGeometry;
	//! ProductView dialog position
	QPoint ExtensionsProductViewPosition;

	//! Executable of the Pro/E
	QString ProeExecutable;

	//! Internal flag if there was change in datasources to be updated in ServersWidget
	bool DataSourcesNeedsUpdate;
	//! List of DataSources
	DataSourceList DataSources;
	//! Filter groups
	QList<FilterGroup> FilterGroups;

	/** A filter regexp for proxy file model in ServerTabWidget.
	 *  Calculated in Settings::recalculateFilters()
	 */
	QRegExp filtersRegex;
	//! Recalculate the filtersRegex by user config
	void recalculateFilters();

	//! Flag to show Pro/E versions \todo what is it?
	bool ShowProeVersions;

	//! Available languages in QLocale::name() form (en_EN,...)
	QStringList Languages;

	//! Map lang string to Languages enum
	int langIndex(const QString &lang);
	//! Map Languages enum to string code
	QString langIndexToName(int lang);

private:
	//! cannot access it directly. Use getCurrentLanguageCode()
	QString Language;

	//! Singleton handling
	static Settings *m_instance;

	Settings();
	Settings(const Settings &) {};
	~Settings();

	void load();
	void loadDataSources();
	void saveDataSources();
	void setupFilterGroups();
	void saveFilters();
};

#endif // SETTINGS_H
