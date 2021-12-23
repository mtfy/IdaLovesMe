#include "Renderer.h"
#include "../../Backend/Misc/Fonts/Fonts.h"
#include "../../Backend/Misc/Textures/Textures.h"

Render::CDraw* Render::Draw = new Render::CDraw();

using namespace Render;
using namespace IdaLovesMe;

namespace Render {
    namespace Fonts {
        LPD3DXFONT TabIcons;
        LPD3DXFONT LegitTabIcons;
        LPD3DXFONT Verdana;
        LPD3DXFONT Tahombd;
    }
}

void CDraw::CreateObjects()
{
    D3DXCreateSprite(m_Device, &m_Sprite);

    D3DPRESENT_PARAMETERS pp = {};
    DWORD nFonts = 0;

    D3DXCreateFontA(m_Device, 12, 0, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH, "Verdana", &Fonts::Verdana);
    D3DXCreateFontA(m_Device, 12, 0, FW_BOLD, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, FIXED_PITCH, "Tahoma", &Fonts::Tahombd);

    this->m_TabFont = AddFontMemResourceEx((void*)(FontsData::TabIcons), (DWORD)5192, nullptr, &nFonts);
    D3DXCreateFontA(m_Device, 47, 0, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DRAFT_QUALITY, DEFAULT_PITCH, "gamesense", &Fonts::TabIcons);

    this->m_LegitTabFont = AddFontMemResourceEx((void*)(FontsData::TabIcons), (DWORD)5192, nullptr, &nFonts);
    D3DXCreateFontA(m_Device, 32, 0, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DRAFT_QUALITY, DEFAULT_PITCH, "undefeated", &Fonts::LegitTabIcons);

    D3DXCreateTextureFromFileInMemoryEx(m_Device, TexturesData::BgTexture, 424852, 4096, 4096, D3DX_DEFAULT, NULL,
        pp.BackBufferFormat, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, NULL, NULL, NULL, &this->m_BgTexture);
}

void CDraw::ReleaseObjects()
{
    if (!Initialized)
        return;

    if (m_Sprite)
        m_Sprite->Release();
    if (Fonts::Verdana)
        Fonts::Verdana->Release();
    if (Fonts::Tahombd)
        Fonts::Tahombd->Release();

    if (Fonts::TabIcons) {
        Fonts::TabIcons->Release();
        RemoveFontMemResourceEx(this->m_TabFont);
    }
    if (Fonts::LegitTabIcons) {
        Fonts::LegitTabIcons->Release();
        RemoveFontMemResourceEx(this->m_LegitTabFont);
    }

    delete Render::Draw;
}

void CDraw::Init(LPDIRECT3DDEVICE9 D3dDevice)
{
    if (!Initialized) {
        m_Device = D3dDevice;
        CreateObjects();
        Initialized = true;
    }
}

void CDraw::Reset()
{
    D3DVIEWPORT9 screen;
    m_Device->GetViewport(&screen);

    Screen.Width = (float)screen.Width;
    Screen.Height = (float)screen.Height;
    Screen.x_center = Screen.Width / 2;
    Screen.y_center = Screen.Height / 2;

    m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    m_Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
    m_Device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);
}

void CDraw::Line(Vec2 Pos, Vec2 Pos2, D3DCOLOR Color)
{
    vertex vertices[2] = {
        { round(Pos.x), round(Pos.y), 0.0f, 1.0f, Color },
        { round(Pos2.x), round(Pos2.y), 0.0f, 1.0f, Color },
    };
    m_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    m_Device->SetTexture(0, nullptr);

    m_Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
    m_Device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);

    m_Device->DrawPrimitiveUP(D3DPT_LINELIST, 1, vertices, sizeof(vertex));

    m_Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
    m_Device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);
}

