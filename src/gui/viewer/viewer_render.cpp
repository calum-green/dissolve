/*
	*** Base Viewer - Rendering
	*** src/gui/viewer/viewer_render.cpp
	Copyright T. Youngs 2013-2019

	This file is part of Dissolve.

	Dissolve is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Dissolve is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Dissolve.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "gui/viewer/viewer.hui"
#include "gui/viewer/render/fontinstance.h"
#include "base/messenger.h"
#include "base/sysfunc.h"
#include <QApplication>
#include <QClipboard>
#include <QOpenGLFramebufferObjectFormat>
#include <QPainter>
#include <QProgressDialog>


// Initialise context widget (when created by Qt)
void BaseViewer::initializeGL()
{
	// Setup function pointers to OpenGL extension functions
	initializeOpenGLFunctions();

	// Setup offscreen context
	Messenger::printVerbose("Setting up offscreen context and surface...");
        offscreenContext_.setShareContext(context());
        offscreenContext_.setFormat(context()->format());
        offscreenContext_.create();
        offscreenSurface_.setFormat(context()->format());
	offscreenSurface_.create();
	Messenger::printVerbose("Done.");

	// Check for vertex buffer extensions
        if ((!hasOpenGLFeature(QOpenGLFunctions::Buffers)) && (PrimitiveInstance::globalInstanceType() == PrimitiveInstance::VBOInstance))
	{
		printf("VBO extension is requested but not available, so reverting to display lists instead.\n");
		PrimitiveInstance::setGlobalInstanceType(PrimitiveInstance::ListInstance);
	}

	valid_ = true;

	// Recalculate viewport
	view_.recalculateViewport(contextWidth_, contextHeight_);
	view_.recalculateView();

	// Perform any custom post-initialisation operations
	postInitialiseGL();
}

// Widget repaint
void BaseViewer::paintGL()
{
	// Do nothing if the canvas is not valid, or we are still drawing from last time
	if ((!valid_) || drawing_ ) return;

	// Set the drawing flag so we don't have any rendering clashes
	drawing_ = true;

	// Setup basic GL stuff
	setupGL();

	// Render full scene
	renderGL();

	// Set the rendering flag to false
	drawing_ = false;
}

// Resize function
void BaseViewer::resizeGL(int newwidth, int newheight)
{
	// Store the new width and height of the widget
	contextWidth_ = (GLsizei) newwidth;
	contextHeight_ = (GLsizei) newheight;

	// Recalculate viewport
	view_.recalculateViewport(contextWidth_, contextHeight_);
	view_.recalculateView();

	// Perform any custom post-resize operations
	postResizeGL();
}

// Main rendering function
void BaseViewer::renderGL(int xOffset, int yOffset)
{
	GLfloat colourBlack[4] = { 0.0, 0.0, 0.0, 1.0 };

	// Set colour mode
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);

	// Clear the colour and depth buffers
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Update the View
	if (view_.autoFollowType() != View::NoAutoFollow) view_.autoFollowData();
	view_.recalculateView();

	// Set-up the GL viewport
	glViewport(view_.viewportMatrix()[0] + xOffset, view_.viewportMatrix()[1] + yOffset, view_.viewportMatrix()[2], view_.viewportMatrix()[3]);
// 		printf("Viewport for pane '%s' is %i %i %i %i (offset = %i %i)\n" , qPrintable(view_.name()), view_.viewportMatrix()[0], view_.viewportMatrix()[1], view_.viewportMatrix()[2], view_.viewportMatrix()[3], xOffset, yOffset);

	// Apply our View's projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(view_.projectionMatrix().matrix());

	// Grab the View's standard and inverse matrices
	Matrix4 viewMatrix = view_.viewMatrix();
	Matrix4 viewRotationInverse = view_.viewRotationInverse();

	// Apply the view matrix, and render the Axes associated to the View
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(viewMatrix.matrix());
	glColor4fv(colourBlack);

	if (axesVisible_)
	{
		int skipAxis = -1;
		if (view_.viewType() == View::FlatXYView) skipAxis = 2;
		else if (view_.viewType() == View::FlatXZView) skipAxis = 1;
		else if (view_.viewType() == View::FlatZYView) skipAxis = 0;

		// -- Render axis text
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_BLEND);
		glEnable(GL_LIGHTING);
		LineStyle().sendToGL(lineWidthScaling_);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (fontInstance_.fontOK())
		{
			fontInstance_.font()->FaceSize(1);
			for (int axis=0; axis<3; ++axis) if (view_.axes().visible(axis) && (axis != skipAxis))
			{
				view_.axes().labelPrimitive(axis).renderAll(fontInstance_, viewMatrix, viewRotationInverse, view_.textZScale());
	// 			if (updateQueryDepth()) setQueryObject(DataViewer::AxisTickLabelObject, DissolveSys::itoa(axis));
				view_.axes().titlePrimitive(axis).renderAll(fontInstance_, viewMatrix, viewRotationInverse, view_.textZScale());
	// 			if (updateQueryDepth()) setQueryObject(DataViewer::AxisTitleLabelObject, DissolveSys::itoa(axis));
			}
		}

		// -- Render axis (and grid) lines
		glLoadMatrixd(viewMatrix.matrix());
		glDisable(GL_LIGHTING);
		glEnable(GL_LINE_SMOOTH);
		for (int axis=0; axis<3; ++axis) if (view_.axes().visible(axis) && (axis != skipAxis))
		{
			view_.axes().gridLineMinorStyle(axis).sendToGL(lineWidthScaling_);
			view_.axes().gridLineMinorPrimitive(axis).sendToGL();
	// 		if (updateQueryDepth()) setQueryObject(DataViewer::GridLineMinorObject, DissolveSys::itoa(axis));
		}
		for (int axis=0; axis<3; ++axis) if (view_.axes().visible(axis) && (axis != skipAxis))
		{
			view_.axes().gridLineMajorStyle(axis).sendToGL(lineWidthScaling_);
			view_.axes().gridLineMajorPrimitive(axis).sendToGL();
	// 		if (updateQueryDepth()) setQueryObject(DataViewer::GridLineMajorObject, DissolveSys::itoa(axis));
		}

		// -- Reset line style, ensure polygons are now filled, and render the axis lines
		LineStyle().sendToGL(lineWidthScaling_);
		for (int axis=0; axis<3; ++axis) if (view_.axes().visible(axis) && (axis != skipAxis))
		{
			view_.axes().axisPrimitive(axis).sendToGL();
	// 		if (updateQueryDepth()) setQueryObject(DataViewer::AxisLineObject, DissolveSys::itoa(axis));
		}
		glEnable(GL_LIGHTING);
		glDisable(GL_LINE_SMOOTH);
	}

	// Enable clip planes to enforce limits in Axes volume
	if (clipToAxesVolume_) enableClipping();

	// Draw all Renderables
	for (Renderable* rend = renderables_.first(); rend != NULL; rend = rend->next)
	{
		if (!rend->isVisible()) continue;

		// Set shininess for Renderable
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, rend->displaySurfaceShininess());

		rend->updateAndSendPrimitives(view_, groupManager_, renderingOffScreen_, renderingOffScreen_, context(), lineWidthScaling_);

		// Update query
// 		if (updateQueryDepth()) setQueryObject(DataViewer::RenderableObject, rend->objectTag());

		glEnable(GL_COLOR_MATERIAL);
	}

	// Disable clip planes
	if (clipToAxesVolume_) disableClipping();

	// Set up an orthographic matrix for any 2D overlays
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, view_.viewportMatrix()[2], 0, view_.viewportMatrix()[3], -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);

	// Call the derived-class overlay function
	render2DOverlay();
}

// Perform post-initialisation operations
void BaseViewer::postInitialiseGL()
{
}

// Perform post-resize operations
void BaseViewer::postResizeGL()
{
}

// Setup basic GL properties (called each time before renderScene())
void BaseViewer::setupGL()
{
	// Define colours etc.
	GLfloat backgroundColour[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat spotlightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat spotlightDiffuse[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat spotlightSpecular[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat spotlightPosition[4] = { 1.0f, 1.0f, 0.0f, 0.0f };
	GLfloat specularColour[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	// Clear (background) colour
	glClearColor(backgroundColour[0], backgroundColour[1], backgroundColour[2], backgroundColour[3]);

	// Perspective hint
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);

	// Smooth shading
	glShadeModel(GL_SMOOTH);

	// Auto-normalise surface normals
	glEnable(GL_NORMALIZE);	

	// Set up the light model
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_AMBIENT, spotlightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, spotlightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spotlightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, spotlightPosition);
	glEnable(GL_LIGHT0);

	// Set specular reflection colour
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColour);

	// Configure antialiasing
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	// Configure fog effects
//	glFogi(GL_FOG_MODE, GL_LINEAR);
//	prefs.copyColour(Prefs::BackgroundColour, col);
//	glFogfv(GL_FOG_COLOR, col);
//	glFogf(GL_FOG_DENSITY, 0.35f);
//	glHint(GL_FOG_HINT, GL_NICEST);
//	glFogi(GL_FOG_START, prefs.depthNear());
//	glFogi(GL_FOG_END, prefs.depthFar());
//	glEnable(GL_FOG);
	glDisable(GL_FOG);

	// Configure face culling
// 	glCullFace(GL_BACK);
// 	glEnable(GL_CULL_FACE);
}

/*
 * Public Functions
 */

