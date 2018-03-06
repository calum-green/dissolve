/*
	*** SpeciesTab Functions
	*** src/gui/speciestab_funcs.cpp
	Copyright T. Youngs 2012-2018

	This file is part of dUQ.

	dUQ is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	dUQ is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with dUQ.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "gui/speciestab.h"
#include "gui/gui.h"
#include "gui/delegates/combolist.hui"
#include "gui/delegates/isotopecombo.hui"
#include "gui/delegates/texponentialspin.hui"
#include "gui/helpers/listwidgetupdater.h"
#include "gui/helpers/tablewidgetupdater.h"
#include "main/duq.h"
#include "classes/atomtype.h"
#include "classes/species.h"
#include <QListWidgetItem>

// Constructor / Destructor
SpeciesTab::SpeciesTab(DUQWindow* duqWindow, DUQ& duq, QTabWidget* parent, const char* title) : MainTab(duqWindow, duq, parent, title, this)
{
	ui.setupUi(this);

	refreshing_ = true;

	// SpeciesAtomTable
	for (int n=1; n<5; ++n) ui.AtomTable->setItemDelegateForColumn(n, new TExponentialSpinDelegate(this));
	ui.AtomTable->setItemDelegateForColumn(5, new ComboListDelegate(this, new ComboNameListItems<AtomType>(duq_.atomTypeList())));
	ui.AtomTable->horizontalHeader()->setFont(duqWindow->font());

	ui.IsotopeTable->setItemDelegateForColumn(1, new IsotopeComboDelegate(this));

	refreshing_ = false;
}

SpeciesTab::~SpeciesTab()
{
}

/*
 * SubWidget / SubWindow Handling
 */

// Return the tab's SubWindow area, if it has one
QMdiArea* SpeciesTab::subWindowArea()
{
	return NULL;
}

// Return the tab's SubWidget layout, if it has one
QLayout* SpeciesTab::subWidgetLayout()
{
	return NULL;
}

/*
 * Update
 */

// Update controls in tab
void SpeciesTab::updateControls()
{
	refreshing_ = true;

	// Species List
	ListWidgetUpdater<SpeciesTab,Species> speciesUpdater(ui.SpeciesList, duq_.species());

	Species* species = currentSpecies();

	// SpeciesAtom Table
	if (!species) ui.AtomTable->clearContents();
	else TableWidgetUpdater<SpeciesTab,SpeciesAtom> speciesAtomUpdater(ui.AtomTable, species->atoms(), this, &SpeciesTab::updateAtomTableRow);

	// Isotopologues List
	if (!species) ui.IsotopologueList->clear();
	else ListWidgetUpdater<SpeciesTab,Isotopologue> isotopologueUpdater(ui.IsotopologueList, species->isotopologues());

	Isotopologue* isotopologue = currentIsotopologue();

	// Isotopologue AtomType/Isotopes Table
	if (!isotopologue) ui.IsotopeTable->clearContents();
	else TableWidgetRefListUpdater<SpeciesTab,AtomType,Isotope*> isotopeUpdater(ui.IsotopeTable, isotopologue->isotopes(), this, &SpeciesTab::updateIsotopeTableRow);

	refreshing_ = false;
}

// Disable sensitive controls within tab, ready for main code to run
void SpeciesTab::disableSensitiveControls()
{
}

// Enable sensitive controls within tab, ready for main code to run
void SpeciesTab::enableSensitiveControls()
{
}

/*
 * Signals / Slots
 */

// Return currently-selected Species
Species* SpeciesTab::currentSpecies()
{
	QListWidgetItem* item = ui.SpeciesList->currentItem();
	if (!item) return NULL;
	return (Species*) VariantPointer<Species>(item->data(Qt::UserRole));
}

// Return currently-selected Isotopologue
Isotopologue* SpeciesTab::currentIsotopologue()
{
	QListWidgetItem* item = ui.IsotopologueList->currentItem();
	if (!item) return NULL;
	return (Isotopologue*) VariantPointer<Isotopologue>(item->data(Qt::UserRole));
}

// SpeciesAtomTable row update function
void SpeciesTab::updateAtomTableRow(int row, SpeciesAtom* speciesAtom, bool createItems)
{
	QTableWidgetItem* item;

	// Element
	if (createItems)
	{
		item = new QTableWidgetItem;
		item->setData(Qt::UserRole, VariantPointer<SpeciesAtom>(speciesAtom));
		ui.AtomTable->setItem(row, 0, item);
	}
	else item = ui.AtomTable->item(row, 0);
	item->setText(PeriodicTable::element(speciesAtom->element()).name());

	// Coordinates
	for (int n=0; n<3; ++n)
	{
		if (createItems)
		{
			item = new QTableWidgetItem;
			item->setData(Qt::UserRole, VariantPointer<SpeciesAtom>(speciesAtom));
			ui.AtomTable->setItem(row, n+1, item);
		}
		else item = ui.AtomTable->item(row, n+1);
		item->setText(QString::number(speciesAtom->r().get(n)));
	}

	// Charge
	if (createItems)
	{
		item = new QTableWidgetItem;
		item->setData(Qt::UserRole, VariantPointer<SpeciesAtom>(speciesAtom));
		ui.AtomTable->setItem(row, 4, item);
	}
	else item = ui.AtomTable->item(row, 4);
	item->setText(QString::number(speciesAtom->charge()));

	// AtomTypes
	if (createItems)
	{
		item = new QTableWidgetItem;
		item->setData(Qt::UserRole, VariantPointer<SpeciesAtom>(speciesAtom));
		ui.AtomTable->setItem(row, 5, item);
	}
	else item = ui.AtomTable->item(row, 5);
	item->setText(speciesAtom->atomType() ? speciesAtom->atomType()->name() : "");
}

