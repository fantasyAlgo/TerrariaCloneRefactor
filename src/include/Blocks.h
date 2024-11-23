#pragma once

#include <memory>
#include <raylib.h>
#include <vector>

enum BlockType{
    EMPTY,
    DIRT,
    GRASS,
    STONE,
    COPPER_ORE,
    WALL_DIRT,
    SOURCE_WATER,
    WATER_RIGHT,
    WATER_LEFT,
    WATER_DOWN,
    TORCH,
    WOOD,
    WORKBENCH,
	EMPTY2,
	EMPTY3,
	EMPTY4,
	EMPTY5,
	EMPTY6,
	EMPTY7,
	EMPTY8,
	EMPTY9,
	EMPTY10,
	EMPTY11,
	EMPTY12,
	EMPTY13,
	EMPTY14,
	EMPTY15,
	EMPTY16,
	EMPTY17,
	EMPTY18,
	EMPTY19,
	EMPTY20,
	EMPTY21,
	EMPTY22,
	EMPTY23,
	EMPTY24,
	EMPTY25,
	EMPTY26,
	EMPTY27,
	EMPTY28,
	EMPTY29,
	EMPTY30,
	EMPTY31,
	EMPTY32,
	EMPTY33,
	EMPTY34,
	EMPTY35,
	EMPTY36,
	EMPTY37,
	EMPTY38,
	EMPTY39,
	EMPTY40,
	EMPTY41,
	EMPTY42,
	EMPTY43,
	EMPTY44,
	EMPTY45,
	EMPTY46,
	EMPTY47,
	EMPTY48,
	EMPTY49,
	EMPTY50,
	EMPTY51,
	EMPTY52,
	EMPTY53,
	EMPTY54,
	EMPTY55,
	EMPTY56,
	EMPTY57,
	EMPTY58,
	EMPTY59,
	EMPTY60,
	EMPTY61,
	EMPTY62,
	EMPTY63,
	EMPTY64,
	EMPTY65,
	EMPTY66,
	EMPTY67,
	EMPTY68,
	EMPTY69,
	EMPTY70,
	EMPTY71,
	EMPTY72,
	EMPTY73,
	EMPTY74,
	EMPTY75,
	EMPTY76,
	EMPTY77,
	EMPTY78,
	EMPTY79,
	EMPTY80,
	EMPTY81,
	EMPTY82,
	EMPTY83,
	EMPTY84,
	EMPTY85,
	EMPTY86,
	EMPTY87,
	EMPTY88,
	EMPTY89,
	EMPTY90,
	EMPTY91,
	EMPTY92,
	EMPTY93,
	EMPTY94,
	EMPTY95,
	EMPTY96,
	EMPTY97,
	EMPTY98,
	EMPTY99,
	EMPTY100,
	EMPTY101,
	EMPTY102,
	EMPTY103,
	EMPTY104,
	EMPTY105,
	EMPTY106,
	EMPTY107,
	EMPTY108,
	EMPTY109,
	EMPTY110,
	EMPTY111,
	EMPTY112,
	EMPTY113,
	EMPTY114,
	EMPTY115,
	EMPTY116,
	EMPTY117,
	EMPTY118,
	EMPTY119,
	EMPTY120,
	EMPTY121,
	EMPTY122,
	EMPTY123,
	EMPTY124,
	EMPTY125,
	EMPTY126,
	EMPTY127,
	EMPTY128,
	EMPTY129,
	EMPTY130,
	EMPTY131,
	EMPTY132,
	EMPTY133,
	EMPTY134,
	EMPTY135,
	EMPTY136,
	EMPTY137,
	EMPTY138,
	EMPTY139,
	EMPTY140,
	EMPTY141,
	EMPTY142,
	EMPTY143,
	EMPTY144,
	EMPTY145,
	EMPTY146,
	EMPTY147,
	EMPTY148,
	EMPTY149,
	EMPTY150,
	EMPTY151,
	EMPTY152,
	EMPTY153,
	EMPTY154,
	EMPTY155,
	EMPTY156,
	EMPTY157,
	EMPTY158,
	EMPTY159,
	EMPTY160,
	EMPTY161,
	EMPTY162,
	EMPTY163,
	EMPTY164,
	EMPTY165,
	EMPTY166,
	EMPTY167,
	EMPTY168,
	EMPTY169,
	EMPTY170,
	EMPTY171,
	EMPTY172,
	EMPTY173,
	EMPTY174,
	EMPTY175,
	EMPTY176,
	EMPTY177,
	EMPTY178,
	EMPTY179,
	EMPTY180,
	EMPTY181,
	EMPTY182,
	EMPTY183,
	EMPTY184,
	EMPTY185,
	EMPTY186,
	EMPTY187,
    TREE_BRANCH,
    TREE_TRUNK,
    TREE_TOP,
  LAST_BLOCK
};

enum ToolType{
    EMPTY_TOOL,
    SWORD,
    PICKAXE,
    SHOVEL,
    AXE,
    BUCKET,
    SLIME_REMAINS,
    WOOD_ARMOR_TOP,
    WOOD_ARMOR_BODY,
    WOOD_ARMOR_LEGS,
    LAST_TOOL,
};

enum GeneralType{
	SWORD_TYPE,
	PICKAXE_TYPE,
	AXE_TYPE,
	ARMOR_HEAD,
	ARMOR_BODY,
	ARMOR_LEGS,
	GENERAL_ITEM,
	NO_BENCH_TYPE,
	WORKBENCH_TYPE,
	ANVIL_TYPE,
	FURNACE_TYPE,
	OTHER_TYPE,
};

struct PlayerItem {
    BlockType id;
    bool puttable;
    ToolType toolId;
    int amount;
};
struct ArmorItem {
	bool isArmor;
	BlockType toolId;
};

struct InventoryBlock {
    GeneralType generalType;
    BlockType id;
    unsigned short iconId;
    bool puttable;
    ToolType toolId;
    int texture;
    int craftable;
    std::vector<PlayerItem> recipe;
};