// Return the current height of the drawing area
GLsizei BaseViewer::contextHeight() const
{
	return contextHeight_;
}

// Return the current width of the drawing area
GLsizei BaseViewer::contextWidth() const
{
	return contextWidth_;
}

// Set up font instance with supplied font
bool BaseViewer::setUpFont(const char* fontFileName)
{
	return fontInstance_.setUp(fontFileName);
}

// Return font instance
FontInstance& BaseViewer::fontInstance()
{
	return fontInstance_;
}

// Check for GL error
void BaseViewer::checkGlError()
{
	GLenum glerr = GL_NO_ERROR;
	do
	{
		switch (glGetError())
		{
			case (GL_INVALID_ENUM): Messenger::printVerbose("GLenum argument out of range\n"); break;
			case (GL_INVALID_VALUE): Messenger::printVerbose("Numeric argument out of range\n"); break;
			case (GL_INVALID_OPERATION): Messenger::printVerbose("Operation illegal in current state\n"); break;
			case (GL_STACK_OVERFLOW): Messenger::printVerbose("Command would cause a stack overflow\n"); break;
			case (GL_STACK_UNDERFLOW): Messenger::printVerbose("Command would cause a stack underflow\n"); break;
			case (GL_OUT_OF_MEMORY): Messenger::printVerbose("Not enough memory left to execute command\n"); break;
			case (GL_NO_ERROR): Messenger::printVerbose("No GL error\n"); break;
			default:
				Messenger::printVerbose("Unknown GL error?\n");
				break;
		}
	} while (glerr != GL_NO_ERROR);
}

