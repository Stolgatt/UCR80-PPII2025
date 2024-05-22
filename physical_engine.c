#include "physical_engine.h"
#include <stdio.h>
#include <stdlib.h>
#include "engine_common.h"

SEGMENT2D CREA_SEGMENT_2D(int x1, int x2, int y1, int y2) {
	SEGMENT2D res;
	VECTEUR2D dir;
	float norme = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
	dir.x = (x2 - x1)/norme;
	dir.y = (y2 - y1)/norme;
	res.direction = dir;
	res.longueur = norme;
	return res;
}

VECTEUR2D Calcul_reflexion(VECTEUR2D direction_voulue, SEGMENT2D* segment) {
    VECTEUR2D vecteur_normal;
    vecteur_normal.x = -segment->direction.y;
    vecteur_normal.y = segment->direction.x;
    float produit_scalaire = direction_voulue.x*vecteur_normal.x + direction_voulue.y*vecteur_normal.y;
    VECTEUR2D reflexion;
    reflexion.x = direction_voulue.x - 2*produit_scalaire*vecteur_normal.x;
    reflexion.y = direction_voulue.y - 2*produit_scalaire*vecteur_normal.y;
    return reflexion;
}
    
VECTEUR2D Calcul_collisions(VECTEUR2D direction_voulue,const float* rayon, const VECTEUR2D* position_disque) {
    	//partie définition des limites de la map
			//liste des points
	int x_lim_exte[]={1885, 1928, 1971, 2049, 2058, 2056, 2045, 2044, 2060, 2078, 2122, 2173, 2209, 2275, 2296, 2382, 2421, 2441, 2484, 2546, 2603, 2645, 2683, 2726, 2733, 2729, 2730, 2735, 2724, 2738, 2743, 2724, 2725, 2679, 2663, 2605, 2588, 2506, 2439, 2371, 2344, 2318, 2296, 2284, 2282, 2264, 2272, 2292, 2298, 2299, 2318, 2350, 2354, 2375, 2455, 2532, 2576, 2585, 2633, 2654, 2639, 2622, 2611, 2534, 2470, 2416, 2321, 2231, 2172, 2105, 2043, 1995, 1914, 1902, 1902, 1904, 1925, 1926, 1892, 1843, 1708, 1653, 1601, 1588, 1522, 1469, 1432, 1438, 1478, 1527, 1542, 1577, 1627, 1699, 1713, 1716, 1722, 1726, 1695, 1660, 1621, 1587, 1578, 1535, 1484, 1418, 1379, 1364, 1339, 1322, 1316, 1312, 1305, 1288, 1281, 1278, 1282, 1279, 1292, 1293, 1324, 1358, 1384, 1405, 1414, 1464, 1529, 1581, 1685, 1701, 1800, 1862, 1884, 1885, 1882, 1877};
	int y_lim_exte[]={3208, 3195, 3189, 3195, 3204, 3231, 3294, 3315, 3362, 3375, 3388, 3400, 3401, 3411, 3416, 3408, 3409, 3404, 3399, 3361, 3329, 3291, 3236, 3152, 3114, 3092, 3046, 3036, 3006, 2958, 2847, 2735, 2695, 2603, 2553, 2472, 2427, 2329, 2262, 2194, 2159, 2042, 1976, 1900, 1791, 1730, 1566, 1482, 1464, 1374, 1330, 1268, 1251, 1208, 1111, 1007, 944, 920, 856, 786, 686, 629, 613, 578, 553, 549, 600, 651, 707, 774, 805, 805, 786, 740, 660, 612, 546, 511, 466, 452, 454, 472, 522, 545, 605, 657, 709, 768, 844, 919, 962, 1008, 1055, 1132, 1146, 1233, 1328, 1397, 1494, 1618, 1728, 1795, 1861, 1961, 2065, 2186, 2298, 2365, 2508, 2573, 2588, 2649, 2694, 2740, 2774, 2835, 2881, 2944, 3003, 3046, 3117, 3180, 3238, 3295, 3312, 3355, 3387, 3404, 3418, 3410, 3412, 3409, 3398, 3341, 3256, 3215};
	int exte_size =sizeof(x_lim_exte)/sizeof(x_lim_exte[0]);
	int x_lim_ilot[]={1905, 1868, 1844, 1829, 1824, 1832, 1831, 1844, 1856, 1869, 1883, 1896, 1999, 2029, 2057, 2073, 2085, 2071, 2044, 2019, 2000, 1985, 1964, 1924};
	int y_lim_ilot[]={2852, 2849, 2859, 2879, 2900, 2965, 2985, 3049, 3131, 3140, 3131, 3123, 3119, 3125, 3133, 3123, 3051, 3007, 2969, 2951, 2919, 2881, 2868, 2855};
	int ilot_size = sizeof(x_lim_ilot)/sizeof(x_lim_ilot[0]);
	int x_lim_inte[]={1903, 1809, 1764, 1725, 1704, 1697, 1688, 1696, 1700, 1709, 1732, 1748, 1743, 1715, 1688, 1671, 1601, 1563, 1531, 1497, 1448, 1423, 1416, 1424, 1424, 1451, 1465, 1465, 1501, 1503, 1525, 1553, 1583, 1607, 1612, 1651, 1675, 1680, 1696, 1721, 1721, 1752, 1779, 1800, 1835, 1859, 1860, 1857, 1855, 1844, 1787, 1768, 1713, 1687, 1629, 1592, 1573, 1579, 1584, 1640, 1716, 1739, 1764, 1771, 1763, 1755, 1760, 1795, 1824, 1891, 1949, 1984, 2019, 2099, 2197, 2219, 2228, 2257, 2280, 2357, 2417, 2460, 2507, 2520, 2520, 2484, 2436, 2372, 2269, 2225, 2203, 2188, 2172, 2153, 2149, 2133, 2123, 2121, 2121, 2145, 2165, 2173, 2180, 2192, 2217, 2225, 2217, 2199, 2193, 2193, 2228, 2239, 2295, 2345, 2381, 2439, 2484, 2572, 2597, 2589, 2544, 2515, 2429, 2329, 2285, 2227, 2201, 2177, 2176, 2197, 2207, 2215, 2201, 2151, 2081, 2048, 2005};
	int y_lim_inte[]={2719, 2716, 2731, 2773, 2823, 2873, 2915, 2995, 3063, 3124, 3181, 3240, 3268, 3284, 3276, 3271, 3265, 3255, 3221, 3172, 3029, 2937, 2852, 2765, 2744, 2651, 2612, 2585, 2452, 2407, 2340, 2275, 2197, 2145, 2121, 2041, 1992, 1967, 1927, 1899, 1873, 1776, 1724, 1645, 1547, 1472, 1336, 1211, 1149, 1119, 1057, 1032, 987, 953, 864, 811, 743, 712, 693, 651, 563, 553, 563, 585, 645, 716, 775, 837, 881, 911, 939, 941, 927, 883, 813, 793, 775, 756, 755, 728, 695, 688, 711, 728, 752, 821, 895, 989, 1117, 1192, 1261, 1299, 1380, 1447, 1533, 1625, 1704, 1793, 1837, 1909, 1993, 2063, 2137, 2183, 2232, 2285, 2335, 2372, 2519, 2545, 2619, 2655, 2716, 2813, 2849, 2868, 2931, 2977, 3065, 3112, 3185, 3227, 3272, 3287, 3275, 3293, 3295, 3275, 3247, 3208, 3085, 3001, 2968, 2908, 2805, 2757, 2725};
	int inte_size = sizeof(x_lim_inte)/sizeof(x_lim_inte[0]);
	int x_lim_lac[]={2358, 2342, 2339, 2348, 2347, 2345, 2345, 2358, 2381, 2397, 2413, 2432, 2438, 2451, 2454, 2469, 2496, 2504, 2508, 2521, 2546, 2557, 2560, 2583, 2601, 2619, 2621, 2602, 2590, 2578, 2551, 2523, 2481, 2467, 2462, 2438, 2426, 2399, 2393, 2394, 2383, 2373};
	int y_lim_lac[]={2347, 2360, 2397, 2416, 2489, 2505, 2517, 2541, 2579, 2605, 2623, 2661, 2679, 2693, 2711, 2731, 2745, 2751, 2763, 2768, 2797, 2825, 2851, 2872, 2887, 2888, 2868, 2844, 2827, 2757, 2681, 2610, 2547, 2534, 2517, 2483, 2475, 2436, 2424, 2407, 2373, 2351};
	int lac_size = sizeof(x_lim_lac)/sizeof(x_lim_lac[0]);
			//création de la liste de segment (list_segment) et de la liste des vecteurs origine associés (list_origine)
	SEGMENT2D list_segment[exte_size+ilot_size+inte_size+lac_size];
	VECTEUR2D list_origine[exte_size+ilot_size+inte_size+lac_size];
	list_segment[0]=CREA_SEGMENT_2D(x_lim_exte[exte_size-1],x_lim_exte[0],y_lim_exte[exte_size-1],y_lim_exte[0]);
	list_origine[0].x = x_lim_exte[exte_size-1];
	list_origine[0].y = y_lim_exte[exte_size-1];
				//pour l'extérieur
	for (int i=1 ; i<exte_size-1 ; i++) {
		list_segment[i]=CREA_SEGMENT_2D(x_lim_exte[i],x_lim_exte[i+1],y_lim_exte[i],y_lim_exte[i+1]);
		list_origine[i].x = x_lim_exte[i];
		list_origine[i].y = y_lim_exte[i];
	}
				//pour l'ilot de terre
	list_segment[exte_size]=CREA_SEGMENT_2D(x_lim_ilot[ilot_size-1],x_lim_ilot[0],y_lim_ilot[ilot_size-1],y_lim_ilot[0]);
	list_origine[exte_size].x = x_lim_ilot[ilot_size-1];
	list_origine[exte_size].y = y_lim_ilot[ilot_size-1];
	for (int i=1 ; i<ilot_size-1 ; i++) {
		list_segment[i+exte_size]=CREA_SEGMENT_2D(x_lim_ilot[i],x_lim_ilot[i+1],y_lim_ilot[i],y_lim_ilot[i+1]);
		list_origine[i+exte_size].x = x_lim_ilot[i];
		list_origine[i+exte_size].y = y_lim_ilot[i];
	}
				//pour l'intérieur
	list_segment[exte_size+ilot_size]=CREA_SEGMENT_2D(x_lim_inte[inte_size-1],x_lim_inte[0],y_lim_inte[inte_size-1],y_lim_inte[0]);
	list_origine[exte_size+ilot_size].x = x_lim_inte[inte_size-1];
	list_origine[exte_size+ilot_size].y = y_lim_inte[inte_size-1];
	for (int i=1 ; i<inte_size-1 ; i++) {
		list_segment[i+exte_size+ilot_size]=CREA_SEGMENT_2D(x_lim_inte[i],x_lim_inte[i+1],y_lim_inte[i],y_lim_inte[i+1]);
		list_origine[i+exte_size+ilot_size].x = x_lim_inte[i];
		list_origine[i+exte_size+ilot_size].y = y_lim_inte[i];
	}
				//pour le lac
	list_segment[exte_size+ilot_size+inte_size]=CREA_SEGMENT_2D(x_lim_lac[lac_size-1],x_lim_lac[0],y_lim_lac[lac_size-1],y_lim_lac[0]);
	list_origine[exte_size+ilot_size+inte_size].x = x_lim_lac[lac_size-1];
	list_origine[exte_size+ilot_size+inte_size].y = y_lim_lac[lac_size-1];
	for (int i=1 ; i<lac_size-1 ; i++) {
		list_segment[i+exte_size+ilot_size+inte_size]=CREA_SEGMENT_2D(x_lim_lac[i],x_lim_lac[i+1],y_lim_lac[i],y_lim_lac[i+1]);
		list_origine[i+exte_size+ilot_size+inte_size].x = x_lim_lac[i];
		list_origine[i+exte_size+ilot_size+inte_size].y = y_lim_lac[i];
	}
		//puis on fais un test de collisions avec chaque segment
    for (int i=0 ; i<exte_size+ilot_size+inte_size+lac_size ; i++) {
        if (INTERSECTION_SEGMENT2D(&list_segment[i], &list_origine[i], rayon, position_disque)) {
            return Calcul_reflexion(direction_voulue, &list_segment[i]);
        }
    }
}
