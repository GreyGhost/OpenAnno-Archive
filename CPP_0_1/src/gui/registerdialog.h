/***************************************************************************
 *   Copyright (C) 2006 by Marc Engelhardt								   *
 *   Marc_Engelhardt@gmx.net                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef REGISTERDIALOG_H_
#define REGISTERDIALOG_H_

#include <string>
#include "gui/dialog.h"
#include "gui/editbox.h"
#include "gui/slider.h"
#include "gui/checkbox.h"
#include "gui/selector.h"
#include "gui/imagebutton.h"
#include "gui/events/eventcommand.h"

class RegisterDialog : public Dialog
{
public:
	RegisterDialog(int x, int y, int width, int height, const std::string& background);
private:
	void onCommand(EventCommand* event);
	Editbox* m_playerName;
	Editbox* m_playerEmail;
	Editbox* m_playerPasswortW; // W stand for the German Word 'Wiederholung'
	Editbox* m_playerPasswort;	
	ImageButton* m_Ok;
	ImageButton* m_Cancel;
};

#endif /*REGISTERDIALOG_H_*/