// Refresh widget / scene
void BaseViewer::postRedisplay()
{
	if ((!valid_) || drawing_) return;
	update();
}

// Set line width and text scaling to use
void BaseViewer::setObjectScaling(double scaling)
{
	lineWidthScaling_ = scaling;

	// Pass this value on to those that depend on it
	// TODO these should be local objects so that we don't interfere with other view types
// 	TextPrimitive::setTextSizeScale(scaling);
}

// Set and enable clip planes suitable for current axis limits and view
void BaseViewer::enableClipping()
{
	// Load identity matrix
	glLoadMatrixd(view_.viewMatrix().matrix());

	// Get clip plane coordinates and indices
	Vec3<double> clipMin = view_.axes().clipMin();
	Vec3<double> clipMax = view_.axes().clipMax();
	static GLenum planes[6] = { GL_CLIP_PLANE0, GL_CLIP_PLANE1, GL_CLIP_PLANE2, GL_CLIP_PLANE3, GL_CLIP_PLANE4, GL_CLIP_PLANE5 };

	// Loop over axes
	Vec3<double> translation;
	for (int axis=0; axis<3; ++axis)
	{
		translation.zero();

		// Positive
		translation[axis] = clipMin[axis];
		GLdouble plane[4] = { 0.0, 0.0, 0.0, 0.0 };
		plane[axis] = 1.0;

		glPushMatrix();
		glTranslated(translation.x, translation.y, translation.z);
		glClipPlane(planes[axis], plane);
		glEnable(planes[axis]);
		glPopMatrix();

		// Negative
		translation[axis] = clipMax[axis];
		plane[axis] = -1.0;

		glPushMatrix();
		glTranslated(translation.x, translation.y, translation.z);
		glClipPlane(planes[axis+3], plane);
		glEnable(planes[axis+3]);
		glPopMatrix();
	}
}

