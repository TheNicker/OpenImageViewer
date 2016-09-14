#include "PreCompiled.h"
#include "ZoomScrollState.h"

namespace OIV
{
    void ZoomScrollState::TranslateOffset(Ogre::Vector2 offset)
    {
        SetOffset(fUVOffset + offset);
    }

    void ZoomScrollState::SetOffset(Ogre::Vector2 offset)
    {
        using namespace Ogre;
        const Vector2 Max_Margin = Vector2(0.25, 0.25);
        
        Vector2 uvScale = GetARFixedUVScale();

        const Vector2 marginFactor = Vector2(Max_Margin.x * uvScale.x, Max_Margin.y * uvScale.y);
        // Image is larger then window
        if (uvScale.x < 1)
        {
            
            Real maxOffset = 1 - uvScale.x + marginFactor.x;
            fUVOffset.x = std::max(static_cast<Real>(-marginFactor.x), std::min(maxOffset, offset.x));
        }
        else
        {
            // Image is smaller then window
            //Keep in center
            fUVOffset.x = -(uvScale.x - 1) / 2;// offset.x;
        }

        if (uvScale.y < 1)
        {
            Real maxOffset = 1 - uvScale.y + marginFactor.y;
            fUVOffset.y = std::max(static_cast<Real>(-marginFactor.y), std::min(maxOffset, offset.y));
        }
        else
        {
            //Keep in center
            fUVOffset.y = -(uvScale.y - 1) / 2;// offset.x;
        }
        NotifyDirty();
    }

    void ZoomScrollState::SetScale(Ogre::Vector2 scale)
    {
        fUVScale = scale;
        NotifyDirty();
    }

    Ogre::Vector2 ZoomScrollState::GetARFixedUVScale()
    {
        using namespace Ogre;
        
        Vector2 windowSize = fListener->GetWindowSize();
        Vector2 textureSize = fListener->GetImageSize();

        Real textureAR = textureSize.x / textureSize.y;
        Real windowAR = windowSize.x / windowSize.y;


        Vector2 fixedUVScale = fUVScale;
        Real ARFix = textureAR / windowAR;;
        if (ARFix < 1)
            fixedUVScale.x /= ARFix;
        else
            fixedUVScale.y *= ARFix;

        return fixedUVScale;
    }

    void ZoomScrollState::Refresh()
    {
        SupressDirty(true);
        RefreshScale();
        RefreshOffset();
        SupressDirty(false);

    }

    void ZoomScrollState::RefreshScale()
    {
        SetScale(fUVScale);
    }

    void ZoomScrollState::RefreshOffset()
    {
        SetOffset(fUVOffset);
    }

    void ZoomScrollState::NotifyDirty()
    {
        if (fSupressDirty == false)
            fListener->NotifyDirty();
        else
            fDirtyQueued = true;
    }

    void ZoomScrollState::SupressDirty(bool surpress)
    {
        if (surpress == true)
        {
            fSupressDirty = true;
        }
        else
        {
            fSupressDirty = false;
            if (fDirtyQueued)
            {
                NotifyDirty();
                fDirtyQueued = false;
            }
        }
    }

    void ZoomScrollState::Zoom(Ogre::Real amount)
    {
        if (amount == 0)
            return;

        SupressDirty(true);
        using namespace Ogre;

        Vector2 mousePos = fListener->GetMousePosition();
        Vector2 windowSize = fListener->GetWindowSize();
        Vector2 screenOffset = (mousePos / windowSize);
        if (fInverted)
        {
            assert(screenOffset.y <= 1.0);
            screenOffset.y = 1 - screenOffset.y;
        }


        Real currentZoom = 1 / GetScale().x;
        
        Real targetZoom;

        if (amount > 0)
            targetZoom = currentZoom * (1 + amount);
        else
        if (amount < 0)
            targetZoom = currentZoom *( 1.0 - std::min(-amount,0.95f));

        Vector2 uvNewScale = Vector2::UNIT_SCALE /  targetZoom;

        Vector2 oldScaleFixed = GetARFixedUVScale();
        SetScale(uvNewScale);
        Vector2 newScaleFixed = GetARFixedUVScale();

        Vector2 totalOffset = (oldScaleFixed - newScaleFixed);

        //zoom relative to mouse position
        Vector2 offsetChange = totalOffset * screenOffset;
        TranslateOffset(offsetChange);
        SupressDirty(false);
    }

    void ZoomScrollState::SetInvertedVCoordinate(bool inverted)
    {
        fInverted = inverted;
    }

    void ZoomScrollState::Pan(Ogre::Vector2 amount)
    {
        //TODO: pan according to X and Y
        Ogre::Vector2 panFactor = amount * GetScale().x;
        panFactor.y *= fInverted ? -1 : 1;
        TranslateOffset(panFactor);
    }
}