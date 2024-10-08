#include "ScriptMgr.h"
#include "Player.h"
#include "Chat.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "Configuration/Config.h"
#include "Creature.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "Define.h"
#include "GossipDef.h"
#include "DataMap.h"
#include "GameObject.h"
#include "Transport.h"
#include "CreatureAI.h"
//#include "Log.h"

int GuildHouseInnKeeper, GuildHouseBank, GuildHouseMailBox, GuildHouseAuctioneer, GuildHouseTrainer, GuildHouseVendor, GuildHouseObject, GuildHousePortal, GuildHouseSpirit, GuildHouseProf, GuildHouseBuyRank;

class GuildHouseSpawner : public CreatureScript
{
private:
    int TBClevel;
    float TBCrate;
    int WOTLKlevel;
    float WOTLKrate;

public:
    GuildHouseSpawner() : CreatureScript("GuildHouseSpawner")
    {
        TBClevel = sConfigMgr->GetOption<int32>("GuildHouse.TBClevel", 65);
        TBCrate = sConfigMgr->GetOption<float>("GuildHouse.TBCrate", 2.0);
        WOTLKlevel = sConfigMgr->GetOption<int32>("GuildHouse.WOTLKlevel", 75);
        WOTLKrate = sConfigMgr->GetOption<float>("GuildHouse.WOTLKrate", 5.0);
    }

    struct GuildHouseSpawnerAI : public ScriptedAI
    {
        GuildHouseSpawnerAI(Creature* creature) : ScriptedAI(creature) {}

        void UpdateAI(uint32 /*diff*/) override
        {
            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        }
    };

    CreatureAI* GetAI(Creature *creature) const override
    {
        return new GuildHouseSpawnerAI(creature);
    }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (player->GetGuild())
        {
            Guild* guild = sGuildMgr->GetGuildById(player->GetGuildId());
            Guild::Member const* memberMe = guild->GetMember(player->GetGUID());

            if (!memberMe->IsRankNotLower(GuildHouseBuyRank))
            {
                ChatHandler(player->GetSession()).PSendSysMessage("You are not authorized to make Guild House purchases.");
                return false;
            }
        }
        else
        {
            ChatHandler(player->GetSession()).PSendSysMessage("You are not in a guild!");
            return false;
        }

