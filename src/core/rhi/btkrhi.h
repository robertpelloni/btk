/***********************************************************************
*
* BTK Rendering Hardware Interface (RHI) abstraction.
*
* BtkRhi provides a unified GPU rendering API that abstracts over
* OpenGL, Vulkan, Direct3D, and Metal backends. This design is
* influenced by Qt 6's RHI and provides BTK with hardware-accelerated
* rendering across all platforms without coupling to any single API.
*
* Architecture:
*   BtkRhi (device/context) -> BtkRhiPipeline (shader program)
*                            -> BtkRhiBuffer (vertex/index/uniform)
*                            -> BtkRhiTexture (image data)
*                            -> BtkRhiRenderTarget (output surface)
*                            -> BtkRhiCommandBuffer (recording draws)
*
***********************************************************************/

#ifndef BTK_RHI_H
#define BTK_RHI_H

#include <qglobal.h>
#include <qstring.h>
#include <qsize.h>
#include <qpoint.h>
#include <qrect.h>
#include <qcolor.h>
#include <qvariant.h>
#include <qlist.h>
#include <qpair.h>
#include <memory>

/**
 * Enumeration of supported graphics API backends.
 */
enum class BtkRhiBackend {
   OpenGL,
   Vulkan,
   Direct3D11,
   Direct3D12,
   Metal,
   Null           // For testing / headless rendering
};

/**
 * Pixel format for textures and render targets.
 */
enum class BtkRhiFormat {
   Unknown,
   R8_UNorm,
   RG8_UNorm,
   RGB8_UNorm,
   RGBA8_UNorm,
   R16_UNorm,
   RG16_UNorm,
   RGBA16_UNorm,
   R16_Float,
   RG16_Float,
   RGBA16_Float,
   R32_Float,
    RG32_Float,
   RGBA32_Float,
   Depth16,
   Depth24,
   Depth32_Float,
   Depth24_Stencil8,
   BC1_RGB_UNorm,       // DXT1
   BC2_RGBA_UNorm,      // DXT3
   BC3_RGBA_UNorm,      // DXT5
   BC7_RGBA_UNorm,
};

/**
 * Primitive topology for draw calls.
 */
enum class BtkRhiTopology {
   Points,
   Lines,
   LineStrip,
   Triangles,
   TriangleStrip,
   TriangleFan,
};

/**
 * Comparison function for depth/stencil operations.
 */
enum class BtkRhiCompareOp {
   Never,
   Less,
   Equal,
   LessOrEqual,
   Greater,
   NotEqual,
   GreaterOrEqual,
   Always,
};

/**
 * Blend factor for color blending.
 */
enum class BtkRhiBlendFactor {
   Zero,
   One,
   SrcColor,
   OneMinusSrcColor,
   DstColor,
   OneMinusDstColor,
   SrcAlpha,
   OneMinusSrcAlpha,
   DstAlpha,
   OneMinusDstAlpha,
   ConstantColor,
   OneMinusConstantColor,
   ConstantAlpha,
   OneMinusConstantAlpha,
   SrcAlphaSaturate,
};

/**
 * Blend operation.
 */
enum class BtkRhiBlendOp {
   Add,
   Subtract,
   ReverseSubtract,
   Min,
   Max,
};

/**
 * Stencil operation.
 */
enum class BtkRhiStencilOp {
   Keep,
   Zero,
   Replace,
   IncrementClamp,
   DecrementClamp,
   Invert,
   IncrementWrap,
   DecrementWrap,
};

/**
 * Buffer usage flags.
 */
enum BtkRhiBufferUsageFlag {
   BtkRhiBufferUsage_Vertex   = 0x0001,
   BtkRhiBufferUsage_Index    = 0x0002,
   BtkRhiBufferUsage_Uniform  = 0x0004,
   BtkRhiBufferUsage_TransferSrc = 0x0008,
   BtkRhiBufferUsage_TransferDst = 0x0010,
};

Q_DECLARE_FLAGS(BtkRhiBufferUsage, BtkRhiBufferUsageFlag)

/**
 * Texture usage flags.
 */