// IsotopologuesIsotopesTable row update function
void SpeciesTab::updateIsotopeTableRow(int row, AtomType* atomType, Isotope* isotope, bool createItems)
{
	QTableWidgetItem* item;

	// AtomType
	if (createItems)
	{
		item = new QTableWidgetItem;
		item->setData(Qt::UserRole, VariantPointer<AtomType>(atomType));
		item->setFlags(Qt::NoItemFlags);
		ui.IsotopeTable->setItem(row, 0, item);
	}
	else item = ui.IsotopeTable->item(row, 0);
	item->setText(atomType->name());

	// Isotope
	if (createItems)
	{
		item = new QTableWidgetItem;
		item->setData(Qt::UserRole, VariantPointer<Isotope>(isotope));
		ui.IsotopeTable->setItem(row, 1, item);
	}
	else item = ui.IsotopeTable->item(row, 1);
	item->setText(IsotopeComboDelegate::textForIsotope(isotope));
}

void SpeciesTab::on_SpeciesList_currentRowChanged(int row)
{
	if (refreshing_) return;

	// Clear relevant tables here to make things more efficient
	ui.AtomTable->clearContents();
	ui.IsotopologueList->clear();
	ui.IsotopeTable->clearContents();

	updateControls();
}

void SpeciesTab::on_AtomTable_itemChanged(QTableWidgetItem* w)
{
	if (refreshing_) return;

	// Get target MasterIntra from the passed widget
	SpeciesAtom* speciesAtom = w ? VariantPointer<SpeciesAtom>(w->data(Qt::UserRole)) : NULL;
	if (!speciesAtom) return;

	// Column of passed item tells us the type of data we need to change
	AtomType* atomType;
	switch (w->column())
	{
		// Element
		case (0):
			break;
		// Coordinates
		case (1):
		case (2):
		case (3):
			speciesAtom->setCoordinate(w->column()-1, w->text().toDouble());
			duqWindow_->setModified();
			break;
		// Charge
		case (4):
			speciesAtom->setCharge(w->text().toDouble());
			duqWindow_->setModified();
			break;
		// AtomType
		case (5):
			// Check the text to see if it is an existing AtomType - if not, we should create it
			atomType = duq_.findAtomType(qPrintable(w->text()));
			if (!atomType)
			{
				atomType = duq_.addAtomType(speciesAtom->element());
				atomType->setName(qPrintable(w->text()));
			}
			speciesAtom->setAtomType(atomType);
			duqWindow_->setModified();
			break;
		default:
			Messenger::error("Don't know what to do with data from column %i of SpeciesAtom table.\n", w->column());
			break;
	}
}

void SpeciesTab::on_IsotopologueList_currentRowChanged(int row)
{
	if (refreshing_) return;

	Isotopologue* isotopologue = currentIsotopologue();

	refreshing_ = true;

	// Isotopologue AtomType/Isotopes Table
	if (!isotopologue) ui.IsotopeTable->clearContents();
	else TableWidgetRefListUpdater<SpeciesTab,AtomType,Isotope*> isotopeUpdater(ui.IsotopeTable, isotopologue->isotopes(), this, &SpeciesTab::updateIsotopeTableRow);

	refreshing_ = false;
}

void SpeciesTab::on_IsotopeTable_itemChanged(QTableWidgetItem* w)
{
	if (refreshing_) return;

	// Get current Isotopologue
	Isotopologue* iso = currentIsotopologue();
	if (!iso) return;

	// Get row from the passed widget, and get target AtomType from column 0
	QTableWidgetItem* item = ui.IsotopeTable->item(w->row(), 0);
	if (!item) return;
	
	AtomType* atomType = VariantPointer<AtomType>(item->data(Qt::UserRole));
	if (!atomType) return;

	// Column of passed item tells us the type of data we need to change
	Isotope* isotope;
	switch (w->column())
	{
		// Element
		case (0):
			// Should never be called
			break;
		// Isotope
		case (1):
			// The new Isotope should have been set in the model data for the column
			isotope = VariantPointer<Isotope>(w->data(Qt::UserRole));
			if (isotope) iso->setAtomTypeIsotope(atomType, isotope);
			duqWindow_->setModified();
			break;
		default:
			Messenger::error("Don't know what to do with data from column %i of Isotopes table.\n", w->column());
			break;
	}
}

/*
 * State
 */

// Write widget state through specified LineParser
bool SpeciesTab::writeState(LineParser& parser)
{
	return true;
}
