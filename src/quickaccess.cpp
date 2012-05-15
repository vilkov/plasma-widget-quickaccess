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



#include "quickaccess.h"

//Qt includes
#include <QGraphicsLinearLayout>
#include <QTimer>


//KDE includes
#include <KIconLoader>
//#include <KDebug>
#include <KUrl>
#include <KConfigDialog>
#include <konq_operations.h>
#include <KMessageBox>
#include <KGlobalSettings>

//Plasma
#include <Plasma/ToolTipManager>

QuickAccess::QuickAccess(QObject *parent, const QVariantList &args)
  :Plasma::Applet(parent, args)
  ,m_settings(new Settings(this))
  ,m_icon(new Plasma::IconWidget(this))
  ,m_dialog(0)
  ,m_dialogSize(QSize())
  ,m_dragOver(false)
  ,m_saveTimer(new QTimer(this))
{
  setHasConfigurationInterface(true);
  setAcceptDrops(true);
  m_saveTimer->setSingleShot(true);
}
 
 
QuickAccess::~QuickAccess()
{
  if (hasFailedToLaunch()) {
    // Do some cleanup here
  } else {
    saveSettings();
    if(m_dialog) {
      delete m_dialog;
    }
  }
  delete m_icon;
  delete m_settings;
  delete m_saveTimer;
}
 
void QuickAccess::init()
{
 
  /*// A small demonstration of the setFailedToLaunch function
  if (m_icon.isNull()) {
    setFailedToLaunch(true, "No world to say hello");
  } else {

  }*/

  //setup the layout
  QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  connect(m_icon, SIGNAL(clicked()), this, SLOT(slot_iconClicked()));
  layout->addItem(m_icon);
  
  Plasma::ToolTipManager::self()->registerWidget(this);

  //read the config
  KConfigGroup cg = config();
  m_dialogSize = cg.readEntry("dialogSize", QSize(300,400));
  
  m_settings->readSettings(&cg);
  connect(m_settings, SIGNAL(settingsChanged(Settings::SettingsType)), this, SLOT(applySettings(Settings::SettingsType)));
  m_icon->setIcon(m_settings->iconName());
  resize(m_icon->iconSize());
  update();

  registerAsDragHandle(m_icon);
  setAspectRatioMode(Plasma::ConstrainedSquare);
  
  connect(m_saveTimer, SIGNAL(timeout()), this, SLOT(saveSettings()));
  
  connect(KGlobalSettings::self(), SIGNAL(iconChanged(int)),
      this, SLOT(iconSizeChanged(int)));
}

void QuickAccess::popupEvent(bool show)
{
    if (show) {
        Plasma::ToolTipManager::self()->clearContent(this);
    }
}

void QuickAccess::toolTipAboutToShow()
{
  Plasma::ToolTipContent toolTip;
  toolTip.setMainText(i18n("QuickAccess Browser"));
  toolTip.setSubText(i18n("Quickly browse folders on your computer"));
  toolTip.setImage(KIcon(m_settings->iconName()));

  Plasma::ToolTipManager::self()->setContent(this, toolTip);
}

void QuickAccess::toolTipHidden()
{
    Plasma::ToolTipManager::self()->clearContent(this);
}

void QuickAccess::applySettings(Settings::SettingsType type)
{
  if(type == Settings::IconName || type == Settings::All) {
    m_icon->setIcon(m_settings->iconName());
    update();
  }
  if(!m_saveTimer->isActive()) {
    m_saveTimer->start(600000); //10 minutes
  }
}

void QuickAccess::saveSettings()
{
  KConfigGroup cg = config();
  bool save = false;
  if(m_settings->needsSaving()){
    save = true;
    m_settings->saveSettings(&cg);
  }
  if(m_dialog) {
    if(m_dialog->size() != m_dialogSize) {
      save = true;
      m_dialogSize = m_dialog->size();
      cg.writeEntry("dialogSize", m_dialogSize);
    }
  }
  if(save) {
    emit configNeedsSaving();
  }
}

