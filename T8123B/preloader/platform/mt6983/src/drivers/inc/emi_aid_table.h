#include "emi_mpu_mt.h"

struct per_aid {
        unsigned char region;
        unsigned char perm;
};

struct aid_table {
        unsigned char aid;
        struct per_aid aid_col[REGION_MAX];
};

struct aid_table AID_TABLE_0 = {10, {{8, RW},{21, RW},{22, RW},{23, RW},{24, RW}}};
struct aid_table AID_TABLE_1 = {11, {{19, RW}}};
struct aid_table AID_TABLE_2 = {30, {{9, READ}}};
struct aid_table AID_TABLE_3 = {33, {{0, READ},{8, RW},{10, RW},{12, RW},{25, RW}}};
struct aid_table AID_TABLE_4 = {34, {{4, READ},{32, READ},{33, READ},{34, RW},{35, RW},{36, RW},
				{37, RW},{38, READ},{41, RW},{42, RW},{43, RW}}};
struct aid_table AID_TABLE_5 = {35, {{4, RW},{32, READ},{33, READ},{34, RW},{35, RW},{36, RW},
				{37, RW},{38, READ},{41, RW},{42, RW},{43, RW}}};
struct aid_table AID_TABLE_6 = {36, {{41, RW},{44, RW},{45, RW},{46, RW},{47, RW}}};
struct aid_table AID_TABLE_7 = {37, {{41, RW},{44, RW},{45, RW},{46, RW},{47, RW}}};
struct aid_table AID_TABLE_8 = {38, {{26, RW},{27, RW},{42, RW},{45, RW}}};
struct aid_table AID_TABLE_9 = {39, {{26, RW},{27, RW},{42, RW},{45, RW}}};
struct aid_table AID_TABLE_10 = {40, {{42, RW}}};
struct aid_table AID_TABLE_11 = {41, {{42, RW}}};
struct aid_table AID_TABLE_12 = {42, {{42, RW}}};
struct aid_table AID_TABLE_13 = {43, {{42, RW}}};
struct aid_table AID_TABLE_14 = {44, {{26, RW},{27, RW},{42, RW}}};
struct aid_table AID_TABLE_15 = {45, {{42, RW}}};
struct aid_table AID_TABLE_16 = {46, {{4, READ},{32, READ},{33, READ},{34, RW},{35, READ},{36, RW},
				{37, READ},{38, RW},{39, RW},{41, RW},{42, RW},{43, RW}}};
struct aid_table AID_TABLE_17 = {47, {{4, READ},{32, READ},{33, READ},{34, RW},{35, READ},{36, RW},
				{37, READ},{38, RW},{39, RW},{41, RW},{42, RW},{43, RW}}};