enum BtkRhiTextureUsageFlag {
   BtkRhiTextureUsage_Sampled      = 0x0001,
   BtkRhiTextureUsage_RenderTarget = 0x0002,
   BtkRhiTextureUsage_DepthStencil = 0x0004,
   BtkRhiTextureUsage_TransferSrc  = 0x0008,
   BtkRhiTextureUsage_TransferDst  = 0x0010,
};

Q_DECLARE_FLAGS(BtkRhiTextureUsage, BtkRhiTextureUsageFlag)

/**
 * Shader stage.
 */
enum class BtkRhiShaderStage {
   Vertex,
   Fragment,
   Geometry,
   TessellationControl,
   TessellationEvaluation,
   Compute,
};

/**
 * Culling mode.
 */
enum class BtkRhiCullMode {
   None,
   Front,
   Back,
   FrontAndBack,
};

/**
 * Front face winding order.
 */
enum class BtkRhiFrontFace {
   CounterClockwise,
   Clockwise,
};

/**
 * Filter mode for texture sampling.
 */
enum class BtkRhiFilter {
   Nearest,
   Linear,
};

/**
 * Texture address mode.
 */
enum class BtkRhiAddressMode {
   Repeat,
   MirroredRepeat,
   ClampToEdge,
   ClampToBorder,
   MirrorClampToEdge,
};

/**
 * Color blend attachment description.
 */
struct BtkRhiBlendAttachment {
   bool blendEnable = false;
   BtkRhiBlendFactor srcColorBlendFactor = BtkRhiBlendFactor::SrcAlpha;
   BtkRhiBlendFactor dstColorBlendFactor = BtkRhiBlendFactor::OneMinusSrcAlpha;
   BtkRhiBlendOp colorBlendOp = BtkRhiBlendOp::Add;
   BtkRhiBlendFactor srcAlphaBlendFactor = BtkRhiBlendFactor::One;
   BtkRhiBlendFactor dstAlphaBlendFactor = BtkRhiBlendFactor::Zero;
   BtkRhiBlendOp alphaBlendOp = BtkRhiBlendOp::Add;
   uint32_t colorWriteMask = 0xF;  // RGBA
};

/**
 * Stencil operation state.
 */
struct BtkRhiStencilOpState {
   BtkRhiStencilOp failOp = BtkRhiStencilOp::Keep;
   BtkRhiStencilOp passOp = BtkRhiStencilOp::Keep;
   BtkRhiStencilOp depthFailOp = BtkRhiStencilOp::Keep;
   BtkRhiCompareOp compareOp = BtkRhiCompareOp::Always;
   uint32_t compareMask = 0xFF;
   uint32_t writeMask = 0xFF;
   uint32_t reference = 0;
};

/**
 * Viewport description.
 */
struct BtkRhiViewport {
   float x = 0.0f;
   float y = 0.0f;
   float width = 0.0f;
   float height = 0.0f;
   float minDepth = 0.0f;
   float maxDepth = 1.0f;
};

/**
 * Scissor rectangle.
 */
struct BtkRhiScissor {
   int x = 0;
   int y = 0;
   int width = 0;
   int height = 0;
};

/**
 * Vertex input binding description.
 */
struct BtkRhiVertexBinding {
   uint32_t binding = 0;
   uint32_t stride = 0;
   enum class Rate { Vertex, Instance } inputRate = Rate::Vertex;
};

/**
 * Vertex input attribute description.
 */
struct BtkRhiVertexAttribute {
   uint32_t location = 0;
   uint32_t binding = 0;
   BtkRhiFormat format = BtkRhiFormat::Unknown;
   uint32_t offset = 0;
};

/**
 * Shader resource binding description.
 */
struct BtkRhiResourceBinding {
   uint32_t binding = 0;
   enum class Type { UniformBuffer, SampledTexture, Sampler, StorageBuffer } type;
   BtkRhiShaderStage stage = BtkRhiShaderStage::Fragment;
};

// Forward declarations
class BtkRhiPipeline;
class BtkRhiBuffer;
class BtkRhiTexture;
class BtkRhiSampler;
class BtkRhiRenderTarget;
class BtkRhiCommandBuffer;
class BtkRhiShader;

