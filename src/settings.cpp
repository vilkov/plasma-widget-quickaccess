/***************************************************************************
 *   Copyright (C) 2008 by Mark Herbert <wirrkpf@googlemail.com>           *
 *   Copyright (C) 2012 by Dmitriy Vilkov <dav.daemon@gmail.com>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 ***************************************************************************/


#include "settings.h"

#include <QDir>

// #include <KDebug>

Settings::Settings(QObject *parent)
  :QObject(parent)
  ,m_iconName("folder-favorites")
  ,m_iconSize(16)
  ,m_showPreviews(false)
  ,m_showHiddenFiles(false)
  ,m_showOnlyDirs(false)
  ,m_filter("*")
  ,m_customLabel("")
  ,m_showCustomLabel(false)
  ,m_url(QDir::homePath())
  ,m_showToolTips(false)
  ,m_needsSaving(false)
  ,m_previewPlugins(QStringList() << "imagethumbnail")
  ,m_viewMode(ItemView::ListMode)
  ,m_allowNavigation(true)
  ,m_singleClickNavigation(true)
//  ,m_enableDolphinSorting(false)
  ,m_sortOrder(Qt::AscendingOrder)
  ,m_sortColumn(KDirModel::Name)
{
  
}

Settings::~Settings()
{
  
}

void Settings::setIconName(const QString &name)
{
  if(name != m_iconName) {
    m_iconName = name;
    m_needsSaving = true;
    emit settingsChanged(Settings::IconName);
  }
}

QString Settings::iconName() const
{
  return m_iconName;
}
    
void Settings::setIconSize(const int &size)
{
  if(size != m_iconSize) {
    m_iconSize = size;
    m_needsSaving = true;
    emit settingsChanged(Settings::IconSize);
  }
}

int Settings::iconSize() const
{
  return m_iconSize;
}
    
void Settings::setShowPreviews(bool show)
{
  if(show != m_showPreviews) {
    m_showPreviews = show;
    m_needsSaving = true;
    emit settingsChanged(Settings::Preview);
  }
}

bool Settings::showPreviews()
{
  return m_showPreviews;
}
    
void Settings::setShowHiddenFiles(bool show)
{
  if( show != m_showHiddenFiles) {
    m_showHiddenFiles = show;
    m_needsSaving = true;
    emit settingsChanged(Settings::ShowHiddenFiles);
  }
}

bool Settings::showHiddenFiles()
{
  return m_showHiddenFiles;
}
    
void Settings::setShowOnlyDirs(bool show)
{
  if(show != m_showOnlyDirs) {
    m_showOnlyDirs = show;
    m_needsSaving = true;
    emit settingsChanged(Settings::ShowOnlyDirs);
  }
}

bool Settings::showOnlyDirs()
{
  return m_showOnlyDirs;
}
    
void Settings::setFilter(const QString &filter)
{
  if(filter != m_filter) {
    m_filter = filter;
    m_needsSaving = true;
    emit settingsChanged(Settings::Filter);
  }
}
 
QString Settings::filter() const
{
  return m_filter;
}
    
void Settings::setCustomLabel(const QString &label)
{
  if(label != m_customLabel) {
    m_customLabel = label;
    m_needsSaving = true;
    emit settingsChanged(Settings::CustomLabel);
  }
}
 
 QString Settings::customLabel() const
{
  return m_customLabel;
}
    
void Settings::setUrl(const KUrl &url)
{
  if(url != m_url) {
    m_url = url;
    m_needsSaving = true;
    emit settingsChanged(Settings::Url);
  }
}

KUrl Settings::url() const
{
  return m_url;
}

void Settings::setShowCustomLabel(bool show)
{
  if(show != m_showCustomLabel) {
    m_showCustomLabel = show;
    m_needsSaving = true;
    emit settingsChanged(Settings::CustomLabel);
  }
}

bool Settings::showCustomLabel()
{
  return m_showCustomLabel;
}

void Settings::setPreviewPlugins(const QStringList &list)
{
  if(list != m_previewPlugins) {
    m_previewPlugins = list;
    m_needsSaving = true;
    emit settingsChanged(Settings::Preview);
  }
}

QStringList Settings::previewPlugins() const
{
  return m_previewPlugins;
}

void Settings::setShowToolTips(bool show)
{
  if(show != m_showToolTips) {
    m_showToolTips = show;
    m_needsSaving = true;
    emit settingsChanged(Settings::ToolTips);
  }
}

bool Settings::showToolTips()
{
  return m_showToolTips;
}

