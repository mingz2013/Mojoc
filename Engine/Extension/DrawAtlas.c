//
// Created by scott.cgi on 2017/1/5.
//

#include "Engine/Extension/DrawAtlas.h"
#include "Engine/Graphics/OpenGL/SubMesh.h"
#include "Engine/Toolkit/Head/Struct.h"
#include "Engine/Toolkit/Head/Def.h"
#include "Engine/Toolkit/Platform/Log.h"


static ArrayList(DrawAtlas*) drawAtlasList[1] = AArrayListInit(DrawAtlas*, 10);


static DrawAtlas* Get(const char* filePath)
{
    TextureAtlas* textureAtlas = ATextureAtlas->Get(filePath);

    ALogA
    (
        textureAtlas->textureList->size == 1,
        "DrawAtlas not support TextureAtlas has multiple texture"
    );

    DrawAtlas* drawAtlas = AArrayListPop(drawAtlasList, DrawAtlas*);

    if (drawAtlas == NULL)
    {
        drawAtlas = (DrawAtlas*) malloc(sizeof(DrawAtlas));

        AMesh->InitWithCapacity
        (
            AArrayListGet(textureAtlas->textureList, 0, Texture*),
            20,
            drawAtlas->mesh
        );

        AArrayList->Init
        (
            sizeof(Drawable*),
            drawAtlas->quadList
        );
    }
    else
    {
        AMesh     ->Clear(drawAtlas->mesh);
        AArrayList->Clear(drawAtlas->quadList);
    }

    drawAtlas->textureAtlas = textureAtlas;

    return drawAtlas;
}


static Drawable* GetQuad(DrawAtlas* drawAtlas, const char* quadName)
{
    TextureAtlasQuad* atlasQuad = ATextureAtlasGetQuad(drawAtlas->textureAtlas, quadName);
    ALogA(atlasQuad != NULL, "DrawAtlas GetQuad not found quadName = %s", quadName);

    Drawable* drawable = AArrayListPop(drawAtlas->quadList, Drawable*);

    if (drawable == NULL)
    {
        drawable = AMesh->AddChildWithQuad(drawAtlas->mesh, atlasQuad->quad)->drawable;
        AMesh->GenerateBuffer(drawAtlas->mesh);
    }
    else
    {
        ADrawable->Init(drawable);

        ASubMesh->SetWithQuad
        (
            AStructGetParent2(drawable, SubMesh),
            drawAtlas->mesh->texture,
            atlasQuad->quad
        );
    }

    return drawable;
}

static void Reuse(DrawAtlas* drawAtlas)
{
    ALogA(drawAtlas->textureAtlas != NULL, "Reuse drawAtlas %p already reused", drawAtlas);

    for (int i = 0; i < drawAtlas->quadList->size; i++)
    {
        ADrawableSetInVisible
        (
            AArrayListGet(drawAtlas->quadList, i, Drawable*)
        );
    }

    drawAtlas->textureAtlas = NULL;

    AArrayListAdd(drawAtlasList, drawAtlas);
}

static void ReuseQuad(DrawAtlas* drawAtlas, Drawable* drawable)
{
    SubMesh* subMesh = AStructGetParent2(drawable, SubMesh);

    ALogA
    (
        drawAtlas->mesh == subMesh->parent,
        "ReuseQuad drawable %p not in this drawAtlas",
        drawable
    );

    ADrawableSetInVisible(drawable);
    AArrayListAdd(drawAtlas->quadList, drawable);
}

struct ADrawAtlas ADrawAtlas[1] =
{
    Get,
    GetQuad,
    Reuse,
    ReuseQuad,
};