        ClearGossipMenuFor(player);
        if (sConfigMgr->GetOption<int32>("GuildstoneID", 0) > 0)
        {
            // TODO: Should really check if they already have one
            AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, "Buy Guild Stone", GOSSIP_SENDER_MAIN, 901, "Buy Guild Stone?", sConfigMgr->GetOption<int32>("GuildstoneCost", 1000000), false);
        }
        AddGossipItemFor(player, GOSSIP_ICON_TALK, "Hire Class Trainers", GOSSIP_SENDER_MAIN, 902);
        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Hire Primary Profession Trainers", GOSSIP_SENDER_MAIN, 903);
        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Hire Secondary Profession Trainers", GOSSIP_SENDER_MAIN, 904);
        AddGossipItemFor(player, GOSSIP_ICON_TALK, "Hire Vendors", GOSSIP_SENDER_MAIN, 905);
        AddGossipItemFor(player, GOSSIP_ICON_TALK, "Hire other Services", GOSSIP_SENDER_MAIN, 906);
        AddGossipItemFor(player, GOSSIP_ICON_TALK, "Purchase Portals", GOSSIP_SENDER_MAIN, 907);
        AddGossipItemFor(player, GOSSIP_ICON_TALK, "Purchase Furniture", GOSSIP_SENDER_MAIN, 908);

        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        uint32 cost;
        bool empty;

        switch (action)
        {
            //
            // Secondary Menus
            //
            case 902: // Hire Class Trainer
                ClearGossipMenuFor(player);
                empty = true;
                // TODO: in theory these should lookup the entries. but also you could completely make this dynamic. 
                if (!player->FindNearestCreature(29195, VISIBILITY_RANGE, true))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Death Knight", GOSSIP_SENDER_MAIN, 12, "Hire Death Knight Trainer?", GuildHouseTrainer, false);
                    empty = false;
                }
                if (!player->FindNearestCreature(26324, VISIBILITY_RANGE, true))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Druid", GOSSIP_SENDER_MAIN, 2, "Hire Druid Trainer?", GuildHouseTrainer, false);
                    empty = false;
                }
                if (!player->FindNearestCreature(26325, VISIBILITY_RANGE, true))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Hunter", GOSSIP_SENDER_MAIN, 3, "Hire Hunter Trainer?", GuildHouseTrainer, false);
                    empty = false;
                }
                if (!player->FindNearestCreature(26326, VISIBILITY_RANGE, true))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Mage", GOSSIP_SENDER_MAIN, 4, "Hire Mage Trainer?", GuildHouseTrainer, false);
                    empty = false;
                }
                if (!player->FindNearestCreature(26327, VISIBILITY_RANGE, true))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Paladin", GOSSIP_SENDER_MAIN, 1, "Hire Paladin Trainer?", GuildHouseTrainer, false);
                    empty = false;
                }
                if (!player->FindNearestCreature(26328, VISIBILITY_RANGE, true))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Priest", GOSSIP_SENDER_MAIN, 5, "Hire Priest Trainer?", GuildHouseTrainer, false);
                    empty = false;
                }
                if (!player->FindNearestCreature(26329, VISIBILITY_RANGE, true))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Rogue", GOSSIP_SENDER_MAIN, 6, "Hire Rogue Trainer?", GuildHouseTrainer, false);
                    empty = false;
                }
                if (!player->FindNearestCreature(26330, VISIBILITY_RANGE, true))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Shaman", GOSSIP_SENDER_MAIN, 7, "Hire Shaman Trainer?", GuildHouseTrainer, false);
                    empty = false;
                }
                if (!player->FindNearestCreature(26331, VISIBILITY_RANGE, true))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Warlock", GOSSIP_SENDER_MAIN, 8, "Hire Warlock Trainer?", GuildHouseTrainer, false);
                    empty = false;
                }
                if (!player->FindNearestCreature(26332, VISIBILITY_RANGE, true))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Warrior", GOSSIP_SENDER_MAIN, 9, "Hire Warrior Trainer?", GuildHouseTrainer, false);
                    empty = false;
                }
                if (empty)
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "All Class Trainers hired", GOSSIP_SENDER_MAIN, 909);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Go Back!", GOSSIP_SENDER_MAIN, 909);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 903: // Hire Profession Trainers
                // TODO: entries should be lookups from DB
                ClearGossipMenuFor(player);
                // Alchemy
                if (!player->FindNearestCreature(33588, VISIBILITY_RANGE, true))
                {
                    if (!player->FindNearestCreature(19052, VISIBILITY_RANGE, true))
                        if (!player->FindNearestCreature(2837, VISIBILITY_RANGE, true))
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Alchemy Trainer", GOSSIP_SENDER_MAIN, 24, "Hire Alchemy Trainer?", GuildHouseProf, false);
                        else if (player->GetLevel() >= TBClevel)
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Master Alchemy Trainer", GOSSIP_SENDER_MAIN, 71, "Hire Master Alchemy Trainer?", GuildHouseProf * TBCrate, false);
                        else
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Master Alchemy Trainer", GOSSIP_SENDER_MAIN, 909);
                    else if (player->GetLevel() >= WOTLKlevel)
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Grand Master Alchemy Trainer", GOSSIP_SENDER_MAIN, 85, "Hire Grand Master Alchemy Trainer?", GuildHouseProf * WOTLKrate, false);
                    else
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Grand Master Alchemy Trainer", GOSSIP_SENDER_MAIN, 909);
                }
                else
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "All Alchemy Trainers hired", GOSSIP_SENDER_MAIN, 909);
                // Blacksmithing
                if (!player->FindNearestCreature(33591, VISIBILITY_RANGE, true))
                {
                    if (!player->FindNearestCreature(33675, VISIBILITY_RANGE, true))
                        if (!player->FindNearestCreature(2836, VISIBILITY_RANGE, true))
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Blacksmithing Trainer", GOSSIP_SENDER_MAIN, 13, "Hire Blacksmithing Trainer?", GuildHouseProf, false);
                        else if (player->GetLevel() >= TBClevel)
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Master Blacksmithing Trainer", GOSSIP_SENDER_MAIN, 63, "Hire Master Blacksmithing Trainer?", GuildHouseProf * TBCrate, false);
                        else
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Master Blacksmithing Trainer", GOSSIP_SENDER_MAIN, 909);
                    else if (player->GetLevel() >= WOTLKlevel)
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Grand Master Blacksmithing Trainer", GOSSIP_SENDER_MAIN, 77, "Hire Grand Master Blacksmithing Trainer?", GuildHouseProf * WOTLKrate, false);
                    else
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Grand Master Blacksmithing Trainer", GOSSIP_SENDER_MAIN, 909);
                }
                else
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "All Blacksmithing Trainers hired", GOSSIP_SENDER_MAIN, 909);
                // Engineering
                if (!player->FindNearestCreature(33586, VISIBILITY_RANGE, true))
                {
                    if (!player->FindNearestCreature(19576, VISIBILITY_RANGE, true))
                        if (!player->FindNearestCreature(8736, VISIBILITY_RANGE, true))
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Engineering Trainer", GOSSIP_SENDER_MAIN, 15, "Hire Engineering Trainer?", GuildHouseProf, false);
                        else if (player->GetLevel() >= TBClevel)
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Master Engineering Trainer", GOSSIP_SENDER_MAIN, 65, "Hire Master Engineering Trainer?", GuildHouseProf * TBCrate, false);
                        else
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Master Engineering Trainer", GOSSIP_SENDER_MAIN, 909);
                    else if (player->GetLevel() >= WOTLKlevel)
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Grand Master Engineering Trainer", GOSSIP_SENDER_MAIN, 79, "Hire Grand Master Engineering Trainer?", GuildHouseProf * WOTLKrate, false);
                    else
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Grand Master Engineering Trainer", GOSSIP_SENDER_MAIN, 909);
                }
                else
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "All Engineering Trainers hired", GOSSIP_SENDER_MAIN, 909);
                // Tailoring
                if (!player->FindNearestCreature(33580, VISIBILITY_RANGE, true))
                {
                    if (!player->FindNearestCreature(33684, VISIBILITY_RANGE, true))
                        if (!player->FindNearestCreature(2627, VISIBILITY_RANGE, true))
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Tailoring Trainer", GOSSIP_SENDER_MAIN, 26, "Hire Tailoring Trainer?", GuildHouseProf, false);
                        else if (player->GetLevel() >= TBClevel)
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Master Tailoring Trainer", GOSSIP_SENDER_MAIN, 73, "Hire Master Tailoring Trainer?", GuildHouseProf * TBCrate, false);
                        else
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Master Tailoring Trainer", GOSSIP_SENDER_MAIN, 909);
                    else if (player->GetLevel() >= WOTLKlevel)
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Grand Master Tailoring Trainer", GOSSIP_SENDER_MAIN, 87, "Hire Grand Master Tailoring Trainer?", GuildHouseProf * WOTLKrate, false);
                    else
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Grand Master Tailoring Trainer", GOSSIP_SENDER_MAIN, 909);
                }
                else
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "All Tailoring Trainers hired", GOSSIP_SENDER_MAIN, 909);
                // Leatherworking
                if (!player->FindNearestCreature(33581, VISIBILITY_RANGE, true))
                {
                    if (!player->FindNearestCreature(19187, VISIBILITY_RANGE, true))
                        if (!player->FindNearestCreature(5784, VISIBILITY_RANGE, true))
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Leatherworking Trainer", GOSSIP_SENDER_MAIN, 22, "Hire Leatherworking Trainer?", GuildHouseProf, false);
                        else if (player->GetLevel() >= TBClevel)
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Master Leatherworking Trainer", GOSSIP_SENDER_MAIN, 69, "Hire Master Leatherworking Trainer?", GuildHouseProf * TBCrate, false);
                        else
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Master Leatherworking Trainer", GOSSIP_SENDER_MAIN, 909);
                    else if (player->GetLevel() >= WOTLKlevel)
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Grand Master Leatherworking Trainer", GOSSIP_SENDER_MAIN, 83, "Hire Grand Master Leatherworking Trainer?", GuildHouseProf * WOTLKrate, false);
                    else
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Grand Master Leatherworking Trainer", GOSSIP_SENDER_MAIN, 909);
                }
                else
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "All Leatherworking Trainers hired", GOSSIP_SENDER_MAIN, 909);
                // Skinning
                if (!player->FindNearestCreature(28696, VISIBILITY_RANGE, true))
                {
                    if (!player->FindNearestCreature(19180, VISIBILITY_RANGE, true))
                        if (!player->FindNearestCreature(6288, VISIBILITY_RANGE, true))
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Skinning Trainer", GOSSIP_SENDER_MAIN, 23, "Hire Skinning Trainer?", GuildHouseProf, false);
                        else if (player->GetLevel() >= TBClevel)
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Master Skinning Trainer", GOSSIP_SENDER_MAIN, 70, "Hire Master Skinning Trainer?", GuildHouseProf * TBCrate, false);
                        else
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Master Skinning Trainer", GOSSIP_SENDER_MAIN, 909);
                    else if (player->GetLevel() >= WOTLKlevel)
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Grand Master Skinning Trainer", GOSSIP_SENDER_MAIN, 84, "Hire Grand Master Skinning Trainer?", GuildHouseProf * WOTLKrate, false);
                    else
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Grand Master Skinning Trainer", GOSSIP_SENDER_MAIN, 909);
                }
                else
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "All Skinning Trainers hired", GOSSIP_SENDER_MAIN, 909);
                // Mining
                if (!player->FindNearestCreature(28698, VISIBILITY_RANGE, true))
                {
                    if (!player->FindNearestCreature(33682, VISIBILITY_RANGE, true))
                        if (!player->FindNearestCreature(8128, VISIBILITY_RANGE, true))
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Mining Trainer", GOSSIP_SENDER_MAIN, 14, "Hire Mining Trainer?", GuildHouseProf, false);
                        else if (player->GetLevel() >= TBClevel)
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Master Mining Trainer", GOSSIP_SENDER_MAIN, 64, "Hire Master Mining Trainer?", GuildHouseProf * TBCrate, false);
                        else
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Master Mining Trainer", GOSSIP_SENDER_MAIN, 909);
                    else if (player->GetLevel() >= WOTLKlevel)
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Grand Master Mining Trainer", GOSSIP_SENDER_MAIN, 78, "Hire Grand Master Mining Trainer?", GuildHouseProf * WOTLKrate, false);
                    else
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Grand Master Mining Trainer", GOSSIP_SENDER_MAIN, 909);
                }
                else
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "All Mining Trainers hired", GOSSIP_SENDER_MAIN, 909);
                // Herbalism
                if (!player->FindNearestCreature(28704, VISIBILITY_RANGE, true))
                {
                    if (!player->FindNearestCreature(33678, VISIBILITY_RANGE, true))
                        if (!player->FindNearestCreature(908, VISIBILITY_RANGE, true))
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Herbalism Trainer", GOSSIP_SENDER_MAIN, 25, "Hire Herbalism Trainer?", GuildHouseProf, false);
                        else if (player->GetLevel() >= TBClevel)
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Master Herbalism Trainer", GOSSIP_SENDER_MAIN, 72, "Hire Master Herbalism Trainer?", GuildHouseProf * TBCrate, false);
                        else
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Master Herbalism Trainer", GOSSIP_SENDER_MAIN, 909);
                    else if (player->GetLevel() >= WOTLKlevel)
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Grand Master Herbalism Trainer", GOSSIP_SENDER_MAIN, 86, "Hire Grand Master Herbalism Trainer?", GuildHouseProf * WOTLKrate, false);
                    else
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Grand Master Herbalism Trainer", GOSSIP_SENDER_MAIN, 909);
                }
                else
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "All Herbalism Trainers hired", GOSSIP_SENDER_MAIN, 909);
                // Enchanting
                if (!player->FindNearestCreature(33583, VISIBILITY_RANGE, true))
                {
                    if (!player->FindNearestCreature(19540, VISIBILITY_RANGE, true))
                        if (!player->FindNearestCreature(11073, VISIBILITY_RANGE, true))
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Enchanting Trainer", GOSSIP_SENDER_MAIN, 18, "Hire Enchanting Trainer?", GuildHouseProf, false);
                        else if (player->GetLevel() >= TBClevel)
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Master Enchanting Trainer", GOSSIP_SENDER_MAIN, 67, "Hire Master Enchanting Trainer?", GuildHouseProf * TBCrate, false);
                        else
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Master Enchanting Trainer", GOSSIP_SENDER_MAIN, 909);
                    else if (player->GetLevel() >= WOTLKlevel)
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Grand Master Enchanting Trainer", GOSSIP_SENDER_MAIN, 81, "Hire Grand Master Enchanting Trainer?", GuildHouseProf * WOTLKrate, false);
                    else
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Grand Master Enchanting Trainer", GOSSIP_SENDER_MAIN, 909);
                }
                else
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "All Enchanting Trainers hired", GOSSIP_SENDER_MAIN, 909);
                // Jewelcrafting
                if (!player->FindNearestCreature(33590, VISIBILITY_RANGE, true))
                {
                    if (!player->FindNearestCreature(19539, VISIBILITY_RANGE, true))
                        if (!player->FindNearestCreature(19778, VISIBILITY_RANGE, true))
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Jewelcrafting Trainer", GOSSIP_SENDER_MAIN, 16, "Hire Jewelcrafting Trainer?", GuildHouseProf, false);
                        else if (player->GetLevel() >= TBClevel)
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Master Jewelcrafting Trainer", GOSSIP_SENDER_MAIN, 66, "Hire Master Jewelcrafting Trainer?", GuildHouseProf * TBCrate, false);
                        else
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Master Jewelcrafting Trainer", GOSSIP_SENDER_MAIN, 909);
                    else if (player->GetLevel() >= WOTLKlevel)
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Grand Master Jewelcrafting Trainer", GOSSIP_SENDER_MAIN, 80, "Hire Grand Master Jewelcrafting Trainer?", GuildHouseProf * WOTLKrate, false);
                    else
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Grand Master Jewelcrafting Trainer", GOSSIP_SENDER_MAIN, 909);
                }
                else
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "All Jewelcrafting Trainers hired", GOSSIP_SENDER_MAIN, 909);
                // Inscription
                // Check for Grand Master
                if (!player->FindNearestCreature(33603, VISIBILITY_RANGE, true))
                {
                    // Check for Master
                    if (!player->FindNearestCreature(33679, VISIBILITY_RANGE, true))
                        // Check for Artisan
                        if (!player->FindNearestCreature(30709, VISIBILITY_RANGE, true))
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Inscription Trainer", GOSSIP_SENDER_MAIN, 21, "Hire Inscription Trainer?", GuildHouseProf, false);
                        else if (player->GetLevel() >= TBClevel)
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Master Inscription Trainer", GOSSIP_SENDER_MAIN, 68, "Hire Master Inscription Trainer?", GuildHouseProf * TBCrate, false);
                        else
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Master Inscription Trainer", GOSSIP_SENDER_MAIN, 909);
                    else if (player->GetLevel() >= WOTLKlevel)
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Grand Master Inscription Trainer", GOSSIP_SENDER_MAIN, 82, "Hire Grand Master Inscription Trainer?", GuildHouseProf * WOTLKrate, false);
                    else
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Grand Master Inscription Trainer", GOSSIP_SENDER_MAIN, 909);
                }
                else
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "All Inscription Trainers hired", GOSSIP_SENDER_MAIN, 909);

                AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Go Back!", GOSSIP_SENDER_MAIN, 909);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 904: // Secondary Profession Trainers
                ClearGossipMenuFor(player);
                // First Aid
                // Check for Grand Master
                if (!player->FindNearestCreature(28706, VISIBILITY_RANGE, true))
                {
                    // Check for Master
                    if (!player->FindNearestCreature(18990, VISIBILITY_RANGE, true))
                        // Check for Artisan
                        if (!player->FindNearestCreature(19184, VISIBILITY_RANGE, true))
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "First Aid Trainer", GOSSIP_SENDER_MAIN, 27, "Hire First Aid Trainer?", GuildHouseProf, false);
                        else if (player->GetLevel() >= TBClevel)
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Master First Aid Trainer", GOSSIP_SENDER_MAIN, 74, "Hire Master First Aid Trainer?", GuildHouseProf * TBCrate, false);
                        else
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Master First Aid Trainer", GOSSIP_SENDER_MAIN, 909);
                    else if (player->GetLevel() >= WOTLKlevel)
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Grand Master First Aid Trainer", GOSSIP_SENDER_MAIN, 88, "Hire Grand Master First Aid Trainer?", GuildHouseProf * WOTLKrate, false);
                    else
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Grand Master First Aid Trainer", GOSSIP_SENDER_MAIN, 909);
                }
                else
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "All First Aid Trainers hired", GOSSIP_SENDER_MAIN, 909);

                // Fishing
                // Check for Grand Master
                if (!player->FindNearestCreature(28742, VISIBILITY_RANGE, true))
                {
                    // Check for Master
                    if (!player->FindNearestCreature(18911, VISIBILITY_RANGE, true))
                        // Check for Artisan
                        if (!player->FindNearestCreature(2834, VISIBILITY_RANGE, true))
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Fishing Trainer", GOSSIP_SENDER_MAIN, 28, "Hire Fishing Trainer?", GuildHouseProf, false);
                        else if (player->GetLevel() >= TBClevel)
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Master Fishing Trainer", GOSSIP_SENDER_MAIN, 75, "Hire Master Fishing Trainer?", GuildHouseProf * TBCrate, false);
                        else
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Master Fishing Trainer", GOSSIP_SENDER_MAIN, 909);
                    else if (player->GetLevel() >= WOTLKlevel)
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Grand Master Fishing Trainer", GOSSIP_SENDER_MAIN, 89, "Hire Grand Master Fishing Trainer?", GuildHouseProf * WOTLKrate, false);
                    else
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Grand Master Fishing Trainer", GOSSIP_SENDER_MAIN, 909);
                }
                else
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "All Fishing Trainers hired", GOSSIP_SENDER_MAIN, 909);

                // Cooking
                // Check for Grand Master
                if (!player->FindNearestCreature(33587, VISIBILITY_RANGE, true))
                {
                    // Check for Master
                    if (!player->FindNearestCreature(18993, VISIBILITY_RANGE, true))
                        // Check for Artisan
                        if (!player->FindNearestCreature(19185, VISIBILITY_RANGE, true))
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Cooking Trainer", GOSSIP_SENDER_MAIN, 29, "Hire Cooking Trainer?", GuildHouseProf, false);
                        else if (player->GetLevel() >= TBClevel)
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Master Cooking Trainer", GOSSIP_SENDER_MAIN, 76, "Hire Master Cooking Trainer?", GuildHouseProf * TBCrate, false);
                        else
                            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Master Cooking Trainer", GOSSIP_SENDER_MAIN, 909);
                    else if (player->GetLevel() >= WOTLKlevel)
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Grand Master Cooking Trainer", GOSSIP_SENDER_MAIN, 90, "Hire Grand Master Cooking Trainer?", GuildHouseProf * WOTLKrate, false);
                    else
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Higher Level required for Grand Master Cooking Trainer", GOSSIP_SENDER_MAIN, 909);
                }
                else
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "All Cooking Trainers hired", GOSSIP_SENDER_MAIN, 909);

                AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Go Back!", GOSSIP_SENDER_MAIN, 909);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 905: // Vendors
                ClearGossipMenuFor(player);
                if (!player->FindNearestCreature(28692, VISIBILITY_RANGE, true))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, "Trade Supplies", GOSSIP_SENDER_MAIN, 46, "Hire Trade Supplies?", GuildHouseVendor, false);
                }
                if (!player->FindNearestCreature(28776, VISIBILITY_RANGE, true))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, "Tabard Vendor", GOSSIP_SENDER_MAIN, 47, "Hire Tabard Vendor?", GuildHouseVendor, false);
                }
                if (!player->FindNearestCreature(4255, VISIBILITY_RANGE, true))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, "Food & Drink Vendor", GOSSIP_SENDER_MAIN, 48, "Hire Food & Drink Vendor?", GuildHouseVendor, false);
                }
                if (!player->FindNearestCreature(29636, VISIBILITY_RANGE, true))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, "Reagent Vendor", GOSSIP_SENDER_MAIN, 51, "Hire Reagent Vendor?", GuildHouseVendor, false);
                }
                if (!player->FindNearestCreature(29493, VISIBILITY_RANGE, true))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, "Ammo & Repair Vendor", GOSSIP_SENDER_MAIN, 52, "Hire Ammo & Repair Vendor?", GuildHouseVendor, false);
                }
                if (!player->FindNearestCreature(2622, VISIBILITY_RANGE, true))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, "Poisons Vendor", GOSSIP_SENDER_MAIN, 55, "Hire Poisons Vendor?", GuildHouseVendor, false);
                }
                if (!player->FindNearestCreature(3572, VISIBILITY_RANGE, true))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, "Fishing Supplies", GOSSIP_SENDER_MAIN, 55, "Hire Fishing Supplies Vendor?", GuildHouseVendor, false);
                }
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Go Back!", GOSSIP_SENDER_MAIN, 909);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 906: // Other Services
                ClearGossipMenuFor(player);
                if (!player->FindNearestCreature(500032, VISIBILITY_RANGE, true))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_TALK, "Hire Innkeeper", GOSSIP_SENDER_MAIN, 10, "Hire an Innkeeper?", GuildHouseInnKeeper, false);
                }
                if (!player->FindNearestCreature(28690, VISIBILITY_RANGE, true))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_TALK, "Hire Stable Master", GOSSIP_SENDER_MAIN, 53, "Hire a Stable Master?", GuildHouseVendor, false);
                }
                if (!player->FindNearestCreature(30605, VISIBILITY_RANGE, true))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, "Hire Banker", GOSSIP_SENDER_MAIN, 11, "Hire a Banker?", GuildHouseBank, false);
                }
                if (player->GetTeamId() == TEAM_ALLIANCE and !player->FindNearestCreature(8719, VISIBILITY_RANGE, true))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, "Hire Auctioneer", GOSSIP_SENDER_MAIN, 30, "Hire an Alliance Auctioneer?", GuildHouseAuctioneer, false);
                }
                else if (player->GetTeamId() == TEAM_HORDE and !player->FindNearestCreature(9856, VISIBILITY_RANGE, true))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, "Hire Auctioneer", GOSSIP_SENDER_MAIN, 31, "Hire a Horde Auctioneer?", GuildHouseAuctioneer, false);
                }
                if (!player->FindNearestCreature(9858, VISIBILITY_RANGE, true))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, "Hire Neutral Auctioneer", GOSSIP_SENDER_MAIN, 54, "Hire a Neutral Auctioneer?", GuildHouseAuctioneer, false);
                }
                if (!player->FindNearestCreature(6491, VISIBILITY_RANGE, true))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_TALK, "Hire Sprirt Healer", GOSSIP_SENDER_MAIN, 49, "Hire a Spirit Healer?", GuildHouseSpirit, false);
                }
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Go Back!", GOSSIP_SENDER_MAIN, 909);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 907: // Portals
                ClearGossipMenuFor(player);

                if (player->GetTeamId() == TEAM_ALLIANCE)
                {
                    // ALLIANCE players get these options
                    if (!player->FindNearestGameObject(190960, VISIBLE_RANGE))
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Portal: Stormwind", GOSSIP_SENDER_MAIN, 35, "Add Stormwind Portal?", GuildHousePortal, false);
                    if (!player->FindNearestGameObject(191008, VISIBLE_RANGE))
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Portal: Ironforge", GOSSIP_SENDER_MAIN, 36, "Add Ironforge Portal?", GuildHousePortal, false);
                    if (!player->FindNearestGameObject(191006, VISIBLE_RANGE))
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Portal: Darnassus", GOSSIP_SENDER_MAIN, 37, "Add Darnassus Portal?", GuildHousePortal, false);
                    if (!player->FindNearestGameObject(191007, VISIBLE_RANGE))
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Portal: Exodar", GOSSIP_SENDER_MAIN, 38, "Add Exodar Portal?", GuildHousePortal, false);
                    // TODO: Theramore?
                }
                else
                {
                    // HORDE players get these options
                    if (!player->FindNearestGameObject(191009, VISIBLE_RANGE))
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Portal: Orgrimmar", GOSSIP_SENDER_MAIN, 41, "Add Orgrimmar Portal?", GuildHousePortal, false);
                    if (!player->FindNearestGameObject(191012, VISIBLE_RANGE))
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Portal: Undercity", GOSSIP_SENDER_MAIN, 41, "Add Undercity Portal?", GuildHousePortal, false);
                    if (!player->FindNearestGameObject(191011, VISIBLE_RANGE))
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Portal: Thunderbluff", GOSSIP_SENDER_MAIN, 40, "Add Thunderbuff Portal?", GuildHousePortal, false);
                    if (!player->FindNearestGameObject(191010, VISIBLE_RANGE))
                        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Portal: Silvermoon", GOSSIP_SENDER_MAIN, 42, "Add Silvermoon Portal?", GuildHousePortal, false);
                    // TODO: Swamp of sorrows?
                }

                // These two portals work for either Team
                // TODO: Portal to Booty Bay
                // TODO: Portal to Gadgetzan
                // TODO: Portal to Winterspring
                // TODO: Portal to Blasted Lands
                if (!player->FindNearestGameObject(500008, VISIBLE_RANGE) and player->GetLevel() >= TBClevel)
                    AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Portal: Shattrath", GOSSIP_SENDER_MAIN, 43, "Add Shattrath Portal?", GuildHousePortal, false);
                if (!player->FindNearestGameObject(500009, VISIBLE_RANGE) and player->GetLevel() >= WOTLKlevel)
                    AddGossipItemFor(player, GOSSIP_ICON_TAXI, "Portal: Dalaran", GOSSIP_SENDER_MAIN, 44, "Add Dalaran Portal?", GuildHousePortal, false);

                AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Go Back!", GOSSIP_SENDER_MAIN, 909);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 908: // Furniture
                ClearGossipMenuFor(player);
                if (!player->FindNearestGameObject(1685, VISIBLE_RANGE))
                    AddGossipItemFor(player, GOSSIP_ICON_TALK, "Forge", GOSSIP_SENDER_MAIN, 33, "Add a forge?", GuildHouseObject, false);
                if (!player->FindNearestGameObject(4087, VISIBLE_RANGE))
                    AddGossipItemFor(player, GOSSIP_ICON_TALK, "Anvil", GOSSIP_SENDER_MAIN, 34, "Add an Anvil?", GuildHouseObject, false);
                if (!player->FindNearestGameObject(187293, VISIBLE_RANGE))
                    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, "Guild Vault", GOSSIP_SENDER_MAIN, 45, "Add Guild Vault?", GuildHouseObject, false);
                if (!player->FindNearestGameObject(191028, VISIBLE_RANGE))
                    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Barber Chair", GOSSIP_SENDER_MAIN, 50, "Add a Barber Chair?", GuildHouseObject, false);
                if (!player->FindNearestGameObject(184137, VISIBLE_RANGE))
                    AddGossipItemFor(player, GOSSIP_ICON_TALK, "Mailbox", GOSSIP_SENDER_MAIN, 32, "Add a Mailbox?", GuildHouseMailBox, false);
                // TODO: Cooking device
                // TODO: Moonwell

                AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Go Back!", GOSSIP_SENDER_MAIN, 909);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 909: // Go back!
                OnGossipHello(player, creature);
                break;

            //
            // Actions
            //
            // Guildstone
            case 901:
                player->AddItem(sConfigMgr->GetOption<int32>("GuildstoneID", 0), 1);
                break;
            //
            // Class Trainers
            case 1:  // Paladin
            case 2:  // Druid
            case 3:  // Hunter
            case 4:  // Mage
            case 5:  // Priest
            case 6:  // Rogue
            case 7:  // Shaman
            case 8:  // Warlock
            case 9:  // Warrior
            case 12: // Death Knight
                cost = GuildHouseTrainer;
                SpawnNPC(player, action, cost);
                break;
            case 13: // Blacksmithing
            case 14: // Mining
            case 15: // Engineering
            case 16: // Jewelcrafting
            case 18: // Enchanting
            case 21: // Inscription
            case 22: // Leatherworking
            case 23: // Skinning
            case 24: // Alchemy
            case 25: // Herbalism
            case 26: // Tailoring
            case 27: // First Aid
            case 28: // Fishing
            case 29: // Cooking
                cost = GuildHouseProf;
                SpawnNPC(player, action, cost);
                break;
            case 63: // Blacksmithing
                cost = GuildHouseProf * TBCrate;
                ReplaceNPC(player, action, 13, cost);
                break;
            case 64: // Mining
                cost = GuildHouseProf * TBCrate;
                ReplaceNPC(player, action, 14, cost);
                break;
            case 65: // Engineering
                cost = GuildHouseProf * TBCrate;
                ReplaceNPC(player, action, 15, cost);
                break;
            case 66: // Jewelcrafting
                cost = GuildHouseProf * TBCrate;
                ReplaceNPC(player, action, 16, cost);
                break;
            case 67: // Enchanting
                cost = GuildHouseProf * TBCrate;
                ReplaceNPC(player, action, 18, cost);
                break;
            case 68: // Inscription
                cost = GuildHouseProf * TBCrate;
                ReplaceNPC(player, action, 21, cost);
                break;
            case 69: // Leatherworking
                cost = GuildHouseProf * TBCrate;
                ReplaceNPC(player, action, 22, cost);
                break;
            case 70: // Skinning
                cost = GuildHouseProf * TBCrate;
                ReplaceNPC(player, action, 23, cost);
                break;
            case 71: // Alchemy
                cost = GuildHouseProf * TBCrate;
                ReplaceNPC(player, action, 24, cost);
                break;
            case 72: // Herbalism
                cost = GuildHouseProf * TBCrate;
                ReplaceNPC(player, action, 25, cost);
                break;
            case 73: // Tailoring
                cost = GuildHouseProf * TBCrate;
                ReplaceNPC(player, action, 26, cost);
                break;
            case 74: // Outland - First Aid Trainer
                cost = GuildHouseProf * TBCrate;
                ReplaceNPC(player, action, 27, cost);
                break;
            case 75: // Outland - Fishing Trainer
                cost = GuildHouseProf * TBCrate;
                ReplaceNPC(player, action, 28, cost);
                break;
            case 76: // Outland - Cooking Trainer
                cost = GuildHouseProf * TBCrate;
                ReplaceNPC(player, action, 29, cost);
                break;
