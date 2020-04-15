#include "Render.hpp"
#include <vector>

namespace aidrive
{
namespace render
{

ImVec2 convert(const Vector2f& in)
{
    return {in[0], in[1]};
}

ImVec2 convert(const Vector3f& in)
{
    return {in[0], in[1]};
}

void drawRect(ImDrawList* drawList,
              const std::vector<ImVec2>& vertices,
              ImU32 color,
              float32_t thickness)
{
    drawList->AddLine(vertices[0], vertices[1], color, thickness);
    drawList->AddLine(vertices[1], vertices[2], color, thickness);
    drawList->AddLine(vertices[2], vertices[3], color, thickness);
    drawList->AddLine(vertices[3], vertices[0], color, thickness);
}

void Renderer::setImDrawList(ImDrawList* list)
{
    m_list = list;
}

void Renderer::setEye(const Vector2f& eye)
{
    // set translation
    m_view(0, 2) = -eye[0];
    m_view(1, 2) = -eye[1];
}

void Renderer::setPixelPerMeter(float32_t pm)
{
    m_projection(0, 0) = pm;
    m_projection(1, 1) = pm;
}

void Renderer::drawRect(const Vector3f& pose,
                        const Rect2f& dim)
{
    Vector2f pos{pose[0], pose[1]};
    float32_t hdg = pose[2];

    Matrix2f rot{};

    rot << std::cos(hdg), std::sin(hdg),
        -std::sin(hdg), std::cos(hdg);

    std::vector<Vector2f> vertices{};
    vertices.push_back(pos + rot * Vector2f{dim.length / 2.f, dim.width / 2.f});
    vertices.push_back(pos + rot * Vector2f{dim.length / 2.f, -dim.width / 2.f});
    vertices.push_back(pos + rot * Vector2f{-dim.length / 2.f, -dim.width / 2.f});
    vertices.push_back(pos + rot * Vector2f{-dim.length / 2.f, dim.width / 2.f});

    // convert to pixel coordinates
    std::vector<ImVec2> verticesInPixel{};

    for (const Vector2f& v : vertices)
    {
        Vector3f vNew = m_projection * m_view * Vector3f{v[0], v[1], 1.0f};
        verticesInPixel.push_back(convert(vNew));
    }

    // now draw
    // TODO: pass via param[in]
    constexpr float32_t thickness = 3.0f;
    const ImU32 color             = COLOR_BLACK;

    render::drawRect(m_list, verticesInPixel, color, thickness);
}

} // namespace render
} // namespace aidrive