void QuickAccess::createConfigurationInterface(KConfigDialog *parent)
{
  QWidget *widgetGeneral = new QWidget;
  QWidget *widgetAppearance = new QWidget;
  QWidget *widgetPreview = new QWidget;
  uiGeneral.setupUi(widgetGeneral);
  uiAppearance.setupUi(widgetAppearance);
  uiPreview.setupUi(widgetPreview);
  
  pluginWidget = new PluginWidget(widgetPreview);
  
  uiGeneral.urlRequester->setMode(KFile::Directory | KFile::ExistingOnly);
  uiGeneral.urlRequester->setUrl(m_settings->url());
  uiGeneral.filterEdit->setText(m_settings->filter());

  uiGeneral.hiddenBox->setChecked(m_settings->showHiddenFiles());

  uiGeneral.onlyDirsBox->setChecked(m_settings->showOnlyDirs());

  uiGeneral.navigationBox->setChecked(m_settings->allowNavigation());

  uiGeneral.singleClickBox->setChecked(m_settings->singleClickNavigation());

//  uiGeneral.dolphinSortingBox->setChecked(m_settings->enableDolphinSorting());

  uiAppearance.customLabelEdit->setEnabled(uiAppearance.customLabelBox->isChecked());

  uiAppearance.iconbutton->setIcon(m_settings->iconName());
  uiAppearance.iconbutton->setIconType(KIconLoader::NoGroup, KIconLoader::Place);
  
  uiAppearance.iconSizeCombo->setCurrentIndex(uiAppearance.iconSizeCombo->findText( QString::number(m_settings->iconSize()) ));
  
  if(m_settings->viewMode() == ItemView::ListMode) {
    uiAppearance.viewModeCombo->setCurrentIndex(0);
  } else {
    uiAppearance.viewModeCombo->setCurrentIndex(1);
  }
  uiAppearance.viewModeCombo->setItemIcon(0, KIcon("view-list-details"));
  uiAppearance.viewModeCombo->setItemIcon(1, KIcon("view-list-icons"));

  uiPreview.previewBox->setChecked(m_settings->showPreviews());
  
  //disable the previewplugins options when previews are disabled
  if(!m_settings->showPreviews()) {
    uiPreview.previewLabel1->setEnabled(false);
    uiPreview.previewLabel2->setEnabled(false);
    pluginWidget->setEnabled(false);
  }
  
  if(!m_settings->showCustomLabel()) {
    uiAppearance.customLabelBox->setChecked(false);
  } else {
    uiAppearance.customLabelBox->setChecked(true);
    uiAppearance.customLabel->setEnabled(true);
    uiAppearance.customLabelEdit->setEnabled(true);
    uiAppearance.customLabelEdit->setText(m_settings->customLabel());
  }
  
  uiAppearance.tooltipBox->setChecked(m_settings->showToolTips());
  
  pluginWidget->setActivePlugins(m_settings->previewPlugins());
  uiPreview.pluginLayout->addWidget(pluginWidget);
  
  
  parent->addPage(widgetGeneral, i18nc("Title of the page that lets the user choose which location the quickaccess should show", "General"), "folder-favorites");
  parent->addPage(widgetAppearance, i18nc("Title of the page that lets the user choose how the quickaccess should be shown", "Appearance"), "preferences-desktop-display");
  parent->addPage(widgetPreview, i18nc("Title of the page that lets the user choose which filetypes should be previewed", "Preview"), "document-preview");
  parent->setButtons(KDialog::Ok | KDialog::Cancel | KDialog::Apply);

  connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));
  connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));

  connect(uiAppearance.customLabelBox, SIGNAL(toggled(bool)), this, SLOT(showCustomLabelToggled(bool)));
  connect(uiPreview.previewBox, SIGNAL(toggled(bool)), this, SLOT(showPreviewToggled(bool)));
  
}

