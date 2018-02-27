/*
	*** Keyword Widget - Bool
	*** src/gui/keywordwidgets/bool_funcs.cpp
	Copyright T. Youngs 2012-2018

	This file is part of DUQ.

	DUQ is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	DUQ is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with DUQ.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "gui/keywordwidgets/bool.hui"
#include "templates/genericlisthelper.h"

// Constructor
BoolKeywordWidget::BoolKeywordWidget(QWidget* parent, ModuleKeywordBase* keyword) : QCheckBox(parent), KeywordWidgetBase()
{
	// Cast the pointer up into the parent class type
	keyword_ = dynamic_cast<BoolModuleKeyword*>(keyword);
	if (!keyword_) Messenger::error("Couldn't cast base module keyword into BoolModuleKeyword.\n");
	else
	{
		// Set current value
		setChecked(keyword_->asBool());
	}

	// Connect the
	connect(this, SIGNAL(clicked(bool)), this, SLOT(myClicked(bool)));
}

/*
 * Signals / Slots
 */

// Check box state changed
void BoolKeywordWidget::myClicked(bool checked)
{
	if (refreshing_) return;

	keyword_->setData(checked);

	emit(keywordValueChanged());
}

/*
 * Update
 */

// Update value displayed in widget, using specified source if necessary
void BoolKeywordWidget::updateValue(GenericList& moduleData, const char* prefix)
{
	refreshing_ = true;

	// Check to see if the associated Keyword may have been stored/updated in the specified moduleData
	if ((keyword_->genericItemFlags()&GenericItem::InRestartFileFlag) && moduleData.contains(keyword_->keyword(), prefix))
	{
		// Retrieve the item from the list
		setChecked(GenericListHelper<bool>::retrieve(moduleData, keyword_->keyword(), prefix));
	}
	else setChecked(keyword_->asBool());

	refreshing_ = false;
}
