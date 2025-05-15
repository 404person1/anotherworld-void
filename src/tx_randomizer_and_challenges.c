#include "global.h"
#include "event_data.h"
#include "item.h"
#include "tx_randomizer_and_challenges.h"
#include "party_menu.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "constants/items.h"
#include "constants/party_menu.h"
#include "constants/region_map_sections.h"

bool8 IsNuzlockeActive(void)
{
    if (!FlagGet(FLAG_SYS_POKEMON_GET))     //Nuzlocke has not started
        return FALSE;
    if (!FlagGet(FLAG_ADVENTURE_STARTED))   //Nuzlocke has not started
        return FALSE;
    if (FlagGet(FLAG_IS_CHAMPION))          //Player is champion and Nuzlocke stopped
        return FALSE;

    return gSaveBlock2Ptr->tx_Challenges_Nuzlocke;
}

// Nuzlocke code
const u8 NuzlockeLUT[] =
{
    //0
    [MAPSEC_ROUTE_101] = 0x0,
    [MAPSEC_ROUTE_102] = 0x1,
    [MAPSEC_ROUTE_103] = 0x2,
    [MAPSEC_ROUTE_104] = 0x3,
    [MAPSEC_ROUTE_105] = 0x4,
    [MAPSEC_ROUTE_106] = 0x5,
    [MAPSEC_ROUTE_107] = 0x6,
    [MAPSEC_ROUTE_108] = 0x7,
    //1
    [MAPSEC_ROUTE_109] = 0x8,
    [MAPSEC_ROUTE_110] = 0x9,
    [MAPSEC_ROUTE_111] = 0xA,
    [MAPSEC_ROUTE_112] = 0xB,
    [MAPSEC_ROUTE_113] = 0xC,
    [MAPSEC_ROUTE_114] = 0xD,
    [MAPSEC_ROUTE_115] = 0xE,
    [MAPSEC_ROUTE_116] = 0xF,
    //2
    [MAPSEC_ROUTE_117] = 0x10,
    [MAPSEC_ROUTE_118] = 0x11,
    [MAPSEC_ROUTE_119] = 0x12,
    [MAPSEC_ROUTE_120] = 0x13,
    [MAPSEC_ROUTE_121] = 0x14,
    [MAPSEC_ROUTE_122] = 0x15,
    [MAPSEC_ROUTE_123] = 0x16,
    [MAPSEC_ROUTE_124] = 0x17,
    //3
    [MAPSEC_ROUTE_125] = 0x18,
    [MAPSEC_ROUTE_126] = 0x19,
    [MAPSEC_ROUTE_127] = 0x1A,
    [MAPSEC_ROUTE_128] = 0x1B,
    [MAPSEC_ROUTE_129] = 0x1C,
    [MAPSEC_ROUTE_130] = 0x1D,
    [MAPSEC_ROUTE_131] = 0x1E,
    [MAPSEC_ROUTE_132] = 0x1F,
    //4
    [MAPSEC_ROUTE_133] = 0x20,
    [MAPSEC_ROUTE_134] = 0x21,
    [MAPSEC_PETALBURG_CITY] = 0x22,
    [MAPSEC_DEWFORD_TOWN] = 0x23,
    [MAPSEC_SLATEPORT_CITY] = 0x24,
    [MAPSEC_LILYCOVE_CITY] = 0x25,
    [MAPSEC_MOSSDEEP_CITY] = 0x26,
    [MAPSEC_PACIFIDLOG_TOWN] = 0x27,
    //5
    [MAPSEC_SOOTOPOLIS_CITY] = 0x28,
    [MAPSEC_EVER_GRANDE_CITY] = 0x29,
    [MAPSEC_PETALBURG_WOODS] = 0x2A,
    [MAPSEC_RUSTURF_TUNNEL] = 0x2B,
    [MAPSEC_GRANITE_CAVE] = 0x2C,
    [MAPSEC_FIERY_PATH] = 0x2D,
    [MAPSEC_METEOR_FALLS] = 0x2E,
    [MAPSEC_JAGGED_PASS] = 0x2F,
    //6
    [MAPSEC_MIRAGE_TOWER] = 0x30,
    [MAPSEC_ABANDONED_SHIP] = 0x31,
    [MAPSEC_NEW_MAUVILLE] = 0x32,
    [MAPSEC_SAFARI_ZONE_AREA1] = 0x33,
    [MAPSEC_SAFARI_ZONE_AREA2] = 0x34,
    [MAPSEC_SAFARI_ZONE_AREA3] = 0x35,
    [MAPSEC_SAFARI_ZONE_AREA4] = 0x36,
    [MAPSEC_MT_PYRE] = 0x37,
    //7
    [MAPSEC_SHOAL_CAVE] = 0x38,
    [MAPSEC_AQUA_HIDEOUT] = 0x39,
    [MAPSEC_MAGMA_HIDEOUT] = 0x3A,
    [MAPSEC_SEAFLOOR_CAVERN] = 0x3B,
    [MAPSEC_CAVE_OF_ORIGIN] = 0x3C,
    [MAPSEC_SKY_PILLAR] = 0x3D,
    [MAPSEC_VICTORY_ROAD] = 0x3E,
    [MAPSEC_UNDERWATER_124] = 0x3F,
    //8
    [MAPSEC_UNDERWATER_126] = 0x3F,
    [MAPSEC_ARTISAN_CAVE] = 0x40,
    [MAPSEC_DESERT_UNDERPASS] = 0x41,
    [MAPSEC_ALTERING_CAVE_FRLG] = 0x42,
    [MAPSEC_SAFARI_ZONE_AREA5] = 0x43,
    [MAPSEC_SAFARI_ZONE_AREA6] = 0x44
};

