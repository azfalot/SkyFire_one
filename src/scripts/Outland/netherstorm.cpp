/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/* ScriptData
SDName: Netherstorm
SD%Complete: 75
SDComment: Quest support: 10337, 10438, 10652 (special flight paths), 10299,10321,10322,10323,10329,10330,10338,10365(Shutting Down Manaforge), 10198
SDCategory: Netherstorm
EndScriptData */

/* ContentData
npc_manaforge_control_console
go_manaforge_control_console
npc_commander_dawnforge
npc_bessy
EndContentData */

#include "ScriptPCH.h"
#include "ScriptedEscortAI.h"

/*######
## npc_manaforge_control_console
######*/

#define EMOTE_START     -1000296
#define EMOTE_60        -1000297
#define EMOTE_30        -1000298
#define EMOTE_10        -1000299
#define EMOTE_COMPLETE  -1000300
#define EMOTE_ABORT     -1000301

#define ENTRY_BNAAR_C_CONSOLE   20209
#define ENTRY_CORUU_C_CONSOLE   20417
#define ENTRY_DURO_C_CONSOLE    20418
#define ENTRY_ARA_C_CONSOLE     20440

#define ENTRY_SUNFURY_TECH      20218
#define ENTRY_SUNFURY_PROT      20436

#define ENTRY_ARA_TECH          20438
#define ENTRY_ARA_ENGI          20439
#define ENTRY_ARA_GORKLONN      20460

#define SPELL_DISABLE_VISUAL    35031
#define SPELL_INTERRUPT_1       35016                       //ACID mobs should cast this
#define SPELL_INTERRUPT_2       35176                       //ACID mobs should cast this (Manaforge Ara-version)

struct npc_manaforge_control_consoleAI : public ScriptedAI
{
    npc_manaforge_control_consoleAI(Creature *c) : ScriptedAI(c) {}

    uint32 Event_Timer;
    uint32 Wave_Timer;
    uint32 Phase;
    bool Wave;
    uint64 someplayer;
    uint64 goConsole;
    Creature* add;

    void Reset()
    {
        Event_Timer = 3000;
        Wave_Timer = 0;
        Phase = 1;
        Wave = false;
        someplayer = 0;
        goConsole = 0;
        Creature* add = NULL;
    }

    void EnterCombat(Unit *who) { return; }

    /*void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        //we have no way of telling the creature was hit by spell -> got aura applied after 10-12 seconds
        //then no way for the mobs to actually stop the shutdown as intended.
        if (spell->Id == SPELL_INTERRUPT_1)
            DoSay("Silence! I kill you!",LANG_UNIVERSAL, NULL);
    }*/

    void JustDied(Unit* killer)
    {
        DoScriptText(EMOTE_ABORT, me);

        if (someplayer)
        {
            Player* p = Unit::GetPlayer(*me, someplayer);
            if (p)
            {
                switch(me->GetEntry())
                {
                    case ENTRY_BNAAR_C_CONSOLE:
                        p->FailQuest(10299);
                        p->FailQuest(10329);
                        break;
                    case ENTRY_CORUU_C_CONSOLE:
                        p->FailQuest(10321);
                        p->FailQuest(10330);
                        break;
                    case ENTRY_DURO_C_CONSOLE:
                        p->FailQuest(10322);
                        p->FailQuest(10338);
                        break;
                    case ENTRY_ARA_C_CONSOLE:
                        p->FailQuest(10323);
                        p->FailQuest(10365);
                        break;
                }
            }
        }

        if (goConsole)
        {
            if (GameObject* go = GameObject::GetGameObject((*me),goConsole))
                go->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
        }
    }