/**
 * BtkRhi is the main interface to the rendering hardware.
 * It owns the device context and provides factory methods for
 * creating GPU resources.
 */
class Q_CORE_EXPORT BtkRhi
{
 public:
   virtual ~BtkRhi();

   /**
    * Create an RHI instance for the specified backend.
    * Returns nullptr if the backend is not available on this platform.
    */
   static std::unique_ptr<BtkRhi> create(BtkRhiBackend backend);

   /**
    * Query the active backend.
    */
   virtual BtkRhiBackend backend() const = 0;

   /**
    * Query backend name for diagnostics.
    */
   virtual QString backendName() const = 0;

   /**
    * Initialize the RHI device. Must be called before any other operations.
    * Returns true if initialization succeeded.
    */
   virtual bool initialize() = 0;

   /**
    * Check if the RHI is successfully initialized.
    */
   virtual bool isInitialized() const = 0;

   /**
    * Begin a new frame. Returns a command buffer for recording.
    */
   virtual BtkRhiCommandBuffer* beginFrame() = 0;

   /**
    * End the current frame and submit for presentation.
    */
   virtual void endFrame() = 0;

   /**
    * Factory methods for GPU resources.
    */
   virtual BtkRhiBuffer* createBuffer(BtkRhiBufferUsage usage, uint32_t size) = 0;
   virtual BtkRhiTexture* createTexture(BtkRhiFormat format, int width, int height,
                                         BtkRhiTextureUsage usage) = 0;
   virtual BtkRhiSampler* createSampler(BtkRhiFilter minFilter, BtkRhiFilter magFilter,
                                         BtkRhiAddressMode addressMode) = 0;
   virtual BtkRhiPipeline* createPipeline() = 0;
   virtual BtkRhiRenderTarget* createRenderTarget() = 0;
   virtual BtkRhiShader* createShader(BtkRhiShaderStage stage, const QByteArray& source) = 0;

   /**
    * Query the maximum texture size supported by the device.
    */
   virtual int maxTextureSize() const = 0;

   /**
    * Query device limits and capabilities.
    */
   virtual bool isFeatureSupported(const QString& feature) const = 0;
};

/**
 * BtkRhiPipeline represents a complete graphics pipeline state.
 * Includes shaders, vertex layout, blend state, depth/stencil state,
 * rasterization state, and resource bindings.
 */
class Q_CORE_EXPORT BtkRhiPipeline
{
 public:
   virtual ~BtkRhiPipeline();

   void setShader(BtkRhiShaderStage stage, BtkRhiShader* shader);
   void setTopology(BtkRhiTopology topology);
   void setVertexBindings(const QList<BtkRhiVertexBinding>& bindings);
   void setVertexAttributes(const QList<BtkRhiVertexAttribute>& attributes);
   void setBlendAttachments(const QList<BtkRhiBlendAttachment>& attachments);
   void setCullMode(BtkRhiCullMode mode);
   void setFrontFace(BtkRhiFrontFace face);
   void setDepthTest(bool enable);
   void setDepthWrite(bool enable);
   void setDepthCompareOp(BtkRhiCompareOp op);
   void setStencilTest(bool enable);
   void setStencilFront(const BtkRhiStencilOpState& front);
   void setStencilBack(const BtkRhiStencilOpState& back);
   void setResourceBindings(const QList<BtkRhiResourceBinding>& bindings);

   virtual bool compile() = 0;

 protected:
   BtkRhiTopology m_topology = BtkRhiTopology::Triangles;
   QList<BtkRhiVertexBinding> m_vertexBindings;
   QList<BtkRhiVertexAttribute> m_vertexAttributes;
   QList<BtkRhiBlendAttachment> m_blendAttachments;
   BtkRhiCullMode m_cullMode = BtkRhiCullMode::None;
   BtkRhiFrontFace m_frontFace = BtkRhiFrontFace::CounterClockwise;
   bool m_depthTest = false;
   bool m_depthWrite = false;
   BtkRhiCompareOp m_depthCompareOp = BtkRhiCompareOp::Less;
   bool m_stencilTest = false;
   BtkRhiStencilOpState m_stencilFront;
   BtkRhiStencilOpState m_stencilBack;
};