/*
void CDraw::Circle(float x, float y, float radius, int rotate, int type, bool smoothing, int resolution, D3DCOLOR Color)
{
    std::vector<vertex> circle(resolution + 2);
    float angle = rotate * D3DX_PI / 180;
    float pi;

    if (type == FULL) pi = D3DX_PI;        // Full circle
    if (type == HALF) pi = D3DX_PI / 2;      // 1/2 circle
    if (type == QUARTER) pi = D3DX_PI / 4;   // 1/4 circle

    for (int i = 0; i < resolution + 2; i++)
    {
        circle[i].x = (float)(x - radius * cos(i * (2 * pi / resolution)));
        circle[i].y = (float)(y - radius * sin(i * (2 * pi / resolution)));
        circle[i].z = 0;
        circle[i].rhw = 1;
        circle[i].Color = Color;
    }

    // Rotate matrix
    int _res = resolution + 2;
    for (int i = 0; i < _res; i++)
    {
        circle[i].x -= x;
        circle[i].y -= y;

        float xnew = circle[i].x * cos(angle) - circle[i].y * sin(angle);
        float ynew = circle[i].x * sin(angle) + circle[i].y * cos(angle);

        circle[i].x = xnew + x;
        circle[i].y = ynew + y;
    }
    /*
    m_Device->CreateVertexBuffer((resolution + 2) * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &m_VB, NULL);

    VOID* pVertices;
    m_VB->Lock(0, (resolution + 2) * sizeof(vertex), (void**)&pVertices, 0);
    memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(vertex));
    m_VB->Unlock();


    m_Device->SetTexture(0, NULL);
    m_Device->SetPixelShader(NULL);
    if (smoothing)
    {
        m_Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
        m_Device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);
    }
    m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    m_Device->SetStreamSource(0, m_VB, 0, sizeof(vertex));
    m_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

    m_Device->DrawPrimitive(D3DPT_LINESTRIP, 0, resolution);
    if (m_VB != NULL) m_VB->Release();

}

void CDraw::CircleFilled(float x, float y, float rad, float rotate, int type, int resolution, D3DCOLOR Color)
{
    std::vector<vertex> circle(resolution + 2);
    float angle = rotate * D3DX_PI / 180;
    float pi = D3DX_PI;

    if (type == FULL) pi = D3DX_PI;        // Full circle
    if (type == HALF) pi = D3DX_PI / 2;      // 1/2 circle
    if (type == QUARTER) pi = D3DX_PI / 4;   // 1/4 circle


    circle[0].x = x;
    circle[0].y = y;
    circle[0].z = 0;
    circle[0].rhw = 1;
    circle[0].Color = Color;

    for (int i = 1; i < resolution + 2; i++)
    {
        circle[i].x = (float)(x - rad * cos(pi * ((i - 1) / (resolution / 2.0f))));
        circle[i].y = (float)(y - rad * sin(pi * ((i - 1) / (resolution / 2.0f))));
        circle[i].z = 0;
        circle[i].rhw = 1;
        circle[i].Color = Color;
    }

    int _res = resolution + 2;
    for (int i = 0; i < _res; i++)
    {
        circle[i].x -= x;
        circle[i].y -= y;

        float xnew = circle[i].x * cos(angle) - circle[i].y * sin(angle);
        float ynew = circle[i].x * sin(angle) + circle[i].y * cos(angle);

        circle[i].x = xnew + x;
        circle[i].y = ynew + y;
    }
    /*
    m_Device->CreateVertexBuffer((resolution + 2) * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &m_VB, NULL);

    VOID* pVertices;
    m_VB->Lock(0, (resolution + 2) * sizeof(vertex), (void**)&pVertices, 0);
    memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(vertex));
    m_VB->Unlock();

    m_Device->SetTexture(0, NULL);
    m_Device->SetPixelShader(NULL);
    m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    m_Device->SetStreamSource(0, m_VB, 0, sizeof(vertex));
    m_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    m_Device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, resolution);
    if (m_VB != NULL) m_VB->Release();

}
*/
void CDraw::Rect(Vec2 Pos, Vec2 Size, float LineWidth, D3DCOLOR Color, bool Antialias)
{
    if (LineWidth == 0 || LineWidth == 1)
    {
        FilledRect(Pos, Vec2(Size.x, 1), Color, Antialias);             // Top
        FilledRect(Vec2(Pos.x, Pos.y + Size.y - 1), Vec2(Size.x, 1), Color, Antialias);         // Bottom
        FilledRect(Vec2(Pos.x, Pos.y + 1), Vec2(1, Size.y - 2 * 1), Color, Antialias);       // Left
        FilledRect(Vec2(Pos.x + Size.x - 1, Pos.y + 1), Vec2(1, Size.y - 2 * 1), Color, Antialias);   // Right
    }
    else
    {
        FilledRect(Pos, Vec2(Size.x, LineWidth), Color, Antialias);                                     // Top
        FilledRect(Vec2(Pos.x, Pos.y + Size.y - LineWidth), Vec2(Size.x, LineWidth), Color, Antialias);                         // Bottom
        FilledRect(Vec2(Pos.x, Pos.y + LineWidth), Vec2(LineWidth, Size.x - 2 * LineWidth), Color, Antialias);               // Left
        FilledRect(Vec2(Pos.x + Size.x - LineWidth, Pos.y + LineWidth), Vec2(LineWidth, Size.y - 2 * LineWidth), Color, Antialias);   // Right
    }
}

