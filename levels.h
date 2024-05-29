#ifndef LEVELS_H
#define LEVELS_H

#include "physical_engine.h"

// l'ensemble des textures à charger pour le jeu (n'incluant pas les menus)
const char * const TEXTURE_FILES[] = {"assets/maps/mapferme.bmp","assets/maps/mapneon.bmp",
									"assets/sprites/Delorean-3Pos.bmp",
									"assets/maps/minimapferme.bmp","assets/maps/minimapneon.bmp",
									"assets/skyboxes/skybox_moomoo.bmp","assets/skyboxes/skybox_neon.bmp",
									"assets/sprites/panneaux.bmp","assets/sprites/zoneinterdite.bmp"};

// l'ensemble des sons à charger pour le jeu (musique inclus)
const char * const SON_FILES[] = {"assets/sounds/musique.wav","assets/sounds/moteur.wav"};

// tous les niveaux, hard codés mdr

// FERME
// voitures
static VECTEUR3D pos_ini_voit_ferme[] = {{2000.-210.,2510.,1.}};
static float angles_ini_voit_ferme[] = {0.};
static float rayons_ini_voit_ferme[] = {10.};
static float echelles_ini_voit_ferme[] = {40.};
static unsigned short int textures_voit_ferme[] = {2}; // delorean.bmp
// sols
static VECTEUR2D pos_ini_sol_ferme[] = {{2000.,2000.}};
static float angles_ini_sol_ferme[] = {0.};
static float echelles_ini_sol_ferme[] = {1.};
static unsigned short int textures_sol_ferme[] = {0};
// décors (y en a pas, pour l'instant)
// segments
static float tx1_ferme[] = {1885, 1928, 1971, 2049, 2058, 2056, 2045, 2044, 2060, 2078, 2122, 2173, 2209, 2275, 2296, 2382, 2421, 2441, 2484, 2546, 2603, 2645, 2683, 2726, 2733, 2729, 2730, 2735, 2724, 2738, 2743, 2724, 2725, 2679, 2663, 2605, 2588, 2506, 2439, 2371, 2344, 2318, 2296, 2284, 2282, 2264, 2272, 2292, 2298, 2299, 2318, 2350, 2354, 2375, 2455, 2532, 2576, 2585, 2633, 2654, 2639, 2622, 2611, 2534, 2470, 2416, 2321, 2231, 2172, 2105, 2043, 1995, 1914, 1902, 1902, 1904, 1925, 1926, 1892, 1843, 1708, 1653, 1601, 1588, 1522, 1469, 1432, 1438, 1478, 1527, 1542, 1577, 1627, 1699, 1713, 1716, 1722, 1726, 1695, 1660, 1621, 1587, 1578, 1535, 1484, 1418, 1379, 1364, 1339, 1322, 1316, 1312, 1305, 1288, 1281, 1278, 1282, 1279, 1292, 1293, 1324, 1358, 1384, 1405, 1414, 1464, 1529, 1581, 1685, 1701, 1800, 1862, 1884, 1885, 1882, 1877};
static float ty1_ferme[] = {792, 805, 811, 805, 796, 769, 706, 685, 638, 625, 612, 600, 599, 589, 584, 592, 591, 596, 601, 639, 671, 709, 764, 848, 886, 908, 954, 964, 994, 1042, 1153, 1265, 1305, 1397, 1447, 1528, 1573, 1671, 1738, 1806, 1841, 1958, 2024, 2100, 2209, 2270, 2434, 2518, 2536, 2626, 2670, 2732, 2749, 2792, 2889, 2993, 3056, 3080, 3144, 3214, 3314, 3371, 3387, 3422, 3447, 3451, 3400, 3349, 3293, 3226, 3195, 3195, 3214, 3260, 3340, 3388, 3454, 3489, 3534, 3548, 3546, 3528, 3478, 3455, 3395, 3343, 3291, 3232, 3156, 3081, 3038, 2992, 2945, 2868, 2854, 2767, 2672, 2603, 2506, 2382, 2272, 2205, 2139, 2039, 1935, 1814, 1702, 1635, 1492, 1427, 1412, 1351, 1306, 1260, 1226, 1165, 1119, 1056, 997, 954, 883, 820, 762, 705, 688, 645, 613, 596, 582, 590, 588, 591, 602, 659, 744, 785};
static float tx2_ferme[] = {1905, 1868, 1844, 1829, 1824, 1832, 1831, 1844, 1856, 1869, 1883, 1896, 1999, 2029, 2057, 2073, 2085, 2071, 2044, 2019, 2000, 1985, 1964, 1924};
static float ty2_ferme[] = {1148, 1151, 1141, 1121, 1100, 1035, 1015, 951, 869, 860, 869, 877, 881, 875, 867, 877, 949, 993, 1031, 1049, 1081, 1119, 1132, 1145};
static float tx3_ferme[] = {1903, 1809, 1764, 1725, 1704, 1697, 1688, 1696, 1700, 1709, 1732, 1748, 1743, 1715, 1688, 1671, 1601, 1563, 1531, 1497, 1448, 1423, 1416, 1424, 1424, 1451, 1465, 1465, 1501, 1503, 1525, 1553, 1583, 1607, 1612, 1651, 1675, 1680, 1696, 1721, 1721, 1752, 1779, 1800, 1835, 1859, 1860, 1857, 1855, 1844, 1787, 1768, 1713, 1687, 1629, 1592, 1573, 1579, 1584, 1640, 1716, 1739, 1764, 1771, 1763, 1755, 1760, 1795, 1824, 1891, 1949, 1984, 2019, 2099, 2197, 2219, 2228, 2257, 2280, 2357, 2417, 2460, 2507, 2520, 2520, 2484, 2436, 2372, 2269, 2225, 2203, 2188, 2172, 2153, 2149, 2133, 2123, 2121, 2121, 2145, 2165, 2173, 2180, 2192, 2217, 2225, 2217, 2199, 2193, 2193, 2228, 2239, 2295, 2345, 2381, 2439, 2484, 2572, 2597, 2589, 2544, 2515, 2429, 2329, 2285, 2227, 2201, 2177, 2176, 2197, 2207, 2215, 2201, 2151, 2081, 2048, 2005};
static float ty3_ferme[] = {1281, 1284, 1269, 1227, 1177, 1127, 1085, 1005, 937, 876, 819, 760, 732, 716, 724, 729, 735, 745, 779, 828, 971, 1063, 1148, 1235, 1256, 1349, 1388, 1415, 1548, 1593, 1660, 1725, 1803, 1855, 1879, 1959, 2008, 2033, 2073, 2101, 2127, 2224, 2276, 2355, 2453, 2528, 2664, 2789, 2851, 2881, 2943, 2968, 3013, 3047, 3136, 3189, 3257, 3288, 3307, 3349, 3437, 3447, 3437, 3415, 3355, 3284, 3225, 3163, 3119, 3089, 3061, 3059, 3073, 3117, 3187, 3207, 3225, 3244, 3245, 3272, 3305, 3312, 3289, 3272, 3248, 3179, 3105, 3011, 2883, 2808, 2739, 2701, 2620,2553, 2467, 2375, 2296, 2207, 2163, 2091, 2007, 1937, 1863, 1817, 1768, 1715, 1665, 1628, 1481, 1455, 1381, 1345, 1284, 1187, 1151, 1132, 1069, 1023, 935, 888, 815, 773, 728, 713, 725, 707, 705, 725, 753, 792, 915, 999, 1032, 1092, 1195, 1243, 1275};
static float tx4_ferme[] = {2358, 2342, 2339, 2348, 2347, 2345, 2345, 2358, 2381, 2397, 2413, 2432, 2438, 2451, 2454, 2469, 2496, 2504, 2508, 2521, 2546, 2557, 2560, 2583, 2601, 2619, 2621, 2602, 2590, 2578, 2551, 2523, 2481, 2467, 2462, 2438, 2426, 2399, 2393, 2394, 2383, 2373};
static float ty4_ferme[] = {1653, 1640, 1603, 1584, 1511, 1495, 1483, 1459, 1421, 1395, 1377, 1339, 1321, 1307, 1289, 1269, 1255, 1249, 1237, 1232, 1203, 1175, 1149, 1128, 1113, 1112, 1132, 1156, 1173, 1243, 1319, 1390, 1453, 1466, 1483, 1517, 1525, 1564, 1576, 1593, 1627, 1649};
static unsigned int t_tab_ferme[] = {sizeof(tx1_ferme)/sizeof(float),sizeof(tx2_ferme)/sizeof(float),sizeof(tx3_ferme)/sizeof(float),sizeof(tx4_ferme)/sizeof(float)};
static float* tx_ferme[] = {tx1_ferme,tx2_ferme,tx3_ferme,tx4_ferme};
static float* ty_ferme[] = {ty1_ferme,ty2_ferme,ty3_ferme,ty4_ferme};
static short int sens_ferme[] = {-1,1,-1,1};
// checkpoints
static CHECKPOINT checkpoints_ferme[1] = {0};
NIVEAU lvl_ferme = {
	.nb_voitures = sizeof(angles_ini_voit_ferme)/sizeof(float),
	.positions_initiales_voit = pos_ini_voit_ferme,
	.angles_initiaux_voit = angles_ini_voit_ferme,
	.rayons_voit = rayons_ini_voit_ferme,
	.echelles_voit = echelles_ini_voit_ferme,
	.texture_ids_voit = textures_voit_ferme,

	.nb_sols = sizeof(angles_ini_sol_ferme)/sizeof(float),
	.positions_initiales_sols = pos_ini_sol_ferme,
	.angles_initiaux_sols = angles_ini_sol_ferme,
	.echelles_sols = echelles_ini_sol_ferme,
	.texture_ids_sols = textures_sol_ferme,

	.nb_decors = 0,

	.nb_tableaux = sizeof(t_tab_ferme)/sizeof(unsigned int),
	.tailles_tableaux = t_tab_ferme,
	.tableaux_x = tx_ferme,
	.tableaux_y = ty_ferme,
	.sens = sens_ferme,

	.nb_checkpoints = sizeof(checkpoints_ferme)/sizeof(CHECKPOINT),
	.tableau_checkpoints = checkpoints_ferme,

	.nb_lignes = 40,
	.nb_colonnes = 40,
	.l = 4000.,
	.h = 4000.,

	.minimap = 3,

	.texture_id_skybox = 5,
	.skybox_rotation = 0.
};