void QuickAccess::configAccepted()
{
  KUrl url = uiGeneral.urlRequester->url();
  url.adjustPath(KUrl::RemoveTrailingSlash);
  
  if(url.protocol() == "applications") {
    KMessageBox::sorry(uiGeneral.urlRequester, i18n("Sorry, but the \"applications:\" KIO slave is not supported, because it will crash QuickAccess/Plasma..."));
  } else {
    m_settings->setUrl(url);
  }
  m_settings->setIconName(uiAppearance.iconbutton->icon());
  m_settings->setIconSize(uiAppearance.iconSizeCombo->currentText().toInt());
  m_settings->setShowPreviews(uiPreview.previewBox->isChecked());
  m_settings->setShowHiddenFiles(uiGeneral.hiddenBox->isChecked());
  m_settings->setShowOnlyDirs(uiGeneral.onlyDirsBox->isChecked());
  m_settings->setAllowNavigation(uiGeneral.navigationBox->isChecked());
  m_settings->setSingleClickNavigation(uiGeneral.singleClickBox->isChecked());
  m_settings->setFilter(uiGeneral.filterEdit->text());
  m_settings->setShowCustomLabel(uiAppearance.customLabelBox->isChecked());
  m_settings->setCustomLabel(uiAppearance.customLabelEdit->text());
  m_settings->setShowToolTips(uiAppearance.tooltipBox->isChecked());
// m_settings->setEnableDolphinSorting(uiGeneral.dolphinSortingBox->isChecked());
  
  QStringList list = pluginWidget->activePlugins();
  qSort(list); //sort it...
  m_settings->setPreviewPlugins(list);
  
  if(uiAppearance.viewModeCombo->currentIndex() == 0) {
    m_settings->setViewMode(ItemView::ListMode);
  } else {
    m_settings->setViewMode(ItemView::IconMode);
  }
  
}

void QuickAccess::showCustomLabelToggled(bool checked)
{
  uiAppearance.customLabel->setEnabled(checked);
  uiAppearance.customLabelEdit->setEnabled(checked);
}

void QuickAccess::showPreviewToggled(bool checked)
{
    uiPreview.previewLabel1->setEnabled(checked);
    uiPreview.previewLabel2->setEnabled(checked);
    pluginWidget->setEnabled(checked);
}

PopupDialog* QuickAccess::dialog()
{
  if(!m_dialog) {
    //create the dialog
    m_dialog = new PopupDialog(m_settings);
    m_dialog->resize(m_dialogSize);
    m_dialog->applySettings(Settings::All);
    connect(m_dialog, SIGNAL(signal_hide()), m_icon, SLOT(setUnpressed()));
  }
  return m_dialog;
}

void QuickAccess::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
  event->setAccepted(event->mimeData()->hasUrls());
  m_dragOver = true;
  QTimer::singleShot(DRAG_ENTER_TIME, this, SLOT(slotDragEvent()));  
}

void QuickAccess::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
  Q_UNUSED(event);
  m_dragOver = false;
  
}

void QuickAccess::dropEvent(QGraphicsSceneDragDropEvent *event)
{
  m_dragOver = false;
  
  KFileItem item = dialog()->startItem();

  QDropEvent ev(event->screenPos(), event->dropAction(), event->mimeData(),
		event->buttons(), event->modifiers());

  KonqOperations::doDrop(item, item.url(), &ev, event->widget());
}

//SLOTS:
void QuickAccess::slotDragEvent()
{
  if(!m_dragOver) {
    return;
  }
  dialog()->move(popupPosition(dialog()->size()));
  dialog()->show();
}

void QuickAccess::slot_iconClicked()
{
  if(!dialog()->isVisible()) {
    m_icon->setPressed(true);
    dialog()->move(popupPosition(dialog()->size()));
    dialog()->show();
  }
}

QSizeF QuickAccess::sizeHint(Qt::SizeHint which, const QSizeF & constraint) const
{
    if (which == Qt::PreferredSize) {
        int iconSize;

        switch (formFactor()) {
            case Plasma::Planar:
            case Plasma::MediaCenter:
                iconSize = IconSize(KIconLoader::Desktop);
                break;

            case Plasma::Horizontal:
            case Plasma::Vertical:
                iconSize = IconSize(KIconLoader::Panel);
                break;
        }

        return QSizeF(iconSize, iconSize);
    }

    return Plasma::Applet::sizeHint(which, constraint);
}

void QuickAccess::iconSizeChanged(int group)
{
    if (group == KIconLoader::Desktop || group == KIconLoader::Panel) {
        updateGeometry();
    }
}

#include "quickaccess.moc"
