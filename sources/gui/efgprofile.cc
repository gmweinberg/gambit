//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Extensive form behavior profile window
//
// This file is part of Gambit
// Copyright (c) 2002, The Gambit Project
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif  // WX_PRECOMP
#include "efgprofile.h"

//-------------------------------------------------------------------------
//                  class EfgProfileList: Member functions
//-------------------------------------------------------------------------

BEGIN_EVENT_TABLE(EfgProfileList, wxListCtrl)
  EVT_RIGHT_DOWN(EfgProfileList::OnRightClick)
END_EVENT_TABLE()

EfgProfileList::EfgProfileList(EfgShow *p_efgShow, wxWindow *p_parent)
  : wxListCtrl(p_parent, idEFG_SOLUTION_LIST, wxDefaultPosition,
	       wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL),
    m_parent(p_efgShow)
{
  m_menu = new wxMenu("Profiles");
  m_menu->Append(efgmenuPROFILES_NEW, "New", "Create a new profile");
  m_menu->Append(efgmenuPROFILES_DUPLICATE, "Duplicate",
		 "Duplicate this profile");
  m_menu->Append(efgmenuPROFILES_DELETE, "Delete", "Delete this profile");
  m_menu->Append(efgmenuPROFILES_PROPERTIES, "Properties",
		 "View and edit properties of this profile");

  UpdateValues();
}

EfgProfileList::~EfgProfileList()
{ }

void EfgProfileList::UpdateValues(void)
{
  ClearAll();
  InsertColumn(0, "Name");
  InsertColumn(1, "Creator");
  InsertColumn(2, "Nash");
  InsertColumn(3, "Perfect");
  InsertColumn(4, "Sequential");
  InsertColumn(5, "Liap Value");
  InsertColumn(6, "Qre Lambda");

  const efgGame &efg = *m_parent->Game();
  int maxColumn = 6;

  for (int pl = 1; pl <= efg.NumPlayers(); pl++) {
    EFPlayer *player = efg.Players()[pl];
    for (int iset = 1; iset <= player->NumInfosets(); iset++) {
      Infoset *infoset = player->Infosets()[iset];
      for (int act = 1; act <= infoset->NumActions(); act++) {
	InsertColumn(++maxColumn,
		     wxString::Format("(%d,%d):%d", pl, iset, act));
      }
    }
  }

  for (int i = 1; i <= m_parent->Profiles().Length(); i++) {
    const BehavSolution &profile = m_parent->Profiles()[i];
    InsertItem(i - 1, (char *) profile.GetName());
    SetItem(i - 1, 1, (char *) ToText(profile.Creator()));
    SetItem(i - 1, 2, (char *) ToText(profile.IsNash()));
    SetItem(i - 1, 3, (char *) ToText(profile.IsSubgamePerfect()));
    SetItem(i - 1, 4, (char *) ToText(profile.IsSequential()));
    SetItem(i - 1, 5, (char *) ToText(profile.LiapValue()));
    if (profile.Creator() == algorithmEfg_QRE_EFG ||
	profile.Creator() == algorithmEfg_QRE_NFG) {
      SetItem(i - 1, 6, (char *) ToText(profile.QreLambda()));
    }
    else {
      SetItem(i - 1, 6, "--");
    }

    int column = 6;
    for (int pl = 1; pl <= efg.NumPlayers(); pl++) {
      EFPlayer *player = efg.Players()[pl];
      for (int iset = 1; iset <= player->NumInfosets(); iset++) {
	Infoset *infoset = player->Infosets()[iset];
	for (int act = 1; act <= infoset->NumActions(); act++) {
	  SetItem(i - 1, ++column,
		  (char *) ToText(profile(infoset->Actions()[act])));
	}
      }
    }
  }    

  if (m_parent->Profiles().Length() > 0) {
    wxListItem item;
    item.m_mask = wxLIST_MASK_STATE;
    item.m_itemId = m_parent->CurrentProfile() - 1;
    item.m_state = wxLIST_STATE_SELECTED;
    item.m_stateMask = wxLIST_STATE_SELECTED;
    SetItem(item);
  }
}

void EfgProfileList::OnRightClick(wxMouseEvent &p_event)
{
  m_menu->Enable(efgmenuPROFILES_DUPLICATE, m_parent->CurrentProfile() > 0);
  m_menu->Enable(efgmenuPROFILES_DELETE, m_parent->CurrentProfile() > 0);
  m_menu->Enable(efgmenuPROFILES_PROPERTIES, m_parent->CurrentProfile() > 0);
  PopupMenu(m_menu, p_event.m_x, p_event.m_y);
}


