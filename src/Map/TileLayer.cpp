#include "TileLayer.h"
#include "../Graphics/TextureManager.h"

TileLayer::TileLayer(
     int tilesize, int colcount, int rowcount,
     bool iscollidable,
     TileMap tilemap, TilesetList tilesets
)
     : m_TileSize(tilesize),
     m_ColCount(colcount),
     m_RowCount(rowcount),
     m_IsCollidable(iscollidable),
     m_Tilemap(tilemap),
     m_Tilesets(tilesets)
{
     for (unsigned int i = 0; i < m_Tilesets.size(); i++)
     {
          Tileset& tileset = m_Tilesets[i];

          //
          // IMAGE COLLECTION TILESET
          //
          if (!tileset.TileImages.empty())
          {
               for (const auto& tileImage : tileset.TileImages)
               {
                    int tileID = tileImage.first;
                    const std::string& source = tileImage.second;

                    // Unique texture id
                    std::string textureID = tileset.Name + "_" + std::to_string(tileID);

                    TextureManager::GetInstance()->Load(textureID, "assets/maps/" + source);
               }
          }
          //
          // STANDARD SPRITESHEET TILESET
          //
          else
          {
               TextureManager::GetInstance()->Load(m_Tilesets[i].Name, "assets/maps/" + m_Tilesets[i].Source);
          }
     }
}

void TileLayer::Render()
{
     for (int i = 0; i < m_RowCount; i++)
     {
          for (int j = 0; j < m_ColCount; j++)
          {
               int globalTileID = m_Tilemap[i][j];

               // Empty tile
               if (globalTileID == 0) continue;


               Tileset* tileset = nullptr;

               for (unsigned int k = 0; k < m_Tilesets.size(); k++)
               {
                    if (globalTileID >= m_Tilesets[k].FirstID && globalTileID <= m_Tilesets[k].LastID)
                    {
                         tileset = &m_Tilesets[k];
                         break;
                    }
               }

               if (tileset == nullptr) continue;


               // Global tile ID -> local tile ID
               int localTileID = globalTileID - tileset->FirstID;

               //
               // IMAGE COLLECTION TILESET
               //
               if (!tileset->TileImages.empty())
               {
                    std::string textureID =
                         tileset->Name + "_" + std::to_string(localTileID);

                    TextureManager::GetInstance()->Draw(
                         textureID,
                         j * m_TileSize,
                         i * m_TileSize,
                         tileset->TileWidths[localTileID],
                         tileset->TileHeights[localTileID]
                    );
               }
               //
               // STANDARD SPRITESHEET TILESET
               //
               else
               {
                    int tileRow = localTileID / tileset->ColCount;
                    int tileCol = localTileID % tileset->ColCount;

                    TextureManager::GetInstance()->DrawTile(
                         tileset->Name,
                         tileset->TileSize,
                         j * tileset->TileSize,
                         i * tileset->TileSize,
                         tileRow,
                         tileCol
                    );
               }
          }
     }
}

void TileLayer::Update()
{

}

bool TileLayer::GetIsCollidable()
{
     return m_IsCollidable;
}