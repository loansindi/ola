/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * ParserActions.h
 * Copyright (C) 2011 Simon Newton
 *
 * These functions all called by the parser.
 */

#ifndef TOOLS_DMX_TRIGGER_PARSERACTIONS_H_
#define TOOLS_DMX_TRIGGER_PARSERACTIONS_H_

#include <string>
#include <vector>

using std::string;
using std::vector;

class Action;

void SetDefaultValue(vector<string> *input);
Action *CreateAssignmentAction(vector<string> *input);
Action *CreateCommandAction(const string &command, vector<string> *input);
void SetSlotAction(unsigned int slot,
                   vector<class ValueInterval*> *slot_values,
                   Action *action);
void SetDefaultAction(unsigned int slot, Action *action);

#endif  // TOOLS_DMX_TRIGGER_PARSERACTIONS_H_