void CDraw::FilledRect(Vec2 Pos, Vec2 Size, D3DCOLOR Color, bool Antialias)
{
    vertex vertices[4] = {
        { Pos.x, Pos.y + Size.y, 0.0f, 1.0f, Color },
        { Pos.x, Pos.y, 0.0f, 1.0f, Color },
        { Pos.x + Size.x, Pos.y + Size.y, 0.0f, 1.0f, Color },
        { Pos.x + Size.x, Pos.y, 0.0f, 1.0f, Color }
    };
    m_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    m_Device->SetTexture(0, nullptr);

    m_Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, Antialias ? TRUE : FALSE);
    m_Device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, Antialias ? TRUE : FALSE);

    m_Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(vertex));

    m_Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
    m_Device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);
}


void CDraw::BorderedRect(Vec2 Pos, Vec2 Size, float BorderWidth, D3DCOLOR Color, D3DCOLOR BorderColor)
{
    FilledRect(Pos, Size, Color);
    Rect(Vec2(Pos.x - BorderWidth, Pos.y - BorderWidth), Vec2(Size.x + 2 * BorderWidth, Size.y + BorderWidth), BorderWidth, BorderColor);
}

void CDraw::Gradient(Vec2 Pos, Vec2 Size, D3DCOLOR LColor, D3DCOLOR ROtherColor, bool Vertical, bool Antialias)
{
    vertex vertices[4] = {
        { Pos.x, Pos.y, 0.0f, 1.0f, LColor },
        { Pos.x + Size.x, Pos.y, 0.0f, 1.0f, Vertical ? LColor : ROtherColor },
        { Pos.x, Pos.y + Size.y, 0.0f, 1.0f, Vertical ? ROtherColor : LColor },
        { Pos.x + Size.x, Pos.y + Size.y, 0.0f, 1.0f, ROtherColor }
    };

    m_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    m_Device->SetTexture(0, nullptr);

    m_Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, Antialias ? TRUE : FALSE);
    m_Device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, Antialias ? TRUE : FALSE);

    m_Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(vertex));

    m_Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
    m_Device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);
}