//
            case 77: // Blacksmithing
                cost = GuildHouseProf * WOTLKrate;
                ReplaceNPC(player, action, 63, cost);
                break;
            case 78: // Mining
                cost = GuildHouseProf * WOTLKrate;
                ReplaceNPC(player, action, 64, cost);
                break;
            case 79: // Engineering
                cost = GuildHouseProf * WOTLKrate;
                ReplaceNPC(player, action, 65, cost);
                break;
            case 80: // Jewelcrafting
                cost = GuildHouseProf * WOTLKrate;
                ReplaceNPC(player, action, 66, cost);
                break;
            case 81: // Enchanting
                cost = GuildHouseProf * WOTLKrate;
                ReplaceNPC(player, action, 67, cost);
                break;
            case 82: // Inscription
                cost = GuildHouseProf * WOTLKrate;
                ReplaceNPC(player, action, 68, cost);
                break;
            case 83: // Leatherworking
                cost = GuildHouseProf * WOTLKrate;
                ReplaceNPC(player, action, 69, cost);
                break;
            case 84: // Skinning
                cost = GuildHouseProf * WOTLKrate;
                ReplaceNPC(player, action, 70, cost);
                break;
            case 85: // Alchemy
                cost = GuildHouseProf * WOTLKrate;
                ReplaceNPC(player, action, 71, cost);
                break;
            case 86: // Herbalism
                cost = GuildHouseProf * WOTLKrate;
                ReplaceNPC(player, action, 72, cost);
                break;
            case 87: // Tailoring
                cost = GuildHouseProf * WOTLKrate;
                ReplaceNPC(player, action, 73, cost);
                break;
                //
            case 88: // Northrend - First Aid Trainer
                cost = GuildHouseProf * WOTLKrate;
                ReplaceNPC(player, action, 74, cost);
                break;
            case 89: // Northrend - Fishing Trainer
                cost = GuildHouseProf * WOTLKrate;
                ReplaceNPC(player, action, 75, cost);
                break;
            case 90: // Northrend - Cooking Trainer
                cost = GuildHouseProf * WOTLKrate;
                ReplaceNPC(player, action, 76, cost);
                break;
            //
            // Vendors 
            case 46: // Trade Supplies
            case 47: // Tabard Vendor
            case 48:  // Food & Drink Vendor
            case 51: // Reagent Vendor
            case 52: // Ammo & Repair Vendor
            case 53: // Stable Master
            case 55:  // Poisons Vendor
            case 56:  // Transmog Vendor
            case 58:  // Fishing Supplies Vendor
                cost = GuildHouseVendor;
                SpawnNPC(player, action, cost);
                break;
            //
            // Other services
            case 30: // Alliance Auctioneer
            case 31: // Horde Auctioneer
            case 54: // Neutral Auctioneer
                cost = GuildHouseAuctioneer;
                SpawnNPC(player, action, cost);
                break;
            case 11: // Banker
                cost = GuildHouseBank;
                SpawnNPC(player, action, cost);
                break;
            case 10: // Innkeeper
                cost = GuildHouseInnKeeper;
                SpawnNPC(player, action, cost);
                break;
            case 59: // Barber                
                cost = GuildHouseVendor;
                SpawnNPC(player, 59, cost);
                SpawnObject(player, 50, 0);
                break;
            case 49: // Spirit Healer
                cost = GuildHouseSpirit;
                SpawnNPC(player, action, cost);
                break;

            //
            // Objects
            // TODO: Cooking device
            case 32: // Mailbox
                cost = GuildHouseMailBox;
                SpawnObject(player, action, cost);
                break;
            case 33: // Forge
            case 34: // Anvil
            case 45: // Guild Vault
                cost = GuildHouseObject;
                SpawnObject(player, action, cost);
                break;
            case 50: // Barber chair
                cost = GuildHouseVendor;
                SpawnNPC(player, 59,cost);
                SpawnObject(player, 50, 0);
                break;
            case 35: // Stormwind Portal
            case 36: // Ironforge Portal
            case 37: // Darnassus Portal
            case 38: // Exodar Portal
            case 39: // Orgrimmar Portal
            case 40: // Thunder Bluff Portal
            case 41: // Undercity Portal
            case 42: // Silvermoon Portal
                cost = GuildHousePortal;
                SpawnObject(player, action, cost);
                break;
            case 43: // Shattrath Portal
                // TODO: Need level req
                cost = GuildHousePortal * TBCrate;
                SpawnObject(player, action, cost);
                break;
            case 44: // Dalaran Portal
                // TODO: Need level req
                cost = GuildHousePortal * WOTLKrate;
                SpawnObject(player, action, cost);
                break;
        }
        return true;
    }

    uint32 GetGuildPhase(Player* player)
    {
        return player->GetGuildId() + 10;
    }

    void SpawnNPC(Player *player, uint32 id, uint32 cost)
    {
        uint32 entry;
        float posX;
        float posY;
        float posZ;
        float ori;

        QueryResult result = WorldDatabase.Query("SELECT entry, posX, posY, posZ, orientation FROM guild_house_spawns WHERE id = {}", id);

        if (!result)
            return;

        do
        {
            Field* fields = result->Fetch();
            entry = fields[0].Get<float>();
            posX = fields[1].Get<float>();
            posY = fields[2].Get<float>();
            posZ = fields[3].Get<float>();
            ori = fields[4].Get<float>();

        } while (result->NextRow());

        if (player->FindNearestCreature(entry, VISIBILITY_RANGE, true))
        {
            ChatHandler(player->GetSession()).PSendSysMessage("You already have this creature!");
            CloseGossipMenuFor(player);
            return;
        }

        Creature *creature = new Creature();

        if (!creature->Create(player->GetMap()->GenerateLowGuid<HighGuid::Unit>(), player->GetMap(), GetGuildPhase(player), entry, 0, posX, posY, posZ, ori))
        {
            delete creature;
            return;
        }
        creature->SaveToDB(player->GetMapId(), (1 << player->GetMap()->GetSpawnMode()), GetGuildPhase(player));
        uint32 db_guid = creature->GetSpawnId();

        creature->CleanupsBeforeDelete();
        delete creature;
        creature = new Creature();
        if (!creature->LoadCreatureFromDB(db_guid, player->GetMap()))
        {
            delete creature;
            return;
        }

        sObjectMgr->AddCreatureToGrid(db_guid, sObjectMgr->GetCreatureData(db_guid));
        player->ModifyMoney(-cost);
        CloseGossipMenuFor(player);
    }

    void ReplaceNPC(Player* player, uint32 id, uint32 baseid, uint32 cost)
    {
        uint32 newEntry;
        uint32 entry;
        float posX;
        float posY;
        float posZ;
        float ori;

        LOG_INFO("module", "GuildHouse: Replacing NPC {} with {}", baseid, id);

        QueryResult result = WorldDatabase.Query("SELECT entry FROM guild_house_spawns WHERE id = {}", id);

        if (!result)
        {
            LOG_INFO("module", "GuildHouse: NPC {} not found", id);
            return;
        }

        do
        {
            Field* fields = result->Fetch();
            newEntry = fields[0].Get<float>();

        } while (result->NextRow());

        if (player->FindNearestCreature(newEntry, VISIBILITY_RANGE, true))
        {
            ChatHandler(player->GetSession()).PSendSysMessage("You already have this creature!");
            CloseGossipMenuFor(player);
            return;
        }

        result = WorldDatabase.Query("SELECT entry, posX, posY, posZ, orientation FROM guild_house_spawns WHERE id = {}", baseid);

        if (!result)
            return;

        do
        {
            Field* fields = result->Fetch();
            entry = fields[0].Get<float>();
            posX = fields[1].Get<float>();
            posY = fields[2].Get<float>();
            posZ = fields[3].Get<float>();
            ori = fields[4].Get<float>();

        } while (result->NextRow());

        Creature* creature = player->FindNearestCreature(entry, VISIBILITY_RANGE, true);

        if (!creature)
        {
            // prior NPC not found
            return;
        }

        uint32 db_guid = creature->GetSpawnId();
        LOG_INFO("module", "GuildHouse: Updating NPC SpawnID {} with entry {}", db_guid, newEntry);
        creature->SetEntry(newEntry);
        creature->SaveToDB();

        creature->CleanupsBeforeDelete();
        delete creature;
        creature = new Creature();
        if (!creature->LoadCreatureFromDB(db_guid, player->GetMap()))
        {
            delete creature;
            return;
        }
        sObjectMgr->AddCreatureToGrid(db_guid, sObjectMgr->GetCreatureData(db_guid));
        
        player->ModifyMoney(-cost);
        CloseGossipMenuFor(player);
    }

    void SpawnObject(Player* player, uint32 id, uint32 cost)
    {
        uint32 entry;
        float posX;
        float posY;
        float posZ;
        float ori;

        QueryResult result = WorldDatabase.Query("SELECT entry, posX, posY, posZ, orientation FROM guild_house_spawns WHERE id = {}", id);

        if (!result)
            return;

        do
        {
            Field *fields = result->Fetch();
            entry = fields[0].Get<uint32>();
            posX = fields[1].Get<float>();
            posY = fields[2].Get<float>();
            posZ = fields[3].Get<float>();
            ori = fields[4].Get<float>();

        } while (result->NextRow());

        if (!entry)
            return;

        if (player->FindNearestGameObject(entry, VISIBLE_RANGE))
        {
            ChatHandler(player->GetSession()).PSendSysMessage("You already have this object!");
            CloseGossipMenuFor(player);
            return;
        }

        const GameObjectTemplate *objectInfo = sObjectMgr->GetGameObjectTemplate(entry);

        if (!objectInfo)
            return;

        if (objectInfo->displayId && !sGameObjectDisplayInfoStore.LookupEntry(objectInfo->displayId))
            return;

        GameObject* object = sObjectMgr->IsGameObjectStaticTransport(objectInfo->entry) ? new StaticTransport() : new GameObject();
        ObjectGuid::LowType guidLow = player->GetMap()->GenerateLowGuid<HighGuid::GameObject>();

        if (!object->Create(guidLow, objectInfo->entry, player->GetMap(), GetGuildPhase(player), posX, posY, posZ, ori, G3D::Quat(), 0, GO_STATE_READY))
        {
            delete object;
            return;
        }

        // fill the gameobject data and save to the db
        object->SaveToDB(player->GetMapId(), (1 << player->GetMap()->GetSpawnMode()), GetGuildPhase(player));
        guidLow = object->GetSpawnId();
        // delete the old object and do a clean load from DB with a fresh new GameObject instance.
        // this is required to avoid weird behavior and memory leaks
        delete object;

        object = sObjectMgr->IsGameObjectStaticTransport(objectInfo->entry) ? new StaticTransport() : new GameObject();
        // this will generate a new guid if the object is in an instance
        if (!object->LoadGameObjectFromDB(guidLow, player->GetMap(), true))
        {
            delete object;
            return;
        }

        // TODO: is it really necessary to add both the real and DB table guid here ?
        sObjectMgr->AddGameobjectToGrid(guidLow, sObjectMgr->GetGameObjectData(guidLow));
        player->ModifyMoney(-cost);
        CloseGossipMenuFor(player);
    }
};

