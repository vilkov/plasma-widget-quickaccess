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

#ifndef DirModel_HEADER
#define DirModel_HEADER


//KDE includes
#include <KDirModel>

class DirModel : public KDirModel
{
  Q_OBJECT

  public:
    DirModel (QObject * parent = 0 );
    virtual QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;

    bool m_showOnlyIcons;
  private:

};

#endif
