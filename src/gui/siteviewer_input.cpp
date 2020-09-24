// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2020 Team Dissolve and contributors

#include "classes/species.h"
#include "gui/render/renderablespecies.h"
#include "gui/siteviewer.hui"
#include <QtGui/QMouseEvent>

// Mouse moved
void SiteViewer::mouseMoved(int dx, int dy)
{
    // If we are not actually interacting with the view, return now
    if (!interacting())
        return;

    auto refresh = false;
    SpeciesAtom *currentAtom = nullptr;

    // What we do here depends on the current mode
    switch (interactionMode())
    {
        case (SiteViewer::SelectAreaInteraction):
            // No action to take - the selection box will be drawn from the clicked and current positions (already
            // stored)
            refresh = true;
            break;
        case (SiteViewer::RotateViewInteraction):
            // Rotate view
            if (mouseDownModifiers_.testFlag(Qt::ShiftModifier))
            {
            }
            else if (mouseDownModifiers_.testFlag(Qt::ControlModifier))
            {
            }
            else
            {
                view_.rotateView(-dy / 2.0, dx / 2.0);
                refresh = true;
            }
            break;
        case (SiteViewer::TranslateViewInteraction):
            // If this is a flat view, shift the axis limits rather than translating the view
            if (view_.isFlatView())
                view_.shiftFlatAxisLimits(dx, dy);
            else
                view_.translateView(dx / 15.0, dy / 15.0, 0.0);
            refresh = true;
            break;
        default:
            break;
    }

    if (refresh)
        postRedisplay();
}
