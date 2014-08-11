#include "elements\Element.h"
#include "effects\ElementEffect.h"

using namespace irr;

/*static void draw2DImage(video::IVideoDriver* driver, const video::ITexture* texture, f32 angleInDegree,
					    const core::rect<s32>& destRect, const core::rect<s32>& sourceRect, const core::rect<s32>* clipRect,
					    const video::SColor * const colors, bool useAlphaChannelOfTexture)
{
	if (angleInDegree == 0.0f)
	{
		driver->draw2DImage(texture, destRect, sourceRect, clipRect, colors, useAlphaChannelOfTexture);
		return;
	}

	if (!texture)
		return;

	// texcoords need to be flipped horizontally for RTTs
	const bool isRTT = texture->isRenderTarget();
	const core::dimension2d<u32>& ss = texture->getOriginalSize();
	const f32 invW = 1.f / static_cast<f32>(ss.Width);
	const f32 invH = 1.f / static_cast<f32>(ss.Height);
	const core::rect<f32> tcoords(sourceRect.UpperLeftCorner.X * invW,
		(isRTT?sourceRect.LowerRightCorner.Y:sourceRect.UpperLeftCorner.Y) * invH,
		sourceRect.LowerRightCorner.X * invW,
		(isRTT?sourceRect.UpperLeftCorner.Y:sourceRect.LowerRightCorner.Y) *invH);

	f32 cx = ((f32)(destRect.UpperLeftCorner.X) + (f32)(destRect.LowerRightCorner.X)) / 2;
	f32 cy = ((f32)(destRect.UpperLeftCorner.Y) + (f32)(destRect.LowerRightCorner.Y)) / 2;

	const video::SColor temp[4] =
	{
		0xFFFFFFFF,
		0xFFFFFFFF,
		0xFFFFFFFF,
		0xFFFFFFFF
	};

	const video::SColor* const useColor = colors ? colors : temp;

	driver->disableTextures(1);
	driver->setActiveTexture(0, texture);
	driver->setRenderStates2DMode(useColor[0].getAlpha()<255 || useColor[1].getAlpha()<255 ||
			useColor[2].getAlpha()<255 || useColor[3].getAlpha()<255,
			true, useAlphaChannelOfTexture);

	glMatrixMode(GL_MODELVIEW);
	glTranslatef(cx, cy, 0.0f);
	glRotatef(angleInDegree, 0.0f, 0.0f, 1.0f);
	glTranslatef(-cx, -cy, 0.0f);

	if (clipRect)
	{
		if (!clipRect->isValid())
			return;

		glEnable(GL_SCISSOR_TEST);
		const core::dimension2d<u32>& renderTargetSize = getCurrentRenderTargetSize();
		glScissor(clipRect->UpperLeftCorner.X, renderTargetSize.Height-clipRect->LowerRightCorner.Y,
			clipRect->getWidth(), clipRect->getHeight());
	}

	glBegin(GL_QUADS);

	glColor4ub(useColor[0].getRed(), useColor[0].getGreen(), useColor[0].getBlue(), useColor[0].getAlpha());
	glTexCoord2f(tcoords.UpperLeftCorner.X, tcoords.UpperLeftCorner.Y);
	glVertex2f(GLfloat(destRect.UpperLeftCorner.X), GLfloat(destRect.UpperLeftCorner.Y));

	glColor4ub(useColor[3].getRed(), useColor[3].getGreen(), useColor[3].getBlue(), useColor[3].getAlpha());
	glTexCoord2f(tcoords.LowerRightCorner.X, tcoords.UpperLeftCorner.Y);
	glVertex2f(GLfloat(destRect.LowerRightCorner.X), GLfloat(destRect.UpperLeftCorner.Y));

	glColor4ub(useColor[2].getRed(), useColor[2].getGreen(), useColor[2].getBlue(), useColor[2].getAlpha());
	glTexCoord2f(tcoords.LowerRightCorner.X, tcoords.LowerRightCorner.Y);
	glVertex2f(GLfloat(destRect.LowerRightCorner.X), GLfloat(destRect.LowerRightCorner.Y));

	glColor4ub(useColor[1].getRed(), useColor[1].getGreen(), useColor[1].getBlue(), useColor[1].getAlpha());
	glTexCoord2f(tcoords.UpperLeftCorner.X, tcoords.LowerRightCorner.Y);
	glVertex2f(GLfloat(destRect.UpperLeftCorner.X), GLfloat(destRect.LowerRightCorner.Y));

	glEnd();

	if (clipRect)
		glDisable(GL_SCISSOR_TEST);

	glLoadIdentity();
}*/

ElementEffect::ElementEffect(Element* element)
 : m_element(element)
{
}

void ElementEffect::drawTile(irr::core::vector2di screenPos, float angle, uint8_t alpha) const
{

}

irr::core::vector2di ElementEffect::getScreenPosition() const
{

}