//tx_randomizer_and_challenges
u8 NuzlockeFlagSet(u16 mapsec) // @Kurausukun
{
    u8 id = NuzlockeLUT[mapsec];
    u8 * ptr = &gSaveBlock1Ptr->NuzlockeEncounterFlags[id / 8];

    if (ptr)
        * ptr |= 1 << (id & 7);

    #ifndef NDEBUG
    u8 i;
    MgbaPrintf(MGBA_LOG_DEBUG, "NuzlockeFlagSet Id=%d", id);
    for (i=0; i<9; i++)
        MgbaPrintf(MGBA_LOG_DEBUG, "gSaveBlock1Ptr->NuzlockeEncounterFlags[%d] = %d" , i, gSaveBlock1Ptr->NuzlockeEncounterFlags[i]);
    #endif

    return 0;
}
u8 NuzlockeFlagClear(u16 mapsec) // @Kurausukun
{
    u8 id = NuzlockeLUT[mapsec];
    u8 * ptr = &gSaveBlock1Ptr->NuzlockeEncounterFlags[id / 8];
    if (ptr)
        * ptr &= ~(1 << (id & 7));
    return 0;
}
u8 NuzlockeFlagGet(u16 mapsec) // @Kurausukun
{
    u8 id = NuzlockeLUT[mapsec];
    u8 * ptr = &gSaveBlock1Ptr->NuzlockeEncounterFlags[id / 8];

    #ifndef NDEBUG
    MgbaPrintf(MGBA_LOG_DEBUG, "NuzlockeFlagGet Id=%d", id);
    #endif

    if (!ptr)
        return 0;

    if (!(((*ptr) >> (id & 7)) & 1))
        return 0;
    return 1;
}

void NuzlockeDeletePartyMon(u8 position)
{
    if (!gSaveBlock2Ptr->tx_Nuzlocke_Deletion)
    {
        struct Pokemon *pokemon = &gPlayerParty[position];
        u8 val[1] = {TRUE};
        
        SetMonData(pokemon, MON_DATA_NUZLOCKE_RIBBON, val);
        CopyMonToPC(&gPlayerParty[position]);
    }
    PurgeMonOrBoxMon(TOTAL_BOXES_COUNT, position);
}