// Disable clip planes
void BaseViewer::disableClipping()
{
	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_CLIP_PLANE1);
	glDisable(GL_CLIP_PLANE2);
	glDisable(GL_CLIP_PLANE3);
	glDisable(GL_CLIP_PLANE4);
	glDisable(GL_CLIP_PLANE5);
}

// Grab current contents of framebuffer
QPixmap BaseViewer::frameBuffer()
{
	return QPixmap::fromImage(grabFramebuffer());
}

// Render or grab image
QPixmap BaseViewer::generateImage(int imageWidth, int imageHeight)
{
	// Flag that we are rendering offscreen
	renderingOffScreen_ = true;

	// Scale current line width and text scaling to reflect size of exported image
	setObjectScaling( double(imageHeight) / double(contextHeight()) );

	// Make the offscreen surface the current context
	offscreenContext_.makeCurrent(&offscreenSurface_);

	// Set tile size
	int tileWidth = 512;
	int tileHeight = 512;

	// Initialise framebuffer format and object
	QOpenGLFramebufferObjectFormat fboFormat;
	fboFormat.setMipmap(true);
	fboFormat.setSamples(4);
	fboFormat.setAttachment(QOpenGLFramebufferObject::Depth);
	QOpenGLFramebufferObject frameBufferObject(tileWidth, tileHeight, fboFormat);

	if (!frameBufferObject.bind())
	{
		Messenger::print("Failed to bind framebuffer object when generating image.");
		return QPixmap();
	}

	// Create a QPixmap of the desired full size and a QPainter for it
	QPixmap pixmap = QPixmap(imageWidth, imageHeight);
	QPainter painter(&pixmap);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::white);
	painter.drawRect(0,0,imageWidth, imageHeight);

	// Calculate scale factors for ViewLayout, so that the context width/height is scaled to the desired image size
// 	double xScale = double(imageWidth) / double(tileWidth);
// 	double yScale = double(imageHeight) / double(tileHeight);
	int nX = imageWidth / tileWidth + ((imageWidth %tileWidth) ? 1 : 0);
	int nY = imageHeight / tileHeight + ((imageHeight %tileHeight) ? 1 : 0);

	// Recalculate view layout - no offset or scaling, just to the desired full image size
	// TODO Need call to a general virtual to do this...
// 	uChromaBase_->viewLayout().setOffsetAndScale(0, 0, 1.0, 1.0);
// 	uChromaBase_->viewLayout().recalculate(imageWidth, imageHeight);

	// Loop over tiles in x and y
	QProgressDialog progress("Generating tiled image", "Cancel", 0, nX*nY);
	progress.setWindowTitle("Dissolve");
	for (int x=0; x<nX; ++x)
	{
		for (int y=0; y<nY; ++y)
		{
			// Set progress value and check for cancellation
			if (progress.wasCanceled()) break;
			progress.setValue(x*nY+y);

			// Generate this tile
			if (!frameBufferObject.bind()) printf("Failed to bind framebuffer object.\n");
			setupGL();
			renderGL(-x*tileWidth, -y*tileHeight);
			QImage fboImage(frameBufferObject.toImage());
			QImage tile(fboImage.constBits(), fboImage.width(), fboImage.height(), QImage::Format_ARGB32);

			// Paste this tile into the main image
			painter.drawImage(x*tileWidth, imageHeight-(y+1)*tileHeight, tile);
// 			tile.save(QString("tile-%1%2.png").arg(x).arg(y), "png");
		}
		if (progress.wasCanceled()) break;
	}

	// Finalise and save
	painter.end();

	// Reset line width and text size
	setObjectScaling(1.0);

	// Make sure the Viewer knows we no longer want offscreen rendering, and revert to normal quality primitives
	renderingOffScreen_ = false;

	// The sizes of panes may now be incorrect, so reset everything
	// TODO Need call to a general virtual to do this...
// 	uChromaBase_->viewLayout().setOffsetAndScale(0, 0, 1.0, 1.0);
// 	uChromaBase_->viewLayout().recalculate(contextWidth_, contextHeight_);

	// Reset context back to main view
	makeCurrent();

	return pixmap;
}

// Copy current view to clipboard as an image
void BaseViewer::copyViewToClipboard(bool checked)
{
	// Generate image of current view
	QPixmap pixmap = frameBuffer();

	// Copy image to clipboard
	QClipboard* clipboard = QApplication::clipboard();
	clipboard->setImage(pixmap.toImage());
}