void CDraw::Text(const char* text, float x_, float y_, int Orientation, LPD3DXFONT Font, bool Bordered, D3DCOLOR Color, Vec2 TextClipSize)
{
    int x((int)x_), y((int)y_);

    bool NoClipRect = (TextClipSize.x == 0 && TextClipSize.y == 0) ? true : false;
    DWORD TextFlags = 0x0;

    RECT rect;
    switch (Orientation)
    {
    case LEFT: TextFlags = NoClipRect ? DT_LEFT | DT_NOCLIP : DT_LEFT; break;
    case CENTER: TextFlags = NoClipRect ? DT_CENTER | DT_NOCLIP : DT_LEFT;  break;
    case RIGHT: TextFlags = NoClipRect ? DT_RIGHT | DT_NOCLIP : DT_LEFT; break;
    }

    if (Bordered)
    {
        NoClipRect ? SetRect(&rect, x - 1, y, x - 1, y) : SetRect(&rect, x - 1, y, (int)TextClipSize.x - 1, (int)TextClipSize.y);
        Font->DrawTextA(NULL, text, -1, &rect, TextFlags, D3DCOLOR_RGBA(0, 0, 0, get_a(Color)));
        NoClipRect ? SetRect(&rect, x - 1, y, x - 1, y) : SetRect(&rect, x + 1, (int)TextClipSize.y, (int)TextClipSize.x + 1, (int)TextClipSize.y);
        Font->DrawTextA(NULL, text, -1, &rect, TextFlags, D3DCOLOR_RGBA(0, 0, 0, get_a(Color)));
        NoClipRect ? SetRect(&rect, x, y - 1, x, y - 1) : SetRect(&rect, x, y - 1, (int)TextClipSize.x, (int)TextClipSize.y - 1);
        Font->DrawTextA(NULL, text, -1, &rect, TextFlags, D3DCOLOR_RGBA(0, 0, 0, get_a(Color)));
        NoClipRect ? SetRect(&rect, x, y + 1, x, y + 1) : SetRect(&rect, x, y + 1, (int)TextClipSize.x, (int)TextClipSize.y + 1);
        Font->DrawTextA(NULL, text, -1, &rect, TextFlags, D3DCOLOR_RGBA(0, 0, 0, get_a(Color)));
    }

    NoClipRect ? SetRect(&rect, x, y, x, y) : SetRect(&rect, x, y, (int)TextClipSize.x, (int)TextClipSize.y);
    Font->DrawTextA(NULL, text, -1, &rect, TextFlags, Color);
}

void CDraw::Sprite(LPDIRECT3DTEXTURE9 Texture, Vec2 Pos, Vec2 Size, D3DCOLOR Color)
{
    D3DXVECTOR3 pos = D3DXVECTOR3(Pos.x, Pos.y, 0.0f);
    RECT DrawArea = { (LONG)pos.x, (LONG)pos.y, LONG(pos.x + Size.x), LONG(pos.y + Size.y) };

    m_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    m_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    m_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

    m_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    m_Device->SetPixelShader(NULL);
    m_Sprite->Begin(D3DXSPRITE_ALPHABLEND);
    m_Sprite->Draw(Texture, &DrawArea, NULL, &pos, D3DCOLOR_RGBA(255, 255, 255, get_a(Color)));
    m_Sprite->End();
}

void CDraw::Triangle(Vec2 Top, Vec2 Left, Vec2 Right, D3DCOLOR Color, bool antialias)
{
    vertex vertices[3] =
    {
        { Top.x, Top.y, 0.0f, 1.0f, Color, },
        { Right.x, Right.y, 0.0f, 1.0f, Color, },
        { Left.x, Left.y, 0.0f, 1.0f, Color, },
    };

    m_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    m_Device->SetTexture(0, nullptr);

    m_Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, antialias ? TRUE : FALSE);
    m_Device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, antialias ? TRUE : FALSE);

    m_Device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, vertices, sizeof(vertex));

    m_Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, antialias ? TRUE : FALSE);
    m_Device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, antialias ? TRUE : FALSE);

}

IDirect3DTexture9* CDraw::GetBgTexture()
{
    return this->m_BgTexture;
}

Vec2 CDraw::GetTextSize(ID3DXFont* Font, const char* Text)
{
    auto rect = RECT();
    Font->DrawTextA(0, Text, strlen(Text), &rect, DT_CALCRECT, D3DCOLOR_ARGB(0, 0, 0, 0));

    return Vec2(float(rect.right - rect.left), float(rect.bottom - rect.top));
}

LPDIRECT3DDEVICE9 CDraw::GetD3dDevice()
{
    return this->m_Device;
}