void NuzlockeDeleteFaintedPartyPokemon(void) // @Kurausukun
{
    u8 i;
    struct Pokemon *pokemon;
    u32 monItem;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        pokemon = &gPlayerParty[i];
        if (GetMonData(pokemon, MON_DATA_SANITY_HAS_SPECIES, NULL) && !GetMonData(pokemon, MON_DATA_IS_EGG, NULL))
        {
            if (GetMonAilment(pokemon) == AILMENT_FNT)
            {
                monItem = GetMonData(pokemon, MON_DATA_HELD_ITEM, NULL);

                if (monItem != ITEM_NONE)
                {
                    AddBagItem(monItem, 1);
                    SetMonData(pokemon, MON_DATA_HELD_ITEM, ITEM_NONE);
                }
                NuzlockeDeletePartyMon(i);
            }
        }
    }
    CompactPartySlots();
}

// DEBUG
void PrintTXSaveData(void)
{
    #ifndef NDEBUG
    
    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Random_WildPokemon"          , gSaveBlock2Ptr->tx_Random_WildPokemon);
    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Random_Trainer"              , gSaveBlock2Ptr->tx_Random_Trainer);
    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Random_Similar"              , gSaveBlock2Ptr->tx_Random_Similar);
    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Random_MapBased"             , gSaveBlock2Ptr->tx_Random_MapBased);
    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Random_IncludeLegendaries"   , gSaveBlock2Ptr->tx_Random_IncludeLegendaries);
    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Random_Type"                 , gSaveBlock2Ptr->tx_Random_Type);
    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Random_Moves"                , gSaveBlock2Ptr->tx_Random_Moves);
    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Random_Abilities"            , gSaveBlock2Ptr->tx_Random_Abilities);
    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Random_Evolutions"           , gSaveBlock2Ptr->tx_Random_Evolutions);
    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Random_EvolutionMethods"     , gSaveBlock2Ptr->tx_Random_EvolutionMethods);
    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Random_TypeEffectiveness"    , gSaveBlock2Ptr->tx_Random_TypeEffectiveness);
    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Random_Items"                , gSaveBlock2Ptr->tx_Random_Items);
    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Random_Chaos"                , gSaveBlock2Ptr->tx_Random_Chaos);
    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Random_OneForOne"            , gSaveBlock2Ptr->tx_Random_OneForOne);

    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Challenges_Nuzlocke"         , gSaveBlock2Ptr->tx_Challenges_Nuzlocke);
    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Challenges_NuzlockeHardcore" , gSaveBlock2Ptr->tx_Challenges_NuzlockeHardcore);
    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Nuzlocke_SpeciesClause"      , gSaveBlock2Ptr->tx_Nuzlocke_SpeciesClause);
    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Nuzlocke_ShinyClause"        , gSaveBlock2Ptr->tx_Nuzlocke_ShinyClause);
    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Nuzlocke_Nicknaming"         , gSaveBlock2Ptr->tx_Nuzlocke_Nicknaming);

    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Challenges_PartyLimit"       , gSaveBlock2Ptr->tx_Challenges_PartyLimit);
    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Challenges_LevelCap"         , gSaveBlock2Ptr->tx_Challenges_LevelCap);
    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Challenges_TrainerDifficulty"    , gSaveBlock2Ptr->tx_Challenges_TrainerDifficulty);
    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Challenges_NoItemPlayer"     , gSaveBlock2Ptr->tx_Challenges_NoItemPlayer);
    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Challenges_NoItemTrainer"    , gSaveBlock2Ptr->tx_Challenges_NoItemTrainer);
    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Challenges_PkmnCenter"       , gSaveBlock2Ptr->tx_Challenges_PkmnCenter);

    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Challenges_EvoLimit"         , gSaveBlock2Ptr->tx_Challenges_EvoLimit);
    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Challenges_OneTypeChallenge" , gSaveBlock2Ptr->tx_Challenges_OneTypeChallenge);
    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Challenges_BaseStatEqualizer", gSaveBlock2Ptr->tx_Challenges_BaseStatEqualizer);
    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Challenges_Mirror"           , gSaveBlock2Ptr->tx_Challenges_Mirror);
    MgbaPrintf(MGBA_LOG_DEBUG, "%d tx_Challenges_Mirror_Thief"     , gSaveBlock2Ptr->tx_Challenges_Mirror_Thief);
    #endif
}
