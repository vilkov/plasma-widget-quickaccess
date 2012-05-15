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

#ifndef QuickAccess_HEADER
#define QuickAccess_HEADER

#include <Plasma/Applet>
#include <plasma/widgets/iconwidget.h>

#include <ui_quickaccessGeneralConfig.h>
#include <ui_quickaccessAppearanceConfig.h>
#include <ui_quickaccessPreviewConfig.h>

#include "popupdialog.h"
#include "pluginwidget.h"


class QuickAccess : public Plasma::Applet
{
	Q_OBJECT
public:
	QuickAccess(QObject *parent, const QVariantList &args);
	~QuickAccess();

	virtual void init();

public slots:
	void applySettings(Settings::SettingsType);
	void toolTipAboutToShow();
	void toolTipHidden();

protected:
	virtual void createConfigurationInterface(KConfigDialog *parent);
	virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
	virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
	virtual void dropEvent(QGraphicsSceneDragDropEvent *event);
	void popupEvent(bool show);
	virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF & constraint = QSizeF()) const;

private slots:
	void slot_iconClicked();
	void slotDragEvent();
	void configAccepted();
	void showCustomLabelToggled(bool checked);
	void showPreviewToggled(bool checked);
	void saveSettings();
	void iconSizeChanged(int group);

private:
	PopupDialog* dialog();

private:
	Settings *m_settings;
	Plasma::IconWidget *m_icon;
	PopupDialog *m_dialog;
	QSize m_dialogSize;
	bool m_dragOver;
	Ui::quickaccessGeneralConfig uiGeneral;
	Ui::quickaccessAppearanceConfig uiAppearance;
	Ui::quickaccessPreviewConfig uiPreview;
	PluginWidget *pluginWidget;
	QTimer *m_saveTimer;
};
 
K_EXPORT_PLASMA_APPLET(quickaccess, QuickAccess)
#endif