struct aid_table AID_TABLE_18 = {48, {{0, RW},{17, RW},{18, RW},}};
struct aid_table AID_TABLE_19 = {49, {{0, RW},{18, RW}}};
struct aid_table AID_TABLE_20 = {52, {{0, RW},{27, RW},{29, RW},{43, RW}}};
struct aid_table AID_TABLE_21 = {53, {{0, RW},{27, RW},{29, RW},{43, RW}}};
struct aid_table AID_TABLE_22 = {54, {{29, RW},{43, RW}}};
struct aid_table AID_TABLE_23 = {55, {{29, RW},{43, RW}}};
struct aid_table AID_TABLE_24 = {56, {{29, RW},{43, RW}}};
struct aid_table AID_TABLE_25 = {57, {{29, RW},{43, RW}}};
struct aid_table AID_TABLE_26 = {58, {{28, RW},{29, RW},{43, RW}}};
struct aid_table AID_TABLE_27 = {59, {{28, RW},{29, RW},{43, RW}}};
struct aid_table AID_TABLE_28 = {60, {{28, RW},{29, RW},{43, RW}}};
struct aid_table AID_TABLE_29 = {61, {{28, RW},{29, RW},{43, RW}}};
struct aid_table AID_TABLE_30 = {62, {{12, READ}}};
struct aid_table AID_TABLE_31 = {63, {{12, READ}}};
struct aid_table AID_TABLE_32 = {76, {{12, RW},{31, RW}}};
struct aid_table AID_TABLE_33 = {92, {{36, RW},{37, RW},{38, RW}}};
struct aid_table AID_TABLE_34 = {93, {{36, RW},{37, RW},{38, RW}}};
struct aid_table AID_TABLE_35 = {116, {{0, RW},{9, RW},{14, RW},{15, RW},{16, RW}}};
struct aid_table AID_TABLE_36 = {128, {{0, RW},{8, READ},{10, READ},{11, READ},{14, READ}}};
struct aid_table AID_TABLE_37 = {130, {{0, RW},{8, RW},{10, READ},{11, RW},{14, READ}}};
struct aid_table AID_TABLE_38 = {132, {{0, RW},{8, RW},{10, READ},{11, RW},{14, READ}}};
struct aid_table AID_TABLE_39 = {134, {{0, RW},{8, READ},{10, READ},{11, READ},{14, READ}}};
struct aid_table AID_TABLE_40 = {136, {{0, RW},{8, RW},{10, READ},{11, RW},{14, READ}}};
struct aid_table AID_TABLE_41 = {144, {{0, RW},{8, READ},{10, READ},{11, READ},{14, READ}}};
struct aid_table AID_TABLE_42 = {146, {{0, RW},{8, RW},{10, READ},{11, RW},{14, READ}}};
struct aid_table AID_TABLE_43 = {148, {{0, RW},{8, RW},{10, READ},{11, RW},{14, READ}}};
struct aid_table AID_TABLE_44 = {150, {{0, RW},{8, READ},{10, READ},{11, READ},{14, READ}}};
struct aid_table AID_TABLE_45 = {152, {{0, RW},{8, RW},{10, READ},{11, RW},{14, READ}}};
struct aid_table AID_TABLE_46 = {176, {{0, RW},{9, RW},{14, RW}}};
struct aid_table AID_TABLE_47 = {177, {{0, RW},{9, RW}}};
struct aid_table AID_TABLE_48 = {178, {{0, RW},{9, RW},{14, RW}}};
struct aid_table AID_TABLE_49 = {179, {{0, RW},{9, RW}}};
struct aid_table AID_TABLE_50 = {184, {{0, RW},{8, RW},{11, RW},{30, RW}}};
struct aid_table AID_TABLE_51 = {206, {{10, RW}}};
struct aid_table AID_TABLE_52 = {208, {{0, RW},{10, RW}}};
struct aid_table AID_TABLE_53 = {210, {{0, RW},{10, RW}}};
struct aid_table AID_TABLE_54 = {216, {{0, RW},{10, RW},{20, RW}}};
struct aid_table AID_TABLE_55 = {217, {{0, RW},{20, RW}}};
struct aid_table AID_TABLE_56 = {240, {{0, RW},{1, RW},{2, RW},{4, READ},{6, RW},{8, RW},{9, RW},
				{11, RW},{13, RW},{14, RW},{15, RW},{16, RW},{18, RW},{19, RW},{20, RW},
				{21, RW},{22, RW},{23, RW},{24, RW},{25, RW},{26, RW},{27, RW},
				{28, RW},{29, RW},{30, RW},{39, READ},{41, READ},{43, RW},{44, RW},
				{46, RW},{47, RW}}};
struct aid_table AID_TABLE_57 = {241, {{0, RW},{5, RW},{8, READ}, {10, READ}, {16 ,READ}, {11, READ},{18, RW},{19, RW},{25, RW},{27, RW},{28, READ},{29, RW},
				{31, RW},{39, READ},{41, READ},{42, RW},{43, RW},{44, RW},{46, RW},{47, RW},}};
