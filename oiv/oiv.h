#pragma once
#include "ZoomScrollState.h"
#include "Interfaces\IPictureRenderer.h"
#include "Image/ImageLoader.h"
#include "interfaces/IRenderer.h"
#include "ViewParameters.h"


namespace OIV
{
    class OIV  :
          public ZoomScrollState::Listener
        , public IPictureRenderer
    {
#pragma region  //-------------Scroll state listener------------------
        virtual Vector2 GetClientSize() override;
        virtual Vector2 GetImageSize() override;
        virtual void NotifyDirty() override;
#pragma endregion 

#pragma region //-------------Private methods------------------
        IRendererSharedPtr CreateBestRenderer();
        void HandleWindowResize();
        bool IsImageLoaded() const;
        void UpdateGpuParams();
        AxisAlignedRTransform ResolveExifRotation(unsigned short exifRotation) const;
        void LoadSettings();
#pragma endregion
    public:
        OIV();

#pragma region //-------------IPictureListener implementation------------------
        virtual double Zoom(double percentage,int x,int y) override;
        virtual int Pan(double x, double y) override;
        virtual int LoadFile(void* buffer, size_t size, char* extension , bool onlyRegisteredExtension) override;
        virtual int Init() override;
        virtual int SetParent(size_t handle) override;
        virtual int Refresh() override;
        virtual Image* GetImage() override;
        virtual int SetFilterLevel(OIV_Filter_type filter_level) override;
        virtual int GetFileInformation(QryFileInformation& information) override;
        virtual int GetTexelAtMousePos(int mouseX, int mouseY, double& texelX, double& texelY) override;
        virtual int SetTexelGrid(double gridSize) override;
        virtual int GetNumTexelsInCanvas(double &x, double &y) override;
        virtual int SetClientSize(uint16_t width, uint16_t height) override;
#pragma endregion

#pragma region //-------------Private member fields------------------
    private:
        IRendererSharedPtr fRenderer;
        ViewParameters fViewParams;
        ZoomScrollState fScrollState;
        size_t fParent;
        bool fIsRefresing;
        std::string fCurrentOpenedFile;
        ImageLoader fImageLoader;
        bool fShowGrid;
        ImageSharedPtr fOpenedImage;
        int fFilterLevel;
        int fClientWidth;
        int fClientHeight;
    };
}