void Settings::setViewMode(ItemView::ViewMode mode)
{
  if(mode != m_viewMode) {
    m_viewMode = mode;
    m_needsSaving = true;
    emit settingsChanged(Settings::ViewMode);
  }
}

ItemView::ViewMode Settings::viewMode()
{
  return m_viewMode;
}

void Settings::setAllowNavigation(bool allow)
{
  if(allow != m_allowNavigation) {
    m_allowNavigation = allow;
    m_needsSaving = true;
  }
}

bool Settings::allowNavigation()
{
  return m_allowNavigation;
}

void Settings::setSingleClickNavigation(bool allow)
{
  if(allow != m_singleClickNavigation) {
    m_singleClickNavigation = allow;
    m_needsSaving = true;
    emit settingsChanged(Settings::SingleClickNav);
  }
}

bool Settings::singleClickNavigation()
{
  return m_singleClickNavigation;
}

//void Settings::setEnableDolphinSorting(bool allow) {
//  if(allow != m_enableDolphinSorting) {
//    m_enableDolphinSorting = allow;
//    m_needsSaving = true;
//    emit settingsChanged(Settings::DolphinSorting);
//  }
//}
//
//bool Settings::enableDolphinSorting() {
//  return m_enableDolphinSorting;
//}


void Settings::setSortOrder(Qt::SortOrder order)
{
  if(order != m_sortOrder) {
    m_sortOrder = order;
    m_needsSaving = true;
    emit settingsChanged(Settings::Sorting);
  }
}

Qt::SortOrder Settings::sortOrder()
{
  return m_sortOrder;
}

void Settings::setSortColumn(KDirModel::ModelColumns column)
{
  if(column != m_sortColumn) {
    m_sortColumn = column;
    m_needsSaving = true;
    emit settingsChanged(Settings::Sorting);
  }
}

KDirModel::ModelColumns Settings::sortColumn()
{
  return m_sortColumn;
}

bool Settings::needsSaving()
{
  return m_needsSaving;
}

void Settings::readSettings(KConfigGroup *cg)
{
  KUrl url(QDir::homePath());
  m_url = cg->readEntry("url", url);
  m_iconName = cg->readEntry("icon", "folder-favorites");
  m_iconSize = cg->readEntry("iconSize", 16);
  m_showPreviews = cg->readEntry("preview", false);
  m_showHiddenFiles = cg->readEntry("hidden", false);
  m_showOnlyDirs = cg->readEntry("onlyDirs", false);
  m_filter = cg->readEntry("filter", "*");
  m_showCustomLabel = cg->readEntry("showCustomLabel", false);
  m_customLabel = cg->readEntry("customLabel", "");
  m_previewPlugins = cg->readEntry("previewPlugins", QStringList() << "imagethumbnail");
  m_showToolTips = cg->readEntry("ToolTips", false);
  m_viewMode = (ItemView::ViewMode)cg->readEntry("ViewMode", 0);
  m_allowNavigation = cg->readEntry("AllowNavigation", true);
  m_sortOrder = (Qt::SortOrder)cg->readEntry("sortOrder", 0);
  m_sortColumn = (KDirModel::ModelColumns)cg->readEntry("sortColumn", 0);
  m_singleClickNavigation = cg->readEntry("SingleClickNavigation", true);
//  m_enableDolphinSorting = cg->readEntry("EnableDolphinSorting", false);
  //emit settingsChanged(Settings::All);
}

void Settings::saveSettings(KConfigGroup *cg)
{
  cg->writeEntry("url", m_url);
  cg->writeEntry("icon", m_iconName);
  cg->writeEntry("iconSize", m_iconSize);
  cg->writeEntry("preview", m_showPreviews);
  cg->writeEntry("hidden", m_showHiddenFiles);
  cg->writeEntry("onlyDirs", m_showOnlyDirs);
  cg->writeEntry("filter", m_filter);
  cg->writeEntry("customLabel", m_customLabel);
  cg->writeEntry("showCustomLabel", m_showCustomLabel);
  cg->writeEntry("previewPlugins", m_previewPlugins);
  cg->writeEntry("ToolTips", m_showToolTips);
  cg->writeEntry("ViewMode", (int)m_viewMode);
  cg->writeEntry("AllowNavigation", m_allowNavigation);
  cg->writeEntry("sortOrder", (int)m_sortOrder);
  cg->writeEntry("sortColumn", (int)m_sortColumn);
  cg->writeEntry("SingleClickNavigation", m_singleClickNavigation);
//  cg->writeEntry("EnableDolphinSorting", m_enableDolphinSorting);
  m_needsSaving = false;
}
  
#include "settings.moc"