struct aid_table AID_TABLE_58 = {243, {{0, RW},{3, RW},{4, RW},{5, RW},{6, RW},{7, RW},{8, RW},{9, RW},{10, RW},
				{11, RW},{13, RW},{21, RW},{24, RW},{25, RW},{48, RW}}};
struct aid_table AID_TABLE_59 = {252, {{19, RW}}};
struct aid_table AID_TABLE_60 = {253, {{19, RW}}};
struct aid_table AID_TABLE_63 = {64, {{0, RW},{8, RW}}};
struct aid_table AID_TABLE_64 = {65, {{0, RW},{8, RW}}};
struct aid_table AID_TABLE_65 = {66, {{0, RW},{8, RW}}};
struct aid_table AID_TABLE_66 = {67, {{0, RW},{8, RW}}};
struct aid_table AID_TABLE_67 = {68, {{0, RW},{8, RW}}};
struct aid_table AID_TABLE_68 = {69, {{0, RW},{8, RW}}};
struct aid_table AID_TABLE_69 = {70, {{0, RW},{8, RW}}};
struct aid_table AID_TABLE_70 = {71, {{0, RW},{8, RW}}};
struct aid_table AID_TABLE_71 = {224, {{0, RW},{8, RW},{11, RW}}};
struct aid_table AID_TABLE_72 = {225, {{0, RW},{8, RW},{11, RW}}};
struct aid_table AID_TABLE_73 = {226, {{0, RW},{8, RW},{11, RW}}};
struct aid_table AID_TABLE_74 = {227, {{0, RW},{8, RW},{11, RW}}};
struct aid_table AID_TABLE_75 = {100, {{0, RW},{9, RW}}};
struct aid_table AID_TABLE_76 = {255, {{0, RW},{9, READ},{13, READ}}};
struct aid_table AID_TABLE_77 = {254, {{0, RW},{9, READ},{13, READ}}};
struct aid_table AID_TABLE_78 = {160, {{0, RW},{8, RW},{10, RW},{11, RW}}};
struct aid_table AID_TABLE_79 = {162, {{0, RW},{8, RW},{10, RW},{11, RW}}};
struct aid_table AID_TABLE_80 = {164, {{0, RW},{8, RW},{10, RW}}};
struct aid_table AID_TABLE_81 = {166, {{0, RW},{8, RW},{10, RW}}};
//NSR
struct aid_table AID_TABLE_82 = {15, {{0, RW}}};
struct aid_table AID_TABLE_83 = {31, {{0, READ},{13, READ}}};
struct aid_table AID_TABLE_84 = {32, {{0, RW}}};
struct aid_table AID_TABLE_85 = {73, {{0, RW}}};
struct aid_table AID_TABLE_86 = {80, {{0, RW}}};
struct aid_table AID_TABLE_87 = {81, {{0, RW}}};
struct aid_table AID_TABLE_88 = {82, {{0, RW}}};
struct aid_table AID_TABLE_89 = {83, {{0, RW}}};
struct aid_table AID_TABLE_90 = {84, {{0, RW}}};
struct aid_table AID_TABLE_91 = {85, {{0, RW}}};
struct aid_table AID_TABLE_92 = {86, {{0, RW}}};
struct aid_table AID_TABLE_93 = {87, {{0, RW}}};
struct aid_table AID_TABLE_94 = {90, {{0, RW}}};
struct aid_table AID_TABLE_95 = {91, {{0, RW}}};
struct aid_table AID_TABLE_96 = {96, {{0, RW}}};
struct aid_table AID_TABLE_97 = {97, {{0, RW}}};
struct aid_table AID_TABLE_98 = {98, {{0, RW}}};
struct aid_table AID_TABLE_99 = {99, {{0, RW}}};
struct aid_table AID_TABLE_100 = {101, {{0, RW}}};
struct aid_table AID_TABLE_101 = {104, {{0, RW}, {16, RW}}};
struct aid_table AID_TABLE_102 = {105, {{0, RW}, {16, RW}}};
struct aid_table AID_TABLE_103 = {108, {{0, RW}}};
struct aid_table AID_TABLE_104 = {109, {{0, RW}}};
struct aid_table AID_TABLE_105 = {110, {{0, RW}}};
struct aid_table AID_TABLE_106 = {111, {{0, RW}}};
struct aid_table AID_TABLE_107 = {112, {{0, RW}}};
struct aid_table AID_TABLE_108 = {113, {{0, RW}}};
struct aid_table AID_TABLE_109 = {114, {{0, RW}}};
struct aid_table AID_TABLE_110 = {115, {{0, RW}}};
struct aid_table AID_TABLE_111 = {117, {{0, RW}}};
struct aid_table AID_TABLE_112 = {118, {{0, RW}}};
struct aid_table AID_TABLE_113 = {119, {{0, RW}}};
struct aid_table AID_TABLE_114 = {120, {{0, RW}}};
struct aid_table AID_TABLE_115 = {121, {{0, RW}}};
struct aid_table AID_TABLE_116 = {122, {{0, RW}}};
struct aid_table AID_TABLE_117 = {123, {{0, RW}}};
struct aid_table AID_TABLE_118 = {124, {{0, RW}}};
struct aid_table AID_TABLE_119 = {125, {{0, RW}}};
struct aid_table AID_TABLE_120 = {126, {{0, RW}}};
struct aid_table AID_TABLE_121 = {127, {{0, RW}}};
struct aid_table AID_TABLE_122 = {129, {{0, RW}}};
struct aid_table AID_TABLE_123 = {131, {{0, RW}}};
struct aid_table AID_TABLE_124 = {133, {{0, RW}}};
struct aid_table AID_TABLE_125 = {135, {{0, RW}}};
struct aid_table AID_TABLE_126 = {137, {{0, RW}}};
struct aid_table AID_TABLE_127 = {138, {{0, RW}}};
struct aid_table AID_TABLE_128 = {139, {{0, RW}}};
struct aid_table AID_TABLE_129 = {140, {{0, RW}}};
struct aid_table AID_TABLE_130 = {141, {{0, RW}}};
struct aid_table AID_TABLE_131 = {142, {{0, RW}}};
struct aid_table AID_TABLE_132 = {143, {{0, RW}}};
struct aid_table AID_TABLE_133 = {145, {{0, RW}}};
struct aid_table AID_TABLE_134 = {147, {{0, RW}}};
struct aid_table AID_TABLE_135 = {149, {{0, RW}}};
struct aid_table AID_TABLE_136 = {151, {{0, RW}}};
struct aid_table AID_TABLE_137 = {153, {{0, RW}}};
struct aid_table AID_TABLE_138 = {154, {{0, RW}}};
struct aid_table AID_TABLE_139 = {155, {{0, RW}}};
struct aid_table AID_TABLE_140 = {156, {{0, RW}}};
struct aid_table AID_TABLE_141 = {157, {{0, RW}}};
struct aid_table AID_TABLE_142 = {158, {{0, RW}}};
struct aid_table AID_TABLE_143 = {159, {{0, RW}}};
struct aid_table AID_TABLE_144 = {161, {{0, RW}}};
struct aid_table AID_TABLE_145 = {163, {{0, RW}}};
struct aid_table AID_TABLE_146 = {165, {{0, RW}}};
struct aid_table AID_TABLE_147 = {167, {{0, RW}}};
struct aid_table AID_TABLE_148 = {168, {{0, RW}}};
struct aid_table AID_TABLE_149 = {169, {{0, RW}}};
struct aid_table AID_TABLE_150 = {170, {{0, RW}}};
struct aid_table AID_TABLE_151 = {171, {{0, RW}}};
struct aid_table AID_TABLE_152 = {172, {{0, RW}}};
struct aid_table AID_TABLE_153 = {173, {{0, RW}}};
struct aid_table AID_TABLE_154 = {174, {{0, RW}}};
struct aid_table AID_TABLE_155 = {175, {{0, RW}}};
struct aid_table AID_TABLE_156 = {180, {{0, RW}}};
struct aid_table AID_TABLE_157 = {181, {{0, RW}}};
struct aid_table AID_TABLE_158 = {182, {{0, RW}}};
struct aid_table AID_TABLE_159 = {183, {{0, RW}}};
struct aid_table AID_TABLE_160 = {186, {{0, RW}}};
struct aid_table AID_TABLE_161 = {187, {{0, RW}}};
struct aid_table AID_TABLE_162 = {188, {{0, RW}}};
struct aid_table AID_TABLE_163 = {189, {{0, RW}}};
struct aid_table AID_TABLE_164 = {190, {{0, RW}}};
struct aid_table AID_TABLE_165 = {191, {{0, RW}}};
struct aid_table AID_TABLE_166 = {192, {{0, RW}}};
struct aid_table AID_TABLE_167 = {193, {{0, RW}}};
struct aid_table AID_TABLE_168 = {194, {{0, RW}}};
struct aid_table AID_TABLE_169 = {195, {{0, RW}}};
struct aid_table AID_TABLE_170 = {196, {{0, RW}}};
struct aid_table AID_TABLE_171 = {197, {{0, RW}}};
struct aid_table AID_TABLE_172 = {198, {{0, RW}}};
struct aid_table AID_TABLE_173 = {199, {{0, RW}}};
struct aid_table AID_TABLE_174 = {200, {{0, RW}}};
struct aid_table AID_TABLE_175 = {201, {{0, RW}}};
struct aid_table AID_TABLE_176 = {202, {{0, RW}}};
struct aid_table AID_TABLE_177 = {203, {{0, RW}}};
struct aid_table AID_TABLE_178 = {204, {{0, RW}}};
struct aid_table AID_TABLE_179 = {205, {{0, RW}}};
struct aid_table AID_TABLE_180 = {207, {{0, RW}}};
struct aid_table AID_TABLE_181 = {72, {{0, RW},{1, RW},{2, RW}}};
struct aid_table AID_TABLE_182 = {209, {{0, RW}}};
struct aid_table AID_TABLE_183 = {211, {{0, RW}}};
struct aid_table AID_TABLE_184 = {212, {{0, RW}}};
struct aid_table AID_TABLE_185 = {213, {{0, RW}}};
struct aid_table AID_TABLE_186 = {214, {{0, RW}}};
struct aid_table AID_TABLE_187 = {215, {{0, RW}}};
struct aid_table AID_TABLE_188 = {218, {{0, RW}}};
struct aid_table AID_TABLE_189 = {219, {{0, RW}}};
struct aid_table AID_TABLE_190 = {220, {{0, RW}}};
struct aid_table AID_TABLE_191 = {221, {{0, RW}}};
struct aid_table AID_TABLE_192 = {222, {{0, RW}}};
struct aid_table AID_TABLE_193 = {223, {{0, RW}}};
struct aid_table AID_TABLE_194 = {232, {{0, RW}}};
struct aid_table AID_TABLE_195 = {233, {{0, RW}}};
struct aid_table AID_TABLE_196 = {234, {{0, RW}}};
struct aid_table AID_TABLE_197 = {235, {{0, RW}}};
struct aid_table AID_TABLE_198 = {237, {{0, RW}}};
struct aid_table AID_TABLE_199 = {238, {{0, RW}}};
struct aid_table AID_TABLE_200 = {239, {{0, RW}}};
struct aid_table AID_TABLE_201 = {248, {{0, RW}}};
struct aid_table AID_TABLE_202 = {249, {{0, RW}}};
struct aid_table AID_TABLE_203 = {250, {{0, RW}}};
struct aid_table AID_TABLE_204 = {251, {{0, RW}}};