/**
 * BtkRhiBuffer represents a GPU buffer (vertex, index, or uniform).
 */
class Q_CORE_EXPORT BtkRhiBuffer
{
 public:
   virtual ~BtkRhiBuffer();

   virtual bool allocate(uint32_t size) = 0;
   virtual bool upload(const void* data, uint32_t size, uint32_t offset = 0) = 0;
   virtual uint32_t size() const = 0;
};

/**
 * BtkRhiTexture represents a GPU texture resource.
 */
class Q_CORE_EXPORT BtkRhiTexture
{
 public:
   virtual ~BtkRhiTexture();

   virtual bool upload(const void* data, int rowPitch = 0) = 0;
   virtual BtkRhiFormat format() const = 0;
   virtual QSize size() const = 0;
};

/**
 * BtkRhiSampler represents a texture sampling state.
 */
class Q_CORE_EXPORT BtkRhiSampler
{
 public:
   virtual ~BtkRhiSampler();
};

/**
 * BtkRhiRenderTarget represents an output surface for rendering.
 */
class Q_CORE_EXPORT BtkRhiRenderTarget
{
 public:
   virtual ~BtkRhiRenderTarget();

   void setColorAttachment(int index, BtkRhiTexture* texture);
   void setDepthStencilAttachment(BtkRhiTexture* texture);
   void setClearColor(int index, const QColor& color);
   void setClearDepth(float depth);
   void setClearStencil(uint32_t stencil);
   void setViewport(const BtkRhiViewport& viewport);
   void setScissor(const BtkRhiScissor& scissor);

   virtual QSize size() const = 0;

 protected:
   QColor m_clearColor;
   float m_clearDepth = 1.0f;
   uint32_t m_clearStencil = 0;
   BtkRhiViewport m_viewport;
   BtkRhiScissor m_scissor;
};

/**
 * BtkRhiShader represents a compiled shader module.
 */
class Q_CORE_EXPORT BtkRhiShader
{
 public:
   virtual ~BtkRhiShader();

   virtual BtkRhiShaderStage stage() const = 0;
   virtual bool isValid() const = 0;
   virtual QString errorMessage() const = 0;
};

/**
 * BtkRhiCommandBuffer records rendering commands for execution.
 */
class Q_CORE_EXPORT BtkRhiCommandBuffer
{
 public:
   virtual ~BtkRhiCommandBuffer();

   virtual void begin() = 0;
   virtual void end() = 0;

   virtual void beginRenderPass(BtkRhiRenderTarget* target) = 0;
   virtual void endRenderPass() = 0;

   virtual void setPipeline(BtkRhiPipeline* pipeline) = 0;
   virtual void setViewport(const BtkRhiViewport& viewport) = 0;
   virtual void setScissor(const BtkRhiScissor& scissor) = 0;

   virtual void setVertexBuffer(uint32_t binding, BtkRhiBuffer* buffer, uint32_t offset = 0) = 0;
   virtual void setIndexBuffer(BtkRhiBuffer* buffer, uint32_t offset = 0,
                                BtkRhiFormat indexFormat = BtkRhiFormat::R16_UNorm) = 0;
   virtual void setUniformBuffer(uint32_t binding, BtkRhiBuffer* buffer) = 0;
   virtual void setTexture(uint32_t binding, BtkRhiTexture* texture, BtkRhiSampler* sampler) = 0;

   virtual void draw(uint32_t vertexCount, uint32_t instanceCount = 1,
                     uint32_t firstVertex = 0, uint32_t firstInstance = 0) = 0;
   virtual void drawIndexed(uint32_t indexCount, uint32_t instanceCount = 1,
                            uint32_t firstIndex = 0, int32_t vertexOffset = 0,
                            uint32_t firstInstance = 0) = 0;

   virtual void drawFullscreenQuad() = 0;  // Convenience for post-processing
};

Q_DECLARE_OPERATORS_FOR_FLAGS(BtkRhiBufferUsage)
Q_DECLARE_OPERATORS_FOR_FLAGS(BtkRhiTextureUsage)

#endif
