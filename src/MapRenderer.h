#ifndef CC_MAPRENDERER_H
#define CC_MAPRENDERER_H
#include "TerrainAtlas.h"
#include "ChunkUpdater.h"
/* Renders the blocks of the world by subdividing it into chunks.
   Copyright 2014-2017 ClassicalSharp | Licensed under BSD-3
*/

int MapRenderer_ChunksX, MapRenderer_ChunksY, MapRenderer_ChunksZ;
#define MapRenderer_Pack(cx, cy, cz) (((cz) * MapRenderer_ChunksY + (cy)) * MapRenderer_ChunksX + (cx))
/* TODO: Swap Y and Z? Make sure to update ChunkUpdater's ResetChunkCache and ClearChunkCache methods! */

/* The count of actual used 1D atlases. (i.e. 1DIndex(maxTextureLoc) + 1*/
int MapRenderer_1DUsedCount;
/* The number of non-empty Normal ChunkPartInfos (across entire world) for each 1D atlas batch.
1D atlas batches that do not have any ChunkPartInfos can be entirely skipped. */
Int32 MapRenderer_NormalPartsCount[ATLAS1D_MAX_ATLASES];
/* The number of non-empty Translucent ChunkPartInfos (across entire world) for each 1D atlas batch.
1D atlas batches that do not have any ChunkPartInfos can be entirely skipped. */
Int32 MapRenderer_TranslucentPartsCount[ATLAS1D_MAX_ATLASES];
/* Whether there are any visible Translucent ChunkPartInfos for each 1D atlas batch.
1D atlas batches that do not have any visible translucent ChunkPartInfos can be skipped. */
bool MapRenderer_HasTranslucentParts[ATLAS1D_MAX_ATLASES];
/* Whether there are any visible Normal ChunkPartInfos for each 1D atlas batch.
1D atlas batches that do not have any visible normal ChunkPartInfos can be skipped. */
bool MapRenderer_HasNormalParts[ATLAS1D_MAX_ATLASES];
/* Whether renderer should check if there are any visible Translucent ChunkPartInfos for each 1D atlas batch. */
bool MapRenderer_CheckingTranslucentParts[ATLAS1D_MAX_ATLASES];
/* Whether renderer should check if there are any visible Normal ChunkPartInfos for each 1D atlas batch. */
bool MapRenderer_CheckingNormalParts[ATLAS1D_MAX_ATLASES];

/* Render info for all chunks in the world. Unsorted.*/
struct ChunkInfo* MapRenderer_Chunks;
/* The number of chunks in the world, or ChunksX * ChunksY * ChunksZ */
int MapRenderer_ChunksCount;
/* Pointers to render info for all chunks in the world, sorted by distance from the camera. */
struct ChunkInfo** MapRenderer_SortedChunks;
/* Pointers to render info for all chunks in the world, sorted by distance from the camera.
Chunks that can be rendered (not empty and are visible) are included in this array. */
struct ChunkInfo** MapRenderer_RenderChunks;
/* The number of actually used pointers in the RenderChunks array.
Entries past this count should be ignored and skipped. */
int MapRenderer_RenderChunksCount;
/* Buffer for all chunk parts. There are (MapRenderer_ChunksCount * Atlas1D_Count) * 2 parts in the buffer,
 with parts for 'normal' buffer being in lower half. */
struct ChunkPartInfo* MapRenderer_PartsBuffer_Raw;
struct ChunkPartInfo* MapRenderer_PartsNormal;
struct ChunkPartInfo* MapRenderer_PartsTranslucent;

struct ChunkInfo* MapRenderer_GetChunk(int cx, int cy, int cz);
void MapRenderer_RefreshChunk(int cx, int cy, int cz);
void MapRenderer_RenderNormal(double delta);
void MapRenderer_RenderTranslucent(double delta);
#endif