//for ddr-reserve mode
struct aid_table AID_TABLE_61 = {240, {{0, RW},{1, RW},{2, RW},{4, READ},{6, RW},{8, RW},{9, RW},{11, RW}, 
				{13, RW},{14, RW},{15, RW},{17, RW},{16, RW},{18, RW},{19, RW},{20, RW},
				{21, RW},{22, RW},{23, RW},{24, RW},{25, RW},{26, RW},{27, RW},
				{28, READ},{29, RW},{30, RW},{39, READ},{41, READ},{43, RW},{44, RW},
				{46, RW},{47, RW}}};
struct aid_table AID_TABLE_62 = {241, {{0, RW},{5, RW}, {16 ,READ}, {17, RW},{18, RW},{19, RW}, {26, RW},{27, RW},{28, READ},{29, RW},
				{31, RW},{39, READ},{41, READ},{42, RW},{43, RW},{44, RW},{46, RW},{47, RW}}};
struct aid_table AID_TABLE_205 = {252, {{0, RW}}};

struct aid_table *AID_TABLE[] = {&(AID_TABLE_0), &(AID_TABLE_1), &(AID_TABLE_2), &(AID_TABLE_3), &(AID_TABLE_4),
				 &(AID_TABLE_5), &(AID_TABLE_6), &(AID_TABLE_7), &(AID_TABLE_8), &(AID_TABLE_9),
				 &(AID_TABLE_10), &(AID_TABLE_11), &(AID_TABLE_12), &(AID_TABLE_13), &(AID_TABLE_14),
				 &(AID_TABLE_15), &(AID_TABLE_16), &(AID_TABLE_17), &(AID_TABLE_18), &(AID_TABLE_19),
				 &(AID_TABLE_20), &(AID_TABLE_21), &(AID_TABLE_22), &(AID_TABLE_23), &(AID_TABLE_24),
				 &(AID_TABLE_25), &(AID_TABLE_26), &(AID_TABLE_27), &(AID_TABLE_28), &(AID_TABLE_29),
				 &(AID_TABLE_30), &(AID_TABLE_31), &(AID_TABLE_32), &(AID_TABLE_33), &(AID_TABLE_34),
				 &(AID_TABLE_35), &(AID_TABLE_36), &(AID_TABLE_37), &(AID_TABLE_38), &(AID_TABLE_39),
				 &(AID_TABLE_40), &(AID_TABLE_41), &(AID_TABLE_42), &(AID_TABLE_43), &(AID_TABLE_44),
				 &(AID_TABLE_45), &(AID_TABLE_46), &(AID_TABLE_47), &(AID_TABLE_48), &(AID_TABLE_49),
				 &(AID_TABLE_50), &(AID_TABLE_51), &(AID_TABLE_52), &(AID_TABLE_53), &(AID_TABLE_54),
				 &(AID_TABLE_55), &(AID_TABLE_56), &(AID_TABLE_57), &(AID_TABLE_58), &(AID_TABLE_59),
				 &(AID_TABLE_60), &(AID_TABLE_63), &(AID_TABLE_64), &(AID_TABLE_65), &(AID_TABLE_66),
				 &(AID_TABLE_67), &(AID_TABLE_68), &(AID_TABLE_69), &(AID_TABLE_70), &(AID_TABLE_71),
				 &(AID_TABLE_72), &(AID_TABLE_73), &(AID_TABLE_74), &(AID_TABLE_75), &(AID_TABLE_76),
				 &(AID_TABLE_77), &(AID_TABLE_78), &(AID_TABLE_79), &(AID_TABLE_80), &(AID_TABLE_81),
				 &(AID_TABLE_82), &(AID_TABLE_83), &(AID_TABLE_84), &(AID_TABLE_85), &(AID_TABLE_86),
				 &(AID_TABLE_87), &(AID_TABLE_88), &(AID_TABLE_89), &(AID_TABLE_90), &(AID_TABLE_91),
				 &(AID_TABLE_92), &(AID_TABLE_93), &(AID_TABLE_94), &(AID_TABLE_95), &(AID_TABLE_96),
				 &(AID_TABLE_97), &(AID_TABLE_98), &(AID_TABLE_99), &(AID_TABLE_100), &(AID_TABLE_101),
				 &(AID_TABLE_102), &(AID_TABLE_103), &(AID_TABLE_104), &(AID_TABLE_105), &(AID_TABLE_106),
				 &(AID_TABLE_107), &(AID_TABLE_108), &(AID_TABLE_109), &(AID_TABLE_110), &(AID_TABLE_111),
				 &(AID_TABLE_112), &(AID_TABLE_113), &(AID_TABLE_114), &(AID_TABLE_115), &(AID_TABLE_116),
				 &(AID_TABLE_117), &(AID_TABLE_118), &(AID_TABLE_119), &(AID_TABLE_120), &(AID_TABLE_121),
				 &(AID_TABLE_122), &(AID_TABLE_123), &(AID_TABLE_124), &(AID_TABLE_125), &(AID_TABLE_126),
				 &(AID_TABLE_127), &(AID_TABLE_128), &(AID_TABLE_129), &(AID_TABLE_130), &(AID_TABLE_131),
				 &(AID_TABLE_132), &(AID_TABLE_133), &(AID_TABLE_134), &(AID_TABLE_135), &(AID_TABLE_136),
				 &(AID_TABLE_137), &(AID_TABLE_138), &(AID_TABLE_139), &(AID_TABLE_140), &(AID_TABLE_141),
				 &(AID_TABLE_142), &(AID_TABLE_143), &(AID_TABLE_144), &(AID_TABLE_145), &(AID_TABLE_146),
				 &(AID_TABLE_147), &(AID_TABLE_148), &(AID_TABLE_149), &(AID_TABLE_150), &(AID_TABLE_151),
				 &(AID_TABLE_152), &(AID_TABLE_153), &(AID_TABLE_154), &(AID_TABLE_155), &(AID_TABLE_156),
				 &(AID_TABLE_157), &(AID_TABLE_158), &(AID_TABLE_159), &(AID_TABLE_160), &(AID_TABLE_161),
				 &(AID_TABLE_162), &(AID_TABLE_163), &(AID_TABLE_164), &(AID_TABLE_165), &(AID_TABLE_166),
				 &(AID_TABLE_167), &(AID_TABLE_168), &(AID_TABLE_169), &(AID_TABLE_170), &(AID_TABLE_171),
				 &(AID_TABLE_172), &(AID_TABLE_173), &(AID_TABLE_174), &(AID_TABLE_175), &(AID_TABLE_176),
				 &(AID_TABLE_177), &(AID_TABLE_178), &(AID_TABLE_179), &(AID_TABLE_180), &(AID_TABLE_181),
				 &(AID_TABLE_182), &(AID_TABLE_183), &(AID_TABLE_184), &(AID_TABLE_185), &(AID_TABLE_186),
				 &(AID_TABLE_187), &(AID_TABLE_188), &(AID_TABLE_189), &(AID_TABLE_190), &(AID_TABLE_191),
				 &(AID_TABLE_192), &(AID_TABLE_193), &(AID_TABLE_194), &(AID_TABLE_195), &(AID_TABLE_196),
				 &(AID_TABLE_197), &(AID_TABLE_198), &(AID_TABLE_199), &(AID_TABLE_200), &(AID_TABLE_201),
				 &(AID_TABLE_202), &(AID_TABLE_203), &(AID_TABLE_204)};

struct aid_table *AID_TABLE_DRM[] = {&(AID_TABLE_61), &(AID_TABLE_62), &(AID_TABLE_205)};