// NEON_CITY
// voitures
static VECTEUR3D pos_ini_voit_neon[] = {{1694.+2500.,186.+2500.,1.}};
static float angles_ini_voit_neon[] = {0.};
static float rayons_ini_voit_neon[] = {10.};
static float echelles_ini_voit_neon[] = {70.};
static unsigned short int textures_voit_neon[] = {2}; // delorean.bmp
// sols
static VECTEUR2D pos_ini_sol_neon[] = {{2500.,2500.}};
static float angles_ini_sol_neon[] = {0.};
static float echelles_ini_sol_neon[] = {1.};
static unsigned short int textures_sol_neon[] = {1};
// décors
static const unsigned int nb_decors = 14;
static float echelles_dec[] = {30.,30.,25,25,25,25,25,25,25,25,25,25,25,25};
static VECTEUR3D positions_dec[] = {{2184,668,20},{3270,828,20}, {1682, 2449, 20}, {1882,2449, 20}, {1682, 2352, 20}, {1792,3447,20},{2452,2542,20},{2352,2442,20},{3232,2342,20},{2452,3442,20},{2452,3248,20},{3232,3452,20},{3342,3352,20},{3342,3442,20}};
static unsigned short int texture_ids_dec[] = {7,7,8,8,8,8,8,8,8,8,8,8,8,8};
static unsigned short int nbs_colonnes[] = {6,6,1,1,1,1,1,1,1,1,1,1,1,1};
static unsigned short int nbs_lignes[] = {2,2,1,1,1,1,1,1,1,1,1,1,1,1};
static long long int durees_frame[] = {300,300,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000};
static unsigned short int tableau_durees_animations[] = {6,6,1,1,1,1,1,1,1,1,1,1,1,1};
static unsigned short int t1[] = {0,1,2,3,4,5};
static unsigned short int t2[] = {6,7,8,9,10,11};
static unsigned short int t3[] = {0};
static unsigned short int* tableau_animations[] = {t2,t2,t3,t3,t3,t3,t3,t3,t3,t3,t3,t3,t3,t3};
// segments
static float tx1_neon[] = {2206, 2190, 1563, 1327, 1194, 970, 874, 823, 742, 702, 647, 643, 576, 575, 617, 696, 824, 856, 922, 1035, 1180, 1298, 1415, 1583, 1674, 1955, 2154, 2438, 2804, 3159, 3772, 4071, 4164, 4229, 4345, 4393, 4430, 4428, 4380, 4377, 4494, 4515, 4496, 4417, 4307, 3839, 3400, 3074, 2748, 3075, 3374, 3171, 2981, 2462, 2371, 2369, 2371, 2546, 2546, 3144, 3144, 2548, 2548, 2373, 2370, 1870, 1873, 1908, 1910, 1868, 1868, 1870, 1967, 2102, 2163, 2228, 2240, 2257, 2258, 2245, 2210};
static float ty1_neon[] = {829, 505, 502, 526, 552, 633, 690, 734, 831, 912, 1366, 1529, 2135, 2493, 2992, 3402, 3824, 3940, 4059, 4161, 4262, 4307, 4338, 4362, 4390, 4424, 4443, 4448, 4434, 4406, 4321, 4233, 4176, 4086, 3858, 3716, 3462, 3118, 2900, 2636, 1979, 1714, 1453, 1300, 1142, 823, 658, 552, 505, 552, 650, 982, 1093, 1606, 1773, 1856, 2566, 2566, 2534, 2532, 3257, 3257, 3232, 3232, 3258, 3260, 2535, 2530, 2360, 2360, 1839, 1821, 1642, 1426, 1322, 1183, 1154, 1069, 996, 913, 853};
static float tx2_neon[] = {1694, 1695, 1763, 1821, 1880, 1956, 1947, 1914, 1810, 1542, 1265, 1096, 1039, 999, 959, 985, 992, 1039, 1072, 1136, 1187, 1243, 1488, 2284, 2605, 3286, 3795, 3941, 4054, 3997, 3992, 4032, 4121, 4139, 4165, 4141, 4010, 3664, 3571, 3305, 3170, 2552, 2545, 2547, 3137, 3139, 3314, 3314, 3343, 3341, 3316, 3314, 3348, 3348, 3316, 3316, 3142, 3141, 2547, 2547, 2372, 2372, 1869, 1867, 1697, 1697, 1697, 1664, 1661, 1690, 1694, 1648, 1650, 1694};
static float ty2_neon[] = {1827, 1748, 1528, 1402, 1303, 1119, 1026, 954, 892, 859, 906, 1046, 1170, 1551, 2169, 2417, 2652, 3117, 3398, 3678, 3788, 3835, 3997, 4101, 4123, 4039, 3868, 3722, 3330, 2913, 2528, 2224, 1943, 1888, 1757, 1604, 1382, 1156, 1143, 1227, 1296, 1821, 1837, 2357, 2355, 2328,2331, 2353, 2353, 2532, 2532, 3261, 3261, 3438, 3438, 3478, 3478, 3443, 3436, 3469, 3467, 3441, 3436, 3465, 3465, 3445, 3434, 3436, 3261, 3261, 2534, 2528, 2357, 2355};
static unsigned int t_tab_neon[] = {sizeof(tx1_neon)/sizeof(float),sizeof(tx2_neon)/sizeof(float)};
static float* tx_neon[] = {tx1_neon,tx2_neon};
static float* ty_neon[] = {ty1_neon,ty2_neon};
static short int sens_neon[] = {1,-1};
// checkpoints
static CHECKPOINT checkpoints_neon[1] = {0};
NIVEAU lvl_neon_city = {
	.nb_voitures = sizeof(angles_ini_voit_neon)/sizeof(float),
	.positions_initiales_voit = pos_ini_voit_neon,
	.angles_initiaux_voit = angles_ini_voit_neon,
	.rayons_voit = rayons_ini_voit_neon,
	.echelles_voit = echelles_ini_voit_neon,
	.texture_ids_voit = textures_voit_neon,

	.nb_sols = sizeof(angles_ini_sol_neon)/sizeof(float),
	.positions_initiales_sols = pos_ini_sol_neon,
	.angles_initiaux_sols = angles_ini_sol_neon,
	.echelles_sols = echelles_ini_sol_neon,
	.texture_ids_sols = textures_sol_neon,

	.nb_decors = nb_decors,
	.echelles_dec = echelles_dec,
	.positions_dec = positions_dec,
	.texture_ids_dec = texture_ids_dec,
	.nbs_colonnes = nbs_colonnes,
	.nbs_lignes = nbs_lignes,
	.durees_frame = durees_frame,
	.tableau_durees_animations = tableau_durees_animations,
	.tableau_animations = tableau_animations,

	.nb_tableaux = sizeof(t_tab_neon)/sizeof(unsigned int),
	.tailles_tableaux = t_tab_neon,
	.tableaux_x = tx_neon,
	.tableaux_y = ty_neon,
	.sens = sens_neon,

	.nb_checkpoints = sizeof(checkpoints_neon)/sizeof(CHECKPOINT),
	.tableau_checkpoints = checkpoints_neon,

	.nb_lignes = 50,
	.nb_colonnes = 50,
	.l = 5000.,
	.h = 5000.,

	.minimap = 4,

	.texture_id_skybox = 6,
	.skybox_rotation = 0.
};

#endif