class GuildHouseButlerConf : public WorldScript
{
public:
    GuildHouseButlerConf() : WorldScript("GuildHouseButlerConf") {}

    void OnBeforeConfigLoad(bool /*reload*/) override
    {
        GuildHouseInnKeeper = sConfigMgr->GetOption<int32>("GuildHouseInnKeeper", 1000000);
        GuildHouseBank = sConfigMgr->GetOption<int32>("GuildHouseBank", 1000000);
        GuildHouseMailBox = sConfigMgr->GetOption<int32>("GuildHouseMailbox", 500000);
        GuildHouseAuctioneer = sConfigMgr->GetOption<int32>("GuildHouseAuctioneer", 500000);
        GuildHouseTrainer = sConfigMgr->GetOption<int32>("GuildHouseTrainerCost", 1000000);
        GuildHouseVendor = sConfigMgr->GetOption<int32>("GuildHouseVendor", 500000);
        GuildHouseObject = sConfigMgr->GetOption<int32>("GuildHouseObject", 500000);
        GuildHousePortal = sConfigMgr->GetOption<int32>("GuildHousePortal", 500000);
        GuildHouseProf = sConfigMgr->GetOption<int32>("GuildHouseProf", 500000);
        GuildHouseSpirit = sConfigMgr->GetOption<int32>("GuildHouseSpirit", 100000);
        GuildHouseBuyRank = sConfigMgr->GetOption<int32>("GuildHouseBuyRank", 4);
    }
};

void AddGuildHouseButlerScripts()
{
    new GuildHouseSpawner();
    new GuildHouseButlerConf();
}