    void DoWaveSpawnForCreature(Creature *creature)
    {
        switch(creature->GetEntry())
        {
            case ENTRY_BNAAR_C_CONSOLE:
                if (rand()%2)
                {
                    add = me->SummonCreature(ENTRY_SUNFURY_TECH,2933.68,4162.55,164.00,1.60,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0,2927.36,4212.97,164.00);
                }
                else
                {
                    add = me->SummonCreature(ENTRY_SUNFURY_TECH,2927.36,4212.97,164.00,4.94,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0,2933.68,4162.55,164.00);
                }
                Wave_Timer = 30000;
                break;
            case ENTRY_CORUU_C_CONSOLE:
                add = me->SummonCreature(ENTRY_SUNFURY_TECH,2445.21,2765.26,134.49,3.93,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                if (add) add->GetMotionMaster()->MovePoint(0,2424.21,2740.15,133.81);
                add = me->SummonCreature(ENTRY_SUNFURY_TECH,2429.86,2731.85,134.53,1.31,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                if (add) add->GetMotionMaster()->MovePoint(0,2435.37,2766.04,133.81);
                Wave_Timer = 20000;
                break;
            case ENTRY_DURO_C_CONSOLE:
                add = me->SummonCreature(ENTRY_SUNFURY_TECH,2986.80,2205.36,165.37,3.74,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                if (add) add->GetMotionMaster()->MovePoint(0,2985.15,2197.32,164.79);
                add = me->SummonCreature(ENTRY_SUNFURY_TECH,2952.91,2191.20,165.32,0.22,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                if (add) add->GetMotionMaster()->MovePoint(0,2060.01,2185.27,164.67);
                Wave_Timer = 15000;
                break;
            case ENTRY_ARA_C_CONSOLE:
                if (rand()%2)
                {
                    add = me->SummonCreature(ENTRY_ARA_TECH,4035.11,4038.97,194.27,2.57,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0,4003.42,4040.19,193.49);
                    add = me->SummonCreature(ENTRY_ARA_TECH,4033.66,4036.79,194.28,2.57,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0,4003.42,4040.19,193.49);
                    add = me->SummonCreature(ENTRY_ARA_TECH,4037.13,4037.30,194.23,2.57,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0,4003.42,4040.19,193.49);
                }
                else
                {
                    add = me->SummonCreature(ENTRY_ARA_TECH,3099.59,4049.30,194.22,0.05,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0,4028.01,4035.17,193.59);
                    add = me->SummonCreature(ENTRY_ARA_TECH,3999.72,4046.75,194.22,0.05,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0,4028.01,4035.17,193.59);
                    add = me->SummonCreature(ENTRY_ARA_TECH,3996.81,4048.26,194.22,0.05,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0,4028.01,4035.17,193.59);
                }
                Wave_Timer = 15000;
                break;
        }
    }
    void DoFinalSpawnForCreature(Creature *creature)
    {
        switch(creature->GetEntry())
        {
            case ENTRY_BNAAR_C_CONSOLE:
                add = me->SummonCreature(ENTRY_SUNFURY_TECH,2946.52,4201.42,163.47,3.54,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                if (add) add->GetMotionMaster()->MovePoint(0,2927.49,4192.81,163.00);
                break;
            case ENTRY_CORUU_C_CONSOLE:
                add = me->SummonCreature(ENTRY_SUNFURY_TECH,2453.88,2737.85,133.27,2.59,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                if (add) add->GetMotionMaster()->MovePoint(0,2433.96,2751.53,133.85);
                add = me->SummonCreature(ENTRY_SUNFURY_TECH,2441.62,2735.32,134.49,1.97,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                if (add) add->GetMotionMaster()->MovePoint(0,2433.96,2751.53,133.85);
                add = me->SummonCreature(ENTRY_SUNFURY_TECH,2450.73,2754.50,134.49,3.29,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                if (add) add->GetMotionMaster()->MovePoint(0,2433.96,2751.53,133.85);
                break;
            case ENTRY_DURO_C_CONSOLE:
                add = me->SummonCreature(ENTRY_SUNFURY_TECH,2956.18,2202.85,165.32,5.45,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                if (add) add->GetMotionMaster()->MovePoint(0,2972.27,2193.22,164.48);
                add = me->SummonCreature(ENTRY_SUNFURY_TECH,2975.30,2211.50,165.32,4.55,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                if (add) add->GetMotionMaster()->MovePoint(0,2972.27,2193.22,164.48);
                add = me->SummonCreature(ENTRY_SUNFURY_PROT,2965.02,2217.45,164.16,4.96,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                if (add) add->GetMotionMaster()->MovePoint(0,2972.27,2193.22,164.48);
                break;
            case ENTRY_ARA_C_CONSOLE:
                add = me->SummonCreature(ENTRY_ARA_ENGI,3994.51,4020.46,192.18,0.91,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                if (add) add->GetMotionMaster()->MovePoint(0,4008.35,4035.04,192.70);
                add = me->SummonCreature(ENTRY_ARA_GORKLONN,4021.56,4059.35,193.59,4.44,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                if (add) add->GetMotionMaster()->MovePoint(0,4016.62,4039.89,193.46);
                break;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (Event_Timer <= diff)
        {
            switch(Phase)
            {
                case 1:
                    if (someplayer)
                    {
                        Unit* u = Unit::GetUnit((*me), someplayer);
                        if (u && u->GetTypeId() == TYPEID_PLAYER) DoScriptText(EMOTE_START, me, u);
                    }
                    Event_Timer = 60000;
                    Wave = true;
                    ++Phase;
                    break;
                case 2:
                    DoScriptText(EMOTE_60, me);
                    Event_Timer = 30000;
                    ++Phase;
                    break;
                case 3:
                    DoScriptText(EMOTE_30, me);
                    Event_Timer = 20000;
                    DoFinalSpawnForCreature(me);
                    ++Phase;
                    break;
                case 4:
                    DoScriptText(EMOTE_10, me);
                    Event_Timer = 10000;
                    Wave = false;
                    ++Phase;
                    break;
                case 5:
                    DoScriptText(EMOTE_COMPLETE, me);
                    if (someplayer)
                    {
                        Player* player = Unit::GetPlayer(*me, someplayer);
                        if (player)
                           player->KilledMonster(me->GetEntry(),me->GetGUID());
                        DoCast(me,SPELL_DISABLE_VISUAL);
                    }
                    if (goConsole)
                    {
                        if (GameObject* go = GameObject::GetGameObject((*me),goConsole))
                            go->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
                    }
                    ++Phase;
                    break;
            }
        } else Event_Timer -= diff;

        if (Wave)
        {
            if (Wave_Timer <= diff)
            {
                DoWaveSpawnForCreature(me);
            } else Wave_Timer -= diff;
        }
    }
};
CreatureAI* GetAI_npc_manaforge_control_console(Creature* pCreature)
{
    return new npc_manaforge_control_consoleAI (pCreature);
}

/*######
## go_manaforge_control_console
######*/

//TODO: clean up this workaround when Oregon adds support to do it properly (with gossip selections instead of instant summon)
bool GOHello_go_manaforge_control_console(Player *player, GameObject* _GO)
{
    if (_GO->GetGoType() == GAMEOBJECT_TYPE_QUESTGIVER)
    {
        player->PrepareQuestMenu(_GO->GetGUID());
        player->SendPreparedQuest(_GO->GetGUID());
    }

    Creature* manaforge;
    manaforge = NULL;

    switch(_GO->GetAreaId())
    {
        case 3726:                                          //b'naar
            if ((player->GetQuestStatus(10299) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(10329) == QUEST_STATUS_INCOMPLETE) &&
                player->HasItemCount(29366,1))
                manaforge = player->SummonCreature(ENTRY_BNAAR_C_CONSOLE,2918.95,4189.98,161.88,0.34,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,125000);
            break;
        case 3730:                                          //coruu
            if ((player->GetQuestStatus(10321) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(10330) == QUEST_STATUS_INCOMPLETE) &&
                player->HasItemCount(29396,1))
                manaforge = player->SummonCreature(ENTRY_CORUU_C_CONSOLE,2426.77,2750.38,133.24,2.14,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,125000);
            break;
        case 3734:                                          //duro
            if ((player->GetQuestStatus(10322) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(10338) == QUEST_STATUS_INCOMPLETE) &&
                player->HasItemCount(29397,1))
                manaforge = player->SummonCreature(ENTRY_DURO_C_CONSOLE,2976.48,2183.29,163.20,1.85,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,125000);
            break;
        case 3722:                                          //ara
            if ((player->GetQuestStatus(10323) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(10365) == QUEST_STATUS_INCOMPLETE) &&
                player->HasItemCount(29411,1))
                manaforge = player->SummonCreature(ENTRY_ARA_C_CONSOLE,4013.71,4028.76,192.10,1.25,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,125000);
            break;
    }

    if (manaforge)
    {
        ((npc_manaforge_control_consoleAI*)manaforge->AI())->someplayer = player->GetGUID();
        ((npc_manaforge_control_consoleAI*)manaforge->AI())->goConsole = _GO->GetGUID();
        _GO->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
    }
    return true;
}

/*######
## npc_commander_dawnforge
######*/

// The Speech of Dawnforge, Ardonis & Pathaleon
#define SAY_COMMANDER_DAWNFORGE_1           -1000128
#define SAY_ARCANIST_ARDONIS_1              -1000129
#define SAY_COMMANDER_DAWNFORGE_2           -1000130
#define SAY_PATHALEON_CULATOR_IMAGE_1       -1000131
#define SAY_COMMANDER_DAWNFORGE_3           -1000132
#define SAY_PATHALEON_CULATOR_IMAGE_2       -1000133
#define SAY_PATHALEON_CULATOR_IMAGE_2_1     -1000134
#define SAY_PATHALEON_CULATOR_IMAGE_2_2     -1000135
#define SAY_COMMANDER_DAWNFORGE_4           -1000136
#define SAY_ARCANIST_ARDONIS_2              -1000136
#define SAY_COMMANDER_DAWNFORGE_5           -1000137

#define QUEST_INFO_GATHERING                10198
#define SPELL_SUNFURY_DISGUISE              34603

// Entries of Arcanist Ardonis, Commander Dawnforge, Pathaleon the Curators Image
int CreatureEntry[3][1] =
{
    {19830},                                                // Ardonis
    {19831},                                                // Dawnforge
    {21504}                                                 // Pathaleon
};

struct npc_commander_dawnforgeAI : public ScriptedAI
{
    npc_commander_dawnforgeAI(Creature *c) : ScriptedAI(c) { Reset (); }


    uint64 playerGUID;
    uint64 ardonisGUID;
    uint64 pathaleonGUID;


    uint32 Phase;
    uint32 PhaseSubphase;
    uint32 Phase_Timer;
    bool isEvent;

    float angle_dawnforge;
    float angle_ardonis;

    void Reset()
    {
        playerGUID = 0;
        ardonisGUID = 0;
        pathaleonGUID = 0;

        Phase = 1;
        PhaseSubphase = 0;
        Phase_Timer = 4000;
        isEvent = false;
    }

    void EnterCombat(Unit *who) { }

    //Select any creature in a grid
    Creature* SelectCreatureInGrid(uint32 entry, float range)
    {
        Creature* pCreature = NULL;

        CellPair pair(Oregon::ComputeCellPair(me->GetPositionX(), me->GetPositionY()));
        Cell cell(pair);
        cell.data.Part.reserved = ALL_DISTRICT;
        cell.SetNoCreate();

        Oregon::NearestCreatureEntryWithLiveStateInObjectRangeCheck creature_check(*me, entry, true, range);
        Oregon::CreatureLastSearcher<Oregon::NearestCreatureEntryWithLiveStateInObjectRangeCheck> searcher(pCreature, creature_check);
        TypeContainerVisitor<Oregon::CreatureLastSearcher<Oregon::NearestCreatureEntryWithLiveStateInObjectRangeCheck>, GridTypeMapContainer> creature_searcher(searcher);
        cell.Visit(pair, creature_searcher,*(me->GetMap()));

        return pCreature;
    }

    void JustSummoned(Creature *summoned)
    {
        pathaleonGUID = summoned->GetGUID();
    }

    // Emote Ardonis and Pathaleon
    void Turn_to_Pathaleons_Image()
    {
        Creature *ardonis = Unit::GetCreature(*me,ardonisGUID);
        Creature *pathaleon = Unit::GetCreature(*me,pathaleonGUID);
        Player *player = Unit::GetPlayer(*me, playerGUID);

        if (!ardonis || !pathaleon || !player)
            return;

        //Calculate the angle to Pathaleon
        angle_dawnforge = me->GetAngle(pathaleon->GetPositionX(), pathaleon->GetPositionY());
        angle_ardonis = ardonis->GetAngle(pathaleon->GetPositionX(), pathaleon->GetPositionY());

        //Turn Dawnforge and update
        me->SetOrientation(angle_dawnforge);
        me->SendUpdateToPlayer(player);
        //Turn Ardonis and update
        ardonis->SetOrientation(angle_ardonis);
        ardonis->SendUpdateToPlayer(player);

        //Set them to kneel
        me->SetStandState(UNIT_STAND_STATE_KNEEL);
        ardonis->SetStandState(UNIT_STAND_STATE_KNEEL);
    }

    //Set them back to each other
    void Turn_to_eachother()
    {
        if (Unit *ardonis = Unit::GetUnit(*me,ardonisGUID))
        {
            Player *player = Unit::GetPlayer(*me, playerGUID);

            if (!player)
                return;

            angle_dawnforge = me->GetAngle(ardonis->GetPositionX(), ardonis->GetPositionY());
            angle_ardonis = ardonis->GetAngle(me->GetPositionX(), me->GetPositionY());

            //Turn Dawnforge and update
            me->SetOrientation(angle_dawnforge);
            me->SendUpdateToPlayer(player);
            //Turn Ardonis and update
            ardonis->SetOrientation(angle_ardonis);
            ardonis->SendUpdateToPlayer(player);

            //Set state
            me->SetStandState(UNIT_STAND_STATE_STAND);
            ardonis->SetStandState(UNIT_STAND_STATE_STAND);
        }
    }

    bool CanStartEvent(Player *player)
    {
        if (!isEvent)
        {
            Creature *ardonis = SelectCreatureInGrid(CreatureEntry[0][0], 10.0f);
            if (!ardonis)
                return false;

            ardonisGUID = ardonis->GetGUID();
            playerGUID = player->GetGUID();

            isEvent = true;

            Turn_to_eachother();
            return true;
        }

        debug_log("TSCR: npc_commander_dawnforge event already in progress, need to wait.");
        return false;
    }

    void UpdateAI(const uint32 diff)
    {
        //Is event even running?
        if (!isEvent)
            return;

        //Phase timing
        if (Phase_Timer >= diff)
        {
            Phase_Timer -= diff;
            return;
        }

        Unit *ardonis = Unit::GetUnit(*me,ardonisGUID);
        Unit *pathaleon = Unit::GetUnit(*me,pathaleonGUID);
        Player *player = Unit::GetPlayer(*me, playerGUID);

        if (!ardonis || !player)
        {
            Reset();
            return;
        }

        if (Phase > 4 && !pathaleon)
        {
            Reset();
            return;
        }

        //Phase 1 Dawnforge say
        switch (Phase)
        {
        case 1:
            DoScriptText(SAY_COMMANDER_DAWNFORGE_1, me);
            ++Phase;
            Phase_Timer = 16000;
            break;
            //Phase 2 Ardonis say
        case 2:
            DoScriptText(SAY_ARCANIST_ARDONIS_1, ardonis);
            ++Phase;
            Phase_Timer = 16000;
            break;
            //Phase 3 Dawnforge say
        case 3:
            DoScriptText(SAY_COMMANDER_DAWNFORGE_2, me);
            ++Phase;
            Phase_Timer = 16000;
            break;
            //Phase 4 Pathaleon spawns up to phase 9
        case 4:
            //spawn pathaleon's image
            me->SummonCreature(CreatureEntry[2][0], 2325.851563, 2799.534668, 133.084229, 6.038996, TEMPSUMMON_TIMED_DESPAWN, 90000);
            ++Phase;
            Phase_Timer = 500;
            break;
            //Phase 5 Pathaleon say
        case 5:
            DoScriptText(SAY_PATHALEON_CULATOR_IMAGE_1, pathaleon);
            ++Phase;
            Phase_Timer = 6000;
            break;
            //Phase 6
        case 6:
            switch(PhaseSubphase)
            {
                //Subphase 1: Turn Dawnforge and Ardonis
            case 0:
                Turn_to_Pathaleons_Image();
                ++PhaseSubphase;
                Phase_Timer = 8000;
                break;
                //Subphase 2 Dawnforge say
            case 1:
                DoScriptText(SAY_COMMANDER_DAWNFORGE_3, me);
                PhaseSubphase = 0;
                ++Phase;
                Phase_Timer = 8000;
                break;
            }
            break;
            //Phase 7 Pathaleons say 3 Sentence, every sentence need a subphase
        case 7:
            switch(PhaseSubphase)
            {
                //Subphase 1
            case 0:
                DoScriptText(SAY_PATHALEON_CULATOR_IMAGE_2, pathaleon);
                ++PhaseSubphase;
                Phase_Timer = 12000;
                break;
                //Subphase 2
            case 1:
                DoScriptText(SAY_PATHALEON_CULATOR_IMAGE_2_1, pathaleon);
                ++PhaseSubphase;
                Phase_Timer = 16000;
                break;
                //Subphase 3
            case 2:
                DoScriptText(SAY_PATHALEON_CULATOR_IMAGE_2_2, pathaleon);
                PhaseSubphase = 0;
                ++Phase;
                Phase_Timer = 10000;
                break;
            }
            break;
            //Phase 8 Dawnforge & Ardonis say
        case 8:
            DoScriptText(SAY_COMMANDER_DAWNFORGE_4, me);
            DoScriptText(SAY_ARCANIST_ARDONIS_2, ardonis);
            ++Phase;
            Phase_Timer = 4000;
            break;
            //Phase 9 Pathaleons Despawn, Reset Dawnforge & Ardonis angle
        case 9:
            Turn_to_eachother();
            //hide pathaleon, unit will despawn shortly
            pathaleon->SetVisibility(VISIBILITY_OFF);
            PhaseSubphase = 0;
            ++Phase;
            Phase_Timer = 3000;
            break;
            //Phase 10 Dawnforge say
        case 10:
            DoScriptText(SAY_COMMANDER_DAWNFORGE_5, me);
            player->AreaExploredOrEventHappens(QUEST_INFO_GATHERING);
            Reset();
            break;
        }
     }
};

CreatureAI* GetAI_npc_commander_dawnforge(Creature* pCreature)
{
    return new npc_commander_dawnforgeAI(pCreature);
}

Creature* SearchDawnforge(Player *source, uint32 entry, float range)
{
    Creature* pCreature = NULL;

    CellPair pair(Oregon::ComputeCellPair(source->GetPositionX(), source->GetPositionY()));
    Cell cell(pair);
    cell.data.Part.reserved = ALL_DISTRICT;
    cell.SetNoCreate();

    Oregon::NearestCreatureEntryWithLiveStateInObjectRangeCheck creature_check(*source, entry, true, range);
    Oregon::CreatureLastSearcher<Oregon::NearestCreatureEntryWithLiveStateInObjectRangeCheck> searcher(pCreature, creature_check);
    TypeContainerVisitor<Oregon::CreatureLastSearcher<Oregon::NearestCreatureEntryWithLiveStateInObjectRangeCheck>, GridTypeMapContainer> creature_searcher(searcher);
    cell.Visit(pair, creature_searcher,*(source->GetMap()));

    return pCreature;
}

bool AreaTrigger_at_commander_dawnforge(Player *player, const AreaTriggerEntry *at)
{
    //if player lost aura or not have at all, we should not try start event.
    if (!player->HasAura(SPELL_SUNFURY_DISGUISE,0))
        return false;

    if (player->isAlive() && player->GetQuestStatus(QUEST_INFO_GATHERING) == QUEST_STATUS_INCOMPLETE)
    {
        Creature* Dawnforge = SearchDawnforge(player, CreatureEntry[1][0], 30.0f);

        if (!Dawnforge)
            return false;

        if (((npc_commander_dawnforgeAI*)Dawnforge->AI())->CanStartEvent(player))
            return true;
    }
    return false;
}

/*######
## npc_professor_dabiri
######*/

#define SPELL_PHASE_DISTRUPTOR  35780
#define GOSSIP_ITEM "I need a new phase distruptor, Professor"
#define WHISPER_DABIRI -1000302

#define QUEST_DIMENSIUS 10439
#define QUEST_ON_NETHERY_WINGS 10438

bool GossipHello_npc_professor_dabiri(Player *player, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        player->PrepareQuestMenu(pCreature->GetGUID());

    if (player->GetQuestStatus(QUEST_ON_NETHERY_WINGS) == QUEST_STATUS_INCOMPLETE && !player->HasItemCount(29778, 1))
        player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    player->SEND_GOSSIP_MENU(player->GetGossipTextId(pCreature), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_professor_dabiri(Player *player, Creature* pCreature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF+1)
    {
        pCreature->CastSpell(player, SPELL_PHASE_DISTRUPTOR, false);
        player->CLOSE_GOSSIP_MENU();
    }

    return true;
}

bool QuestAccept_npc_professor_dabiri(Player *player, Creature *creature, Quest const *quest)
{
    if (quest->GetQuestId() == QUEST_DIMENSIUS)
        DoScriptText(WHISPER_DABIRI, creature, player);

    return true;
}

/*######
## mob_phase_hunter
######*/

#define SUMMONED_MOB            19595
#define EMOTE_WEAK              -1000303

// Spells
#define SPELL_PHASE_SLIP        36574
#define SPELL_MANA_BURN         13321
#define SPELL_MATERIALIZE       34804
#define SPELL_DE_MATERIALIZE    34804

struct mob_phase_hunterAI : public ScriptedAI
{

    mob_phase_hunterAI(Creature *c) : ScriptedAI(c) {}

    bool Weak;
    bool Materialize;
    bool Drained;

    int WeakPercent;
    uint64 PlayerGUID;
    uint32 Health;
    uint32 Level;
    uint32 PhaseSlipVulnerabilityTimer;
    uint32 ManaBurnTimer;

    void Reset()
    {
        Weak = false;
        Materialize = false;
        Drained = false;

        WeakPercent = 25 + (rand()%16); // 25-40
        PlayerGUID = 0;
        ManaBurnTimer = 5000 + (rand()%3 * 1000); // 5-8 sec cd
    }

    void EnterCombat(Unit *who)
    {
        if (Player *player = who->GetCharmerOrOwnerPlayerOrPlayerItself())
            PlayerGUID = player->GetGUID();
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        DoCast(me, SPELL_DE_MATERIALIZE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!Materialize)
        {
            DoCast(me, SPELL_MATERIALIZE);
            Materialize = true;
        }

        if (me->HasAuraType(SPELL_AURA_MOD_DECREASE_SPEED) || me->hasUnitState(UNIT_STAT_ROOT)) // if the mob is rooted/slowed by spells eg.: Entangling Roots, Frost Nova, Hamstring, Crippling Poison, etc. => remove it
            DoCast(me, SPELL_PHASE_SLIP);

        if (!UpdateVictim())
            return;

        if (ManaBurnTimer <= diff) // cast Mana Burn
        {
            if (me->getVictim()->GetCreateMana() > 0)
            {
                DoCast(me->getVictim(), SPELL_MANA_BURN);
                ManaBurnTimer = 8000 + (rand()%10 * 1000); // 8-18 sec cd
            }
        } else ManaBurnTimer -= diff;

        if (PlayerGUID) // start: support for quest 10190
        {
            Player* pTarget = Unit::GetPlayer(*me, PlayerGUID);

            if (pTarget && !Weak && me->GetHealth() < (me->GetMaxHealth() / 100 * WeakPercent)
                && pTarget->GetQuestStatus(10190) == QUEST_STATUS_INCOMPLETE)
            {
                DoScriptText(EMOTE_WEAK, me);
                Weak = true;
            }
            if (Weak && !Drained && me->HasAura(34219, 0))
            {
                Drained = true;

                Health = me->GetHealth(); // get the normal mob's data
                Level = me->getLevel();

                me->AttackStop(); // delete the normal mob
                me->DealDamage(me, me->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                me->RemoveCorpse();

                Creature* DrainedPhaseHunter = NULL;

                if (!DrainedPhaseHunter)
                    DrainedPhaseHunter = me->SummonCreature(SUMMONED_MOB, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 60000); // summon the mob

                if (DrainedPhaseHunter)
                {
                    DrainedPhaseHunter->SetLevel(Level); // set the summoned mob's data
                    DrainedPhaseHunter->SetHealth(Health);
                    DrainedPhaseHunter->AddThreat(pTarget, 10000.0f);
                    DrainedPhaseHunter->AI()->AttackStart(pTarget);
                }
            }
        }// end: support for quest 10190

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_phase_hunter(Creature* pCreature)
{
    return new mob_phase_hunterAI (pCreature);
}

/*######
## npc_bessy
######*/

#define Q_ALMABTRIEB    10337
#define N_THADELL       20464
#define SPAWN_FIRST     20512
#define SPAWN_SECOND    19881
#define SAY_THADELL_1   -1000304
#define SAY_THADELL_2   -1000305

struct npc_bessyAI : public npc_escortAI
{

    npc_bessyAI(Creature *c) : npc_escortAI(c) {}

    void JustDied(Unit* killer)
    {
        if (Player* pPlayer = GetPlayerForEscort())
            pPlayer->FailQuest(Q_ALMABTRIEB);
    }

    void WaypointReached(uint32 i)
    {
        Player* pPlayer = GetPlayerForEscort();

        if (!pPlayer)
            return;

        switch(i)
        {
            case 3: //first spawn
                me->SummonCreature(SPAWN_FIRST, 2449.67, 2183.11, 96.85, 6.20, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                me->SummonCreature(SPAWN_FIRST, 2449.53, 2184.43, 96.36, 6.27, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                me->SummonCreature(SPAWN_FIRST, 2449.85, 2186.34, 97.57, 6.08, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                break;

            case 7:
                me->SummonCreature(SPAWN_SECOND, 2309.64, 2186.24, 92.25, 6.06, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                me->SummonCreature(SPAWN_SECOND, 2309.25, 2183.46, 91.75, 6.22, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                break;

            case 12:
                if (pPlayer)
                    pPlayer->GroupEventHappens(Q_ALMABTRIEB, me);
                if (Unit* Thadell = FindCreature(N_THADELL, 30, me))
                    DoScriptText(SAY_THADELL_1, me); break;
            case 13:
                if (Unit* Thadell = FindCreature(N_THADELL, 30, me))
                    DoScriptText(SAY_THADELL_2, me, pPlayer); break;
        }
    }

    void JustSummoned(Creature* summoned)
    {
        summoned->AI()->AttackStart(me);
    }

    void EnterCombat(Unit* who){}

    void Reset()
    {
        me->RestoreFaction();
    }

};

bool QuestAccept_npc_bessy(Player* pPlayer, Creature* pCreature, Quest const* quest)
{
    if (quest->GetQuestId() == Q_ALMABTRIEB)
    {
        pCreature->setFaction(113);
        pCreature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        CAST_AI(npc_escortAI, (pCreature->AI()))->Start(true, false, pPlayer->GetGUID());
    }
    return true;
}

CreatureAI* GetAI_npc_bessy(Creature* pCreature)
{
    npc_bessyAI* bessyAI = new npc_bessyAI(pCreature);

    bessyAI->AddWaypoint(0, 2488.77, 2184.89, 104.64);
    bessyAI->AddWaypoint(1, 2478.72, 2184.77, 98.58);
    bessyAI->AddWaypoint(2, 2473.52, 2184.71, 99.00);
    bessyAI->AddWaypoint(3, 2453.15, 2184.96, 97.09,4000);
    bessyAI->AddWaypoint(4, 2424.18, 2184.15, 94.11);
    bessyAI->AddWaypoint(5, 2413.18, 2184.15, 93.42);
    bessyAI->AddWaypoint(6, 2402.02, 2183.90, 87.59);
    bessyAI->AddWaypoint(7, 2333.31, 2181.63, 90.03,4000);
    bessyAI->AddWaypoint(8, 2308.73, 2184.34, 92.04);
    bessyAI->AddWaypoint(9, 2303.10, 2196.89, 94.94);
    bessyAI->AddWaypoint(10, 2304.58, 2272.23, 96.67);
    bessyAI->AddWaypoint(11, 2297.09, 2271.40, 95.16);
    bessyAI->AddWaypoint(12, 2297.68, 2266.79, 95.07,4000);
    bessyAI->AddWaypoint(13, 2297.67, 2266.76, 95.07,4000);

    return (CreatureAI*)bessyAI;
}

/*######
##
######*/

void AddSC_netherstorm()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "go_manaforge_control_console";
    newscript->pGOHello = &GOHello_go_manaforge_control_console;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_manaforge_control_console";
    newscript->GetAI = &GetAI_npc_manaforge_control_console;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_commander_dawnforge";
    newscript->GetAI = &GetAI_npc_commander_dawnforge;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "at_commander_dawnforge";
    newscript->pAreaTrigger = &AreaTrigger_at_commander_dawnforge;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_professor_dabiri";
    newscript->pGossipHello =   &GossipHello_npc_professor_dabiri;
    newscript->pGossipSelect =  &GossipSelect_npc_professor_dabiri;
    newscript->pQuestAccept = &QuestAccept_npc_professor_dabiri;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_phase_hunter";
    newscript->GetAI = &GetAI_mob_phase_hunter;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_bessy";
    newscript->GetAI = &GetAI_npc_bessy;
    newscript->pQuestAccept = &QuestAccept_npc_bessy;
    newscript->RegisterSelf();
}
